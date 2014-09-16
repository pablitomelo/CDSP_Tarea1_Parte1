#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

class HuffmanTree
{
private:
    int value;
    string code;
    int freq;
    HuffmanTree* left;
    HuffmanTree* rigth;

public:
    HuffmanTree();
    HuffmanTree(int,int);
    virtual ~HuffmanTree();
    int getFreq();
    int getValue();
    string getCode();
    HuffmanTree* getLeft();
    HuffmanTree* getRigth();

    void setValue(int);
    void setCode(string);
    void setFreq(int);
    void setLeft(HuffmanTree*);
    void setRight(HuffmanTree*);
    bool isLeaf();

};

#endif
