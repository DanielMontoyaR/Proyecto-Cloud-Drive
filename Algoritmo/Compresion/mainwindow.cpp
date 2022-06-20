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

//--------------------------------------Inicio Huffman-----------------------------------------------

//Creación de una estructura Node que permite la aplicación del algoritmo de Huffman y el LZ78
struct Node{

    //Datos para el algoritmo de Huffman
    char simbolo;
    int frecuencia;
    Node *izq;//Punteros de izquierda y derecha del árbol de huffman
    Node *der;

    //Datos para el algoritmo LZ78
    int index;
    string data;
    Node *next;
};

//Funcion para asignar un nuevo nodo al arbol de Huffman
Node* getNode(char simbolo, int frecuencia, Node* izq, Node* der){
    Node* nodo = new Node();

    //Asignación de los valores
    nodo->simbolo = simbolo;
    nodo->frecuencia=frecuencia;
    nodo->izq=izq;
    nodo->der=der;

    return nodo;
}


//Objeto de comparación que será usado para ordenar la pila

struct comp{
    bool operator()(Node* i, Node* d){
        return i->frecuencia > d->frecuencia;//Retorna la frecuencia más baja en la cola de prioridad (priority_queue pq)
    }
};

//Revisa el arbol de huffman y guarda los códigos en un mapa
void compressHuff(Node* raiz, string str, unordered_map<char, string> &CodigoHuffman){//Toma el código ingresado por el usuario y le asigna los 1 y los 0 para cada código.
    if (raiz == nullptr){
        return;
    }
    if(!raiz->izq && !raiz->der){
        CodigoHuffman[raiz->simbolo]=str;
    }
    compressHuff(raiz->izq,str+"0",CodigoHuffman);
    compressHuff(raiz->der,str+"1",CodigoHuffman);
}

//Revisa el arbol y decodifica los simbolos codificados
void decompressHuff(Node* raiz, int &index, string str){ //Revisa el árbol resultante y descomprime el
    if(raiz == nullptr){
        return;
    }
    if(!raiz->izq && !raiz->der){
        cout << raiz->simbolo;
        return;
    }
    index++;
    if(str[index]=='0'){
        decompressHuff(raiz->izq,index,str);
    }
    else{
        decompressHuff(raiz->der, index,str);
    }
}

