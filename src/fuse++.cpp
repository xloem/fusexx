#include "../include/fuse++"

#include <cerrno>
#include <cstdlib>

#if defined(_WIN32) || defined(_WIN64)
#define thread_local _declspec(thread)
#else
#define thread_local __thread
#endif

#ifndef FUSE_USE_VERSION
#define FUSE_USE_VERSION 26
#endif

#include <fuse/fuse.h>

class fuse::detail
{
public:
    static class fuse & fuse()
    {
        struct fuse_context * ctx = fuse_get_context();
        class fuse * fuseptr =
            static_cast<class fuse*>(ctx->private_data);
        if (fuseptr->pid == 0) {
            fuseptr->uid = ctx->uid;
            fuseptr->gid = ctx->gid;
            fuseptr->pid = ctx->pid;
        #if FUSE_VERSION >= 28
            fuseptr->umask = ctx->umask;
        #endif
        }
        return *fuseptr;
    }

    static int getattr(const char *pathname, struct stat *buf) { return fuse().getattr(pathname, buf); }
    static int readlink(const char *pathname, char *buffer, size_t size) { return fuse().readlink(pathname, buffer, size); }
    static int mknod(const char *pathname, mode_t mode, dev_t dev) { return fuse().mknod(pathname, mode, dev); }
    static int mkdir(const char *pathname, mode_t mode) { return fuse().mkdir(pathname, mode); }
    static int unlink(const char *pathname) { return fuse().unlink(pathname); }
    static int rmdir(const char *pathname) { return fuse().rmdir(pathname); }
    static int symlink(const char *target, const char *linkpath) { return fuse().symlink(target, linkpath); }
    static int rename(const char *oldpath, const char *newpath) { return fuse().rename(oldpath, newpath); }
    static int link(const char *oldpath, const char *newpath) { return fuse().link(oldpath, newpath); }
    static int chmod(const char *pathname, mode_t mode) { return fuse().chmod(pathname, mode); }
    static int chown(const char *pathname, uid_t uid, gid_t gid) { return fuse().chown(pathname, uid, gid); }
    static int truncate(const char *path, off_t length) { return fuse().truncate(path, length); }
    static int open(const char *pathname, struct fuse_file_info *fi) { return fuse().open(pathname, fi); }
    static int read(const char *pathname, char *buf, size_t count, off_t offset, struct fuse_file_info *fi) { return fuse().read(pathname, buf, count, offset, fi); }
    static int write(const char *pathname, const char *buf, size_t count, off_t offset, struct fuse_file_info *fi) { return fuse().write(pathname, buf, count, offset, fi); }
    static int statfs(const char *path, struct statvfs *buf) { return fuse().statfs(path, buf); }
    static int flush(const char *pathname, struct fuse_file_info *fi) { return fuse().flush(pathname, fi); }
    static int release(const char *pathname, struct fuse_file_info *fi) { return fuse().release(pathname, fi); }
    
    #if FUSE_VERSION > 21
    static int fsync(const char *pathname, int datasync, struct fuse_file_info *fi) { return fuse().fsync(pathname, datasync, fi); }
    static int setxattr(const char *path, const char *name, const char *value, size_t size, int flags) { return fuse().setxattr(path, name, value, size, flags); }
    static int getxattr(const char *path, const char *name, char *value, size_t size) { return fuse().getxattr(path, name, value, size); }
    static int listxattr(const char *path, char *list, size_t size) { return fuse().listxattr(path, list, size); }
    static int removexattr(const char *path, const char *name) { return fuse().removexattr(path, name); }
    #endif // FUSE_VERSION > 21
    
    static thread_local void * filler_handle;
    #if FUSE_VERSION > 22
    static int opendir(const char *opendir, struct fuse_file_info *fi) { return fuse().opendir(opendir, fi); }

    static thread_local fuse_fill_dir_t filler;
    static int readdir(const char *pathname, void *buf, fuse_fill_dir_t filler, off_t off, struct fuse_file_info *fi)
    {
        detail::filler_handle = buf;
        detail::filler = filler;
        return fuse().readdir(pathname, off, fi);
    }

    static int releasedir(const char *pathname, struct fuse_file_info *fi) { return fuse().releasedir(pathname, fi); }
    static int fsyncdir(const char *pathname, int datasync, struct fuse_file_info *fi) { return fuse().fsyncdir(pathname, datasync, fi); }
        
