//
// Created by jose on 06/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_IMAGEN_H
#define MYINVINCIBLELIBRARY_METADATADB_IMAGEN_H

#include <iostream>
#include "DBList.h"

using namespace std;

class imagen {

public:
    imagen();
    imagen(string _id);
    void setMetadata(int index, string data);
    string* getId();
    string* getMetadata(int index);

private:
    string id;
    DBList metadata;
};


#endif //MYINVINCIBLELIBRARY_METADATADB_IMAGEN_H