//Crea el arbol de Huffman
string CrearArbol(string text){
    //Cuenta la cantidad de veces que aparece cada símbolo y la guarda en el mapa
    unordered_map<char,int> frecuencia; //Mapa que toma el texto ingresado por el usuario y cuenta la frecuencia en la que aparece cada símbolo
    for(char simbolo: text){
        frecuencia[simbolo]++;
    }

    //Crea una cola de prioridad para guardar los nodos del arbol
    priority_queue<Node*,vector<Node*>, comp> pq;

    //Agrega a la cola de prioridad los nodos de cada símbolo
    for(auto pair:frecuencia){
        pq.push(getNode(pair.first,pair.second,nullptr,nullptr));//Colocar los caracteres y su frecuencia a la cola de prioridad
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
    unordered_map<char,string> CodigoHuffman; //Se establece otro mapa que contiene los códigos o resultados al comprimir el texto ingresado por el usuario
    compressHuff(raiz, "", CodigoHuffman);//Función que comprime el mensaje
    //Los códigos huffman son
    for(auto pair: CodigoHuffman){
      cout << pair.first << "  " << pair.second << '\n';
    }
    //Imprime el original
    cout <<"El original era: "<< text <<'\n';

    //Imprime el texto comprimido
    string str;//Variable de retorno que contiene el resultado de comprimir el texto de huffman
    for(char simbolo:text){
        str += CodigoHuffman[simbolo];
    }
    cout <<"El texto comprimido es"<< str;

    int index = -1;
    cout <<"El texto descomprimido es";
    while(index < (int)str.size()-2){
        decompressHuff(raiz,index,str);
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
//--------------------------------------Fin Huffman-----------------------------------------------



//--------------------------------------Inicio LZW-----------------------------------------------
vector<int> compressLZW(string s1) //Función que implementa
{
    cout << "Encoding\n";
    unordered_map<string, int> table;//Creación de un mapa que contiene el diccionario con su valor en el ASCII
    for (int i = 0; i <= 255; i++) {//El ciclo asigna al diccionario los chars con su respectivo valor entero (de la tabla ascii)
        string ch = "";
        ch += char(i);//Char referente al caracter del valor i en la tabla ASCII
        table[ch] = i; //Asigna el int al diccionario
    }

    string p = "", c = "";
    p += s1[0];//String que contiene el valor inicial del texto ingresado por el usuario
    int code = 256;//Codeword mayor al mayor valod de la tabla ASCII
    vector<int> output_code;//Creación de una pila que contiene los códigos del texto comprimido.
    cout << "String\tOutput_Code\tAddition\n";
    for (int i = 0; i < s1.length(); i++) {//Ciclo que recorre el largo del texto a comprimir.
        if (i != s1.length() - 1)//Compara si el valor de el indice del ciclo es diferente al tamaño del texto -1.
            c += s1[i + 1];
        if (table.find(p + c) != table.end()) {//Condición que evalúa si el nuevo valor no se encuentra en el diccionario
            p = p + c;
        }
        else {//Si el nuevo valor se encuentra en el diccionario
            cout << p << "\t" << table[p] << "\t\t"
                 << p + c << "\t" << code << endl;
            output_code.push_back(table[p]);//Se actualiza la salida para asignar el código del diccionario del string p
            table[p + c] = code;//Asigna al diccionario el nuevo código y se incrementa el valor del nuevo Codeword
            code++;
            p = c;//Se iguala el antiguo valor de char al nuevo
        }
        c = "";//Se borra el valor del nuevo char para continuar el ciclo
    }
    cout << p << "\t" << table[p] << endl;
    output_code.push_back(table[p]); //Se actualiza la salida para asignar el código del diccionario del string p
    return output_code;//Se retorna el valor del
}

void decompressLZW(vector<int> op)//Algoritmo que descomprime los códigos LZW.
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
    myFile.open(ruta.toStdString(), ios::in);//Abre el archivo de acuerdo con la ruta ingresada
    string line;
    string resultado = "";
    if(myFile.is_open()){

        getline(myFile,line);

        vector<int> output_code = compressLZW(line);//Llamada al algoritmo de compresión.
        std::stringstream result;

        std::copy(output_code.begin(),output_code.end(),std::ostream_iterator<int>(result, " "));
        resultado = result.str();
    }
    myFile.close();
    return resultado;
}

//--------------------------------------Fin LZW-----------------------------------------------




//--------------------------------------Inicio LZ77-----------------------------------------------

string LZ77(string input)
{
    //Se inicializan las variables a utilizar el algoritmo de LZ77
    string result;
    int length, char_info_selc = 0; //char_info_selc es el selector de informacion de caracteres.

    check_char:
        length = (int)input.length();	// Se obtiene el largo del string de entrada
        // Evaluar si el largo del input es menor a 3.
        if (length <= 2)
        {
            cout << "Ingrese 3 caracteres \n";
            getline(cin, input);		// Lee el texto de entrada
            goto check_char;
        }

        // Creación de una matriz para el resultado de la compresion
        int** result_ary = new int*[3];
        for (int i = 0; i < length; ++i)
            result_ary[i] = new int[length];
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < length; j++)
                result_ary[i][j] = 0;//Se le asignasn todos los valores en 0 para evitar valores erróneos.
        }

        int** char_info = new int*[3];//Array que almacena la información de cada caracter
        for (int i = 0; i < length; ++i)
            char_info[i] = new int[length];

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < length; j++)
                char_info[i][j] = 0;//Se le asignasn todos los valores en 0 para evitar valores erróneos.
        }

        //Asigna al primer char su información incial (0,0,char)
        result_ary[0][0] = 0;
        result_ary[1][0] = 0;
        result_ary[2][0] = input[0];

        int result_count = 1;// Contador que permite obtener los resultados.

        // Ciclo que permite realizar las operaciones del algoritmo al texto de entrada
        for (int i = 1; i < length; i++)
        {
            // Ciclo que permite evaluar si el caracter ya existe dentro de la lista char_info.
            for (int j = 0; j < i; j++)
            {

                if (input[i] == input[j])// Se evalúa si el valor existe
                {

                    char_info[0][char_info_selc] = i - j;// Se establece el puntero de posición


                    char_info_selc++;// Icremento del car_info_selc en 1
                }

            }

            // Ciclo que evalua la longitud de cada posición de caracteres.
            for (int j = 0; j < length; j++)
            {
                // Evaluar que la posición actual de ma matriz de info caracteres sea distinta de 0
                if (char_info[0][j] != 0)
                {

                    int start = i - char_info[0][j];// Variable que es el punto de inicio.

                    int count = 1;// Contador que calcula el largo para una posición específica del char


                    for (int k = 0; k < length; k++)// Ciclo que evalúa la longitud de una posición específica de caracter
                    {

                        if (input[start + count] == input[i + count])// Evalúa el siguiente elemento de inicio por el siguiente elemento de entrada
                            count++;	// Iincremento del contador
                        else
                        {
                            char_info[1][j] = count;	// Almacenar el valor de conteo

                            if (i != (length - 1))//Evaluar si el input char es el último.
                            {
                                // Almacenar siguiente info de caracter.
                                if (char_info[0][j] + count == length)
                                    char_info[2][j] = 0;	// Set 0 in next char field
                                else
                                    char_info[2][j] = input[char_info[0][j] + count];	// Set the next char
                            }
                            else
                                char_info[2][j] = NULL;		// Establece null el siguiente campo de caracteres

                            break;	// Stop loop
                        }
                    }
                }
            }

            int large = 0;	// Selector de largo igual a 0

            for (int k = 1; k < length; k++)//Ciclo que verifica la longitud más grande para cada char_info
            {
                // Evalúa el largo más grande para cada char_info
                if (char_info[1][large] == char_info[1][k])
                    large = k;	// Establece como el más grande
                else if (char_info[1][large] < char_info[1][k])
                    large = k;	// Establece como el más grande
            }

            // Evalúa si el largo más grande es igual a 0
            if (char_info[1][large] == 0)
                char_info[2][large] = input[i];		// Establece el char_info el valor input[i]
            else
            {
                i += char_info[1][large];		// increase loop counter by length of the largest char info element
                char_info[2][large] = input[i];		// Set char info
            }

            // Establece la información de resultado final
            result_ary[0][result_count] = char_info[0][large];
            result_ary[1][result_count] = char_info[1][large];
            result_ary[2][result_count] = char_info[2][large];

            result_count++;//Incremento en el contador de resultado

            // Prepare char info array for next char by set it to 0
            // Establece la matriz de información para que el siguiente caracter se establezca en 0
            for (int z = 0; z < 2; z++)
            {
                for (int j = 0; j < length; j++)
                    char_info[z][j] = 0;	// Se establecen todos los elementos del char_info en 0
            }

            // Se establece el char_info_selc para que el siguiente char sea 0
            char_info_selc = 0;
        }

        // Ciclo que muestran los resultados finales
        for (int j = 0; j < length; j++)
        {


            if (result_ary[0][j] == 0 && result_ary[1][j] == 0)
            {
                if (result_ary[2][j] != NULL || result_ary[2][j] != 0)
                {
                    char z = result_ary[2][j];
                    result += to_string(result_ary[0][j]) + "," + to_string(result_ary[1][j]) + "," + z + " ";//Se coloca en el string result el resultado final del algoritmo de compresíón
                }
            }
            else
            {
                char z = result_ary[2][j];
                result += to_string(result_ary[0][j]) + "," + to_string(result_ary[1][j]) + "," + z + " ";//Se coloca en el string result el resultado final del algoritmo de compresíón
            }
        }

        // Se limpia la memoria
        for (int i = 0; i < 3; ++i) {
            {
                delete[] result_ary[i];	delete[] char_info[i];
            }
        }
        delete[] result_ary;
        delete[] char_info;

        return result;
    }



