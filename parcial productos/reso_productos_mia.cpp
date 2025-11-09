#include "vector_dinamico_fran.h"
#include <cstring>
#include <iostream>
#include <ostream>
using namespace std;

char* copystr(const char* str) {
    char* newstr = new char[strlen(str) + 1];
    strcpy(newstr, str);
    return newstr;
}

class Producto {
protected:
    char* nombre;
    double precio;
    double precio_viejo;
public:
    Producto(char* nombre, double precio) {
        this->nombre = copystr(nombre);
        this->precio = precio;
        this->precio_viejo = this->precio;
    }
    virtual void imprimir(std::ostream& os) = 0;
    double get_precio() { return precio; }
    char* get_nombre() { return nombre; }
    double get_precioviejo() { return precio_viejo; }
};

std::ostream& operator<<(std::ostream& os, Producto& p) {
    p.imprimir(os);
    return os;
}

// ====================== NACIONAL ======================
class Nacional : public Producto {
public:
    Nacional(char* nombre, double precio) : Producto(nombre, precio) {};
    void imprimir(std::ostream& os);
    void actualizar_p(double porcentaje);
};

void Nacional::actualizar_p(double porcentaje) {
    this->precio *= ((porcentaje / 100) + 1);
}

void Nacional::imprimir(std::ostream& os) {
    double aumento = this->precio - this->precio_viejo;
    os << "Producto nacional: " << this->nombre
       << " | Precio actual: $" << this->precio
       << " | Aumento: $" << aumento << endl;
}

// ====================== EXTRANJERO ======================
class Extranjero : public Producto {
public:
    Extranjero(char* nombre, double precio) : Producto(nombre, precio) {};
    void actualizar_p(double dolar_antes, double dolar);
    void imprimir(std::ostream& os);
};

void Extranjero::actualizar_p(double dolar_antes, double dolar) {
    this->precio = (this->precio / dolar_antes) * dolar;
}

void Extranjero::imprimir(std::ostream& os) {
    double aumento = this->precio - this->precio_viejo;
    os << "Producto extranjero: " << this->nombre
       << " | Precio actual: $" << this->precio
       << " | Aumento: $" << aumento << endl;
}

// ====================== OFERTA ======================
class Oferta {
private:
    vectordinamico<Producto*> ofertas;
public:
    void add_p(Producto* p);
    void imprimir(std::ostream& os);
    void eliminar(Producto* p);
    double getPrecio(); // Precio total con descuento del 10%
};

void Oferta::add_p(Producto* p) {
    ofertas.add(p);
}

double Oferta::getPrecio() {
    double total = 0;
    for (size_t i = 0; i < ofertas.getsize(); i++)
        total += ofertas[i]->get_precio();
    return total * 0.9; // Descuento del 10%
}

void Oferta::imprimir(std::ostream& os) {
    os << "\n=== Oferta ===" << endl;
    for (size_t i = 0; i < ofertas.getsize(); i++) {
        os << " - " << ofertas[i]->get_nombre()
           << " ($" << ofertas[i]->get_precio() << ")" << endl;
    }
    os << "Precio total con descuento (10%): $" << getPrecio() << endl;
}

void Oferta::eliminar(Producto* p) {
    ofertas.removeData(p);
}

// ====================== EMPRESA ======================
class Empresa {
private:
    vectordinamico<Producto*> productos;
    vectordinamico<Oferta> ofertas;
    int cont = 0;
public:
    Empresa() {};
    void add_p(Producto* p);
    void add_o(Oferta o);
    void mostrar();
};

void Empresa::add_p(Producto* p) {
    if (cont < 100) {
        productos.add(p);
        cont++;
    }
}

void Empresa::add_o(Oferta o) {
    if (cont < 100) {
        ofertas.add(o);
        cont++;
    }
}

void Empresa::mostrar() {
    cout << "=== AUMENTO DE PRODUCTOS ===" << endl;
    for (size_t i = 0; i < productos.getsize(); i++) {
        productos[i]->imprimir(cout);
    }
    for (size_t i = 0; i < ofertas.getsize(); i++) {
        ofertas[i].imprimir(cout);
    }
}
