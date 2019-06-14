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

    bool addGallery(string _galleryName);
    bool addImage(string _galleryName, string _imgId);

    void addMetadata(string _galleryName, string _imgId, string _imgName,
                                                        string _author,
                                                        int _year,
                                                        string _size,
                                                        string _description);

    string consultMetadata(string _galleryName, string _imgId, string _metadataId);

    void deleteMetadata(string _galleryName, string _imgId);
    void modifyMetadata(string _galleryName, string _imgId, string _metadataId, string _data);
    void readJson();
    void restartDataBase();
    void updateBackup();

    vector<string> getColumn(string _galleryName, string _columnId);


private:



};


#endif //MYINVINCIBLELIBRARY_METADATADB_DATABASE_H
