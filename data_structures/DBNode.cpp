//
// Created by jose on 05/06/19.
//

#include "DBNode.h"

DBNode :: DBNode(DBNode* _nxtPtr, string* _data) {
    nxtPtr = _nxtPtr;
    data = _data;
}
void DBNode::set_data(string* _data) {
    data = _data;
}
void DBNode::set_nxtNd(DBNode* _nxtNd) {
    nxtPtr = _nxtNd;
    //std::cout<<&_nxtNd<<std::endl;
}
string* DBNode::get_data() {
    //std::cout<<data<<std::endl;
    return data;
}
DBNode* DBNode::get_nxtPtr() {
    //std::cout<<nxtPtr<<std::endl;
    return nxtPtr;
}