    static void *init(struct fuse_conn_info *conn)
    {
        class fuse * fuseptr = &fuse();
        fuseptr->init();
        return fuseptr;
    }
    static void destroy(void *private_data)
    {
        class fuse * fuseptr = static_cast<class fuse *>(private_data);
        fuseptr->destroy();
    }
    #else // FUSE_VERSION > 22
    static thread_local fuse_dirfil_t filler;
    static int getdir(const char *pathname, fuse_dirh_t handle, fuse_dirfil_t filler)
    {
        detail::filler_handle = handle;
        detail::filler = filler;
        return fuse().readdir(pathname, 0, 0);
    }
    #endif // FUSE_VERSION > 22
    
    #if FUSE_VERSION >= 25
    static int access(const char *pathname, int mode) { return fuse().access(pathname, mode); }
    static int create(const char *pathname, mode_t mode, struct fuse_file_info *fi) { return fuse().create(pathname, mode, fi); }
    static int ftruncate(const char *pathname, off_t length, struct fuse_file_info *fi) { return fuse().ftruncate(pathname, length, fi); }

    static int fgetattr(const char *pathname, struct stat *buf, struct fuse_file_info *fi) { return fuse().fgetattr(pathname, buf, fi); }

    #endif // FUSE_VERSION >= 25
    
    #if FUSE_VERSION >= 26
    static int lock(const char *pathname, struct fuse_file_info *fi, int cmd, struct flock *lock) { return fuse().lock(pathname, fi, cmd, lock); }
    static int utimens(const char *pathname, const struct timespec tv[2]) { return fuse().utimens(pathname, tv); }
    static int bmap(const char *pathname, size_t blocksize, uint64_t *idx) { return fuse().bmap(pathname, blocksize, idx); }
    static int ioctl(const char *pathname, int cmd, void *arg, struct fuse_file_info *fi, unsigned int flags, void *data) { return fuse().ioctl(pathname, cmd, arg, fi, flags, data); }
    static int poll(const char *pathname, struct fuse_file_info *fi, struct fuse_pollhandle *ph, unsigned *reventsp) { return fuse().poll(pathname, fi, ph, reventsp); }
    static int write_buf(const char *pathname, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *fi) { return fuse().write_buf(pathname, buf, off, fi); }
    static int read_buf(const char *pathname, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *fi) { return fuse().read_buf(pathname, bufp, size, off, fi); }
    static int flock(const char *pathname, struct fuse_file_info *fi, int op) { return fuse().flock(pathname, fi, op); }
    static int fallocate(const char *pathname, int mode, off_t offset, off_t len, struct fuse_file_info *fi) { return fuse().fallocate(pathname, mode, offset, len, fi); }
    #else // FUSE_VERSION >= 26

    static int utime(const char *pathname, struct utimbuf *times)
    {
        struct timespec tv[2] = {
            {
                .tv_sec = times->actime,
                .tv_nsec = 0
            }, {
                .tv_sec = times->modtime,
                .tv_nsec = 0
            }
        }
        return fuse().utimens(pathname, tv);
    }

    #endif // FUSE_VERSION >= 26
};

thread_local void * fuse::detail::filler_handle;
#if FUSE_VERSION > 22
thread_local fuse_fill_dir_t fuse::detail::filler;
#else
thread_local fuse_dirfil_t fuse::detail::filler;
#endif

int fuse::getattr(const std::string &pathname, struct stat *buf) { return -ENOSYS; }
int fuse::readlink(const std::string &pathname, char *buffer, size_t size) { return -ENOSYS; }
int fuse::mknod(const std::string &pathname, mode_t mode, dev_t dev) { return -ENOSYS; }
int fuse::mkdir(const std::string &pathname, mode_t mode) { return -ENOSYS; }
int fuse::unlink(const std::string &pathname) { return -ENOSYS; }
int fuse::rmdir(const std::string &pathname) { return -ENOSYS; }
int fuse::symlink(const std::string &target, const std::string &linkpath) { return -ENOSYS; }
int fuse::rename(const std::string &oldpath, const std::string &newpath) { return -ENOSYS; }
int fuse::link(const std::string &oldpath, const std::string &newpath) { return -ENOSYS; }
int fuse::chmod(const std::string &pathname, mode_t mode) { return -ENOSYS; }
int fuse::chown(const std::string &pathname, uid_t uid, gid_t gid) { return -ENOSYS; }
int fuse::truncate(const std::string &path, off_t length) { return -ENOSYS; }
int fuse::open(const std::string &pathname, struct fuse_file_info *fi) { return 0; }
int fuse::read(const std::string &pathname, char *buf, size_t count, off_t offset, struct fuse_file_info *fi) { return -ENOSYS; }
int fuse::write(const std::string &pathname, const char *buf, size_t count, off_t offset, struct fuse_file_info *fi) { return -ENOSYS; }
int fuse::statfs(const std::string &path, struct statvfs *buf) { return 0; }
int fuse::flush(const std::string &pathname, struct fuse_file_info *fi) { return -ENOSYS; }
int fuse::release(const std::string &pathname, struct fuse_file_info *fi) { return 0; }

