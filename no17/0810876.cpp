#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
using namespace std;

# define MAX_SEQUENCE 10
typedef struct{
long fib_sequence[MAX_SEQUENCE];
int sequence_size;
} shared_data;

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
    else if (n > MAX_SEQUENCE)
    {
        printf("Out of range, please enter a number <10\n");
        return 0;
    }
    int a = 0, b = 1, c;
    int segment_id = shmget(IPC_PRIVATE, n, S_IRUSR | S_IWUSR);
    shared_data *shared_memory = (shared_data *)shmat(segment_id, NULL, 0);
    shared_memory->sequence_size = n;
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
            shared_memory->fib_sequence[n - 1] = a;
            c = a + b;
            a = b;
            b = c;
            n--;
        }
    }
    else
    { 
        wait(NULL);
        for(int i = shared_memory->sequence_size-1; i >= 0; i--)
            printf("%ld ", shared_memory->fib_sequence[i]);
        printf("\n");
    }
    return 0;
}