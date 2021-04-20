#include <stdio.h>
#include <string.h>
#include <mpi.h>

#include "utilities.h"

int main(int argc, char *argv[])
{
  char *prg, *cmd;

  MPI_Init(&argc, &argv);

  cmd  =argv[0];
  prg = cmd + strlen(cmd) - 1;

  while(prg > cmd && *(prg-1) != '/') prg--;

  printlocation(prg);

  MPI_Finalize();
}
