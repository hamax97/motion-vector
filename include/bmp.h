#ifndef BMP_H
#define BMP_H

/* Size in bytes of the header that describes the BMP file */
#define HEADER_SIZE 54

/* Structure to represent the Bitmap image file structure */
typedef struct {
  unsigned char signature[2]; /* BMP images have 'BM' as its first two bytes */
  unsigned int width; /* BMP image width */
  unsigned int height; /* BMP image width */
  unsigned short int bpp; /* Bits per pixel */
  /* Each .bmp image is padded to the right to have a width multiple
     of 4 bytes */
  unsigned int padding;
  unsigned char** pixels; /* Pixels matrix */
} BMP;

#endif
