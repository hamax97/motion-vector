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

  //////////////////////////////////////////////////////////////////////////////
  /* Initialize MPI environment */
  MPI_Init(NULL, NULL);

  int world_size; /* Number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int world_rank; /* MPI Process ID */
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  //////////////////////////////////////////////////////////////////////////////

  /* ----------------------- Split and Scatterv orginial_frame  ------------- */
  /* New frame size */
  int subframe_height = original_frame.height / world_size;
  int missing_pixels = original_frame.height % world_size;

  if( subframe_height < 16 )
    {
      fprintf(stderr, "Select a shorter number of processes, each process"\
	      " must process minimum a matrix of size 16 x <FrameWidth>\n");
      exit(EXIT_FAILURE);
    }

  /* Sub matrix sizes for each process and displacements calculation */
  int sendcounts[world_size];
  int displs[world_size];

  /* Process 0: processes the same size as others plus the missing pixels */
  sendcounts[0] = (subframe_height + missing_pixels) * original_frame.width;
  displs[0] = 0;

  int sum = sendcounts[0];
  for(int i = 1; i < world_size; ++i)
    {
      sendcounts[i] = subframe_height * original_frame.width;
      displs[i] = sum;
      sum += sendcounts[i];
    }

  /* Buffer where data will be received */
  BMP my_original_frame;

  my_original_frame.width = original_frame.width;
  if(world_rank == 0)
    my_original_frame.height = subframe_height + missing_pixels;
  else
    my_original_frame.height = subframe_height;

  my_original_frame.pixels =
    (unsigned char *) malloc(my_original_frame.height *
			     my_original_frame.width *
			     sizeof(unsigned char));
  /* Scatter the original frame */
  MPI_Scatterv((void *) original_frame.pixels, sendcounts, displs, MPI_BYTE,
	       my_original_frame.pixels,
	       my_original_frame.height * my_original_frame.width, MPI_BYTE,
	       0, MPI_COMM_WORLD);

  //////////////////////////////////////////////////////////////////////////////

  /* ----------------------- Broadcast the compressed_frame ------------------ */

  MPI_Bcast(next_frame.pixels,
	    next_frame.height * next_frame.width, MPI_BYTE, 0,
	    MPI_COMM_WORLD);

  //////////////////////////////////////////////////////////////////////////////

  /* ----------------------- Execute the algorithm -------------------------- */
  MotionVector compressed_frame = calc_motion_vector(original_frame,
  						     next_frame);
  //print_vector(compressed_frame);

  //////////////////////////////////////////////////////////////////////////////

  /* ----------------------- Gather results --------------------------------- */

  /* Position datatype */
  MPI_Datatype MPI_POS;
  MPI_Type_contiguous(2, MPI_INT, &MPI_POS);
  MPI_Type_commit(&MPI_POS);

  int size = compressed_frame.rows * compressed_frame.cols;
  Position result[size];

  /* Convert current matrix position to plain array */
  for(int i = 0; i < compressed_frame.rows; i++)
    {
      for(int j = 0; j < compressed_frame.cols; j++)
	{
	  result[(i*compressed_frame.rows) + j] =
	    compressed_frame.macro_block[i][j];
	}
    }

  int receive_buffer_size = (original_frame.height - 16) *
    (original_frame.width - 16);

  Position recvbuff[receive_buffer_size];
  int recvcounts[world_size];
  int displs[world_size];


  // HOW TO KNOW HOW MUCH DATA WILL ONE PROCESS SEND ?
  recvcounts[];

  MPI_Gatherv(result, size, MPI_POS, recvbuff, );


  /* Free allocated space in function read_bmp() */
  free(original_frame.pixels);
  free(next_frame.pixels);
  free(my_original_frame.pixels);

  MPI_Finalize();

  return EXIT_SUCCESS;
}
