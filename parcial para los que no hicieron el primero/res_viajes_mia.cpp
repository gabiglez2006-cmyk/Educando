#include <iostream>
#include <cstring>
#include "vector_dinamico_fran.h" 

using namespace std;

class Viaje {
protected:
    char* destino;

    char* copystr(const char* str) {
        if (!str) str = "";
        char* newstr = new char[strlen(str) + 1];
        strcpy(newstr, str);
        return newstr;
    }

public:
    Viaje(const char* destino) {
        this->destino = copystr(destino);
    }
    
    virtual double getprecio() = 0;
    
    const char* getdestino() { return this->destino; }

    // 1. Obliga a todas las clases hijas a tener una función 'imprimir'.
    virtual void imprimir(ostream& os) = 0; //sobrecarga polimorfica
};


class Viajecomun : public Viaje {
private:
    double precio;

public:
    Viajecomun(const char* destino, double precio)
        : Viaje(destino) {
        this->precio = precio;
    }

    double getprecio() override {
        return this->precio;
    }

    // 2. Implementación específica de 'imprimir' para Viajecomun.
    void imprimir(ostream& os) override {
        os << "Viaje Comun a: " << getdestino() 
           << ". Costo: $" << getprecio();
    }
};


class viaje_destino : public Viaje {
private:
    double precio_por_km;
    int km;

public:
    viaje_destino(const char* destino, double precio_por_km, int km)
        : Viaje(destino) {
        this->precio_por_km = precio_por_km;
        this->km = km;
    }

    double getprecio() override;
    
    int getKm() { return this->km; }
    double getPrecioPorKm() { return this->precio_por_km; }

    // 2. Implementación específica de 'imprimir' para viaje_destino.
    void imprimir(ostream& os) override {
        os << "Viaje por Destino a: " << getdestino()
           << " (" << getKm() << " km @ $" << getPrecioPorKm() << "/km)."
           << " Costo: $" << getprecio();
    }
};

double viaje_destino :: getprecio() {
    return this->precio_por_km * this->km;
}


class viaje_mixto : public Viaje {
private:
    Viajecomun* viaje_comun_v;
    vectordinamico<viaje_destino> viaje_destino_v;

public:
    viaje_mixto(const char* destino, Viajecomun* viaje_comun_v)
        : Viaje(destino) {
        this->viaje_comun_v = viaje_comun_v;
    }

    void addviaje_destino(viaje_destino& a);
    double getprecio() override;
    
    Viajecomun* getViajeComun() { return this->viaje_comun_v; }
    vectordinamico<viaje_destino>& getViajesDestino() { return this->viaje_destino_v; }
    // 2. Implementación específica de 'imprimir' para viaje_mixto.
    void imprimir(ostream& os) override {
        os << "Viaje Mixto a: " << getdestino() << ".\n";
        os << "  - (Parte Comun) ";
        getViajeComun()->imprimir(os); // Llama al 'imprimir' de Viajecomun
        os << "\n";
        
        vectordinamico<viaje_destino>& v_destinos = getViajesDestino();
        for (size_t i = 0; i < v_destinos.getsize(); i++) {
            os << "  - (Parte Destino) ";
            v_destinos[i].imprimir(os); // Llama al 'imprimir' de viaje_destino
            os << "\n";
        }
        
        os << "  Costo Total (Mixto): $" << getprecio();
    }
};
void viaje_mixto::addviaje_destino(viaje_destino& a) {
    viaje_destino_v.add(a);
}

double viaje_mixto::getprecio() {
    double monto = 0;
    for (size_t i = 0; i < viaje_destino_v.getsize(); i++) {
        monto += viaje_destino_v[i].getprecio();
    }
    return viaje_comun_v->getprecio() + monto;
}

// 3. Un solo 'operator<<' para la clase base 'Viaje'.
ostream& operator<<(ostream& os, Viaje& v) {
    // Llama a la función virtual 'imprimir()' que corresponda (polimorfismo).
    v.imprimir(os); 
    return os;
}



class gestora{
    private:
    vectordinamico<Viaje*> viajes_v;
    public:
    void add_viaje(Viaje* v);
    void mostrar_viajes();
};

void gestora :: add_viaje(Viaje* v){
    if(viajes_v.getsize()==100){return;}
    viajes_v.add(v);
}

void gestora :: mostrar_viajes(){
    cout << "\n--- Listado de Todos los viajes ---" << endl;
        if (viajes_v.getsize() == 0) {
            cout << "(No hay viajes para mostrar)" << endl;
            return;
        }
        
        for (size_t i = 0; i < viajes_v.getsize(); i++) {
           cout << *(viajes_v[i]) << endl; 
        }
}

int main() {
    return 0;
}