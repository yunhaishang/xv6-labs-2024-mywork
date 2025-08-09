#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  int p1[2];  //parent -> child
  pipe(p1);

  int p2[2];  //child -> parent
  pipe(p2);

  if(fork() == 0){
    close(p1[1]);
    close(p2[0]);

    char buffer[8];

    read(p1[0], buffer,8);
    close(p1[0]);
    fprintf(1, "%d: received ping\n", getpid());

    write(p2[1], buffer, 8);
    close(p2[1]);
  } else {
    close(p1[0]);
    close(p2[1]);

    char buffer[8];

    write(p1[1], "a  byte", 8);
    close(p1[1]);

    read(p2[0], buffer, 8);
    close(p2[0]);
    fprintf(1, "%d: received pong\n", getpid());
  }
  exit(0);
}