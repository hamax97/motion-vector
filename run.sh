#!/bin/bash

# Arguments
processes=$1
hosts_file=$2
binary=$3
original_frame=$4
compressed_frame=$5

# Env Vars
#export OMP_NUM_THREADS=10

mpirun -np $processes -f $hosts_file $binary $original_frame $compressed_frame
