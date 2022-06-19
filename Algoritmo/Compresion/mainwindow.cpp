#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QInputDialog>
#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <bits/stdc++.h>
#include <sstream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    Archivo();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//Nodo del arbol
struct Node{

    char simbolo;
    int frecuencia;
    Node *izq;
    Node *der;
    int index;
    string data;
    Node *next;
} ;

//Funcion para asignar un nuevo nodo al arbol
Node* getNode(char simbolo, int frecuencia, Node* izq, Node* der){
    Node* nodo = new Node();

    nodo->simbolo = simbolo;
    nodo->frecuencia=frecuencia;
    nodo->izq=izq;
    nodo->der=der;

    return nodo;
}

void st_Node(Node *head, int index, string data){
    head->index = index;
    head->data = data;
    head->next = NULL;
}

void insert_Node(Node *head, int index, string data){
    Node *new_Node = new Node;
    new_Node->index = index;
    new_Node->data = data;
    new_Node->next = NULL;

    Node *curr = head;
    while (curr != NULL)
    {
        if (curr->next == NULL)
        {
            curr->next = new_Node;
            return;
        }
        curr = curr->next;
    }
}

Node *search_Node(Node *head, string data)
{
    Node *curr = head;
    while (curr != NULL)
    {
        if (data.compare(curr->data) == 0)
            return curr;
        else
            curr = curr->next;
    }
    return NULL;
}

Node *search_Node(Node *head, int index)
{
    Node *curr = head;
    while (curr != NULL)
    {
        if (index == curr->index)
            return curr;
        else
            curr = curr->next;
    }
    return NULL;
}

bool delete_Node(Node *head, Node *to_delete){
    if (to_delete == NULL)
        return false;
    else if (to_delete == head)
    {
        head = to_delete->next;
        delete to_delete;
        return true;
    }
    else{
        Node *curr = head;
        while (curr)
        {
            if (curr->next == to_delete)
            {
                curr->next = to_delete->next;
                delete to_delete;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }
}

//Objeto de comparación que será usado para ordenar la pila

struct comp{
    bool operator()(Node* i, Node* d){
        return i->frecuencia > d->frecuencia;
    }
};

//Revisa el arbol de huffman y guarda los códigos en un mapa
void encode(Node* raiz, string str, unordered_map<char, string> &CodigoHuffman){
    if (raiz == nullptr){
        return;
    }
    if(!raiz->izq && !raiz->der){
        CodigoHuffman[raiz->simbolo]=str;
    }
    encode(raiz->izq,str+"0",CodigoHuffman);
    encode(raiz->der,str+"1",CodigoHuffman);
}

//Revisa el arbol y decodifica los simbolos codificados
void decode(Node* raiz, int &index, string str){
    if(raiz == nullptr){
        return;
    }
    if(!raiz->izq && !raiz->der){
        cout << raiz->simbolo;
        return;
    }
    index++;
    if(str[index]=='0'){
        decode(raiz->izq,index,str);
    }
    else{
        decode(raiz->der, index,str);
    }
}

//Crea el arbol de Huffman y decodifica dados los símbolos
string CrearArbol(string text){
    //Cuenta la cantidad de veces que aparece cada símbolo y la guarda en el mapa
    unordered_map<char,int> frecuencia;
    for(char simbolo: text){
        frecuencia[simbolo]++;
    }

    //Crea una cola de prioridad para guardar los nodos del arbol
    priority_queue<Node*,vector<Node*>, comp> pq;

    //Agrega a la cola de prioridad los nodos de cada símbolo
    for(auto pair:frecuencia){
        pq.push(getNode(pair.first,pair.second,nullptr,nullptr));
    }

    //Hacer todo el proceso hasta que haya más de 1 nodo en la cola
    while(pq.size() !=1){
        Node *izq = pq.top(); pq.pop();
        Node *der = pq.top(); pq.pop();
        int sum = izq->frecuencia + der->frecuencia;
        pq.push(getNode('\0', sum, izq, der));
    }
    Node* raiz = pq.top();

    //Imprime los códigos de cada símbolo
    unordered_map<char,string> CodigoHuffman;
    encode(raiz, "", CodigoHuffman);
    //Los códigos huffman son
    for(auto pair: CodigoHuffman){
      cout << pair.first << "  " << pair.second << '\n';
    }
    //Imprime el original
    cout <<"El original era: "<< text <<'\n';

    //Imprime el texto comprimido
    string str = "";
    for(char simbolo:text){
        str += CodigoHuffman[simbolo];
    }
    cout <<"El texto comprimido es"<< str;

    //Agregar aquí función para escribir en el archivo

    int index = -1;
    cout <<"El texto descomprimido es";
    while(index < (int)str.size()-2){
        decode(raiz,index,str);
    }

    return str;

}//Tomar el archivo y retornar solamente el texto comprimido


string Huffman(QString ruta){//Algoritmo de Huffman.
    fstream myFile;
    myFile.open(ruta.toStdString(), ios::in);
    string line;
    string resultado = "";
    if(myFile.is_open()){
        getline(myFile,line);
        resultado = CrearArbol(line);
    }
    myFile.close();
    return resultado;

}


vector<int> encoding(string s1)
{
    cout << "Encoding\n";
    unordered_map<string, int> table;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[ch] = i;
    }

    string p = "", c = "";
    p += s1[0];
    int code = 256;
    vector<int> output_code;
    cout << "String\tOutput_Code\tAddition\n";
    for (int i = 0; i < s1.length(); i++) {
        if (i != s1.length() - 1)
            c += s1[i + 1];
        if (table.find(p + c) != table.end()) {
            p = p + c;
        }
        else {
            cout << p << "\t" << table[p] << "\t\t"
                 << p + c << "\t" << code << endl;
            output_code.push_back(table[p]);
            table[p + c] = code;
            code++;
            p = c;
        }
        c = "";
    }
    cout << p << "\t" << table[p] << endl;
    output_code.push_back(table[p]);
    return output_code;
}

void decoding(vector<int> op)
{
    cout << "\nDecoding\n";
    unordered_map<int, string> table;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[i] = ch;
    }
    int old = op[0], n;
    string s = table[old];
    string c = "";
    c += s[0];
    cout << s;
    int count = 256;
    for (int i = 0; i < op.size() - 1; i++) {
        n = op[i + 1];
        if (table.find(n) == table.end()) {
            s = table[old];
            s = s + c;
        }
        else {
            s = table[n];
        }
        cout << s;
        c = "";
        c += s[0];
        table[count] = table[old] + c;
        count++;
        old = n;
    }
}




