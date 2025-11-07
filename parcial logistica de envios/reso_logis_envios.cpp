#include <iostream>
#include <cstring>
#include "vector_dinamico_fran.h"
using namespace std;
class Envio{
    protected:
    char* codigo;
    double monto;
    char* copystr(const char* str) {
     if (!str) str = "";
        char* newstr = new char[strlen(str) + 1];
        strcpy(newstr, str);
        return newstr;
    }
    public:
        Envio(char* codigo,double monto){
            this->codigo=copystr(codigo);
            this->monto=monto;
        }
        virtual double getmonto()=0;
        char* getcodigo(){return this->codigo;}
    
        friend ostream& operator<<(ostream& os, Envio& p);
};
ostream& operator<<(ostream& os,Envio&p){
    os<<p.getcodigo()<<endl;
    os<<p.getmonto()<<endl;
    return os;
}
class Envio_comun: public Envio{
    public:
    Envio_comun(char* codigo,double precio):Envio(codigo,precio){}
    double getmonto();
};

double Envio_comun ::getmonto(){
    return this->monto;
}

class Envio_certificado: public Envio{
    private:
    int km;
    double seguro;
    public:
    Envio_certificado(char* codigo,double monto,int km,double seguro):Envio(codigo,monto){
        this->km=km;
        this->seguro=seguro;
    }
    double getmonto();
};

double Envio_certificado ::getmonto(){
    return this->monto+(seguro*km);
}

class Envio_peso: public Envio{
    private:
    double peso;
    public:
    Envio_peso(char* codigo,double monto,double peso):Envio(codigo,monto){
        this->peso=peso;
    }
    double getmonto();
};

double Envio_peso ::getmonto(){
    return this->monto*(1+peso/100.0);
}


class gestora{
    private:
        vectordinamico<Envio*>envios;
   
    public:
        void add_envios(Envio* a);
        void mostrar_envios();
};



void gestora::add_envios(Envio* a){
    envios.add(a);
}



void gestora::mostrar_envios(){
    for(size_t i=0;i<envios.getsize();i++){
        cout<< *(envios[i])<<endl;
    }
};


int main(int argc, char *argv[]) {	
	return 0;
}

