//
// Created by jose on 05/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_DBNODE_H
#define MYINVINCIBLELIBRARY_METADATADB_DBNODE_H

#include <iostream>

using namespace std;

class DBNode {

public:
    DBNode();
    DBNode(DBNode* _nxtPtr, string* _data);
    void set_data(string* _data);
    void set_nxtNd(DBNode* node);
    string* get_data();
    DBNode* get_nxtPtr();
private:
    string* data;
    DBNode* nxtPtr;
};

#endif //MYINVINCIBLELIBRARY_METADATADB_DBNODE_H
