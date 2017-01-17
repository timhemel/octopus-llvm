#!/bin/sh

basedir=`dirname "$0"`

clang -g -O0 -c -Xclang -load -Xclang "$basedir"/build/octopusgraph/libOctopusGraphPass.so "$@"

