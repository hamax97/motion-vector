/* Structure to represent the Bitmap image file structure */ 
typedef struct {
  unsigned char header[54]; /* A BMP image contains a 54 byte header */
  int width; /* BMP image width */
  int height; /* BMP image width */
} BMP;
