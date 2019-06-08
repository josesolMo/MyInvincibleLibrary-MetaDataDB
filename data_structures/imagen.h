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
    void setMetadata(string data1, string data2, string data3, string data4, string data5);
    string getId();
    DBList getMetadata();

private:
    string id;
    DBList metadata;
};


#endif //MYINVINCIBLELIBRARY_METADATADB_IMAGEN_H
