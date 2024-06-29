#pragma once

#include <cstdint>
#include <iostream>
#include <list>
#include <cmath>


template<typename T>
class LineaCache {
    private:
        int etiquetaLinea;
        bool validez;
    public:
        LineaCache(){
            validez=false;
        }
        LineaCache(int nuevaEtiqueta){
            etiquetaLinea = nuevaEtiqueta;
            validez=false;
        }
        int getEtiqueta(){
            return etiquetaLinea;
        }
        bool getValidez(){
            return validez;
        }
        void setEtiqueta(int nuevaEtiqueta){
            etiquetaLinea = nuevaEtiqueta;
        }
        void setValidez(bool nuevaValidez){
            validez = nuevaValidez;
        }

};


template<typename T>
class ConjuntoCache{
    private:
        int indice;
        std::list<LineaCache<T>*> lineas;
        int lruIndice;
    public:


        //Constructor
        ConjuntoCache(int numLineas, int nuevoIndice){
            for(int i = 0; i < numLineas;i++){
                lineas.push_back(new LineaCache<T>(-1));
            }
            lruIndice = 0;
            indice = nuevoIndice;
        }

        //buscar si un bloque esta en la cache
        bool encontrarLineaCache(int etiquetaSolicitada){
            bool aux = false;
            int pos = 0;
            if(lineas.size()>1){
                typename std::list<LineaCache<T>*>::iterator it = lineas.begin();
                while(it != lineas.end() && !aux){
                    if((*it)->getValidez()){
                        if((*it)->getEtiqueta()==etiquetaSolicitada){
                            aux = true;
                            if(pos == lruIndice){
                                lruIndice = (lruIndice + 1) % lineas.size();
                            }
                        }else{
                            it++;
                            pos++;
                        }
                    }else{
                        it++;
                        pos++;
                    }
                }

            }else{
                if((*lineas.begin())->getValidez()){
                    if((*lineas.begin())->getEtiqueta()==etiquetaSolicitada){
                        aux = true;
                    }
                }
            }
            return aux;
        }

        //cargar un bloque en la cache
        void cargarLineaCache(int etiquetaInsertar){
            typename std::list<LineaCache<T>*>::iterator it = lineas.begin();
            //casos para asociativa por conjuntos y completamente asociativa
            if(lineas.size()>1){
                bool aux = false;
                int pos = 0;
                //buscando una linea de cache con bit de validez desactivado
                while(it != lineas.end() && !aux){
                    if(!((*it)->getValidez())){
                        (*it)->setValidez(true);
                        (*it)->setEtiqueta(etiquetaInsertar);
                        aux = true;
                    }else{
                        it++;
                    }
                }//caso en donde todos los bits estan activados asi que se reemplaza el lru
                if(!aux){
                    it = lineas.begin();
                    for(int i = 0; i < lruIndice; i++){
                        it++;
                    }
                    (*it)->setValidez(true);
                    (*it)->setEtiqueta(etiquetaInsertar);
                    lruIndice = (lruIndice + 1) % lineas.size();
                }
                //caso de cache de correspondencia directa
            }else{

                (*it)->setValidez(true);
                (*it)->setEtiqueta(etiquetaInsertar);
                
            }

        }

        int getIndice(){
            return indice;
        }

};



template <typename T>
class Cache{
    private:
        int nLineas;
        int nConjuntos;
        int tamBloque;
        std::list<ConjuntoCache<LineaCache<T>>> conjuntos; 
    public:

        Cache(int cantLineas, int cantConjuntos, int palabrasBloque){
            nLineas = cantLineas;
            nConjuntos = cantConjuntos;
            tamBloque = palabrasBloque; // de momento se ignora porque es 1 palabra por bloque
            
            int tipoCache;
            if(cantConjuntos == cantLineas) tipoCache = 1;   //caso de correspondencia directa
            if(cantConjuntos>1 && cantConjuntos < cantLineas) tipoCache = 2; //caso de asociativa por conjuntos
            if(cantConjuntos == 1) tipoCache = 3;    //caso de completamente asociativa

            switch(tipoCache){
                case 1:
                    for(int i=0; i < cantConjuntos;i++){
                        conjuntos.push_back(ConjuntoCache<LineaCache<T>>(1,i));
                    }
                break;

                case 2:
                    for(int i=0; i < cantConjuntos;i++){
                        conjuntos.push_back(ConjuntoCache<LineaCache<T>>(cantLineas/cantConjuntos,i));
                    }
                break;

                case 3:
                    conjuntos.push_back(ConjuntoCache<LineaCache<T>>(cantLineas,0));
                break;

                default:
                break;
            }

        }


        //retorna True para los Aciertos y False para los Fallos
        bool leerDireccion(int direccion){
            unsigned int etiqueta;
            unsigned int indice;
            unsigned int desplBytes;
            bool encontrado;

            //quitando el desplazamiento del byte de la direccion
            desplBytes = direccion % 1; //fijo porque son 4 bytes por palabra
            direccion = direccion >> 1;

            //cache de correspondencia directa y asociativa por conjuntos (mas de un conjunto)
            if(nConjuntos>1){
                //calculando el indice
                indice = direccion % int(round(log2(nConjuntos)));
                indice = indice << 1;

                //calculando la etiqueta
                etiqueta = direccion >>  int(round(log2(nConjuntos)));
                etiqueta = (etiqueta << int(round(log2(nConjuntos)))) << 1;

            }else{
                //caso de cache completamente asociativa
                indice = 0;
                etiqueta = direccion<<1;
            }

            //std::cout<< desplBytes <<std::endl;
            //std::cout<<"indice:"<< indice <<std::endl;
            //std::cout<< "etiqueta:"<<etiqueta <<std::endl;

        // recorriendo los conjuntos buscando el correspondiente con el indice
            typename std::list<ConjuntoCache<LineaCache<T>>>::iterator it = conjuntos.begin();
            while(it != conjuntos.end() && (*it).getIndice()!=indice){
                it++;
            }
            encontrado = (*it).encontrarLineaCache(etiqueta);
            if(!encontrado){
                (*it).cargarLineaCache(etiqueta);
                return encontrado;
            }
            return encontrado;
        }

};
