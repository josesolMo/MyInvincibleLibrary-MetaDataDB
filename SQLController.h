//
// Created by andreyzartty on 14/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_SQLCONTROLLER_H
#define MYINVINCIBLELIBRARY_METADATADB_SQLCONTROLLER_H

#include <string>

using namespace std;

class SQLController {

private:
    string stringToRead;

public:
    SQLController();
    string getStringToRead();
    void setStringToRead(string str);

    void funcionInsert(string comando);

    void funcionSelect(string comando);

    void funcionUpdate(string comando);

    void funcionDelete(string comando);

    string columnaGET(string evaluar);

    void addToTable(string columna, string value);

    void makeFunction(string comando);
};




#endif //MYINVINCIBLELIBRARY_METADATADB_SQLCONTROLLER_H
