#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define CHILD_STACK_SIZE (1024 * 1024)

static int child_func(void *arg)
{
    printf("UID inside the namespace is %ld\n", (long)geteuid());
    printf("GID inside the namespace is %ld\n", (long)getegid());
    return 0;
}

static char child_stack[CHILD_STACK_SIZE];

int main(int argc, char *argv[])
{
    pid_t child_pid;
    child_pid = clone(
            &child_func,
            child_stack + CHILD_STACK_SIZE,
            CLONE_NEWUSER,
            0
    );
    if (child_pid < 0) {
        printf("clone failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("UID outside the namespace is %ld\n", (long)geteuid());
    printf("GID outside the namespace is %ld\n", (long)getegid());
    waitpid(child_pid, NULL, 0);
    exit(EXIT_SUCCESS);
}
