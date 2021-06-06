# fuse++

C++ wrapper for libfuse.

This is presently a partial implementation, with a goal of exceeding what is
presently available from alternative libraries such as
[Fusepp](https://github.com/jachappell/Fusepp).  The basics of the "easy"
interface are in [`#include <fuse++>`](include/fuse++) and hopefully are mostly
working with feature parity up to fuse 2.6 and stability through fuse 3.0.  The
lowlevel interface is not implemented yet, but a
[lowlevel header](include/fuse++_lowlevel) is stubbed out.

Feel free to extend this library, or maybe I will complete it.

The end goal would to have feature-parity with libfuse using C++ idioms
compatible with C++03, to make it easy for old projects to add fuse support.
