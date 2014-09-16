#ifndef HUFFMANCODE_H
#define HUFFMANCODE_H
#include "HuffmanTree.h"
#include <iostream>
#include <fstream>
#include <list>

using namespace std;
class HuffmanCode
{
    public :
        HuffmanCode();
        virtual ~HuffmanCode();
        HuffmanTree* getHuffmanTree(int,int*,int&);
        void showHuffmanCode(HuffmanTree*);
        void getHuffmanValue(HuffmanTree*,int,string, string&);
        void huffman_write(unsigned char i, ofstream & outfile);
        unsigned char huffman_read(ifstream & infile);
        bool get_huffman_value(HuffmanTree* tree,string s, int& i);

};

#endif
