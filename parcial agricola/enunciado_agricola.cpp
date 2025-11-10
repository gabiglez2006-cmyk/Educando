#include <iostream>
#include <cstring>
#include "vector_dinamico_fran.h"

using namespace std;

char* copystr(const char* str) {
    if (!str) str = "";
    char* newstr = new char[strlen(str) + 1];
    strcpy(newstr, str);
    return newstr;
}

class Cosecha{
  private:
    char* nombre;
    int kg;
    char tipo;
  public:
    Cosecha(char* nombre,int kg,char tipo){
        this->nombre=copystr(nombre);
        this->kg=kg;
        this->tipo=tipo;
    }
    char* getnombre(){return this->nombre;}
    int getkg(){return this->kg;}
    char gettipo(){return this->tipo;}
    double getproductividad();
};
double  Cosecha :: getproductividad(){
    switch(gettipo()){
        case 'L':
            return (1.2*getkg());

        case 'O':
            return (1.5*getkg());

        case 'C':
            return (1.1*getkg());
    }
}

class  Cultivo{
    private:
        char* nombre;
        vectordinamico<Cosecha>cosechas;
    public:
        Cultivo(char* nombre){
            this->nombre=copystr(nombre);
        }
        char* getnombre(){return this->nombre;}
        double productividad_total();
       friend ostream& operator<<(ostream& os, Cultivo& p){}
       void add_cosecha(Cosecha& c);
};
void Cultivo::add_cosecha(Cosecha& c){
     cosechas.add(c);
}

double Cultivo :: productividad_total(){
    double tot=0;
    for(size_t i=0;i<cosechas.getsize();i++){
        tot+=cosechas[i].getproductividad();
    }
    return tot;
}
ostream& operator<<(ostream& os, Cultivo& p) {
    os<<"nombre"<<p.getnombre()<<"la productividad total: "<<p.productividad_total()<<endl;
    
    return os;
}

class Empresa{
 private:
    vectordinamico<Cultivo>cultivos;
 public:
    Empresa(){}
    void add_cultivo(Cultivo &c);
    void mostrar();
};
void Empresa::add_cultivo(Cultivo &c){
    cultivos.add(c);
}
void Empresa :: mostrar(){
    for(size_t i=0;i<cultivos.getsize();i++){
         cout<<cultivos[i];
    }
}
int main() {return 0;}