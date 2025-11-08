#include <QCoreApplication>
#include <cstring>   // <-- AÑADE ESTO
#include <ostream>
using namespace std;

char* copystr(const char* str){
    char* newstr=new char[strlen(str)+1];
    strcpy(newstr,str);
    return newstr;
}



class Producto{
protected:
    char* nombre;
    double precio;

public:
    Producto(char* nombre, double precio){
        this->nombre=copystr(nombre);
        this->precio=precio;
    }
    virtual void imprimir(std::ostream& os)=0;
    double get_precio();
    char* get_nombre();
};

std::ostream& operator<<(std::ostream& os, Producto& p) {
    p.imprimir(os);
    return os;
}




class Nacional : public Producto{
public:
    Nacional(char* nombre, double precio):Producto(nombre,precio){};
    void imprimir(std::ostream& os);
    void actualizar_p(double porcentaje);

};

void Nacional :: actualizar_p(double porcentaje){
    this->precio*=((porcentaje/100)+1);
}

void Nacional :: imprimir(std::ostream& os){
    os<<"Producto: "<<this->nombre<<" precio: "<<this->precio;
}

class Extranjero : public Producto{
public:
    Extranjero(char* nombre, double precio):Producto(nombre,precio){};
    void actualizar_p(double dolar_antes,double dolar);
    void imprimir(std::ostream& os);

};

void Extranjero::actualizar_p(double dolar_antes,double dolar){
    this->precio=(this->precio/dolar_antes)*dolar;
}

void Extranjero :: imprimir(std::ostream& os){
    os<<"Producto: "<<this->nombre<<" precio: "<<this->precio;
}

class Oferta {
private:
    vector<Producto*>ofertas;
public:
    void add_p(Producto* p);
    void imprimir(std::ostream& os);
    void eliminar(Producto* p);
};

void Oferta::add_p(Producto* p){
    ofertas.push_back(p);
}
void Oferta :: imprimir(std::ostream& os){
    os<<"ofertas"<<endl;
    for(int i =0;i<ofertas.size();i++){
        os<<"Producto: "<<ofertas[i]->get_nombre()<<" precio: "<<ofertas[i]->get_precio();
    }
}
void Oferta::eliminar(Producto* p){
 //Ofertas.removerData(p)
 }

class Empresa{

};




