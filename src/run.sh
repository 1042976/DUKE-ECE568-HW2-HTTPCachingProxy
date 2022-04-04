#!/bin/bash
make clean
make
if [[ $? == 0 ]]; then
    ./httpCachingProxy
fi