string LZW(QString ruta){ //Algoritmo de LZW
    fstream myFile;
    myFile.open(ruta.toStdString(), ios::in);
    string line;
    string resultado = "";
    if(myFile.is_open()){

        getline(myFile,line);

        vector<int> output_code = encoding(line);
        std::stringstream result;

        std::copy(output_code.begin(),output_code.end(),std::ostream_iterator<int>(result, " "));
        resultado = result.str();
    }
    myFile.close();
    return resultado;


}





string LZ77(string input)
{
    // Initialized variables
    string result;
    int length, char_info_selc = 0;

    check_char:		// Length checker pointer
        length = (int)input.length();	// Calculate input string length
        // Check input line length is less than 3
        if (length <= 2)
        {
            cout << "enter at leaset 3 characters \n";
            getline(cin, input);		// Read input string
            goto check_char;
        }

        // Declare an arry for final result called 'result_ary'
        int** result_ary = new int*[3];
        for (int i = 0; i < length; ++i)
            result_ary[i] = new int[length];
        // Set result_ary elements value to 0 to prevent previous values
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < length; j++)
                result_ary[i][j] = 0;
        }

        // Declare an arry to store every char info in input string called 'char_info'
        int** char_info = new int*[3];
        for (int i = 0; i < length; ++i)
            char_info[i] = new int[length];
        // Set char_info elements value to 0 to prevent previous values
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < length; j++)
                char_info[i][j] = 0;
        }

        // Set first char info to (0,0,'<first char>')
        result_ary[0][0] = 0;
        result_ary[1][0] = 0;
        result_ary[2][0] = input[0];

        // Set result counter
        int result_count = 1;

        // A loop to perform some operations in every char in input string
        for (int i = 1; i < length; i++)
        {
            // A loop to check input char in position i is equal to any of
            // previous char in input and save this info in char_info array
            for (int j = 0; j < i; j++)
            {
                // Check position of previous view of element i
                if (input[i] == input[j])
                {
                    // Set position pointer
                    char_info[0][char_info_selc] = i - j;

                    // Increase char info array selector by 1
                    char_info_selc++;
                }

            }

            // A loop to check length for every char position
            for (int j = 0; j < length; j++)
            {
                // Check current char info array position is not equal to 0
                if (char_info[0][j] != 0)
                {
                    // Set start point
                    int start = i - char_info[0][j];

                    // Set count to calculate length for this char position
                    int count = 1;

                    // A loop to check length for this char position
                    for (int k = 0; k < length; k++)
                    {
                        // Check next element of start by next element of input
                        if (input[start + count] == input[i + count])
                            count++;	// Increase count by 1
                        else
                        {
                            char_info[1][j] = count;	// Store count value in length

                            // Check if this input char is the last char
                            if (i != (length - 1))
                            {
                                // Store next char to char info
                                // Check this postion is equal to length
                                if (char_info[0][j] + count == length)
                                    char_info[2][j] = 0;	// Set 0 in next char field
                                else
                                    char_info[2][j] = input[char_info[0][j] + count];	// Set the next char
                            }
                            else
                                char_info[2][j] = NULL;		// Set NULL in next char field

                            break;	// Stop loop
                        }
                    }
                }
            }

            // Set large selector
            int large = 0;	// large selector equal 0

            // Loop to check the largest length for every char info
            for (int k = 1; k < length; k++)
            {
                // Check largest
                if (char_info[1][large] == char_info[1][k])
                    large = k;	// Set largest
                else if (char_info[1][large] < char_info[1][k])
                    large = k;	// Set largest
            }

            // Check largest length is equal to 0
            if (char_info[1][large] == 0)
                char_info[2][large] = input[i];		// Set char info
            else
            {
                i += char_info[1][large];		// increase loop counter by length of the largest char info element
                char_info[2][large] = input[i];		// Set char info
            }

            // Set final result info
            result_ary[0][result_count] = char_info[0][large];
            result_ary[1][result_count] = char_info[1][large];
            result_ary[2][result_count] = char_info[2][large];

            // Increase result counter
            result_count++;

            // Prepare char info array for next char by set it to 0
            for (int z = 0; z < 2; z++)
            {
                for (int j = 0; j < length; j++)
                    char_info[z][j] = 0;	// Set every element in char info to 0
            }

            // Prepare char info selector for next char by set it to 0
            char_info_selc = 0;
        }

        // Display final results
        for (int j = 0; j < length; j++)
        {


            if (result_ary[0][j] == 0 && result_ary[1][j] == 0)
            {
                if (result_ary[2][j] != NULL || result_ary[2][j] != 0)
                {
                    char z = result_ary[2][j];
                    result += to_string(result_ary[0][j]) + "," + to_string(result_ary[1][j]) + "," + z + " ";
                }
            }
            else
            {
                char z = result_ary[2][j];
                result += to_string(result_ary[0][j]) + "," + to_string(result_ary[1][j]) + "," + z + " ";
            }
        }

        // Clean up memory
        for (int i = 0; i < 3; ++i) {
            {
                delete[] result_ary[i];	delete[] char_info[i];
            }
        }
        delete[] result_ary;
        delete[] char_info;

        return result;
    }



