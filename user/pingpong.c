#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p); // p[0]读，p[1]写
  char buf[1];
  if (fork() == 0) {
    read(p[0], buf, 1);
    close(p[0]);
    printf("%d: received ping\n", getpid());
    write(p[1], "b", 1);
    close(p[1]);
    exit(0);
  } else {
    write(p[1], "a", 1);
    close(p[1]);
    wait(0); // 必须在write和read之间wait，否则可能父进程自写自读
    read(p[0], buf, 1);
    close(p[0]);
    printf("%d: received pong\n", getpid());
  }

  exit(0);
}
