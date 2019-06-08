//
// Created by jose on 05/06/19.
//

#include "DataBase.h"


DataBase::DataBase() {}

/**
 * Metodo que permite agregar galerias
 * @param _galeryName
 */
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
/**
 * Metodo para agragar metadata a una imagen especifica en una galeria especifica
 * @param _galeryName
 * @param _imgId
 * @param _imgName
 * @param _autor
 * @param _year
 * @param _size
 * @param _description
 */
void DataBase::addMetadata(string _galeryName, string _imgId, string _imgName,string _autor, string _year,
                           string _size, string _description) {
    bool galeryexist=0;
    bool imagexist=0;
    for (Galery *g:galeries){
        if (g->getName()==_galeryName){
            galeryexist=1;
            cout<<g->getSize()<<endl;
            for(int i=0; i<g->getSize();i++) {
                cout<<"Hey"<<endl;
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
        cout<<"Error: The meta data for the image "<<_imgId<<" already exists"<<endl;
    }
}

/**
 * Metodo que permite consultar la metadata de una imagen especifica en una galeria especifica
 * @param _galeryName
 * @param _imgId
 * @return
 */
DBList DataBase::consultMetadata(string _galeryName, string _imgId) {
    for (Galery *g:galeries){
        if (g->getName()==_galeryName){
            for(int i=0; i<g->getSize();i++) {
                //cout << g->recorrer(i)->getId() << endl;
                if (g->recorrer(i)->getId() == _imgId) {
                    cout<<g->recorrer(i)->getMetadata().recorrer(0)<<endl;
                    cout<<g->recorrer(i)->getMetadata().recorrer(1)<<endl;
                    cout<<g->recorrer(i)->getMetadata().recorrer(2)<<endl;
                    cout<<g->recorrer(i)->getMetadata().recorrer(3)<<endl;
                    cout<<g->recorrer(i)->getMetadata().recorrer(4)<<endl;
                    return g->recorrer(i)->getMetadata();
                }
            }
            cout<<"Error: The meta data for the image "<<_imgId<<" doesn't exist"<<endl;
            DBList x;
            return x;
        }
    }
    cout<<"Error: The gallery "<<_galeryName<<" doesn't exist"<<endl;
}

void DataBase::deleteMetadata(string _galeryName, string _imgId) {
    for (Galery *g:galeries){
        if (g->getName()==_galeryName){
            for(int i=0; i<g->getSize();i++) {
                //cout << g->recorrer(i)->getId() << endl;
                if (g->recorrer(i)->getId() == _imgId) {
                    g->sacar(i);
                    cout<<"The metadata has been eliminated successfully!"<<endl;
                    return;
                }
            }
            cout<<"Error: The meta data for the image "<<_imgId<<" doesn't exist"<<endl;
            return;
        }
    }
    cout<<"Error: The gallery "<<_galeryName<<" doesn't exist"<<endl;
}

void DataBase::modifyMetadata(string _galeryName, string _imgId, string _metadataId, string _data) {}