string LZ77(QString ruta){
    fstream myFile;
    myFile.open(ruta.toStdString(), ios::in);
    string line;
    string resultado = "";
    if(myFile.is_open()){

        getline(myFile,line);

        resultado = LZ77(line);
    }
    myFile.close();
    return resultado;

}


string LZ78(string input)
{
    Node *dictionary = new Node;
    string word, result;
    int length, last_seen, index = 1;

    length = (int)input.length();
    word = input[0];
    st_Node(dictionary, 1, word);
    result += "0," + word;

    for (int i = 1; i < length; i++)
    {
        string data;
        data = input[i];

    re_check:
        Node *search = search_Node(dictionary, data);

        if (search)
        {
            i++;
            data += input[i];
            last_seen = search->index;
            goto re_check;
        }
        else
        {
            char zero;
            if (input[i] == ' ')
                zero = '0';
            else
                zero = input[i];

            if ((int)data.length() < 2)
                result += " " + to_string(0) + "," + zero;
            else
                result += " " + to_string(last_seen) + "," + zero;

            index++;
            if (i != length)
                insert_Node(dictionary, index, data);
        }
    }

    return result;
}

string LZ78(QString ruta){
    fstream myFile;
    myFile.open(ruta.toStdString(), ios::in);
    string line;
    string resultado = "";
    if(myFile.is_open()){

        getline(myFile,line);
        resultado = LZ78(line);

    }
    myFile.close();
    return resultado;

}

QString MainWindow::Archivo(){ //Función solicita al usuario ingresar el archivo y el algoritmo a utilizar

    QString fileName = QFileDialog::getOpenFileName(this, "Ingrese el archivo a comprimir");

    int Algoritmo = QInputDialog::getInt(this, "Algoritmo a utilizar", "Ingrese el algoritmo que desea utilizar: "
                                                                       "\n1.Huffman"
                                                                       "\n2.LZW"
                                                                       "\n3.LZ77"
                                                                       "\n4.LZ78", 1,1,4);

    if(Algoritmo == 1){
        Huffman(fileName);//Llamada al algoritmo de Huffman
    }
    else if(Algoritmo == 2){
        LZW(fileName);//Llamada al algoritmo de LZW
    }
    else if(Algoritmo == 3){
        LZ77(fileName);//Llamada al algoritmo de LZ77
    }
    else{
        LZ78(fileName);//Llamada al algoritmo de LZ78
    }

    return fileName;
}





