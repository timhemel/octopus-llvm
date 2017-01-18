#!/bin/sh

basedir=`dirname "$0"`

# clang -g -O0 -c -Xclang -load -Xclang "$basedir"/build/octopusgraph/libOctopusGraphPass.so "$@"

# Compile a C module and output graphviz
clang -g -O0 -c -Xclang -load -Xclang "$basedir"/build/octopusgraph/libOctopusGraphPass.so -mllvm -graphviz -o /dev/null "$@"

