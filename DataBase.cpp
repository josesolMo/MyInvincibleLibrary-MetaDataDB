//
// Created by jose on 05/06/19.
//

#include "DataBase.h"


DataBase::DataBase() {}

void DataBase::addGalery(string _galeryName){
    bool galeryexist=0;
    for (Galery *g:galeries){
        if (g->getName()==_galeryName){
            cout<<"Error: The gallery "<<_galeryName<<" already exists"<<endl;
            galeryexist=1;
            break;
        }
    }
    if (not galeryexist){
        Galery *galery = new Galery(_galeryName);
        galeries.push_back(galery);
        cout<<"The gallery has created successfully!"<<endl;
    }
}

void DataBase::addMetadata(string _galeryName, string _imgId, string _imgName,string _autor, string _year,
                           string _size, string _description) {
    bool galeryexist=0;
    bool imagexist=0;
    for (Galery *g:galeries){
        if (g->getName()==_galeryName){
            galeryexist=1;
            cout<<g->getSize()<<endl;
            for(int i=0; i<g->getSize();i++) {
                cout<<g->recorrer(i)->getId()<<endl;
                if (g->recorrer(i)->getId()==_imgId) {
                    imagexist=1;
                    break;
                }
            }
            if (not imagexist) {
                imagen *img = new imagen(_imgId);
                img->setMetadata(_imgName, _autor, _year, _size, _description);
                g->insertFirst(img);
                cout<<"Metadata has been added successfully!"<<endl;
            }
            break;
        }
    }
    if (not galeryexist){
        cout<<"Error: The gallery "<<_galeryName<<" doesn't exist"<<endl;
    }
    if (galeryexist && imagexist){
        cout<<"Error: The image "<<_imgId<<" already exists"<<endl;
    }
}