#!/bin/bash

make clean && make btest

echo "===================="

if [[ -z "$1" ]]; then
    echo "Empty arguments"
    ./btest -g
else
    echo "$1 detected"
    ./btest -f $1
fi