string LZ77(QString ruta){//Algoritmo LZ77
    //Se abre el archivo
    fstream myFile;
    myFile.open(ruta.toStdString(), ios::in);
    //Se crea una variable line que contiene el valor al leer la línea del archivo y un string resultado que es el texto comprimido.
    string line;
    string resultado = "";
    if(myFile.is_open()){

        getline(myFile,line);

        resultado = LZ77(line);
    }
    myFile.close();
    return resultado;

}

//--------------------------------------Fin LZ77-----------------------------------------------


//--------------------------------------Fin LZ78-----------------------------------------------
void st_Node(Node *head, int index, string data){//Metodo que contien el indice, la información y el valor siguiente
    head->index = index;
    head->data = data;
    head->next = NULL;
}

void insert_Node(Node *head, int index, string data){//Metodo que inserta un nodo al arbol "trie"
    Node *new_Node = new Node;//Se crea un nuevo objeto nodo y se agregan sus variables iniciales
    new_Node->index = index;
    new_Node->data = data;
    new_Node->next = NULL;

    Node *curr = head;//Cabeza del árbol

    while (curr != NULL)//Ciclo que actualiza e inserta los nodos
    {
        if (curr->next == NULL)
        {
            curr->next = new_Node;//Se insterta el nuevo Nodo
            return;
        }
        curr = curr->next;
    }
}

