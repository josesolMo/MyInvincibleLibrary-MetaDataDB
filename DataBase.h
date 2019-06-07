//
// Created by jose on 05/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_DATABASE_H
#define MYINVINCIBLELIBRARY_METADATADB_DATABASE_H


#include "data_structures/DBList.h"
#include "data_structures/imagen.h"
#include "User.h"
#include <iostream>

using namespace std;

class DataBase {

public:
    DataBase();

    void addUser(string _id, string _password);
    void addGalery(string _userId, string _galeryName);

    void addMetadata(string _userId, string _galeryName, string _imgId, string _imgName,
                                                                        string _autor,
                                                                        string _year,
                                                                        string _size,
                                                                        string _description);

    DBList consultMetadata(string _userId, string _galeryName, string _imgId);
    void deleteMetadata(string _userId, string _galeryName, string _imgId);
    void modifyMetadata(string _userId, string _galeryName, string _imgId, string _metadataId, string _data);

    string getGalery();
    User getUser();
    vector<User>* getUsers();
    imagen getImage(User _user, string _galeryName, string _imgId);


private:
    vector<User> users;
    DBList tmpGalery;


};


#endif //MYINVINCIBLELIBRARY_METADATADB_DATABASE_H
