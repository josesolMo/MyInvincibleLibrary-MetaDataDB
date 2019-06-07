//
// Created by jose on 06/06/19.
//

#include "Galery.h"

Galery::Galery(string _name) {
    name = _name;
}

GaleryNode* Galery::getPointerHead() {
    return pointerHead;
}

void Galery::setPointerHead(GaleryNode* head) {
    pointerHead = head;
}
void Galery::insertFirst(imagen* img) {
    GaleryNode* pointeraux = new GaleryNode(pointerHead, img);
    setPointerHead(pointeraux);
}

int Galery::getSize() {
    GaleryNode *current = getPointerHead();
    int c=0;
    while(current != nullptr){
        current = current->get_nxtPtr();
        c++;
    }
    return c;
}

imagen* Galery::recorrer(int indice){
    GaleryNode *current = getPointerHead();
    while(indice>0){
        current = current->get_nxtPtr();
        indice--;
    }
    return current->get_data();
}

imagen* Galery::sacar(int indice){
    imagen* res;
    GaleryNode *current = getPointerHead();
    if (indice==0){
        current = current->get_nxtPtr();
        res = pointerHead->get_data();
        pointerHead->set_nxtNd(nullptr);
        setPointerHead(current);
    }else if (indice == getSize()-1) {
        while(indice-1>0){
            current = current->get_nxtPtr();
            indice--;
        }
        res = current->get_data();
        current->set_nxtNd(nullptr);
    }else {
        while(indice-1>0){
            current = current->get_nxtPtr();
            indice--;
        }
        GaleryNode *temp = current->get_nxtPtr();
        current->set_nxtNd(temp->get_nxtPtr());
        res = temp->get_data();
        temp->set_nxtNd(nullptr);
    }
    return res;
}


GaleryNode* Galery::getLast() {
    GaleryNode *current = getPointerHead();
    while(current->get_nxtPtr() != nullptr){
        current = current->get_nxtPtr();
    }
    return current;
}

void Galery::insertLast(imagen* img) {
    GaleryNode* aux = new GaleryNode(nullptr, img);
    if(pointerHead==nullptr){
        setPointerHead(aux);
    }
    else{
        GaleryNode *current = getPointerHead();
        while(current->get_nxtPtr() != nullptr){
            current = current->get_nxtPtr();
        }
        current->set_nxtNd(aux);
    }
}

void Galery::vaciar(){
    //cout<<"El tamano de la lista antes es: "<<getSize()<<endl;
    int n = getSize();
    while(n>0) {
        sacar(n-1);
        n-=1;
    }
    //cout<<"El tamano de la lista es: "<<getSize()<<endl;
}

string Galery::getName() {
    return name;
}