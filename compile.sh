#!/bin/bash

cmake -G "Unix Makefiles" -S . -B ./unix-build
cd ./unix-build
make
./$1