# Motion Vector
Parallel algorithm for solving the motion vector problem.

# Team members
- Diego Cifuentes
- Hamilton Tobon Mosquera

# How to compile and run?

To test each different implementation, that is, *serial, openmp, mpi, mpi + openmp*, change to each different branch, and follow the instructions there:

```
$ git branch <implementation>
```

Run from this directory:
```
$ make
$ ./run.sh <num-processes> <hosts-file> 2> mpi-times
```

Where <num-processes> is the number of processes to crete. Note that the height of the image divided by <num-processes> must be >= 16.

Where <hosts-file> indicates the file with the hosts to use.

The results in *mpi-times* were obtained using 24 processes in nodes 192.168.10.81-2.

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

The tested results are plotted in [speedup]()https://github.com/hamax97/motion-vector/blob/master/grafica.ipynb and [speedup2](https://github.com/hamax97/motion-vector/blob/master/out_1.xlsx)
The image dimensions must be divisible by 16.

The methodology used to implement these algorithms is described in [PCAM](https://github.com/hamax97/motion-vector/blob/master/pcam.md)

The description of the openmp implementation is decribed in [OpenMP](https://github.com/hamax97/motion-vector/blob/master/openmp.md)
