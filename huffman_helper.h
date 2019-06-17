//
// Created by jose on 14/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_HUFFMAN_HELPER_H
#define MYINVINCIBLELIBRARY_METADATADB_HUFFMAN_HELPER_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <bits/stdc++.h>
#define MAX_TREE_HT 256
using namespace std;

class huffman_helper {

public:

    huffman_helper();

    void printCodes(struct MinHeapNode* root, string str);

    void storeCodes(struct MinHeapNode* root, string str);


private:



};


#endif //MYINVINCIBLELIBRARY_METADATADB_HUFFMAN_HELPER_H
