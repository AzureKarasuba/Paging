//
// Created by karas on 2022/4/19.
//

#include "PageTable.h"

void PageTable::LFUinsert(int pageNum, char access) {
    Page* page = new Page(pageNum, false,true,false,1); // default: read
    if(access == 'W'){
        page->dirty = true;
        page->write = true;
    }

    if(!check(pageNum)){ //page not found
        missCount++;

        if(count < size){ //free frame exists
            pageTable.insert(make_pair(pageNum,*page)); // load the page into frame
            count++;
        }else{ // no free frame
            unordered_map<int, Page>::iterator iterator;
            Page victim = pageTable.begin()->second;
            int leastFrequency = victim.frequency;

            for(iterator = pageTable.begin(); iterator != pageTable.end(); iterator++){ //loop to find lfu
                if(iterator->second.frequency < leastFrequency){
                    victim = iterator->second;
                    leastFrequency = victim.frequency;
                }//if
            }//end for

            int victimPageNum = victim.page;

            if(victim.dirty){ // page has been modified
                writeCount++;
                if(verbose){
                    printVerbose(victimPageNum, pageNum, "swaps");
                }
            }else{
                dropCount++;
                if(verbose) {
                    printVerbose(victimPageNum, pageNum, "overwrites");
                }
            }


            pageTable.erase(victimPageNum); //remove old page
            pageTable.insert(make_pair(pageNum,*page));// load the new page into frame

        }
    }else{//page already exists
        Page p = pageTable.at(pageNum);
        p.frequency++; //increment frequency

        if(access == 'W'){
            p.write = true;
            p.dirty = true;
        }
    }
}

void PageTable::MFUinsert(int pageNum, char access) {
    Page *page = new Page(pageNum, false,true,false,1); // default: read
    if(access == 'W'){
        page->dirty = true;
        page->write = true;
    }

    if(!check(pageNum)){ //page not found
        missCount++;

        if(count < size){ //free frame exists
            pageTable.insert(make_pair(pageNum,*page)); // load the page into frame
            count++;
        }else{ // no free frame
            unordered_map<int, Page>::iterator iterator;
            Page victim = pageTable.begin()->second;
            int mostFrequency = victim.frequency;

            for(iterator = pageTable.begin(); iterator != pageTable.end(); iterator++){ //loop to find mfu
                if(iterator->second.frequency > mostFrequency){
                    victim = iterator->second;
                    mostFrequency = victim.frequency;
                }//if
            }//end for

            int victimPageNum = victim.page;

            if(victim.dirty){ // page has been modified
                writeCount++;
                if(verbose) {
                    printVerbose(victimPageNum, pageNum, "swaps");
                }
            }else{
                dropCount++;
                if(verbose) {
                    printVerbose(victimPageNum, pageNum, "overwrites");
                }
            }

            pageTable.erase(victimPageNum); //remove old page
            pageTable.insert(make_pair(pageNum,*page));// load the new page into frame

        }
    }else{//page already exists
        Page p = pageTable.at(pageNum);
        p.frequency++; //increment frequency

        if(access == 'W'){
            p.write = true;
            p.dirty = true;
        }
    }
}//MFU insert

void PageTable::LRUinsert(int pageNum, char access) {
    Page *page = new Page(pageNum, false,true,false,1); // default: read
    if(access == 'W'){
        page->dirty = true;
        page->write = true;
    }

    list<Page>::iterator iter = checkInList(pageNum); // looks for the page in list
    if(iter == recentUsedList.end()){ //page not found
        missCount++;

        if(count < size){ //free frame exists
            recentUsedList.push_back(*page); // put the page into the recent used list
            count++;
        }else{ // no free frame
            Page victim = recentUsedList.front();

            if(victim.dirty){ // page has been modified
                writeCount++;
                if(verbose) {
                    printVerbose(victim.page, pageNum, "swaps");
                }
            }else{
                dropCount++;
                if(verbose) {
                    printVerbose(victim.page, pageNum, "overwrites");
                }
            }
            recentUsedList.pop_front(); //delete least recently used element
            recentUsedList.push_back(*page);
        }
    }else{//page already exists
        if(access == 'W'){
            iter->write = true;
            iter->dirty = true;
        }
        recentUsedList.splice(recentUsedList.end(),recentUsedList,iter); // move the Page to the end of list

    }


}//LRU

