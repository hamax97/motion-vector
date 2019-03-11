#include <stdio.h>
#include <bmp.h>
#include <mpeg2.h>
#include <motion_vector.h>
#include <mpi.h>

int rank;

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

  rank = world_rank;
  printf("MPI initialized correctly, from process %d\n", world_rank);

  //////////////////////////////////////////////////////////////////////////////

  /* ----------------------- Split and Scatterv orginial_frame  ------------- */
  /* New frame size */
  int subframe_height = original_frame.height / world_size;
  int missing_pixels = original_frame.height % world_size;
  int* sendcounts = NULL; /* How many pixels will be sent to each process */
  int* displs = NULL; /* Starting address for each piece of the matrix */
  /* Frame where data will be received */
  BMP my_original_frame;

  if( subframe_height < 16 )
    {
      fprintf(stderr, "Select a shorter number of processes, each process"\
	      " must process minimum a matrix of size 16 x <FrameWidth>\n");
      exit(EXIT_FAILURE);
    }

  if(world_rank == 0)
    {
      sendcounts = malloc(world_size * sizeof(int));
      displs = malloc(world_size * sizeof(int));
      /* Process 0: processes the same size as others plus the missing pixels */
      sendcounts[0] = (subframe_height + missing_pixels) * original_frame.width;
      displs[0] = 0;
      printf("Sendcounts: \n");
      printf("%d ", sendcounts[0]);
      int sum = sendcounts[0];
      for(int i = 1; i < world_size; ++i)
	{
	  sendcounts[i] = subframe_height * original_frame.width;
	  displs[i] = sum;
	  sum += sendcounts[i];
	  //sum += sendcounts[i] + 1;
	  printf("%d ", sendcounts[0]);
	}
      printf("\n");
    }

  /* Pixel matrix sizes */
  my_original_frame.width = original_frame.width;
  if(world_rank == 0)
    my_original_frame.height = subframe_height + missing_pixels;
  else
    my_original_frame.height = subframe_height;

  my_original_frame.pixels =
    (unsigned char *) malloc(my_original_frame.height *
			     my_original_frame.width *
			     sizeof(unsigned char));

  MPI_Scatterv((void *) original_frame.pixels, sendcounts, displs,
	       MPI_UNSIGNED_CHAR, my_original_frame.pixels,
	       my_original_frame.height * my_original_frame.width,
	       MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

  printf("MPI scatterv correctly, from process %d\n", world_rank);

  //////////////////////////////////////////////////////////////////////////////

  /* ----------------------- Broadcast the compressed_frame ------------------ */

  /* MPI_Bcast(next_frame.pixels, */
  /* 	    next_frame.height * next_frame.width, MPI_BYTE, 0, */
  /* 	    MPI_COMM_WORLD); */

  //////////////////////////////////////////////////////////////////////////////

  /* ----------------------- Execute the algorithm -------------------------- */
  MotionVector compressed_frame = calc_motion_vector(my_original_frame,
  						     next_frame);

  printf("Calculation correctly, from process %d\n", world_rank);

  //////////////////////////////////////////////////////////////////////////////

  /* ----------------------- Gather results --------------------------------- */

  /* Position datatype */
  MPI_Datatype MPI_POS;
  MPI_Type_contiguous(2, MPI_INT, &MPI_POS);
  MPI_Type_commit(&MPI_POS);

  int my_size = compressed_frame.rows * compressed_frame.cols;
  Position my_result[my_size];

  /* Convert current matrix position to plain array */
  for(int i = 0; i < compressed_frame.rows; ++i)
    for(int j = 0; j < compressed_frame.cols; ++j)
      my_result[(i*compressed_frame.rows) + j] =
	compressed_frame.macro_blocks[i][j];

  Position* recvbuff = NULL;
  int* recvcounts = NULL;

  if(world_rank == 0)
    {
      int receive_buffer_size = (original_frame.height / 16) *
	(original_frame.width / 16);

      recvbuff = malloc(receive_buffer_size * sizeof(Position));
      recvcounts = malloc(world_size * sizeof(int));

      recvcounts[0] = my_size;
      displs[0] = 0; /* Recycle of first displs */

      printf("Recvcounts\n");
      printf("%d ", recvcounts[0]);
      int sum = recvcounts[0];
      for(int i = 1; i < world_size; ++i)
	{
	  /* Slave processes height and width */
	  int process_height = (my_original_frame.height - missing_pixels) / 16;
	  int process_width = my_original_frame.width / 16;
	  recvcounts[i] = process_height * process_width;
	  printf("%d ", recvcounts[i]);
	  /* Displacements */
	  displs[i] = sum;
	  sum += recvcounts[i];
	  //sum += recvcounts[i] + 1;
	}
      printf("\n");
    }

  MPI_Gatherv(my_result, my_size, MPI_POS,
	      recvbuff, recvcounts, displs, MPI_POS,
	      0, MPI_COMM_WORLD);

  //PROBLEM WHEN GATHERING AND PRINTING RESULTS

  //////////////////////////////////////////////////////////////////////////////

  /* Print result */
  print_positions(recvbuff,
		  original_frame.height / 16,
		  original_frame.width / 16);

  /* Free allocated space in function read_bmp() */
  if(world_rank == 0)
    {
      free(original_frame.pixels);
      free(next_frame.pixels);
    }
  free(my_original_frame.pixels);

  /* Free allocated space during scatter and gather */
  if(world_rank == 0)
    {
      free(sendcounts);
      free(displs);
      free(recvbuff);
      free(recvcounts);
    }
  free(my_original_frame.pixels);

  MPI_Finalize();

  return EXIT_SUCCESS;
}
