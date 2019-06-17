//
// Created by jose on 14/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_HUFFMAN_HELPER_H
#define MYINVINCIBLELIBRARY_METADATADB_HUFFMAN_HELPER_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

class huffman_helper {

public:

    huffman_helper();

    void swapMinHeapNode(struct MinHeapNode** a,
                         struct MinHeapNode** b);

    void minHeapify(struct MinHeap* minHeap, int idx);

    int isSizeOne(struct MinHeap* minHeap);

    void insertMinHeap(struct MinHeap* minHeap,
                       struct MinHeapNode* minHeapNode);

    void buildMinHeap(struct MinHeap* minHeap);

    void printArr(int arr[], int n);

    int isLeaf(struct MinHeapNode* root);

    vector<vector<string>> printCodes(struct MinHeapNode* root, int arr[], int top, vector<string> letras, vector<string> codigos, vector<vector<string>> res);

    vector<vector<string>> HuffmanCodes(char data[], int freq[], int size, vector<string> letras, vector<string> codigos);


    struct MinHeapNode* newNode(char data, unsigned freq);

    struct MinHeap* createMinHeap(unsigned capacity);

    struct MinHeapNode* extractMin(struct MinHeap* minHeap);

    struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size);

    struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size);

    string compress(string _cadena);



private:



};


#endif //MYINVINCIBLELIBRARY_METADATADB_HUFFMAN_HELPER_H
