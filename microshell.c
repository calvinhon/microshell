#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void    execute(char *cmdPath, char **cmdArgs, char *env) {
    if (execve(cmdPath, cmdArgs, env) < 0)

}

int main(int ac, char **av) {
    int     i = 0;
    char    *cmdPath;
    char    **cmdArgs;

    findPath()
    execute();
    return 0;
}