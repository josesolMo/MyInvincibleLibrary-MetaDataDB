//
// Created by jose on 14/06/19.
//

#include <vector>
#include <sstream>
#include "huffman_helper.h"


// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#define MAX_TREE_HT 100

huffman_helper::huffman_helper() {}

// A Huffman tree node
struct MinHeapNode {

    // One of the input characters
    char data;

    // Frequency of the character
    unsigned freq;

    // Left and right child of this node
    struct MinHeapNode *left, *right;
};

// A Min Heap:  Collection of
// min-heap (or Huffman tree) nodes
struct MinHeap {

    // Current size of min heap
    unsigned size;

    // capacity of min heap
    unsigned capacity;

    // Array of minheap node pointers
    struct MinHeapNode** array;
};

// A utility function allocate a new
// min heap node with given character
// and frequency of the character
struct MinHeapNode* huffman_helper::newNode(char data, unsigned freq)
{
    struct MinHeapNode* temp
            = (struct MinHeapNode*)malloc
                    (sizeof(struct MinHeapNode));

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

// A utility function to create
// a min heap of given capacity
struct MinHeap* huffman_helper::createMinHeap(unsigned capacity)

{

    struct MinHeap* minHeap
            = (struct MinHeap*)malloc(sizeof(struct MinHeap));

    // current size is 0
    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array
            = (struct MinHeapNode**)malloc(minHeap->
            capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to
// swap two min heap nodes
void huffman_helper::swapMinHeapNode(struct MinHeapNode** a,
                     struct MinHeapNode** b)

{

    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// The standard minHeapify function.
void huffman_helper::minHeapify(struct MinHeap* minHeap, int idx)

{

    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->
            freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->
            freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// A utility function to check
// if size of heap is 1 or not
int huffman_helper::isSizeOne(struct MinHeap* minHeap)
{

    return (minHeap->size == 1);
}

// A standard function to extract
// minimum value node from heap
struct MinHeapNode* huffman_helper::extractMin(struct MinHeap* minHeap)

{

    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0]
            = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

// A utility function to insert
// a new node to Min Heap
void huffman_helper::insertMinHeap(struct MinHeap* minHeap,
                   struct MinHeapNode* minHeapNode)

{

    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {

        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap
void huffman_helper::buildMinHeap(struct MinHeap* minHeap)

{

    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// A utility function to print an array of size n
void huffman_helper::printArr(int arr[], int n)
{
    int i;
    string res;
    stringstream ss;
    string s;

    for (i = 0; i < n; ++i)
        s="";

        //cout<<arr[i]<<endl;
        res+=arr[i];

        ss << arr[i];
        ss >> s;

        res+=s;

        //cout<<arr[i];

        printf("%d", arr[i]);

    printf("\n");
    //return res;
}

// Utility function to check if this node is leaf
int huffman_helper::isLeaf(struct MinHeapNode* root)

{

    return !(root->left) && !(root->right);
}

// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
struct MinHeap* huffman_helper::createAndBuildMinHeap(char data[], int freq[], int size)

{

    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

// The main function that builds Huffman tree
struct MinHeapNode* huffman_helper::buildHuffmanTree(char data[], int freq[], int size)

{
    struct MinHeapNode *left, *right, *top;

    // Step 1: Create a min heap of capacity
    // equal to size.  Initially, there are
    // modes equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap)) {

        // Step 2: Extract the two minimum
        // freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Step 3:  Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // Step 4: The remaining node is the
    // root node and the tree is complete.
    return extractMin(minHeap);
}

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
vector<vector<string>> huffman_helper::printCodes(struct MinHeapNode* root, int arr[], int top, vector<string> letras, vector<string> codigos, vector<vector<string>> res)

{


    // Assign 0 to left edge and recur
    if (root->left) {

        arr[top] = 0;
        return printCodes(root->left, arr, top + 1, letras, codigos, res);
    }

    // Assign 1 to right edge and recur
    if (root->right) {

        arr[top] = 1;
        return printCodes(root->right, arr, top + 1, letras, codigos, res);
    }

    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (isLeaf(root)) {

        cout<<"hay"<<endl;
        printf("%c: ", root->data);
        printArr(arr, top);

        stringstream ss;
        string s;
        ss << root->data;
        ss >> s;


        res[0].push_back(s);
        //res[1].push_back(printArr(arr, top));

        return res;

    }
}

// The main function that builds a
// Huffman Tree and print codes by traversing
// the built Huffman Tree
vector<vector<string>> huffman_helper::HuffmanCodes(char data[], int freq[], int size, vector<string> letras, vector<string> codigos)
{
    // Construct Huffman Tree
    struct MinHeapNode* root
            = buildHuffmanTree(data, freq, size);

    // Print Huffman codes using
    // the Huffman tree built above
    int arr[MAX_TREE_HT], top = 0;

    vector<vector<string>> res;

    return printCodes(root, arr, top, letras, codigos, res);
}

string huffman_helper::compress(string _cadena) {

    vector<char> characters;

    vector<int> frecuencias;

    vector<string> letras;

    vector<string> codigos;

    bool found;

    int frec;

    for (int i=0;i<_cadena.size();i++){
        frec=0;
        found = 0;
        char c = _cadena[i];
        for (int j=0;j<characters.size();j++){
            if(characters[j]==c){
                found=1;
                break;
            }
        }
        if (!found){
            for (int j=0;j<_cadena.size();j++){
                if (_cadena[j]==c){
                    frec+=1;
                }
            }
            characters.push_back(c);
            frecuencias.push_back(frec);
        }
        cout<<"evaluando letra: "<<c<<" frecuencia: "<<frec<<endl;
    }

    for (int i=0;i<characters.size();i++){
        cout<<characters[i]<<": "<<frecuencias[i]<<endl;
    }

    char arr[characters.size()];
    int freq[characters.size()];

    for (int i=0;i<characters.size();i++){
        arr[i]=characters[i];
        freq[i]=frecuencias[i];
    }

    vector<vector<string>> res = HuffmanCodes(arr,freq,characters.size(),letras,codigos);

    cout<<"*************************************************************************"<<endl;

    for (int i=0;i<res[0].size();i++){
        cout<<res[0][i]<<endl;
    }

    for (int i=0;i<res[1].size();i++){
        cout<<res[1][i]<<endl;
    }


    return "";
}
