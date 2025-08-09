#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int[]);

int
main(int argc, char* argv[])
{
  int pip0[2];
  pipe(pip0);

  if(fork() == 0){
    primes(pip0);
  } else {
    close(pip0[0]);
    for(int i = 2; i <= 280; ++i){
      write(pip0[1], (int*)&i, sizeof(int));
    }
    close(pip0[1]);
    wait((int*)0);
  }
  
  exit(0);
}

void
primes(int leftpip[])
{
  close(leftpip[1]);

  int number;
  int flag = read(leftpip[0], &number, sizeof(int));

  if(flag < 0){
    fprintf(2, "write error\n");
    close(leftpip[0]);
    exit(1);
  } else if(flag == 0) {
    close(leftpip[0]);
  } else {
    int p = number;
    fprintf(1, "prime %d\n", p);

    int rightpip[2];
    pipe(rightpip);

    if(fork() == 0){
      close(leftpip[0]);
      primes(rightpip);
    } else {
      close(rightpip[0]);
      for(;read(leftpip[0], &number, sizeof(int)) != 0;){
        int n = number;
        if(n % p != 0){
          write(rightpip[1], (int*)&n, sizeof(int));
        }
      }
      close(leftpip[0]);
      close(rightpip[1]);
      wait((int*)0);
    }
  }
}