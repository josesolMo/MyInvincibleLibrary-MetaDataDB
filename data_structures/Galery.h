//
// Created by jose on 06/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_GALERY_H
#define MYINVINCIBLELIBRARY_METADATADB_GALERY_H

#include <iostream>
#include "GaleryNode.h"
#include "imagen.h"

using namespace std;

class Galery {

public:

    Galery();
    Galery(string _name);

    GaleryNode* getPointerHead();
    void setPointerHead(GaleryNode* head);
    void insertFirst(imagen *img);
    int getSize();
    GaleryNode* getLast();
    void insertLast(imagen *img);
    imagen* recorrer(int indice);
    imagen* sacar(int indice);
    void vaciar();
    string getName();

private:

    GaleryNode* pointerHead= nullptr;
    string name;

};


#endif //MYINVINCIBLELIBRARY_METADATADB_GALERY_H
