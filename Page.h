//
// Created by Karen Ren on 2022/4/19.
//

#ifndef PAGING_PAGE_H
#define PAGING_PAGE_H

class Page {
public:
    Page(int page, bool dirty, bool read, bool write, int frequency);

    int page;

    bool dirty;
    bool read;
    bool write;

    bool read_copy;
    bool write_copy;

    int frequency;


};


#endif //PAGING_PAGE_H
