#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main(){
    ifstream archivo; //variable de archivo
    archivo.open("DireccionesMemoria.txt"); //abriendo el archivo
    if(archivo.fail()){ //validacion de apertura
        cout<<"Error abriendo el archivo"<<endl;
    }else{
    const int lineasCache =4;   //lineas o bloques de una cache
    const int conjuntosCache =2;    //conjuntos de la cache asociativa por conjuntos
    const int lineasPorConjunt =2;  //lineas o bloques por conjunto
    const int palabrasBloque = 1;   //palabras por linea o bloque

    //Inicializando la cache
    vector<int>bloquesCache(4,-1);
    int opcionMenu = 0; //opcion del menu
    //variables parar cada caso
    string direccion ;
    int direccionAux;
    int direccionCache;
    //asociativa por conjuntos
    int conjuntoInicio1 =0;
    int conjuntoFinal1 =1;
    int conjuntoInicio2 =2;
    int conjuntoFinal2 =3;
    int lruConjunto1=0;
    int lruConjunto2=2;
    int indice;
    bool encontrado;
    //completamente asociativa
    int lru =0;
    int contador = 0;
    //Menu
    cout << "Seleccione el tipo de cache a utilizar " << endl;
    cout << "1- Correspondencia Directa"<< endl;
    cout << "2- Asociativa por conjuntos"<< endl;
    cout << "3- Completamente asociativa"<< endl; 
    cin >> opcionMenu;
    switch(opcionMenu){

        case 1: //correspondencia directa

            while(!archivo.eof()){
                getline(archivo,direccion); //leyendo la linea
                direccionAux = stoi(direccion); //convirtiendo de cadena a entero
                direccionCache = direccionAux % lineasCache;
                if(bloquesCache[direccionCache] == direccionAux){
                    cout<<"Acierto"<<endl;
                }else{
                    bloquesCache[direccionCache-1] = direccionAux;
                    cout<<"Fallo"<<endl;
                }
            }
            break;

        case 2: //asociativa por conjuntos de 2 vias
            while(!archivo.eof()){
                getline(archivo,direccion); //leyendo la linea
                direccionAux = stoi(direccion); //convirtiendo de cadena a entero
                direccionCache = direccionAux % conjuntosCache;
                encontrado = false;
                if(direccionCache==0){//primer conjunto
                    indice = conjuntoInicio1;
                    while(indice <= conjuntoFinal1 && !encontrado){
                        if(bloquesCache[indice] == direccionAux){//Acierto
                            encontrado = true;
                            cout<<"Acierto"<<endl;
                            if(lruConjunto1==indice){
                                lruConjunto1++;
                                if(lruConjunto1>conjuntoFinal1){
                                    lruConjunto1 = conjuntoInicio1;
                                }
                            }else{
                                lruConjunto1++;
                            }               
                        }else{
                            indice ++;
                        }
                    }
                    //Caso de fallo
                    if(encontrado ==false){
                        cout<<"Fallo"<<endl;
                        bloquesCache[lruConjunto1] = direccionAux;
                        if(lruConjunto1>=conjuntoFinal1){
                            lruConjunto1 = conjuntoInicio1;
                        }else{
                            lruConjunto1++;
                        }
                    }
                    
                }else{
                    //segundo conjunto
                    indice = conjuntoInicio2;
                    while(indice <= conjuntoFinal2){
                    indice = conjuntoInicio2;
                        while(indice < conjuntoFinal2){
                            if(bloquesCache[indice] = direccionAux){//Acierto
                                encontrado = true;
                                cout<<"Acierto"<<endl;
                                if(lruConjunto2==indice){
                                    lruConjunto2++;
                                    if(lruConjunto2>conjuntoFinal2){
                                        lruConjunto2 = conjuntoInicio2;
                                    }
                                }else{
                                    lruConjunto2++;
                                }
                            }else{
                                indice ++;
                            }
                        }
                    }
                    //Caso de fallo
                    if(encontrado ==false){
                        cout<<"Fallo"<<endl;
                        bloquesCache[lruConjunto2] = direccionAux;
                        if(lruConjunto2>=conjuntoFinal2){
                            lruConjunto2 = conjuntoInicio2;
                        }else{
                            lruConjunto2++;
                        }
                    }
                }
            }
            for(int i=0; i < lineasCache;i++){
                cout<<bloquesCache[i];
            }
            break;

        case 3://completamente asociativa 

            while(!archivo.eof()){
                getline(archivo,direccion); //leyendo la linea
                direccionAux = stoi(direccion); //convirtiendo de cadena a entero
                encontrado = false;
                indice =0;
                while(indice < lineasCache && !encontrado){ //recorriendo la cache buscando la direccion
                    if(bloquesCache[indice]==direccionAux){ //acierto
                        cout<<"Acierto"<<endl;
                        encontrado = true;
                        if(lru == indice){
                            lru++;
                            if(lru>=lineasCache){
                                lru = 0;
                            }
                        }
                    }
                    indice++;
                }
                if(encontrado == false){    //fallo
                    cout<<"Fallo"<<endl;
                    if(bloquesCache[contador]!=-1){ //añadiendo la la direccion a uno de los bloques de la cache
                        lru++;
                        if(lru>=lineasCache){
                            lru = 0;
                        }
                    }
                    bloquesCache[contador] = direccionAux;
                    contador++;
                    if(contador >= lineasCache){
                        contador = 0;
                    }
                }
            }

            break;
        default:
            cout<<"Invalid option"<<endl;
            break;
        }
    }
        archivo.close();

    return 0;
}