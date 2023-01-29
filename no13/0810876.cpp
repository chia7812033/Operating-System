#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;
int main(int argc, char** argv)
{
    istringstream iss;
    iss.str(argv[1]);
    int n;
    iss >> n;
    if (n <= 0)
    {
        printf("Enter a non-negative number\n");
        return 0;
    }
    int a = 0, b = 1, c;
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0)
    { 
        while (n > 0)
        {
            c = a + b;
            printf("%d ", a);
            a = b;
            b = c;
            n--;
        }
    }
    else
    { 
        wait(NULL);
        printf("\n");
    }
    return 0;
}