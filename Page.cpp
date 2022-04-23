//
// Created by Karen Ren on 2022/4/19.
//

#include "Page.h"

Page::Page(int page, bool dirty, bool read, bool write, int frequency){
    this->page = page;
    this->dirty = dirty;
    this->read = read;
    this->write = write;
    this->frequency = frequency;

    write_copy = this->write;
    read_copy = this->read;
}
