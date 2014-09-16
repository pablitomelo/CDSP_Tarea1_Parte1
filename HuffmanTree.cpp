#include "HuffmanTree.h"

HuffmanTree::HuffmanTree(){

}

//constructor con valor y frecuencia
HuffmanTree::HuffmanTree(int val,int fr){
    value = val;
    freq = fr;
    code="";
    left= NULL;
    rigth=NULL;
}

//devuelve frecuencia
int HuffmanTree::getFreq(){
    return freq;
}

//setea frecuencia
void HuffmanTree::setFreq(int fr){
    freq = fr;
}

//setea arbol derecho
void HuffmanTree::setRight(HuffmanTree* rigthTree)
{
    rigth =rigthTree;
}

//setea arbol izquierdo
void HuffmanTree::setLeft(HuffmanTree* leftTree)
{
    left = leftTree;
}

//verifica si es una hoja
bool HuffmanTree::isLeaf(){
    return (left== NULL and rigth==NULL);
}

//obtiene el valor
int HuffmanTree::getValue(){
    return value;
}

//obtiene el codigo
string HuffmanTree::getCode(){
    return code;
}

//setea el valor
void HuffmanTree::setValue(int val){
    value = val;
}

//sete el codigo
void HuffmanTree::setCode(string cod){
    code = cod;
}

//obtiene el arbol izq
HuffmanTree* HuffmanTree::getLeft(){
    return left;
}

//obtiene el arbol der
HuffmanTree* HuffmanTree::getRigth(){
    return rigth;
}


//destructor
HuffmanTree::~HuffmanTree()
{

}

