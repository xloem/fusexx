# fuse++

C++ wrapper for libfuse.

This is presently a partial implementation, with a goal of exceeding what is
presently available from alternative libraries such as
[Fusepp](https://github.com/jachappell/Fusepp).  The basics of the "easy"
interface are in [`#include <fuse++>`](include/fuse++) and hopefully are mostly
working.  The lowlevel interface is not implemented yet, but the [lowlevel
header](include/fuse++_lowlevel) is stubbed out.

Feel free to extend this library, or maybe I will complete it.

The end goal would to have feature-parity with libfuse using C++ idioms
compatible with C++03, to make it easy for old projects to add fuse support.
