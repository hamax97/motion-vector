#!/bin/bash

# Arguments
num_processes=$1
hosts_file=$2

time mpirun -np $num_processes -f $hosts_file ./motion-vector tests/PrestigiousEnchantedKusimanse-size_restricted_frame_0001.bmp tests/PrestigiousEnchantedKusimanse-size_restricted_frame_0021.bmp

time mpirun -np $num_processes -f $hosts_file ./motion-vector tests/PrestigiousEnchantedKusimanse-size_restricted_frame_0002.bmp tests/PrestigiousEnchantedKusimanse-size_restricted_frame_0020.bmp

time mpirun -np $num_processes -f $hosts_file ./motion-vector tests/PrestigiousEnchantedKusimanse-size_restricted_frame_0003.bmp tests/PrestigiousEnchantedKusimanse-size_restricted_frame_0019.bmp

time mpirun -np $num_processes -f $hosts_file ./motion-vector tests/PrestigiousEnchantedKusimanse-size_restricted_frame_0004.bmp tests/PrestigiousEnchantedKusimanse-size_restricted_frame_0018.bmp
