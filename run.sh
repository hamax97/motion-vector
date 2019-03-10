#!/bin/bash

processes=$1
binary=$2
original_frame=$3
compressed_frame=$4

mpirun -n $processes $binary $original_frame $compressed_frame
