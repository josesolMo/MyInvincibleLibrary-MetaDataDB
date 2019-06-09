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

void DataBase::addImage(string _galeryName, string _imgId) {
    for (Galery *g:galeries){
        if (g->getName()==_galeryName){
            for(int i=0; i<g->getSize();i++) {
                if (g->recorrer(i)->getId()==_imgId) {
                    cout<<"Error: The image "<<_imgId<<" already exists"<<endl;
                    return;
                }
            }
            imagen *img = new imagen(_imgId);
            g->insertFirst(img);
            cout<<"The image "<<_imgId<<" been added successfully!"<<endl;
            return;
        }
    }
    cout<<"Error: The gallery "<<_galeryName<<" doesn't exist"<<endl;
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
void DataBase::addMetadata(string _galeryName, string _imgId, string _imgName,string _author, string _year,
                           string _size, string _description) {
    for (Galery *g:galeries){
        if (g->getName()==_galeryName){
            cout<<g->getSize()<<endl;
            for(int i=0; i<g->getSize();i++) {
                cout<<g->recorrer(i)->getId()<<endl;
                if (g->recorrer(i)->getId()==_imgId) {
                    g->recorrer(i)->setMetadata(_imgName, _author, _year, _size, _description);
                    cout<<"Metadata has been added successfully!"<<endl;
                    return;
                }
            }
            cout<<"Error: The image "<<_imgId<<" doesn't exist"<<endl;
            return;
        }
    }
    cout<<"Error: The gallery "<<_galeryName<<" doesn't exist"<<endl;
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

void DataBase::modifyMetadata(string _galeryName, string _imgId, string _metadataId, string _data) {
    for (Galery *g:galeries){
        if (g->getName()==_galeryName){
            for(int i=0; i<g->getSize();i++) {
                //cout << g->recorrer(i)->getId() << endl;
                if (g->recorrer(i)->getId() == _imgId) {
                    g->recorrer(i)->setMetadata(_metadataId,_data);
                    cout<<"The metadata "<<_metadataId<<" has been modified successfully!"<<endl;
                    return;
                }
            }
            cout<<"Error: The metadata for the image "<<_imgId<<" doesn't exist"<<endl;
            return;
        }
    }
    cout<<"Error: The gallery "<<_galeryName<<" doesn't exist"<<endl;
}