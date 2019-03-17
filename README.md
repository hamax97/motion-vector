# Motion Vector
Parallel algorithm for solving the motion vector problem.

# Team members
- Diego Cifuentes
- Hamilton Tobon Mosquera

# How to compile and run?

Run from this directory:
```
$ make
$ ./run.sh 2> serial-times
```

The execution times can be found in the file *serial-times*.
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
