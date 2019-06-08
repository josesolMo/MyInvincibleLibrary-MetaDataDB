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

string imagen::getId() {
    return id;
}

DBList imagen::getMetadata() {
    return metadata;
}