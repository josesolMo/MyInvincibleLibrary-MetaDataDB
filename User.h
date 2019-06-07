//
// Created by jose on 06/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_USER_H
#define MYINVINCIBLELIBRARY_METADATADB_USER_H

#include <iostream>
#include <vector>
#include "data_structures/DBList.h"
#include "data_structures/imagen.h"
#include "data_structures/Galery.h"

using namespace std;

class User {

public:
    User();
    User(string id, string password);
    void addGalery(string name);

private:
    string id;
    string password;
    vector<Galery> galeries;


};


#endif //MYINVINCIBLELIBRARY_METADATADB_USER_H
