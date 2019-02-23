#include <stdio.h>
#include <stdlib.h>
#include <bmp.h>

int
main(int argc, char* argv[]){

  if(argc != 2){
    fprintf(stderr, "Usage:\n$ motion-vector <path/to/file.bmp>\n");
    return EXIT_FAILURE;
  }

  /* Variable declaration */
  BMP input_image, compressed_image;
  FILE *input_bmp;

  /* Open BMP files */
  input_bmp = fopen(argv[1], "r");

  if(input_bmp != NULL){

    /* Read header */
    if(fread((void *)input_image.header, sizeof(unsigned char), 54, input_bmp)
       != 54){
      fprintf(stderr, "Error reading file %s", argv[1]);
      return EXIT_FAILURE;
    }

    /* Obtain width and height from BMP header */
    input_image.width = input_image.header[20 -1] * 256 + input_image.header[19 -1];
    input_image.height = input_image.header[24 -1] * 256 + input_image.header[23 -1];

    printf("Image %s width: %d x height: %d\n", argv[1],
	   input_image.width, input_image.height);
    
  }else
    perror("Error reading BMP image file");
  
  return EXIT_SUCCESS;
}
