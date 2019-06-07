//
// Created by jose on 06/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_GALERYNODE_H
#define MYINVINCIBLELIBRARY_METADATADB_GALERYNODE_H

#include "imagen.h"

class GaleryNode {

public:
    GaleryNode();
    GaleryNode(GaleryNode* _nxtPtr, imagen* _img);
    void set_data(imagen* _img);
    void set_nxtNd(GaleryNode* node);
    imagen* get_data();
    GaleryNode* get_nxtPtr();
private:
    imagen* img;
    GaleryNode* nxtPtr;
};


#endif //MYINVINCIBLELIBRARY_METADATADB_GALERYNODE_H
