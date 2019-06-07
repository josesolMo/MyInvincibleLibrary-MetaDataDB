//
// Created by jose on 06/06/19.
//

#include "GaleryNode.h"

GaleryNode :: GaleryNode(GaleryNode* _nxtPtr, imagen* _img) {
    nxtPtr = _nxtPtr;
    img = _img;
}
void GaleryNode::set_data(imagen* _img) {
    img = _img;
}
void GaleryNode::set_nxtNd(GaleryNode* _nxtNd) {
    nxtPtr = _nxtNd;
    //std::cout<<&_nxtNd<<std::endl;
}
imagen* GaleryNode::get_data() {
    //std::cout<<data<<std::endl;
    return img;
}
GaleryNode* GaleryNode::get_nxtPtr() {
    //std::cout<<nxtPtr<<std::endl;
    return nxtPtr;
}