#if FUSE_VERSION > 21
int fuse::fsync(const std::string &pathname, int datasync, struct fuse_file_info *fi) { return -ENOSYS; }
int fuse::setxattr(const std::string &path, const std::string &name, const std::string &value, size_t size, int flags) { return -ENOSYS; }
int fuse::getxattr(const std::string &path, const std::string &name, char *value, size_t size) { return -ENOSYS; }
int fuse::listxattr(const std::string &path, char *list, size_t size) { return -ENOSYS; }
int fuse::removexattr(const std::string &path, const std::string &name) { return -ENOSYS; }
#endif // FUSE_VERSION > 21

#if FUSE_VERSION > 22
int fuse::opendir(const std::string &opendir, struct fuse_file_info *fi) { return 0; }
int fuse::readdir(const std::string &pathname, off_t off, struct fuse_file_info *fi) { return -ENOSYS; }

int fuse::fill_dir(const std::string &name, const struct stat *stbuf, off_t off)
{
    return detail::filler(detail::filler_handle, name.c_str(), stbuf, off);
}

int fuse::releasedir(const std::string &pathname, struct fuse_file_info *fi) { return 0; }
int fuse::fsyncdir(const std::string &pathname, int datasync, struct fuse_file_info *fi) { return -ENOSYS; }
void fuse::init() { }
void fuse::destroy() { }
#else
int fuse::fill_dir(const std::string &name, const struct stat *stbuf, off_t off)
{
    return detail::filler(detail::filler_handle, name.c_str(), (stbuf->st_mode & S_IFMT) >> 12, stbuf->st_ino, 0);
}
#endif // FUSE_VERSION > 22

#if FUSE_VERSION >= 25
int fuse::access(const std::string &pathname, int mode) { return -ENOSYS; }
int fuse::create(const std::string &pathname, mode_t mode, struct fuse_file_info *fi) { return -ENOSYS; }

int fuse::ftruncate(const std::string &pathname, off_t length, struct fuse_file_info *fi)
{
    return truncate(pathname, length);
}

int fuse::fgetattr(const std::string &pathname, struct stat *buf, struct fuse_file_info *fi)
{
    return getattr(pathname, buf);
}

#endif // FUSE_VERSION >= 25

#if FUSE_VERSION >= 26
int fuse::lock(const std::string &pathname, struct fuse_file_info *fi, int cmd, struct flock *lock) { return -ENOSYS; }
int fuse::utimens(const std::string &pathname, const struct timespec tv[2]) { return -ENOSYS; }
int fuse::bmap(const std::string &pathname, size_t blocksize, uint64_t *idx) { return -ENOSYS; }
int fuse::ioctl(const std::string &pathname, int cmd, void *arg, struct fuse_file_info *fi, unsigned int flags, void *data) { return -ENOSYS; }
int fuse::poll(const std::string &pathname, struct fuse_file_info *fi, struct fuse_pollhandle *ph, unsigned *reventsp) { return -ENOSYS; }

int fuse::write_buf(const std::string &pathname, struct fuse_bufvec *bufvec, off_t off, struct fuse_file_info *fi)
{
    int total = 0;
    while (bufvec->idx < bufvec->count) {
        struct fuse_buf & buf = bufvec->buf[bufvec->idx];
        size_t & bufoff = bufvec->off;
        int subsize = buf.size - bufoff;
        int subtotal;
        if (buf.flags & FUSE_BUF_IS_FD) {
            return -ENOSYS;
        }
        subtotal = write(pathname, (char*)buf.mem + bufoff, subsize, off, fi);
        if (subtotal < 0) {
            return subtotal;
        }
        bufoff += subtotal;
        off += subtotal;
        total += subtotal;
        if (subtotal < subsize) {
            return total;
        }
        bufoff = 0;
        ++ bufvec->idx;
    }
    return 0;
}

