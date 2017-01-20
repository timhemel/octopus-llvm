#!/bin/sh

basedir=`dirname "$0"`

# clang -g -O0 -c -Xclang -load -Xclang "$basedir"/build/octopusgraph/libOctopusGraphPass.so "$@"

# Compile a C module and output graphviz
# -mllvm -noddg  # do not generate data dependence edges
# -mllvm -noloc  # do not generate location nodes and edges
clang -g -O0 -c -Xclang -load -Xclang "$basedir"/build/octopusgraph/libOctopusGraphPass.so -mllvm -graphviz -o /dev/null "$@"

