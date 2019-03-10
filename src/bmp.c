#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <bmp.h>

BMP read_bmp(char* file_name)
{
  /* Variable declaration */
  BMP bmp_frame;
  FILE *bmp_file;
  unsigned char bmp_header[HEADER_SIZE]; /* BMP image information header */
  unsigned int matrix_addr; /* Pixel matrix intial address */

  /* Open BMP files */
  bmp_file = fopen(file_name, "r");

  if(bmp_file != NULL)
    {

      /* Read information header */
      if( fread((void *)bmp_header, sizeof(unsigned char),
		HEADER_SIZE, bmp_file) != HEADER_SIZE )
	{
	  fprintf(stderr, "Error reading file %s", file_name);
	  exit(EXIT_FAILURE);
	}

      /* Obtain BMP signature */
      bmp_frame.signature[0] = bmp_header[0];
      bmp_frame.signature[1] = bmp_header[1];

      if(bmp_frame.signature[0] != 'B' || bmp_frame.signature[1] != 'M')
	{
	  fprintf(stderr, "Wrong file format: %s", file_name);
	  exit(EXIT_FAILURE);
	}

      /* Obtain width and height from BMP header */
      bmp_frame.width = *(int *) &bmp_header[18];
      bmp_frame.height = fabs(*(int *) &bmp_header[22]);

      /* Obtain bits per pixel */
      bmp_frame.bpp = *(short *) &bmp_header[28];

      if(bmp_frame.bpp != 8)
	{
	  fprintf(stderr, "Only supported gray scaled images: %s", file_name);
	  exit(EXIT_FAILURE);
	}

      /* Obtain image padding */
      bmp_frame.padding = 0;
      while((bmp_frame.width + bmp_frame.padding) % 4 != 0)
	{
	  ++bmp_frame.padding;
	}

      bmp_frame.width += bmp_frame.padding;

      /* Obtain matrix starting offset */
      matrix_addr = *(int *) &bmp_header[10];

      /* Allocate space for pixels matrix */
      bmp_frame.pixels =
	(unsigned char *) malloc(bmp_frame.height *
				  bmp_frame.width * sizeof(unsigned char));

      if(bmp_frame.pixels == NULL)
	{
	  fprintf(stderr, "Error allocating pixel matrix in file: %s", file_name);
	  exit(EXIT_FAILURE);
	}

      /* Read pixels matrix */
      fseek(bmp_file, matrix_addr - HEADER_SIZE, SEEK_CUR);

      int row = 0;
      for(int height = 0; height < bmp_frame.height; ++height)
      	{
      	  if( fread( (void *)(bmp_frame.pixels + row), sizeof(unsigned char),
		     bmp_frame.width, bmp_file) != bmp_frame.width )
      	    {
      	      fprintf(stderr, "Error reading pixels matrix on file: %s",
      		      file_name);
      	      exit(EXIT_FAILURE);
      	    }
	  row += bmp_frame.width;
      	}

    }else
    perror("Error reading BMP image file");

  /* Close opened */
  fclose(bmp_file);

  return bmp_frame;
}
