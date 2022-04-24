//
// Created by karas on 2022/4/19.
//

#ifndef PAGING_PAGETABLE_H
#define PAGING_PAGETABLE_H

#include <unordered_map>
#include <string>
#include <queue>
#include <list>
#include <iostream>
#include "Page.h"

using namespace std;


class PageTable {
private:
    //tuple: modified bit, read, write
    unordered_map<int, Page> pageTable;
    queue<int> q;
    list<Page> recentUsedList;

    string algo;
    int count;
    int size;

    bool verbose;

    int missCount;
    int ObligatoryMiss;
    int writeCount;
    int dropCount;

public:

    PageTable(string algo, int size, bool verbose);

    int getMisses();

    int getWriteCount();

    int getDropCount();

    int getObligatoryMiss();

    void insert(int pageNum, char access);

    void FIFOinsert(int pageNum, char access);

    void LFUinsert(int pageNum, char access);

    void MFUinsert(int pageNum, char access);

    void LRUinsert(int pageNum, char access);

    void GlobalClockInsert(int pageNum, char access);

    bool check(int pageNum);

    list<Page>::iterator checkInList(int pageNum);

    void setVerbose(bool verbose);

    void printVerbose(int oldPage, int newPage, string type);

};


#endif //PAGING_PAGETABLE_H
