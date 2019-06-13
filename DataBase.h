//
// Created by jose on 05/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_DATABASE_H
#define MYINVINCIBLELIBRARY_METADATADB_DATABASE_H


#include "data_structures/DBList.h"
#include "data_structures/imagen.h"
#include "User.h"
#include <json-c/json.h>
#include <string.h>
#include <iostream>

using namespace std;

class DataBase {

public:
    DataBase();

    void addImage(string _galeryName, string _imgId);

    void addMetadata(string _galeryName, string _imgId, string _imgName,
                                                        string _author,
                                                        string _year,
                                                        string _size,
                                                        string _description);

    DBList consultMetadata(string _galeryName, string _imgId);
    void deleteMetadata(string _galeryName, string _imgId);
    void modifyMetadata(string _galeryName, string _imgId, string _metadataId, string _data);
    void readJson();
    void restartDataBase();
    void updateBackup();
    void addGallery(string _galleryName);

    Galery* getGalery();
    vector<Galery>* getGaleries();
    imagen getImage(string _galeryName, string _imgId);


private:
    vector<Galery*> galeries;
    DBList tmpGalery;


};


#endif //MYINVINCIBLELIBRARY_METADATADB_DATABASE_H