void PageTable::GlobalClockInsert(int pageNum, char access) {
    Page *page = new Page(pageNum, false,true,false,1); // default: read
    if(access == 'W'){
        page->dirty = true;
        page->write = true;

        page->write_copy = true;
    }

    if(!check(pageNum)){ // page not found in frame
        missCount++;

        if(count < size){ // free frame exists
            pageTable.insert(make_pair(pageNum,*page)); // load the page into frame
            count++;
        }else{ //no free frame; need to loop through all elements
            auto iterator = pageTable.begin();
            bool found = false;
            while(!found){
                for(iterator = pageTable.begin(); iterator!= pageTable.end(); iterator++){
                    if(!iterator->second.read_copy && !iterator->second.write_copy){//able to be swapped
                        found = true;
                        break;
                    }else if(iterator->second.write_copy){
                        iterator->second.write_copy = false;
                    }else if(iterator->second.read_copy){
                        iterator->second.read_copy = false;
                    }
                }//end for
            }//end while
            Page victim = iterator->second;
            if(victim.dirty){ // page has been modified
                writeCount++;
                if(verbose) {
                    printVerbose(victim.page, pageNum, "swaps");
                }
            }else{
                dropCount++;
                if(verbose) {
                    printVerbose(victim.page, pageNum, "overwrites");
                }
            }
            pageTable.erase(iterator->first); // remove victim
            pageTable.insert(make_pair(pageNum,*page));

        }
    }else{ //page already exists; check if read/write changes
        if(access == 'W'){
            Page current = pageTable.at(pageNum);
            current.write = true;
            current.dirty = true;
        }
    }
}//GlobalClockInsert

list<Page>::iterator PageTable::checkInList(int pageNum) {
    list<Page>::iterator iter;
    for(iter = recentUsedList.begin(); iter != recentUsedList.end(); iter++){
        if(iter->page == pageNum){
            return iter;
        }
    }
    return recentUsedList.end(); //return an invalid page
}

bool PageTable::check(int pageNum) {
    if(pageTable.find(pageNum) == pageTable.end()){
        return false;
    }
    return true;
}

void PageTable::insert(int pageNum, char access) {
    if(algo == "FIFO"){
        FIFOinsert(pageNum,access);
    }else if(algo == "LRU"){
        LRUinsert(pageNum,access);
    }else if(algo == "LFU"){
        LFUinsert(pageNum,access);
    }else if(algo == "MFU"){
        MFUinsert(pageNum,access);
    }else if(algo == "CLOCK"){
        GlobalClockInsert(pageNum,access);
    }
}

void PageTable::FIFOinsert(int pageNum, char access) {
    Page *page = new Page(pageNum, false,true,false,1); // default: read
    if(access == 'W'){
        page->dirty = true;
        page->write = true;
    }

    if(!check(pageNum)){ // page not found in frame
        missCount++;

        if(count < size){ // free frame exists
            pageTable.insert(make_pair(pageNum,*page)); // load the page into frame
            q.push(pageNum); //push the pageNum into the queue

            count++;
        }else{ //no free frame; need to pop the first element from queue
            int firstInQueue = q.front();
            q.pop(); // delete first element

            Page victim = pageTable.at(firstInQueue); // the victim to be kicked out
            if(victim.dirty){ // page has been modified
                writeCount++;

                if(verbose) {
                    printVerbose(firstInQueue, pageNum, "swaps");
                }
            }else{
                dropCount++;
                if(verbose) {
                    printVerbose(firstInQueue, pageNum, "overwrites");
                }
            }

            pageTable.erase(firstInQueue); //remove victim from frame

            pageTable.insert(make_pair(pageNum,*page)); // load the page into frame
            q.push(pageNum); //push the pageNum into the queue
        }
    }else{ //page already exists; check if read/write changes
        if(access == 'W'){
            Page *current = &pageTable.at(pageNum);
            current->write = true;
            current->dirty = true;
        }
    }
}


//check In List
PageTable::PageTable(string algo, int size, bool verbose) {
    this->algo = algo;

    count = 0;
    this->size = size;
    this->verbose = verbose;

    missCount = 0;
    dropCount=0;
    writeCount = 0;

}

void PageTable::printVerbose(int oldPage, int newPage, string type) {
    cout << newPage << " " << type << " " << oldPage << endl;
}

void PageTable::setVerbose(bool verbose) {
    this->verbose = verbose;
}

int PageTable::getDropCount() {
    return dropCount;
}

int PageTable::getWriteCount() {
    return writeCount;
}

int PageTable::getMisses() {
    return missCount;
}
