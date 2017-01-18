# Add LLVM bitcode to Octopus

This LLVM pass will analyze the LLVM bitcode that was emitted by an LLVM front-end (such as clang)
and create a program graph of it that can be loaded into [Octopus](https://github.com/octopus-platform/joern).


# Building

To build, do the following:

    $ cd octopus-llvm
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ cd ..

# Running

The basic form for running the plugin is:

    $ clang -Xclang -load -Xclang build/skeleton/libOctopusGraphPass.* something.c

In practice, you want to add extra options, examples are in `create_octopus_graph.sh`.

