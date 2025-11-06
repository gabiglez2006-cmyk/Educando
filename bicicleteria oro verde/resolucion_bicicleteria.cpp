#include <iostream>
#include <cstring>
#include "vector_dinamico_fran.h"


using namespace std;

class Producto {
protected:
    int numero;
    char* descripcion;
    double precio;

    char* copystr(const char* str) {
        if (!str) str = "";
        char* newstr = new char[strlen(str) + 1];
        strcpy(newstr, str);
        return newstr;
    }

public:
    Producto(int numero, char* descripcion, double precio) {
        this->numero = numero;
        this->descripcion = copystr(descripcion);
        this->precio = precio;
    }

    int getnumero() { return this->numero; }
    const char* getdescripcion() const { return descripcion; }
    virtual double getprecio() = 0;
};

class Parte : public Producto {
public:
    Parte(int numero, char* descripcion, double precio) : Producto(numero, descripcion, precio) {}
    
    double getprecio() ;
};
double Parte::getprecio() {
    return this->precio;
}

class Bicicleta_armada : public Producto {
private:
    vector<Parte> partes;
public:
    Bicicleta_armada(int numero, char* descripcion) : Producto(numero, descripcion, 0.0) {} // Precio inicial 0

    double getprecio() ;
    void add_parte(Parte& a);
};
void Bicicleta_armada::add_parte(Parte& a) {
    partes.add(a); 
}
double Bicicleta_armada::getprecio() {
    double monto = 0;
    for (size_t i = 0; i < partes.getsize(); i++) {
        monto += partes[i].getprecio();
    }
    return monto;
}

class Oferta {
private:
    int numero;
    char* descripcion;
    vector<Producto*> productos_v;

    char* copystr(const char* str) {
        if (!str) str = "";
        char* newstr = new char[strlen(str) + 1];
        strcpy(newstr, str);
        return newstr;
    }

public:
    Oferta(int numero, char* descripcion) {
        this->numero = numero;
        this->descripcion = copystr(descripcion);
    }
    void add_producto(Producto* p) {
        productos_v.add(p);
    }
    double getprecio() {
        double monto_total = 0;
        for (size_t i = 0; i < productos_v.getsize(); i++) {
            monto_total += productos_v[i]->getprecio();
        }
        return monto_total * 0.80;
    }

    friend ostream& operator<<(ostream& os, Oferta& p) {
        os << "========================================" << endl;
        os << "Oferta N" << p.numero << " - " << p.descripcion << ": [" << endl;

        for (size_t i = 0; i < p.productos_v.getsize(); i++) {
            os << "  - " << p.productos_v[i]->getdescripcion() 
               << " ($" << p.productos_v[i]->getprecio() << ")" << endl;
        }

        os << "] Precio total (con 20% desc): $" << p.getprecio() << endl;
        os << "========================================" << endl;
        return os;
    }
};

class Bicicleteria {
private:
    vector<Producto*> inventario_productos;
    vector<Oferta*> ofertas_v;

public:
    Bicicleteria() {};
    void add_producto(Producto* p) {
        inventario_productos.add(p);
    }

    void mostrar_inventario() {
        cout << "\n--- Inventario General de la Tienda ---" << endl;
        if (inventario_productos.getsize() == 0) {
            cout << "(Inventario vacío)" << endl;
            return;
        }
        for (size_t i = 0; i < inventario_productos.getsize(); i++) {
            cout << " - " << inventario_productos[i]->getdescripcion() 
                 << " ($" << inventario_productos[i]->getprecio() << ")" << endl;
        }
    }
    void add_oferta(Oferta* o) {
        ofertas_v.add(o);
    }

    void mostraroferta() {
        cout << "\n--- Listado de Todas las Ofertas ---" << endl;
        if (ofertas_v.getsize() == 0) {
            cout << "(No hay ofertas para mostrar)" << endl;
            return;
        }
        for (size_t i = 0; i < ofertas_v.getsize(); i++) {
            cout << *(ofertas_v[i]);
        }
    }
};
int main(){
    return 0;
}