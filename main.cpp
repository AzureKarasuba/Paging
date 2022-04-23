//
// Created by Karen Ren on 2022/4/19.
//

#include <iostream>
#include "PageTable.h"
using namespace std;

int main (int argc, char* argv[])
{

    string line;
    string algo = argv[2];
    int frameNum = stoi(argv[1]);

    int memAccessNum = 0;

    PageTable pageTable(algo, frameNum, false);
    if(argc == 4 && argv[3] == "verbose"){
        pageTable.setVerbose(true);
    }

    while (getline (cin, line) && !line.empty())
    {
        cout << line << endl; // std::getline skips the newline
        int pageNum = stoi(line.substr(0,line.find(" ")));
        char access = line.at(line.size()-1);

        pageTable.insert(pageNum,access);
        memAccessNum++;

    }

    cout << "Number of memory access: " << memAccessNum << endl;
    cout << "Number of misses: " << pageTable.getMisses() << endl;
    cout << "Number of writes: " << pageTable.getWriteCount() << endl;
    cout << "Number of drops: " << pageTable.getDropCount() << endl;

    return 0;
}