int fuse::read_buf(const std::string &pathname, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *fi)
{
    *bufp = (struct fuse_bufvec *)malloc(sizeof(**bufp));
    struct fuse_bufvec & bufvec = **bufp;
    bufvec.count = 1;
    bufvec.idx = 0;
    bufvec.off = 0;
    bufvec.buf[0].size = size;
    bufvec.buf[0].flags = (fuse_buf_flags)0;
    bufvec.buf[0].mem = malloc(size);
    bufvec.buf[0].fd = 0;
    bufvec.buf[0].pos = 0;
    ssize_t amount = read(pathname, (char*)bufvec.buf[0].mem, size, off, fi);
    bufvec.buf[0].size = amount;
    bufvec.off = amount;
    bufvec.buf[0].pos = amount;
    return amount;
}

int fuse::flock(const std::string &pathname, struct fuse_file_info *fi, int op) { return -ENOSYS; }
int fuse::fallocate(const std::string &pathname, int mode, off_t offset, off_t len, struct fuse_file_info *fi) { return -ENOSYS; }
#endif // FUSE_VERSION >= 26

fuse::fuse()
: uid(0),
  gid(0),
  pid(0),
  umask(022)
{ }

int fuse::main(int argc, char *argv[])
{
    int ret;
    static struct fuse_operations ops = {
        .getattr     = fuse::detail::getattr,
        .readlink    = fuse::detail::readlink,
    #if FUSE_VERSION > 22
        .getdir      = 0,
    #else
        .getdir      = fuse::detail::getdir,
    #endif
        .mknod       = fuse::detail::mknod,
        .mkdir       = fuse::detail::mkdir,
        .unlink      = fuse::detail::unlink,
        .rmdir       = fuse::detail::rmdir,
        .symlink     = fuse::detail::symlink,
        .rename      = fuse::detail::rename,
        .link        = fuse::detail::link,
        .chmod       = fuse::detail::chmod,
        .chown       = fuse::detail::chown,
        .truncate    = fuse::detail::truncate,
    #if FUSE_VERSION >= 26
        .utime       = 0,
    #else
        .utime       = fuse::detail::utime,
    #endif
        .open        = fuse::detail::open,
        .read        = fuse::detail::read,
        .write       = fuse::detail::write,
        .statfs      = fuse::detail::statfs,
        .flush       = fuse::detail::flush,
        .release     = fuse::detail::release,
    #if FUSE_VERSION > 21
        .fsync       = fuse::detail::fsync,
        .setxattr    = fuse::detail::setxattr,
        .getxattr    = fuse::detail::getxattr,
        .listxattr   = fuse::detail::listxattr,
        .removexattr = fuse::detail::removexattr,
    #endif // FUSE_VERSION > 21
    #if FUSE_VERSION > 22
        .opendir     = fuse::detail::opendir,
        .readdir     = fuse::detail::readdir,
        .releasedir  = fuse::detail::releasedir,
        .fsyncdir    = fuse::detail::fsyncdir,
        .init        = fuse::detail::init,
        .destroy     = fuse::detail::destroy,
    #endif // FUSE_VERSION > 22
    #if FUSE_VERSION >= 25
        .access      = fuse::detail::access,
        .create      = fuse::detail::create,
        .ftruncate   = fuse::detail::ftruncate,
        .fgetattr    = fuse::detail::fgetattr,
    #endif // FUSE_VERSION >= 25
    #if FUSE_VERSION >= 26
        .lock        = fuse::detail::lock,
        .utimens     = fuse::detail::utimens,
        .bmap        = fuse::detail::bmap,

        /* these 2 flags apply to read, write, flush, release, fsync, readdir,
           releasedir, fsyncdir, ftruncate, fgetattr, lock, ioctl and poll */
        .flag_nullpath_ok = false, /* accept NULL pathnames, lets -ohard_remove work on unlinked files */
        .flag_nopath = false, /* do not calculate a path */

        .flag_utime_omit_ok = false, /* use UTIME_NOW, UTIME_OMIT in utimens */
        .flag_reserved = 0,

        .ioctl       = fuse::detail::ioctl,
        .poll        = fuse::detail::poll,
        .write_buf   = 0,//fuse::detail::write_buf,
        .read_buf    = 0,//fuse::detail::read_buf,
        .flock       = fuse::detail::flock,
        .fallocate   = fuse::detail::fallocate,
    #endif
    }; 

    #if FUSE_VERSION < 23
    init()
    #endif

    ret = fuse_main(argc, argv, &ops, this);

    #if FUSE_VERSION < 23
    destroy()
    #endif

    return ret;
}

fuse::~fuse()
{ }
