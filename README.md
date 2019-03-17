# Motion Vector
Parallel algorithm for solving the motion vector problem.

# Team members
- Diego Cifuentes
- Hamilton Tobon Mosquera

# How to compile and run?

Run from this directory:
```
$ make
$ ./run.sh <num-processes> <hosts-file> <num-threads> 2> mpi-openmp-times
```

Where <num-processes> is the number of processes to crete. Note that the height of the image divided by <num-processes> must be >= 16.

Where <hosts-file> indicates the file with the hosts to use.

Where <num-threads> indicates the number of threads to use. Note that this number must the sum of the threads in each node, not just threads per node.

The results in *mpi-openmp-times* were obtained using 2 processes and 48 threads in nodes 192.168.10.81-2.

Each section of time represents each combination of images, A, B, C and D respectively, where:
- A:
  * PrestigiousEnchantedKusimanse-size_restricted_frame_0001.bmp as original image.
  * PrestigiousEnchantedKusimanse-size_restricted_frame_0021.bmp as compressed or moved image.

- B:
  * PrestigiousEnchantedKusimanse-size_restricted_frame_0002.bmp as original image.
  * PrestigiousEnchantedKusimanse-size_restricted_frame_0020.bmp as compressed or moved image.

- C:
  * PrestigiousEnchantedKusimanse-size_restricted_frame_0003.bmp as original image.
  * PrestigiousEnchantedKusimanse-size_restricted_frame_0019.bmp as compressed or moved image.

- D:
  * PrestigiousEnchantedKusimanse-size_restricted_frame_0004.bmp as original image.
  * PrestigiousEnchantedKusimanse-size_restricted_frame_0018.bmp as compressed or moved image.

# Important notes

The image dimensions must be divisible by 16.