//
// Created by andreyzartty on 14/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_SQLCONTROLLER_H
#define MYINVINCIBLELIBRARY_METADATADB_SQLCONTROLLER_H

#include <string>
#include "DataBase.h"


/**
 * Header de SQLController
 *
 * @since 13/06/19
 */


using namespace std;

class SQLController {

private:
    DataBase *dataBase;


public:

    SQLController();


    void funcionInsert(string comando);

    void funcionSelect(string comando);

    void funcionUpdate(string comando);

    void funcionDelete(string comando);

    string columnaGET(string evaluar);

    void addToTable(string columna, string value, string imagen);

    void makeFunction(string comando);

    ///Getters & Setters
    DataBase* getDataBase();
    void setDataBase(DataBase* _dataBase);

};


#endif //MYINVINCIBLELIBRARY_METADATADB_SQLCONTROLLER_H
