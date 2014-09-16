#include "HuffmanCode.h"

HuffmanCode::HuffmanCode(){

}

HuffmanCode::~HuffmanCode()
{

}
//funcion auxiliar que muestra las frecuencias.
//solo testing
void showFrequencies(int* freq,long size){
    for(int i = 0; i<256; i++){
        cout<<i<<" "<<freq[i]<<endl;;
    }
}

//funcion auxiliar inserta nodo en lista ordenada por frecuencia
void InsertNodeInOrderByFrequency(list<HuffmanTree*> &list, HuffmanTree* htree)
{
    if(list.size()==0){
        list.push_back(htree);
    }
    else {
      bool inserted = false;
      for (std::list<HuffmanTree*>::iterator it=list.begin(); (it!=list.end() and !inserted); ++it){
        if((*it)->getFreq() > (htree)->getFreq()){
            list.insert(it,htree);
            inserted=true;
        }
      }
      if(!inserted){
        list.push_back(htree);
      }
    }
}

//funcion obtiene el arbol para la codificacion de Huffman
HuffmanTree* HuffmanCode::getHuffmanTree(int size,int* frequencies,int &count)
{
    list<HuffmanTree*> list;
    HuffmanTree* newNode;

    //recorremos el array de frecuencias y nos quedamos solo con los que tienen frecuencia mayor que 0
    for(int i = 0; i< size; i++){
        if(frequencies[i]!=0){
            //creamos el nodo y lo insertamos en la lista de nodos ordenada por frecuencias
            HuffmanTree* node = new HuffmanTree(i,frequencies[i]);
            InsertNodeInOrderByFrequency(list, node);
            count++;
        }
    }

    while (list.size()>=2){

        //obtengo el nodo con menor frecuencia
        HuffmanTree* node1 = list.front();
        list.pop_front();

        //obtengo el nodo con lo sigue con menor frecuencia
        HuffmanTree* node2 = list.front();
        list.pop_front();

        //creo un nuevo nodo con la suma de las frecuencias y lo agrego a la lista ordenada por frecuencia
        //seteo arbol izq y der.
        newNode = new HuffmanTree(-1,node1->getFreq() + node2->getFreq());
        newNode->setLeft(node1);
        newNode->setRight(node2);
        InsertNodeInOrderByFrequency(list, newNode);
    }

    return newNode;
}


//funcion auxiliar que muestra el codigo de huffman dado el arbol.
// Para testing
void HuffmanCode::showHuffmanCode(HuffmanTree* tree)
{
    if(tree->isLeaf()){
        cout<< tree->getValue() <<" : " <<tree->getCode()<<endl;;
    }
    else{
        tree->getLeft()->setCode(tree->getCode() + "0");
        showHuffmanCode(tree->getLeft());
        tree->getRigth()->setCode(tree->getCode() + "1");
        showHuffmanCode(tree->getRigth());
    }
}

//Escribe el bit en el archivo segun la entrada i
//0,1 o 2 = EOF
void HuffmanCode::huffman_write(unsigned char i, ofstream & outfile)
{
    static int bit_pos=0; //0 a 7 (izq a der) en byte
    static unsigned char c='\0';

    if(i<2) //si no es EOF
    {
        if(i==1)
            c=c | (i<<(7-bit_pos)); //si es 1 agrego 1 al byte
        else //i==0
            c=c & static_cast<unsigned char>(255-(1<<(7-bit_pos))); //agrego 0 al byte
        ++bit_pos;
        bit_pos%=8;
        if(bit_pos==0) // ya tengo el byte completo, escribo en el archivo
        {
            outfile.put(c);
            c='\0';
        }
    }
    else
    {
        //si es EOF escribo en el archivo
        outfile.put(c);
    }
}

//Lee el bit del archivo y retorna 0,1,2
//0,1 para los bits y 2 para EOF
unsigned char HuffmanCode::huffman_read(ifstream & infile)
{
    static int bit_pos=0; //0 a 7 (der a izq) el el byte
    static unsigned char c1 = infile.get();

    unsigned char i;

    i=(c1>>(7-bit_pos))%2; //obtengo el bit
    ++bit_pos;
    bit_pos%=8;
    if(bit_pos==0){ // si es el ultimo
        if(infile.good()) // todavia puedo seguir leyendo
        {
            c1=infile.get();
        }
        else // finde la archivo. retorno 2
            i=2;
    }

    return i;
}

//Retorna si el string s es el codigo de un elemento del arbol
//Si es hoja.
bool HuffmanCode::get_huffman_value(HuffmanTree* tree,string s, int& i)
{
    for(unsigned int j=0;j<s.size();++j)
    {
        if(s[j]=='0') //Si es 0 voy a la izquiera
            tree=tree->getLeft();
        if(s[j]=='1') //si es 1 voy a la derecha
            tree=tree->getRigth();
    }

    bool found=false;

    if(tree->isLeaf()) //si es la hoja obtengo el valor
    {
        found=true;
        i=tree->getValue();
    }

    return found;
}
