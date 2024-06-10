#include <iostream>
#include <fstream>
#include <list>
#include "cache.hpp"

//Proyecto de Memoria Cache 
//Primera entrega
// Juan Diego Suarez CI:28 083 693

int main(){
    std::ifstream file("DireccionesMemoria.txt");
    std::list<int>valores;
    std::ofstream resultado("ResultadosDirecciones.txt");
    int valor;
    bool aux;

    //leyendo las direcciones del archivo y guardandolos en una lista
    while(!file.eof()){
        file >> valor;
        valores.push_back(valor);
    }
    file.close();

    //creando los 3 tipos de cache
    Cache<int>correspondenciaDirecta(4,4,1);
    Cache<int>asociativaPorConjuntos(4,2,1);
    Cache<int>completamenteAsociativa(4,1,1);

    //Escribiendo los diferentes resultados al archivo

    resultado << "Correspondencia directa\n";
    for(std::list<int>::iterator it = valores.begin(); it != valores.end();it++){
        if(correspondenciaDirecta.leerDireccion(*it)){
            resultado<<"Acierto\n";
        }else{
            resultado<<"Fallo\n";
        }
    }

    resultado<<"\n";

    resultado << "Asociativa por Conjuntos\n";
    for(std::list<int>::iterator it = valores.begin(); it != valores.end();it++){
        if(asociativaPorConjuntos.leerDireccion(*it)){
            resultado<<"Acierto\n";
        }else{
            resultado<<"Fallo\n";
        }
    }

    resultado<<"\n";

    resultado << "Completamente Asociativa\n";
    for(std::list<int>::iterator it = valores.begin(); it != valores.end();it++){
        if(completamenteAsociativa.leerDireccion(*it)){
            resultado<<"Acierto\n";
        }else{
            resultado<<"Fallo\n";
        }
    }

    resultado.close();

    return 0;
}