//
// Created by jose on 05/06/19.
//

#include "DBList.h"


DBNode* DBList::getPointerHead() {
    return pointerHead;
}

void DBList::setPointerHead(DBNode* head) {
    pointerHead = head;
}
void DBList::insertFirst(string _data) {
    DBNode* pointeraux = new DBNode(pointerHead, _data);
    setPointerHead(pointeraux);
}

int DBList::getSize() {
    DBNode *current = getPointerHead();
    int c=0;
    while(current != nullptr){
        current = current->get_nxtPtr();
        c++;
    }
    return c;
}

string DBList::recorrer(int indice){
    DBNode *current = getPointerHead();
    while(indice>0){
        current = current->get_nxtPtr();
        indice--;
    }
    return current->get_data();
}

string DBList::sacar(int indice){
    string res;
    DBNode *current = getPointerHead();
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
        DBNode *temp = current->get_nxtPtr();
        current->set_nxtNd(temp->get_nxtPtr());
        res = temp->get_data();
        temp->set_nxtNd(nullptr);
    }
    return res;
}


DBNode* DBList::getLast() {
    DBNode *current = getPointerHead();
    while(current->get_nxtPtr() != nullptr){
        current = current->get_nxtPtr();
    }
    return current;
}

void DBList::insertLast(string _data) {
    DBNode* aux = new DBNode(nullptr, _data);
    if(pointerHead==nullptr){
        setPointerHead(aux);
    }
    else{
        DBNode *current = getPointerHead();
        while(current->get_nxtPtr() != nullptr){
            current = current->get_nxtPtr();
        }
        current->set_nxtNd(aux);
    }
}

void DBList::swap(string *xp, string *yp)
{
    string temp = *xp;
    *xp = *yp;
    *yp = temp;
}
/*
void DBList::bubbleSort()
{
    int n = getSize();
    int i, j;
    for (i = 0; i < n-1; i++){
        for (j = 0; j < n-i-1; j++){
            if (recorrer(j)->getResistencia() < recorrer(j+1)->getResistencia()){
                swap(recorrer(j), recorrer(j+1));
            }
        }
    }
}
*/

void DBList::vaciar(){
    //cout<<"El tamano de la lista antes es: "<<getSize()<<endl;
    int n = getSize();
    while(n>0) {
        sacar(n-1);
        n-=1;
    }
    //cout<<"El tamano de la lista es: "<<getSize()<<endl;
}