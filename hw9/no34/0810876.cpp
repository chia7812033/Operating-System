#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

class PageFrameTable
{
protected:
    vector<int> table;
    int endIndex = 0;
    int pageFaults = 0;

public:
    PageFrameTable(int size)
    {
        table = vector<int>(size, -1);
    }

    int getPageFaults()
    {
        return pageFaults;
    }
};

class FIFO : public PageFrameTable
{
    int fifoPtr = 0;
public:
    FIFO(int size) : PageFrameTable(size) {}

    void start(const vector<int> inputString)
    {
        for (auto page : inputString)
        {
            auto index = find(table.begin(), table.end(), page);
            if (index == table.end())
            {
                pageFaults++;
                if (endIndex >= table.size())
                {
                    table[selectIndex()] = page;
                }
                else 
                {
                    table[endIndex] = page;
                    endIndex++;
                }

                fifoPtr++;
                fifoPtr %= table.size();
            }
        }
    }

    int selectIndex()
    {
        return fifoPtr;
    }
};

class LRU : public PageFrameTable
{
    int counter = 0;
    vector<int> changeCounter;

public:
    LRU(int size) : PageFrameTable(size)
    {
        changeCounter = vector<int>(size, 0);
    }

    int selectIndex()
    {
        return min_element(changeCounter.begin(), changeCounter.end()) - changeCounter.begin();
    }

    void start(const vector<int> inputString)
    {
        for (auto page : inputString)
        {
            int index;
            auto indexPtr = find(table.begin(), table.end(), page);
            if (indexPtr == table.end())
            {
                pageFaults++;
                if (endIndex >= table.size())
                {
                    index = selectIndex();
                    table[index] = page;
                }
                else 
                {
                    index = endIndex;
                    table[index] = page;
                    endIndex++;
                }
            }
            else
            {
                index = indexPtr - table.begin();
            }
            changeCounter[index] = counter;
            counter++;
        }
    }
};

int main(int argc, char **argv)
{
    int size = atoi(argv[1]);

    vector<int> inputString;
    string s = argv[2];
    for (int i = 0; i < 12; i++)
    {
        int n = int(s[i]) - 48;
        inputString.push_back(n);
    }

    FIFO FIFO_Table(size);
    LRU LRU_Table(size);

    FIFO_Table.start(inputString);
    LRU_Table.start(inputString);

    cout << "FIFO: " << FIFO_Table.getPageFaults() << " page faults" << endl;
    cout << "LRU: " << LRU_Table.getPageFaults() << " page faults" << endl;
}