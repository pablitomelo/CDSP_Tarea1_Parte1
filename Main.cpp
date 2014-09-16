#include "HuffmanCode.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char BYTE;


//retorna entero  0..255
int getIntOfByte(BYTE b){
   return  b & 0xFF;
}

//retorna byte (0 = 00000000)
BYTE getBYTEOfInt(int x){
    return (x % 256);
}

//verifica si el comando es -c (codificacion)
bool isEncoderMode(string param){
    char *cstr = new char[param.length() + 1];
        strcpy(cstr, param.c_str());

    bool encode = (strcmp(cstr,"-c")==0);

    delete cstr;

    return encode;
}

//verifica si el comando es -d (decodificador)
bool isDecoderMode(string param){
    char *cstr = new char[param.length() + 1];
    strcpy(cstr, param.c_str());

    bool decode = (strcmp(cstr,"-d")==0);

    delete cstr;

    return decode;
}

//verifica si el comando es -h (Ayuda)
bool isHelpMode(string param){
    char *cstr = new char[param.length() + 1];
    strcpy(cstr, param.c_str());

    bool decode = (strcmp(cstr,"-h")==0);

    delete cstr;

    return decode;
}

//verfica si el comando es salir (No se utiliza)
bool isQuitMode(string param){
    char *cstr = new char[param.length() + 1];
    strcpy(cstr, param.c_str());

    bool decode = (strcmp(cstr,"-q")==0);

    delete cstr;

    return decode;
}

//funciona auxiliar para mostrar ayuda
void showHelpMessage(){
    cout<<"  Ayuda"<<endl;
    cout<<"  -----"<<endl<<endl;;
    cout<<"Ejecute -c pathOrigen pathDestino para comprimir el archivo que se encuentra en pathOrigen y dejarlo en pathDestino."<<endl;
    cout<<"Ejecute -d pathOrigen pathDestino para descomprimir el archivo que se encuentra en pathOrigen y dejarlo en pathDestino."<<endl;
}

//funciona auxiliar para mostrar mensaje de error por comando no ingresado
void showErrorMessage(){
    cout<<"El comando ingresado no es correcto. Ejecute -h para obtener ayuda."<<endl;
    cout<<"Ejecute -h para obtener ayuda"<<endl;
}

//funciona auxiliar que retorna la codificacion para el elemento value
void getHuffmanValue(HuffmanTree* tree,int value,string str,string &code){
    if(tree!=NULL){
        if(tree->isLeaf() and tree->getValue() == value){ //si es hoja y el valor es el q estoy buscando devuelvo el codigo
            code = str;
        }
        else{
                getHuffmanValue(tree->getLeft() ,value,str + "0",code); // busco en la izq con un 0
                getHuffmanValue(tree->getRigth(),value,str + "1", code); //busco en la der con un 1
        }
    }
}

//funcion que realiza la codificacion del archivo con path paramInput y lo
//escribe en el archivo con path paramOutput
//Errores: No se puede abrir el archivo  o ya existe el archivo con path paramOutput
bool EncodeFile(string paramInput,string paramOutput ){

    //abrimos el archivo a codificar
    ifstream infile(paramInput.c_str(), ios::in|ios::binary);

    if(!infile)
    {
        cout<<"No se puedo abrir el archivo"<<endl;
        return false;
    }

    //verifico que no exista el archivo
    if(ifstream(paramOutput.c_str()))
    {
        cout<<"Ya existe un archivo con el mismo nombre."<<endl;

        return false;
    }

    //Abro el archivo donde voy a guardar
    ofstream outfile(paramOutput.c_str(), ios::out|ios::binary);
    if(!outfile)
    {
        cerr<<"No se pudo comprimir el archivo"<<endl;
        return false;
    }

    int freq [256] = {0};

    //recorro el archivo a codificar y seteo las frecuencias.
    BYTE b;
    char c;
    while(infile.get(c))
    {
        b=c;
        freq[getIntOfByte(b)]++;
    }

    infile.clear();
    infile.seekg(0); //vuelvo el puntero al principio ya que lo reutilizo mas adelante

    HuffmanCode* code = new HuffmanCode();

    //obtenemos el arbol de Huffman para las frecuencias y retorna las cantidad de hojas del arbol
    int countOfBytes = 0;
    HuffmanTree* tree = code->getHuffmanTree(256,freq,countOfBytes);

    //Los primeros 4 bytes son para la cantidad de elementos codificados
    outfile.put(static_cast<unsigned char>(countOfBytes>>24));
    outfile.put(static_cast<unsigned char>((countOfBytes>>16)%256));
    outfile.put(static_cast<unsigned char>((countOfBytes>>8)%256));
    outfile.put(static_cast<unsigned char>(countOfBytes%256));

    for(int i=0;i<256;++i)
    {
        if(freq[i]>0){
            //Agrego el byte
            outfile.put(getBYTEOfInt(i));

            //Agrego la frecuencia del byte
            outfile.put(static_cast<unsigned char>(freq[i]>>24));
            outfile.put(static_cast<unsigned char>((freq[i]>>16)%256));
            outfile.put(static_cast<unsigned char>((freq[i]>>8)%256));
            outfile.put(static_cast<unsigned char>(freq[i]%256));
        }
    }

    //Final de la tabla de Huffman, ahora codifico el archivo.
    //Leo los bytes del archivo y codifico.

    BYTE ch2;
    BYTE ch;
    while(infile.get(c))
    {
        ch=c;

        string strCode = "";
        getHuffmanValue(tree,getIntOfByte(ch),"",strCode);
        //Escribo bit a bit el archivo de salida segun el codigo de huffman para el byte leido
        for(unsigned int i=0;i<strCode.size();++i)
        {
            if(strCode.at(i)=='0'){
                ch2=0;
            }

            if(strCode.at(i)=='1')
                ch2=1;
            code->huffman_write(ch2, outfile);
        }
    }
    ch2=2; //Terrmine de leer, escribo el EOF
    code->huffman_write(ch2, outfile);

    infile.close();
    outfile.close();

    delete code;
    delete tree;
    return true;
}


