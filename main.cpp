//
// Created by Karen Ren on 2022/4/19.
//

#include <iostream>
#include "PageTable.h"
using namespace std;

int main (int argc, char* argv[])
{

    cout << "Number of memory access: " << endl;

    string line;
    string algo = argv[2];
    int frameNum = stoi(argv[1]);

    cout << argc << endl;

    int memAccessNum = 0;


    PageTable pageTable(algo, frameNum, false);

    if(argc == 4){
        pageTable.setVerbose(true);
    }

    while (getline (cin, line) && !line.empty())
    {
        //cout << line << endl; // std::getline skips the newline

        string firstPart = line.substr(0,line.find(" "));
        firstPart = firstPart.substr(2,firstPart.size()-4);
        if(firstPart.size() < 5){
            firstPart = "0" + firstPart;
        }
        int pageNum = stoi(firstPart);

        char access = line.at(line.size()-1);

        //cout << pageNum << endl;
        //cout << access << endl;

        pageTable.insert(pageNum,access);
        memAccessNum++;

    }

    cout << "Number of memory access: " << memAccessNum << endl;
    cout << "Number of misses: " << pageTable.getMisses() << endl;
    cout << "Number of writes: " << pageTable.getWriteCount() << endl;
    cout << "Number of drops: " << pageTable.getDropCount() << endl;

    return 0;
}