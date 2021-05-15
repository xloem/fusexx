#include "fuse++"

#include <cerrno>
#include <cstring>
#include <iostream>
#include <map>
#include <set>

class FS : public fuse
{
public:
    struct File
    {
        std::string name;
        mode_t mode;
        std::string content;

        File(std::string const & name, mode_t mode, std::string const & content = "")
        : name(name), mode(mode), content(content)
        { }
        File() { }
        File& operator=(File const& other) { name = other.name; mode = other.mode; content = other.content; return *this; }
    };
    std::map<std::string,File> files;

    void init()
    {
        files.clear();
        files["/"] = File("", S_IFDIR | (0777 ^ umask));
        files["/helloworld.txt"] = File("helloworld.txt", S_IFREG | (0666 ^ umask), "Hello, world.\n");
    }

    void destroy()
    { }

    /* reading functions */

    int getattr(const std::string &pathname, struct stat *st)
    {
        memset(st, 0, sizeof(*st));
        st->st_uid = uid;
        st->st_gid = gid;
        if (files.count(pathname)) {
            File & file = files[pathname];
            st->st_mode = file.mode;
            st->st_size = file.content.size();
            return 0;
        } else {
            return -ENOENT;
        }
    }

    int readdir(const std::string &pathname, off_t off, struct fuse_file_info *fi)
    {
        struct stat st;
        memset(&st, 0, sizeof(st));
        for (std::map<std::string,File>::iterator it = files.begin(); it != files.end(); ++ it) {
            std::string const & filepath = it->first;
            File & file = it->second;
            if (file.name.size() + pathname.size() != filepath.size() || 0 != filepath.compare(0, pathname.size(), pathname)) {
                // path not in dir
                std::cout << pathname << ": Skipping " << filepath << ": " << file.name << std::endl;
                continue;
            }
            getattr(filepath, &st);
            fill_dir(file.name, &st, 0);
        }
        return 0;
    }

    int read(const std::string &pathname, char *buf, size_t count, off_t offset, struct fuse_file_info *fi)
    {
        std::string &content = files[pathname].content;
        if (count + offset > content.size()) {
            if ((size_t)offset > content.size()) {
                count = 0;
            } else {
                count = content.size() - offset;
            }
        }
        memcpy(buf, content.data() + offset, count);
        return count;
    }

    /* writing functions */

    int chmod(const std::string &pathname, mode_t mode)
    {
        if (files.count(pathname)) {
            files[pathname].mode = mode;
            return 0;
        } else {
            return -ENOENT;
        }
    }

    int write(const std::string &pathname, const char *buf, size_t count, off_t offset, struct fuse_file_info *fi)
    {
        std::string & content = files[pathname].content;
        size_t precount = offset + count > content.size() ? content.size() - offset : count;
        content.replace(offset, precount, std::string(buf, buf + count));
        return count;
    }

    int truncate(const std::string &pathname, off_t length)
    {
        if (files.count(pathname)) {
            files[pathname].content.resize(length);
            return 0;
        } else {
            return -ENOENT;
        }
    }

    int mknod(const std::string &pathname, mode_t mode, dev_t dev)
    {
        files[pathname] = File(pathname.substr(pathname.rfind('/') + 1), mode);
        return 0;
    }

    int mkdir(const std::string &pathname, mode_t mode)
    {
        files[pathname] = File(pathname.substr(pathname.rfind('/') + 1), S_IFDIR | mode);
        return 0;
    }

    int unlink(const std::string &pathname)
    {
        files.erase(pathname);
        return 0;
    }

    int rmdir(const std::string &pathname)
    {
        files.erase(pathname);
        return 0;
    }

    int rename(const std::string &oldpath, const std::string &newpath)
    {
        files[oldpath].name = newpath.substr(newpath.rfind('/') + 1, newpath.size());

        // loop moves files within directory when moving a directory by matching start of pathname
        std::map<std::string,File> moving;
        for (std::map<std::string,File>::iterator it = files.begin(); it != files.end();) {

            std::string const & filepath = it->first;
            File & file = it->second;

            if (0 != filepath.compare(0, oldpath.size(), oldpath)) {
                // unrelated path
                ++ it;
                continue;
            }
            std::string newsubpath = newpath + filepath.substr(oldpath.size());
            moving[newsubpath] = file;

            std::map<std::string,File>::iterator old = it;
            ++ it;
            files.erase(old);
        }
        for (std::map<std::string,File>::iterator it = moving.begin(); it != moving.end(); ++it) {
            files[it->first] = it->second;
        }
        return 0;
    }
};

int main(int argc, char *argv[])
{
    return FS().main(argc, argv);
}
