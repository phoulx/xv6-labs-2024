#include "kernel/types.h"
#include "user/user.h"

#define STOPAT 280

void handle(int p[2]);

int main(int argc, char* argv[])
{
    int p[2];
    pipe(p);
    // 必须fork，将写入操作放在父进程中，否则会在130左右失败
    if (fork() == 0) {
        close(p[1]);
        handle(p);
    } else {
        close(p[0]);
        for (int i = 2; i <= STOPAT; i++) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0);
    }
    return 0;
}

// 参数p为管道，且写端p[1]应已关闭
void handle(int p[2])
{
    int curr, num;
    if (read(p[0], &curr, sizeof(curr)) == 0) {
        close(p[0]);
        return;
    }
    printf("prime %d\n", curr);

    int newp[2];
    pipe(newp);
    if (fork() == 0) {
        close(p[0]);
        close(newp[1]);
        handle(newp);
    } else {
        close(newp[0]);
        while (read(p[0], &num, sizeof(num))) {
            if (num % curr != 0) {
                write(newp[1], &num, sizeof(num));
            }
        }
        close(newp[1]);
        close(p[0]);
        wait(0);
    }
}