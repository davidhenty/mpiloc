#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <mpi.h>

void printlocation(char *prog);
void cpuset_to_cstr(cpu_set_t *mask, char *str);

int main(int argc, char *argv[])
{
  char *prg, *cmd;

  MPI_Init(&argc, &argv);

  cmd = argv[0];
  prg = cmd + strlen(cmd) - 1;

  while(prg > cmd && *(prg-1) != '/') prg--;

  printlocation(prg);

  MPI_Finalize();
}


void printlocation(char *prog)
{
    int rank, namelen;
    char nodename[MPI_MAX_PROCESSOR_NAME];
    cpu_set_t coremask;
    char clbuf[7 * CPU_SETSIZE];

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    
    MPI_Get_processor_name(nodename, &namelen);

    sched_getaffinity(0, sizeof(coremask), &coremask);
    cpuset_to_cstr(&coremask, clbuf);

    printf("Program <%s>: rank %d on core %s of node <%s>\n", prog, rank, clbuf, nodename);
}

/* Borrowed from util-linux-2.13-pre7/schedutils/taskset.c */
void cpuset_to_cstr(cpu_set_t *mask, char *str)
{
    char *ptr = str;
    int i, j, entry_made = 0;

    for (i = 0; i < CPU_SETSIZE; i++)
      {
        if (CPU_ISSET(i, mask))
          {
            int run = 0;
            entry_made = 1;

            for (j = i + 1; j < CPU_SETSIZE; j++)
              {
                if (CPU_ISSET(j, mask)) run++;
                else break;
              }

            if (!run)
              {
                sprintf(ptr, "%d,", i);
              }
            else if (run == 1)
              {
                sprintf(ptr, "%d,%d,", i, i + 1);
                i++;
              }
            else
              {
                sprintf(ptr, "%d-%d,", i, i + run);
                i += run;
              }
            while (*ptr != 0) ptr++;
          }
      }

    ptr -= entry_made;
    *ptr = 0;
}
