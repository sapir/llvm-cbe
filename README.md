llvm-cbe
========

Resurrected LLVM "C Backend", with improvements


Installation Instructions
=========================

This version of the LLVM-CBE library works with LLVM 6.0, 7.0 and 8.0. You will
have to compile one of these versions of LLVM to use LLVM-CBE.

The library is known to compile on various Linux versions (Redhat, Mageia,
Ubuntu, Debian), Mac OS X, and Windows (Mingw-w64).

This guide will walk you through the compilation and installation of both tools,
and show usage statements to verify that the LLVM-CBE library is compiled
correctly.

Step 1: Getting LLVM and LLVM-CBE
=================================

LLVM-CBE relies on specific LLVM internals, and so it is best to use it with a
specific revision of the LLVM development tree. Currently, llvm-cbe works with
the LLVM 6.0, 7.0 and 8.0 release versions and autotools.

Note: to run the tests, you will also need both Clang and GCC compilers.

The first step is to compile LLVM on your machine (this assumes an in-tree
build, but out-of-tree may also work):

```sh
    cd $HOME
    git clone https://github.com/llvm-mirror/llvm

    cd $HOME/llvm
    git checkout release_80

    cd $HOME/llvm/projects
    git clone https://github.com/JuliaComputing/llvm-cbe
```

Step 2: Compiling LLVM-CBE
==========================

Next, compile llvm-cbe (along with required parts of llvm):

```sh
    cd $HOME/llvm
    mkdir build
    cd build
    cmake ..
    make llvm-cbe
```

For development, `BUILD_SHARED_LIBS` is recommended:

```sh
    cd $HOME/llvm/build
    cmake .. -DBUILD_SHARED_LIBS=ON
    make llvm-cbe
```

Step 3: Usage Examples
======================

If `llvm-cbe` compiles, you should be able to run it with the following
commands.

```sh
    $ cd $HOME/llvm/projects/llvm-cbe/test/selectionsort
    $ ls
    main.c
    $ clang -S -emit-llvm main.c
    $ ls
    main.c main.ll
    $ $HOME/llvm/build/bin/llvm-cbe main.ll
```

The `llvm-cbe` executable converts your `.ll` file to a C file, which you can
then compile:

```sh
    $ gcc -o main.cbe main.cbe.c
    $ ls
    main.c  main.cbe  main.cbe.c  main.ll
    $ ./main.cbe
```