//funcion que realiza la decodificacion del archivo con path paramInput y lo
//escribe en el archivo con path paramOutput
//Errores: No se puede abrir el archivo con path paramInput  o ya existe el archivo con path paramOutput
bool DecodeFile(string paramInput,string paramOutput){

    //Abrimos el archivo a decodificar
    ifstream infile(paramInput.c_str(), ios::in|ios::binary);

    if(!infile)
    {
        cout<<"No se pudo descomprimir el archivo."<<endl;
        return false;
    }

    //verifico que no exista ya un archivo con el mismo nombre
    string outFileName = paramOutput;
    if(ifstream(outFileName.c_str()))
    {
        cout<<"Ya existe un archivo con el mismo nombre."<<endl;

        return false;
    }

     //abrimos el archivo donde se escribira la decodificacion
    ofstream outfile(outFileName.c_str(), ios::out|ios::binary);
    if(!outfile)
    {
        cout<<"No se pudo descomprimir el archivo."<<endl;
        return false;
    }

    char c;
    BYTE ch;
    unsigned int j=1;

    //primeros 4 bytes para cantidad de elementos decodificados
    int countOfElements = 0;
    for(int k=3;k>=0;--k)
    {
        infile.get(c);
        ch=c;
        countOfElements+=ch*(j<<(8*k));
    }

    int freq[256] = {0};

    //obtenermos 1 byte para elemento y 4 para frecuencia y lo seteamos en array de frecuencias
    int totalBytesDecode = 0;
    for (int i = 0;i<countOfElements;i++){
        //obtengo el elemento
        infile.get(c);
        ch=c;
        int elem = getIntOfByte(ch);

        //obtengo la frecuencia del elemento, 4 bytes
        int freq_elem = 0;
        for(int k=3;k>=0;--k)
        {
            infile.get(c);
            ch=c;
            freq_elem+=ch*(j<<(8*k));
        }
        freq[elem] = freq_elem;
        totalBytesDecode= totalBytesDecode + freq_elem;
    }

    // Fin de la tabla de codificacion.

    //Obtengo la codificacion de Huffman
    int countOfBytes = 0;
    HuffmanCode* code = new HuffmanCode();
    HuffmanTree* tree = code->getHuffmanTree(256,freq,countOfBytes);

    //Comienzo a leer de a bit para obtener byte codificado.
    string st="";
    int value = -2 ;

    while(totalBytesDecode>0) //Mientras tenga elementos para decodificar
    {
        st="";
        do
        {
            //leo el archivo bit a bit.
            ch=code->huffman_read(infile);
            if(ch==0)
                st=st+'0';
            if(ch==1)
                st=st+'1';
        } //mientras que no encontramos el elemento (no es una hoja)
        while(!code->get_huffman_value(tree,st, value));

        //Escribimos el byte encontrado en el archivo
        outfile.put(static_cast<char>(getBYTEOfInt(value)));
        --totalBytesDecode;
    }

    infile.clear();
    infile.seekg(0);

    infile.close();
    outfile.close();

    delete tree;
    delete code;

    return true;
}

//Main nombrePrograma command pathOrigen pathDestino
int main(int argc, char **argv) {

    string command;

    bool error = false;

    if(argc==2){
        if (isHelpMode(argv[1])){
            showHelpMessage();
        }
        else if(isQuitMode(argv[1])){
            return 0;
        }
        else{
            error = true;
        }
    }
    else if(argc==4){
        if(isEncoderMode(argv[1])){
            bool result = EncodeFile(argv[2],argv[3]);

            if(result){
                cout<<"El archivo se ha comprimido correctamente"<<endl;
            }
            else{
                cout<<"Se ha producido un error al comprimir el archivo"<<endl;
            }
        }
        else if(isDecoderMode(argv[1])){
            bool result = DecodeFile(argv[2],argv[3]);
            if(result){
                cout<<"El archivo se ha descomprimido correctamente"<<endl;
            }
            else{
                cout<<"Se ha producido un error al comprimir el archivo"<<endl;
            }
        }
        else {
            error = true;
        }

    }
    else{
        error = true;
    }

    if(error){
        showErrorMessage();
    }

    return 0;

}