Node *search_Node(Node *head, string data)//Función que busca la información de un nodo dado
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

Node *search_Node(Node *head, int index)//Función que busca el índice de un nodo dado
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

bool delete_Node(Node *head, Node *to_delete){//Función que borra un nodo dado.

    if (to_delete == NULL)
        return false;
    else if (to_delete == head)
    {
        head = to_delete->next;//Se remplaza el nodo principal
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
                delete to_delete;//FUnción que borra el nodo
                return true;
            }
            curr = curr->next;
        }
        return false;
    }
}



string LZ78(string input)
{
    Node *dictionary = new Node;//Se crea el diccionario
    string word, result;//Se crean las variables palabra y resultado
    int length, last_seen, index = 1;

    length = (int)input.length();//Largo del texto de entrada
    word = input[0];//Caracter en la posición inicial
    st_Node(dictionary, 1, word);//Llamada al método que para asignar el indice, la información y el valor siguiente
    result += "0," + word; //Resultado inicial de la compresión.

    for (int i = 1; i < length; i++)//Ciclo que recorre el largo del texto
    {
        string data;
        data = input[i];//Se asigna a la variable data el primer valor del texto de entrada

    re_check:
        Node *search = search_Node(dictionary, data);//Se busca la info de un nodo dado

        if (search)//Si no retorna NULL se incrementa el índice, se asigna la variable data el siguiente valor del texto de entrada, y se asigna el último indice visto a la variable last_seen
        {
            i++;
            data += input[i];
            last_seen = search->index;
            goto re_check;
        }
        else
        {
            char zero;
            if (input[i] == ' ')//Si la entrada de texto es un espacio
                zero = '0';
            else
                zero = input[i];//Si no se le asigna a la variable zero el valor de la entrada de texto en la posición i

            if ((int)data.length() < 2)//Evalúa si el largo del data es menor a dos
                result += " " + to_string(0) + "," + zero;//Asigna al resultado el valor de 0 más el valor del char zero
            else
                result += " " + to_string(last_seen) + "," + zero;//Asigna al resultado el indice del valor antes visto y el char zero

            index++;//INcremento en el índice
            if (i != length) //Si el i es distinto al largo
                insert_Node(dictionary, index, data);//Se inserta el nodo
        }
    }

    return result;//Valor de retorno
}

string LZ78(QString ruta){
    //Se abre el archivo
    fstream myFile;
    //Se crea una variable line que contiene el valor al leer la línea del archivo y un string resultado que es el texto comprimido.
    myFile.open(ruta.toStdString(), ios::in);
    string line;
    string resultado = "";
    if(myFile.is_open()){

        getline(myFile,line);
        resultado = LZ78(line);

    }
    myFile.close();
    return resultado;//Se retorna el resultado

}

//--------------------------------------Fin LZ78-----------------------------------------------

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





