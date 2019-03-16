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

  //////////////////////////////////////////////////////////////////////////////
  /* Initialize MPI environment */
  MPI_Init(NULL, NULL);

  int mpi_status;
  
  int world_size; /* Number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int world_rank; /* MPI Process ID */
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  /* int level; */
  /* MPI_Query_thread(&level); */
  /* printf("AAA: %d\n", level); */
  /* MPI_Finalize(); */
  /* return 0; */

  BMP original_frame, next_frame;
  if(world_rank == 0)
    {
      original_frame = read_bmp(argv[1]);
      next_frame = read_bmp(argv[2]);
    }

  rank = world_rank;
  printf("MPI initialized correctly, from process %d\n", world_rank);

  //////////////////////////////////////////////////////////////////////////////

  /* ----------------------- Split and Scatterv orginial_frame  ------------- */
  /* Send frame sizes to slave processes
   * frame_dimensions[0] -> frame height
   * frame_dimensions[1] -> frame width
   */
  int frame_dimensions[2];
  int height_macro_blocks, macro_blocks_node;
  
  if(world_rank == 0)
    {

      height_macro_blocks = original_frame.height / 16;
      macro_blocks_node = height_macro_blocks / world_size;
      
      frame_dimensions[0] = macro_blocks_node * 16;
      frame_dimensions[1] = original_frame.width;

      if( frame_dimensions[0] < 16 )
	{
	  fprintf(stderr, "Select a shorter number of processes, each process" \
		  " must process minimum a matrix of size 16 x <FrameWidth>\n");
	  MPI_Finalize();
	  exit(EXIT_FAILURE);
	}
    }

  mpi_status = MPI_Bcast(frame_dimensions, 2, MPI_INT,
			 0, MPI_COMM_WORLD);

  if(mpi_status != 0)
    {
      fprintf(stderr,
	      "Process: %d. MPI Failed in Bcast with status code %d\n",
	      mpi_status);
      MPI_Finalize();
      exit(EXIT_FAILURE);
    }
  
  /* New frame size */
  int missing_pixels, missing_macro_blocks;
  int* sendcounts = NULL; /* How many pixels will be sent to each process */
  int* displs = NULL; /* Starting address for each piece of the matrix */
  /* Frame where data will be received */
  BMP my_original_frame;

  if(world_rank == 0)
    {

      missing_macro_blocks = height_macro_blocks % macro_blocks_node;
      missing_pixels = missing_macro_blocks * 16;

      sendcounts = (int *) malloc(world_size * sizeof(int));
      displs = (int *) malloc(world_size * sizeof(int));
      /* Process 0: processes the same size as others plus the missing pixels */
      sendcounts[0] = (frame_dimensions[0] + missing_pixels) *
	original_frame.width;
      displs[0] = 0;
      printf("Sendcounts: \n");
      printf("%d ", sendcounts[0]);
      int sum = sendcounts[0];
      for(int i = 1; i < world_size; ++i)
	{
	  sendcounts[i] = frame_dimensions[0] * frame_dimensions[1];
	  displs[i] = sum;
	  sum += sendcounts[i];
	  //sum += sendcounts[i] + 1;
	  printf("%d ", sendcounts[i]);
	}
      printf("\n");

      printf("Check form root: %x %x %x\n", original_frame.pixels[0],
	     original_frame.pixels[displs[1]], original_frame.pixels[displs[2]]);
    }

  /* Pixel matrix sizes */
  my_original_frame.width = frame_dimensions[1];
  if(world_rank == 0)
    my_original_frame.height = frame_dimensions[0] + missing_pixels;
  else
    my_original_frame.height = frame_dimensions[0];
    

  int my_frame_size = my_original_frame.height * my_original_frame.width;
  my_original_frame.pixels =
    (unsigned char *) malloc(my_frame_size *
			     sizeof(unsigned char));

  mpi_status =
    MPI_Scatterv(original_frame.pixels, sendcounts, displs, MPI_UNSIGNED_CHAR,
		 my_original_frame.pixels, my_frame_size, MPI_UNSIGNED_CHAR,
		 0, MPI_COMM_WORLD);

  if(mpi_status != 0)
    {
      fprintf(stderr,
	      "Process: %d. MPI Failed in Scatterv with status code %d\n",
	      world_rank, mpi_status);
      MPI_Finalize();
      exit(EXIT_FAILURE);
    }
  
  printf("MPI scatterv correctly, from process %d\n", world_rank);
  printf("%d My original frame pix %x\n", world_rank, my_original_frame.pixels[0]);

  //////////////////////////////////////////////////////////////////////////////

  /* ----------------------- Broadcast the compressed_frame ------------------ */

  /* Send frame sizes to slave processes
   * next_frame_dimensions[0] -> next frame height
   * next_frame_dimensions[1] -> next frame width
   */
  int next_frame_dimensions[2];
  BMP my_next_frame;

  if(world_rank == 0)
    {
      next_frame_dimensions[0] = next_frame.height;
      next_frame_dimensions[1] = next_frame.width;
    }

  mpi_status = MPI_Bcast(next_frame_dimensions, 2, MPI_INT,
			 0, MPI_COMM_WORLD);
  
  if(mpi_status != 0)
    {
      fprintf(stderr,
	      "Process: %d. MPI Failed in Bcast with status code %d\n",
	      mpi_status);
      MPI_Finalize();
      exit(EXIT_FAILURE);
    }
  
  if(world_rank == 0)
    {
      my_next_frame.height = next_frame.height;
      my_next_frame.width = next_frame.width;
      my_next_frame.pixels = next_frame.pixels;
    }
  else
    {
      my_next_frame.height = next_frame_dimensions[0];
      my_next_frame.width = next_frame_dimensions[1];
      my_next_frame.pixels =
	(unsigned char *) malloc(my_next_frame.height *
				 my_next_frame.width *
				 sizeof(unsigned char));
    }

  mpi_status =
    MPI_Bcast(my_next_frame.pixels,
	      my_next_frame.height * my_next_frame.width, MPI_UNSIGNED_CHAR,
	      0, MPI_COMM_WORLD);
  
  if(mpi_status != 0)
    {
      fprintf(stderr,
	      "Process: %d. MPI Failed in Bcast with status code %d\n",
	      mpi_status);
      MPI_Finalize();
      exit(EXIT_FAILURE);
    }

  //////////////////////////////////////////////////////////////////////////////

  /* ----------------------- Execute the algorithm -------------------------- */
  MotionVector compressed_frame = calc_motion_vector(my_original_frame,
  						     my_next_frame);

  printf("Calculation correctly, from process %d\n", world_rank);

  //////////////////////////////////////////////////////////////////////////////

  /* ----------------------- Gather results --------------------------------- */

  /* Position datatype */
  /* int block_lengths[2] = {1, 1}; */
  /* int displacements[2]; */
  /* MPI_Datatype types[2] = {MPI_INT, MPI_INT}; */
  /* MPI_Datatype MPI_POS; */

  /* displacements[0] = offsetof(Position, x); */
  /* displacements[1] = offsetof(Position, y); */

  /* mpi_status = MPI_Type_create_struct(2, block_lengths, displacements, */
  /* 				      types, &MPI_POS); */

  /* if(mpi_status != 0) */
  /*   { */
  /*     fprintf(stderr, */
  /* 	      "Process: %d. MPI Failed creating type with status code %d\n", */
  /* 	      mpi_status); */
  /*     MPI_Finalize(); */
  /*     exit(EXIT_FAILURE); */
  /*   } */
  
  /* mpi_status = MPI_Type_commit(&MPI_POS); */
  
  /* if(mpi_status != 0) */
  /*   { */
  /*     fprintf(stderr, */
  /* 	      "Process: %d. MPI Failed committing type with status code %d\n", */
  /* 	      mpi_status); */
  /*     MPI_Finalize(); */
  /*     exit(EXIT_FAILURE); */
  /*   } */

  /* printf("Type created correctly\nAAAAAAAAAAAAAAAAAAAAA\n"); */
  
  MPI_Datatype MPI_POS;
  MPI_Type_contiguous(2, MPI_INT, &MPI_POS);
  MPI_Type_commit(&MPI_POS);

  int* sizes = NULL;
  int my_size = compressed_frame.rows * compressed_frame.cols;
  Position my_result[my_size];

  printf("%d my result size: %d\n", world_rank, my_size);

  if(world_rank == 0)
    {
      sizes = (int *) malloc(world_size * sizeof(int));
    }
  
  mpi_status = MPI_Gather(&my_size, 1, MPI_INT,
  			  sizes, 1, MPI_INT,
  			  0, MPI_COMM_WORLD);

  if(mpi_status != 0)
    {
      fprintf(stderr,
  	      "Process: %d. MPI Failed in Gather with status code %d\n",
  	      mpi_status);
      MPI_Finalize();
      exit(EXIT_FAILURE);
    }

  /* Convert current matrix position to plain array */
  for(int i = 0; i < compressed_frame.rows; ++i)
    for(int j = 0; j < compressed_frame.cols; ++j)
      
	my_result[(i*compressed_frame.cols) + j] =
	  compressed_frame.macro_blocks[i][j];

  Position* recvbuff = NULL;
  int* recvcounts = NULL;
  int receive_buffer_size;

  if(world_rank == 0)
    {
      receive_buffer_size = 0;

      for(int i = 0; i < world_size; ++i)
	receive_buffer_size += sizes[i];

      recvbuff = (Position *) malloc(receive_buffer_size * sizeof(Position));
      
      recvcounts = (int *) malloc(world_size * sizeof(int));

      recvcounts[0] = my_size;
      displs[0] = 0; /* Recycle of first displs */

      printf("Displs and Recvcount\n");
      printf("Dis: %d ", displs[0]);
      printf("cou: %d ", recvcounts[0]);
      int sum = recvcounts[0];
      for(int i = 1; i < world_size; ++i)
  	{
  	  recvcounts[i] = sizes[i];
  	  /* Displacements */
  	  displs[i] = sum;
	  printf("Dis: %d ", displs[i]);
  	  printf("cou: %d ", recvcounts[i]);
  	  sum += recvcounts[i];
  	  //sum += recvcounts[i] + 1;
  	}
      printf("\n");
    }

  
  mpi_status = MPI_Gatherv(my_result, my_size, MPI_POS,
			   recvbuff, recvcounts, displs, MPI_POS,
			   0, MPI_COMM_WORLD);

  if(mpi_status != 0)
    {
      fprintf(stderr,
	      "Process: %d. MPI Failed in Gatherv with status code %d\n",
	      mpi_status);
      MPI_Finalize();
      exit(EXIT_FAILURE);
    }

  //////////////////////////////////////////////////////////////////////////////

  if(world_rank == 0){
    for(int i = 0; i < receive_buffer_size; ++i){
      printf("(%d, %d)  ",
	     recvbuff[i].x,
	     recvbuff[i].y);
    }
  }
  

  /* Print result */
  if(world_rank == 0){
    //print_positions(recvbuff,
    //		    original_frame.height / 16,
    //		    original_frame.width / 16);
  }

  /* Free allocated space during scatter and gather */
  if(world_rank == 0)
    {
      /* Allocated in read_bmp() */
      free(original_frame.pixels);
      free(next_frame.pixels);
      /* Allocated during scatter and gather */
      free(sendcounts);
      free(displs);
      free(recvbuff);
      free(recvcounts);
      free(sizes);
    }
  else
    free(my_original_frame.pixels);

  MPI_Finalize();

  return EXIT_SUCCESS;
}
