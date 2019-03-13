#include <stdio.h>
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

  /* Main algorithm */
  MotionVector compressed_frame = calc_motion_vector(original_frame,
						     next_frame);

  print_vector(compressed_frame);
  
  /* Free alocated memory in read_bmp */
  free(original_frame.pixels);
  free(next_frame.pixels);

  return EXIT_SUCCESS;
}
