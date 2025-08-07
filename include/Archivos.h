#ifndef ARCHIVOS_H
#define ARCHIVOS_H
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ArrayList.h"

// CLASE QUE MANEJA ARCHIVOS.
// CREADA POR MARCO.

using namespace std;

class Archivos{

private:
    string nombreArchivo;
    ArrayList<string> *lista;

public:
    Archivos(string archivo) {
        nombreArchivo = archivo;
        lista = new ArrayList<string>(1000000);
    }

    string getArchivo(){
        return nombreArchivo;
    }

    void cargarArchivo(){
        ifstream archivo;
        archivo.open(nombreArchivo,ios::in);
        if (archivo.is_open()){
            string linea;
            while(getline(archivo, linea)){
                lista->append(linea);
            }
            archivo.close();
        }
    }

    ArrayList<string>* getLista() {
        return lista;
    }
};

#endif // ARCHIVOS_H
