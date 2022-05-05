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

    if(argc == 4){
        pageTable.setVerbose(true);
    }

    while (getline (cin, line) && !line.empty())
    {

        string firstPart = line.substr(0,line.find(" "));
        firstPart = firstPart.substr(2,firstPart.size()-5);
        if(firstPart.size() < 5){
            firstPart = "0" + firstPart;
        }
        int pageNum = stoi(firstPart,nullptr,16);

        char access = line.at(line.length()-1);

        //cout << pageNum << endl;
        //cout << access << endl;

        pageTable.insert(pageNum,access);
        memAccessNum++;

    }

    cout << "Number of memory access: " << std::dec << memAccessNum << endl;
    cout << "Number of obligatory misses: " << std::dec << pageTable.getObligatoryMiss() << endl;
    cout << "Number of misses: " << std::dec << pageTable.getMisses() << endl;
    cout << "Number of writes: " << std::dec << pageTable.getWriteCount() << endl;
    cout << "Number of drops: " << std::dec << pageTable.getDropCount() << endl;

    return 0;
}