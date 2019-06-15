//
// Created by jose on 14/06/19.
//

#ifndef MYINVINCIBLELIBRARY_METADATADB_HUFFMAN_HELPER_H
#define MYINVINCIBLELIBRARY_METADATADB_HUFFMAN_HELPER_H
#include <stdio.h>
#include <stdlib.h>

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

    void printCodes(struct MinHeapNode* root, int arr[], int top);

    void HuffmanCodes(char data[], int freq[], int size);


    struct MinHeapNode* newNode(char data, unsigned freq);

    struct MinHeap* createMinHeap(unsigned capacity);

    struct MinHeapNode* extractMin(struct MinHeap* minHeap);

    struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size);

    struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size);



private:



};


#endif //MYINVINCIBLELIBRARY_METADATADB_HUFFMAN_HELPER_H
