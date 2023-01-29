#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
    const unsigned int pageSize = 4096;
    unsigned int address = strtoul(argv[1], NULL, 10);
    cout << address / pageSize << " " << address % pageSize << "\n";
}