//
// Created by jose on 06/06/19.
//

#include "User.h"

User::User() {}

User::User(string _id, string _password) {
    id = _id;
    password = _password;
}

void User::addGalery(string _name) {
    Galery *galery=new Galery(_name);
    galeries.push_back(*galery);
}

string User::getId() {
    return id;
}

string User::getPassword() {
    return password;
}

vector<Galery>* User::getGalery() {
    return &galeries;
}
