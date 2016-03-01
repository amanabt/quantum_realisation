// #include <mpi.h>
// 
// int main (int argc, char *argv[])
// {
// 	char message[20];
// 	int myrank;
// 	MPI_Status status;
// 	MPI_Init( &argc, &argv );
// 	MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
// 	std::string processor_name;
// 	std::int32_t len;
// 	MPI_Get_processor_name (const_cast<char*> (processor_name.c_str()),
// 							&len);
// 	std::cerr << MPI_Wtick() << " : "
// 			  << processor_name << " : " << len << std::endl;
// 	if (myrank == 0) /* code for process zero */
// 	{
// 		strcpy(message,"Hello, there");
// 		MPI_Send(message, strlen(message)+1, MPI_CHAR, 1, 99, MPI_COMM_WORLD);
// 	}
// 	else if (myrank == 1) /* code for process one */
// 	{
// 		MPI_Recv(message, 20, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
// 		printf("received :%s:\n", message);
// 	}
// 	MPI_Finalize();
// 	return 0;
// }

   #include "mpi.h"
   #include <stdio.h>

   int main(int argc, char *argv[]) {
   int  numtasks, rank, len, rc; 
   char hostname[MPI_MAX_PROCESSOR_NAME];

   rc = MPI_Init(&argc,&argv);
   if (rc != MPI_SUCCESS) {
     printf ("Error starting MPI program. Terminating.\n");
     MPI_Abort(MPI_COMM_WORLD, rc);
     }

   MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Get_processor_name(hostname, &len);
   printf ("Number of tasks= %d My rank= %d Running on %s\n", numtasks,rank,hostname);

   /*******  do some work *******/

   MPI_Finalize();
   }