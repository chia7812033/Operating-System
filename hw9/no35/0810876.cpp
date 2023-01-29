#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

long long factorial(int n)
{
    long long result = 1;
    for (int i = 1; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

class Catalan
{
    long long seq[10];
    int size;

public:
    Catalan(){};

    void calculate(int size_in)
    {
        size = size_in;
        for (int i = 0; i < size; i++)
        {
            double n = i + 1;;
            seq[i] = factorial(2.0 * n) / (factorial(n + 1) * factorial(n));
        }
    }

    void print()
    {
        for (int i = 0; i < size; i++)
            cout << seq[i] << " ";
    }
};

int main(int argc, char **argv)
{
    string s = argv[1];
    int size = int(s[0]) - 48;

    if (size < 0)
    {
        cout << "Error" << endl;
        return -1;
    }

    auto shmid = shmget(IPC_PRIVATE, sizeof(class Catalan), 0666 | IPC_CREAT);

    auto pid = fork();

    if (pid == 0)
    {
        Catalan *C = (Catalan *)shmat(shmid, NULL, 0);
        C->calculate(size);

        exit(0);
    }
    else if (pid > 0)
    {
        wait(NULL);

        Catalan *C = (Catalan *)shmat(shmid, NULL, 0);
        C->print();
        cout << endl;
    }

    return 0;
}