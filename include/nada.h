#include <iostream>
#include <fstream>

#include "ArrayList.h"
#include <string>

using namespace std;

string st;
string st02;
ArrayList<string> listaLineas = new ArrayList<string>(10000);



string formatoString(string str){

    for(int i=0;i< str.size();i++){
        if(str[i]==','|str[i]=='.'|str[i]==';'|str[i]=='!'){
            str.erase(i);
        }
    }
    return str;
}

template<typename T_it>
void SequenceToLowerCase( T_it begin, T_it end )
{
    for ( auto it = begin; it != end; ++it ){
        if(it != 0xA){
            it |= 0x20;
        }
        if(it == 0x2E){
            *it == 0x0;
        }
    }
}


int main(){
    ifstream ArchivoTexto("quijote.txt");
    while (getline (ArchivoTexto, st02)) {
        st02 = formatoString(st02);
        listaLineas->append(st02);
        //st += '\n';
    }
    SequenceToLowerCase(st.begin(),st.end());
    listaLineas->print();
    //listaLineas->goToPos(101);
    //cout<<listaLineas->getElement()<<endl;
    return 0;
}
