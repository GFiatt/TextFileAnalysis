#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <windows.h>
#include <conio.h>

#include "ArrayList.h"
#include "Archivos.h"
#include "Trie.h"
#include "MaxHeap.h"
#include "BSTDictionary.h"
#include "HashTable.h"

using namespace std;
void menuPrincipal();
//Definición de Variables globales
string st;
string lineaTexto;
string nombreArchivo;
int opcion;
ArrayList<string> *listaLineasArregladas = new ArrayList<string>(4000000);
ArrayList<string> *listaLineas = new ArrayList<string>(4000000);
Trie *palabras = new Trie();
MaxHeap<KVPair<int, string>> *heap = new MaxHeap<KVPair<int, string>>();

void gotoxy(int x, int y) { //Función para implementar texto en coordenadas X e Y
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

string formatoString(string str){   //Subrutina que checkea el formato del string

        string resultado;
        for(int i=0;i< str.size();i++){
            if(isalpha(str[i])||str[i]==' '){
                resultado += str[i];
            }
        }
        return resultado;
    }

template<typename T_it>
    void SequenceToLowerCase( T_it begin, T_it end ){
        for ( auto it = begin; it != end; ++it ){
            if(*it != 0xA){
                *it |= 0x20;
            }
            if(*it == 0x2E){
                *it == 0x0;
            }
        }
    }

void agregarPalabras(Trie *triePalabras, ArrayList<string> *lista) {
    //Subrutina que inserta las palabras de la lista en el hash table y en el trie
    // Creado por Gabriel Nuñez.
    string palabra;
    for(int i=0;i<lista->getSize();i++){
        palabra="";
        lista->goToPos(i);
        for (int j=0;j<lista->getElement().size();j++){
            if(lista->getElement()[j]==0x20){
                triePalabras->insert(palabra,i);
                palabra="";
            }
            else{
                palabra+=lista->getElement()[j];
            }
        }
    }
}

void popularMaxHeap(MaxHeap<KVPair<int, string>> *heap, Trie *arbol){
    //subrutina que inserta llena el max heap utilizando los valores del hash table cuando está lleno
    // Creada Gabriel Nuñez
    KVPair<int,string> kvPair;
    List<KVPair<int, string>> *matches = arbol->getMatchesV2("");
    for (matches->atStart(); !matches->atEnd(); matches->next()) {
        kvPair = matches->getElement();\
        heap->insert(kvPair);
    }
}

void consultarPrefijo(){
    //subrutina encargada de consultar un prefijo y mostrar las palabras con ese prefijo junto a las veces que aparece.
    // Creada por Jose R.
    string prefix;

    gotoxy(10, 2);
    cout <<"===============Consultar Prefijo==============="<<endl;

    gotoxy(10, 4);
    cout<<"A continuación, usted podrá ingresar un prefijo y";
    gotoxy(10, 5);
    cout<<"se le mostrarán todas las palabras con ese prefijo.";
    gotoxy(10, 7);
    cout<<"Ingrese el prefijo: ";


    cin >> prefix;
    cout << endl;

    List<string> *prefijos = palabras->getMatches(prefix); // Guarda una lista de palabras con un prefijo.
    if (prefijos->getSize() == 0) {
        cout << "- No hay palabras con ese prefijo.";
    }
    for (prefijos->goToStart(); !prefijos->atEnd(); prefijos->next()) {
        cout << "- " << prefijos->getElement() <<"  " << palabras->getAparicionesPalabra(prefijos->getElement())->getSize() << endl << endl; //dictPalabras->getValue(prefijos->getElement()) << endl << endl;
    }
    delete prefijos;

    getchar();
    getchar();
    system("cls");
    menuPrincipal();
}

void buscarPalabra(){
    //subrutina que busca una palabra y se encarga de mostrar la linea donde aparece junto al número de linea.
    // Creada por Gabriel Fiatt.
    string palabra;

    gotoxy(10, 2);
    cout <<"===============Buscar Palabra==============="<<endl;

    gotoxy(10, 4);
    cout<<"A continuación, usted podrá ingresar un palabra y";
    gotoxy(10, 5);
    cout<<"se le mostrarán todas las ocurrencias de esa palabra.";
    gotoxy(10, 7);

    cout<<"Ingrese la palabra: ";

    cout<<palabra;

    getline(cin, palabra);

    if (palabras->containsWord(palabra)) {
        List<int> *palabrasEnTexto = palabras->getAparicionesPalabra(palabra);
        for (palabrasEnTexto->goToStart(); !palabrasEnTexto->atEnd(); palabrasEnTexto->next()) {
            cout << "Linea #" << palabrasEnTexto->getElement();
            listaLineas->goToPos(palabrasEnTexto->getElement());
            cout << "   " << listaLineas->getElement() << endl << endl;
        }
    }
    else {
        cout << "El texto no contiene la palabra " << palabra;
    }
    getchar();
    system("cls");
}

void buscarCantidadLetras(){
    //subrutina que busca palabras que contengan la cantidad de letras de letras ingresadas por el usuario e imprime las palabras.
    // Creado por Marco.
    int tam;
    string tamAux;

    gotoxy(10, 2);
    cout <<"===============Buscar Cantidad Letras==============="<<endl;

    gotoxy(10, 4);
    cout<<"A continuación, usted podrá ingresar un número y";
    gotoxy(10, 5);
    cout<<"se le mostrarán todas las palabras con ese tamaño.";
    gotoxy(10, 7);
    cout<<"Ingrese la cantidad: ";

    getline(cin,tamAux);
    try{
    tam = stoi(tamAux);
    }
    catch(exception& e){
        cout<<"Error:opcion invalida"<<endl;
        getchar();
        system("cls");
        return;
    }

    List<string> *cantidadLetras = palabras->getMatchesN(tam);
    if (cantidadLetras->getSize() == 0) {
        cout << "No hay palabras del largo ingresado.";
    }
    else {
        for (cantidadLetras->goToStart(); !cantidadLetras->atEnd(); cantidadLetras->next()) {
            int cantidad = palabras->getMatchesV2(cantidadLetras->getElement())->getSize(); //dictPalabras->getValue(cantidadLetras->getElement());
            cout <<"- " << cantidadLetras->getElement() << "     " << cantidad << endl;
        }
    }
    getchar();

    system("cls");
}

void palabrasMasUtilizadas(){
    //subrutina que genera un top de las palabras más utilizadas segun el input que le ingresa el usuario.
    // Creado por Gabriel Fiatt.
    int tam;
    string tamAux;

    gotoxy(10, 2);
    cout <<"===============Top de palabras más utilizadas==============="<<endl;

    gotoxy(10, 4);
    cout<<"A continuación, usted podrá ingresar un número y";
    gotoxy(10, 5);
    cout<<"se le mostrará el top de palabras más utilizadas en ese rango.";
    gotoxy(10, 7);
    cout<<"Ingrese la cantidad: ";

    getline(cin,tamAux);
    try{
    tam = stoi(tamAux);
    }
    catch(exception& e){
        cout<<"Error:opcion invalida"<<endl;
        getchar();
        system("cls");
        return;
    }


    if (tam > heap->getSize()) {
        cout << endl << "- Cantidad excedida.";
        getchar();
        system("cls");
        menuPrincipal();
    }
    else {
        List<KVPair<int, string>> *top = heap->removeTop(tam);
        int contador = 0;
        for (top->goToStart(); !top->atEnd(); top->next()) {
            contador++;
            cout << contador << "." << top->getElement().value << "   ";
            cout << top->getElement().key << endl;
        }
    }

    getchar();
    system("cls");

    heap->clear();
    popularMaxHeap(heap, palabras);
}

void cargarOtroArchivo(){
    //subrutina encargada de cargar un archivo nuevo, limpiando todo lo relacionado con el anterior para que no quede almacenado en memoria.
    // Creado por Marco.
    string nombreArchivo;

    gotoxy(10, 2);
    cout <<"===============Cargar otro Archivo==============="<<endl;

    gotoxy(10, 4);
    cout<<"A continuación, ingrese el nombre de otro archivo para";
    gotoxy(10, 5);
    cout<<"volver a probar el programa.";
    gotoxy(10, 7);
    cout<<"Ingrese el nombre del archivo: ";

    cin.clear();
    getline(cin, nombreArchivo);

    try{
        string rutaCompleta = "textos/" + nombreArchivo;
        ifstream ArchivoTexto(rutaCompleta);
        if(ArchivoTexto.fail()){
            cout<<"Error: Archivo no encontrado."<<endl;
            getchar();
            system("cls");
            return;
        }
        delete listaLineasArregladas;
        delete listaLineas;
        delete palabras;
        delete heap;
        listaLineasArregladas = new ArrayList<string>(4000000);
        listaLineas = new ArrayList<string>(4000000);
        palabras = new Trie();
        heap = new MaxHeap<KVPair<int, string>>();
        while (getline (ArchivoTexto, lineaTexto)) {
            listaLineas->append(lineaTexto);
            SequenceToLowerCase(lineaTexto.begin(),lineaTexto.end());
            lineaTexto = formatoString(lineaTexto);
            listaLineasArregladas->append(lineaTexto);
        }
        ArchivoTexto.close();
    }
    catch(exception const& E){
        cout<<"Error: "<<E.what()<<endl;
        return;
    }
    system("CLS");

    agregarPalabras(palabras, listaLineasArregladas);
    popularMaxHeap(heap, palabras);

    menuPrincipal();
    getchar();
    getchar();
    system("cls");
}

void menuPrincipal(){
    //subrutina encargada de mostrar el menu principal donde el usuario seleccione una opción para ejecutar.
    // Creaod por Gabriel Fiatt.
    char key;
    int counter = 1;

    for (int i = 0 ;;) {
            fflush(stdin);
        gotoxy(30, 2);
        cout <<"===============Menu===============";

        gotoxy(30, 4);
        cout<<"1. Consulta por prefijo                                      ";

        gotoxy(30, 5);
        cout<<"2. Buscar Palabra                                            ";

        gotoxy(30, 6);
        cout<<"3. Buscar por cantidad de letras                             ";

        gotoxy(30, 7);
        cout<<"4. Palabras mas utilizadas                                   ";

        gotoxy(30, 8);
        cout<<"5. Cargar otro archivo                                       ";

        gotoxy(30, 9);
        cout<<"Salir                                                        ";

        if (counter == 1) {
            gotoxy(64, 4);
            cout << "<---";
        }
        if (counter == 2) {
            gotoxy(64, 5);
            cout << "<---";
        }
        if (counter == 3) {
            gotoxy(64, 6);
            cout << "<---";
        }
        if (counter == 4) {
            gotoxy(64, 7);
            cout << "<---";
        }
        if (counter == 5) {
            gotoxy(64, 8);
            cout << "<---";
        }
        if (counter == 6) {
            gotoxy(64, 9);
            cout << "<---";
        }

        key = _getch();

        if ((key == 72) && (counter >= 2 && counter <= 6)) // 72 es la flecha de arriba.
            counter--;
        if ((key == 80) && (counter >= 1 && counter <= 5)) // 80 es la flecha de abajo.
            counter++;
        if (key == '\r') {
            if (counter == 1) {
                system("cls");
                consultarPrefijo();
            }
            if (counter == 2) {
                system("cls");
                buscarPalabra();
            }
            if (counter == 3) {
                system("cls");
                buscarCantidadLetras();
            }
            if (counter == 4) {
                system("cls");
                palabrasMasUtilizadas();
            }
            if (counter == 5) {
                system("cls");
                cargarOtroArchivo();
            }
            if (counter == 6) {;
                system("cls");
                exit(0);
            }
        }

    }
}

int main() {
    // Funcion principal del programa. Corre todas las subrutinas necesarias para
    // hacer el proyecto.
    // Creado por todo el grupo.
    setlocale(LC_ALL, "spanish");
    SetConsoleCP(1252);
    SetConsoleOutputCP(1252);

    //Se imprime mensaje de bienvenida
    gotoxy(10, 2);
    cout<<"Bienvenido al programa! El proposito de este es cargar un archivo de texto";
    gotoxy(10, 3);
    cout << "al que se lo pueden hacer diferente consultas relacionadas con su contenido";
    gotoxy(10, 4);
    cout << "============================================================================";
    gotoxy(10, 6);
    cout<<"Ingrese nombre del archivo de texto que desea abrir: ";
    getline(cin, nombreArchivo);


    //Cargar archivo con el nombre proporcionado por el usuario en consola
    try{
        string rutaCompleta = "textos/" + nombreArchivo;
        ifstream ArchivoTexto(rutaCompleta);
        if(ArchivoTexto.fail()){
            system("cls");
            cout<<"Error: Archivo no encontrado."<<endl;
            getchar();
            system("cls");
            main();
            return 0;
        }
        while (getline (ArchivoTexto, lineaTexto)) {
            listaLineas->append(lineaTexto);
            SequenceToLowerCase(lineaTexto.begin(),lineaTexto.end());
            lineaTexto = formatoString(lineaTexto);
            listaLineasArregladas->append(lineaTexto);
        }
        ArchivoTexto.close();

    }
    catch(exception const& E){
        cout<<"Error: "<<E.what()<<endl;
        return 0;
    }
    system("CLS");

    agregarPalabras(palabras, listaLineasArregladas);
    popularMaxHeap(heap, palabras);
    menuPrincipal();

    return 0;
}

