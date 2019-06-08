//
// Created by jose on 06/06/19.
//

#include "imagen.h"

imagen::imagen() {}

imagen::imagen(string _id) {
    id=_id;
}

void imagen::setMetadata(string data1, string data2, string data3, string data4, string data5) {
    metadata.insertLast(data1);
    metadata.insertLast(data2);
    metadata.insertLast(data3);
    metadata.insertLast(data4);
    metadata.insertLast(data5);
}

void imagen::setMetadata(string _metadataId, string _data) {
    //cout<<_data<<endl;

    DBList *meta1 = &metadata;
    if (_metadataId=="NAME"){
        meta1->reinsert(0,_data);
    }else if(_metadataId=="AUTHOR"){
        meta1->reinsert(1,_data);
    }else if(_metadataId=="YEAR"){
        meta1->reinsert(2,_data);
    }else if(_metadataId=="SIZE"){
        meta1->reinsert(3,_data);
    }else if(_metadataId=="DESCRIPTION"){
        meta1->reinsert(4,_data);
    }
    //cout<<metadata.recorrer(2)<<endl;
}

string imagen::getId() {
    return id;
}

DBList imagen::getMetadata() {
    return metadata;
}