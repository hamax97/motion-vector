#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <bmp.h>

/* Function to read a file with BMP format */
BMP read_bmp(char* file_name);

int
main(int argc, char* argv[])
{

  if(argc != 2)
    {
      fprintf(stderr, "Usage:\n$ ./motion-vector <path/to/file.bmp>\n");
      return EXIT_FAILURE;
    }

  BMP input_frame, compressed_frame;
  input_frame = read_bmp(argv[1]);
  //compressed_frame = read_bmp(argv[2]);

  // CALL TO ALGORITHM HERE
  // ......................

  /* Free allocated space in function read_bmp() */
  for(int i = 0; i < input_frame.height; ++i)
    free(input_frame.pixels[i]);

  free(input_frame.pixels);

  /* for(int i = 0; i < compressed_frame.height; ++i) */
  /*   free(compressed_frame.pixels[i]); */

  /* free(compressed_frame.pixels); */

  return EXIT_SUCCESS;
}

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
      bmp_frame.pixels = (unsigned char **) malloc((size_t) bmp_frame.height * sizeof(unsigned char **));

      if(bmp_frame.pixels == NULL)
	{
	  fprintf(stderr, "Error allocating pixel matrix in file: %s", file_name);
	  exit(EXIT_FAILURE);
	}

      for(int row = 0; row < bmp_frame.height; ++row)
      	{
      	  bmp_frame.pixels[row] =
      	    (unsigned char *) malloc((size_t) bmp_frame.width);
      	}

      /* Read pixels matrix */
      fseek(bmp_file, matrix_addr - HEADER_SIZE, SEEK_CUR);

      for(int row = 0; row < bmp_frame.height; ++row)
      	{
      	  if(fread((void *)bmp_frame.pixels[row], sizeof(unsigned char),
      		   (size_t) bmp_frame.width, bmp_file) != bmp_frame.width)
      	    {
      	      fprintf(stderr, "Error reading pixels matrix on file: %s",
      		      file_name);
      	      exit(EXIT_FAILURE);
      	    }
      	}

    }else
    perror("Error reading BMP image file");

  /* Close opened */
  fclose(bmp_file);

  return bmp_frame;
}
