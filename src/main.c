#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <bmp.h>
#include <mpeg2.h>
#include <motion_vector.h>
#include <mpi.h>

int
main(int argc, char* argv[])
{
  if(argc != 3)
    {
      fprintf(stderr,
	      "Usage:\n$ ./motion-vector <path/to/file.bmp> <path/to/file.bmp>\n");
      return EXIT_FAILURE;
    }

  BMP original_frame, next_frame;
  original_frame = read_bmp(argv[1]);
  next_frame = read_bmp(argv[2]);

  
  /* /\* Initialize MPI environment *\/ */
  /* MPI_Init(NULL, NULL); */

  /* int world_size; /\* Number of processes *\/ */
  /* MPI_Comm_size(MPI_COMM_WORLD, &world_size); */

  /* int world_rank; /\* MPI Process ID *\/ */
  /* MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); */

  /* /\* New frame size *\/ */
  /* int subframe_height = original_frame.height / world_size; */
  /* int missing_pixels = original_frame.height % world_size; */

  /* /\* Scatter the matrix *\/ */
  /* MPI_Scatterv(); */

  /* MotionVector compressed_frame = calc_motion_vector(original_frame, */
  /* 						     next_frame); */
  /* print_vector(compressed_frame); */

  /* /\* Free allocated space in function read_bmp() *\/ */
  /* for(int i = 0; i < original_frame.height; ++i) { */
  /*   free(original_frame.pixels[i]); */
  /*   free(next_frame.pixels[i]); */
  /* } */

  compressed_frame.rows = compressed_frame.rows;
  free(original_frame.pixels);
  free(next_frame.pixels);

  return EXIT_SUCCESS;
}

