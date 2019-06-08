//
// Created by jose on 05/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_DBLIST_H
#define MYINVINCIBLELIBRARY_METADATADB_DBLIST_H

#include "DBNode.h"
#include <iostream>

using namespace std;

class DBList {

public:

    DBNode* getPointerHead();
    void setPointerHead(DBNode* head);
    void insertFirst(string _data);
    int getSize();
    DBNode* getLast();
    void insertLast(string _data);
    void reinsert(int index, string _data);
    string recorrer(int indice);
    string sacar(int indice);
    void swap(string *xp, string *yp);
    void bubbleSort();
    void vaciar();

private:

    DBNode* pointerHead= nullptr;

};

#endif //MYINVINCIBLELIBRARY_METADATADB_DBLIST_H
