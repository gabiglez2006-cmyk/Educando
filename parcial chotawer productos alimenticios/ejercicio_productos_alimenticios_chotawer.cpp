#include <iostream>
#include "template_producto_al.h" 
#include <cstring>
using namespace std;

class Producto_base{
protected:
    int codigo;
    char * nombre;

    // DECLARACIONES DE MÉTODOS PROTEGIDOS
    char* copyStr(const char * str);
    char * append(char * &primero, const char * segundo);

public:
    Producto_base(int codigo, const char* nombre);
    
    // DESTRUCTOR VIRTUAL: NECESARIO
    virtual ~Producto_base() { delete[] nombre; } 

    friend ostream& operator<< (ostream& os, Producto_base& p);
    
    // MÉTODO VIRTUAL PURO
    virtual char* getingrediente() = 0; 
    
    char* getnombre(){ return copyStr(this->nombre); }
    int getcodigo(){ return this->codigo; }
};

// Implementación del constructor
Producto_base::Producto_base(int codigo, const char* nombre) {
    this->codigo = codigo;
    this->nombre = this->copyStr(nombre);
}


char* Producto_base::copyStr(const char * str){ 
    int tam = strlen(str);
    char * newStr = new char[tam + 1];
    strcpy(newStr, str);
    return newStr;
}

char * Producto_base::append(char * &primero, const char * segundo){
    char * suma = new char[strlen(primero) + strlen(segundo) + 1];
    strcpy(suma, primero);
    strcat(suma, segundo);
    delete[] primero; // Se libera la cadena antigua.
    primero = suma;
    return primero;
}


ostream& operator<< (ostream& os, Producto_base& p){
    os << "Producto: " << p.getnombre() << " Codigo: " << p.getcodigo() << endl
       << "Ingredientes: \n";
    
    os << p.getingrediente(); 
    
    return os;
}

class Producto_simple :public Producto_base{
    public:
    Producto_simple(int codigo,char* nombre):Producto_base(codigo,nombre){}
    char* getingrediente();
    void addingrediente(char* a);
    private:
    Vector<char*>ingredientes_v;
};

void Producto_simple :: addingrediente(char* a){
ingredientes_v.push_back(this->copyStr(a));
}
char * Producto_simple::getingrediente(){
    // Obtiene la cantidad total de ingredientes almacenados en el vector.
    size_t tam = this->ingredientes_v.size();
    
    // Verifica si el vector está vacío. Si lo está, retorna un mensaje por defecto.
    if (tam == 0) return this->copyStr("(Sin Ingredientes)\n");

    // Paso 1: Inicialización
    // Crea la cadena dinámica inicial haciendo una copia del primer ingrediente (índice 0).
    // Esta será la base que se irá extendiendo.
    char * ingredientes_resultante = this->copyStr(ingredientes_v[0]);

    // Paso 2 y 3: Iteración y Concatenación
    // Itera a partir del segundo ingrediente (índice 1).
    for (size_t i = 1; i < tam; i++){
        // Concatena el separador de formato ("\n - ") a la cadena resultante,
        // liberando la memoria antigua y reasignando una nueva y más grande.
        this->append(ingredientes_resultante, "\n - ");
        
        // Concatena el ingrediente actual (i) a la cadena resultante.
        this->append(ingredientes_resultante, ingredientes_v[i]);
    }
    
    // Agrega un salto de línea final para mantener una buena presentación.
    this->append(ingredientes_resultante, "\n"); 

    // Paso 4: Retorno
    // Retorna la cadena dinámica final con todos los ingredientes.
    // (NOTA: La memoria de esta cadena queda sin liberar, según la consigna general.)
    return ingredientes_resultante;
}

class Producto_compuesto : public Producto_base{
    private:
    Vector <Producto_base*>productocomp_v;
    public:
    Producto_compuesto(int codigo,char* nombre): Producto_base(codigo,nombre){}
    char* getingrediente();
    void addingrediente(Producto_base* b);
   
};
void Producto_compuesto :: addingrediente(Producto_base* b){
    if (dynamic_cast<Producto_compuesto*>(b)) {
        return; 
    }
    productocomp_v.push_back(b);
}
char * Producto_compuesto::getingrediente(){
    size_t tam = this->productocomp_v.size();
    
    // Inicializa la cadena de resultado con un string vacío.
    char * compuesto = this->copyStr(""); 

    // Itera sobre el vector de punteros a Producto_base.
    for (size_t i = 0; i < tam; i++) {
        
        // 1. Obtener el nombre del componente (Polimorfismo para getnombre())
        // Usamos getnombre(), que nos devuelve un char* dinámico (copia).
        char* nombre_comp = productocomp_v[i]->getnombre();
        
        // Formato: Añadir el nombre del componente a la cadena resultante.
        this->append(compuesto, "\n=== Componente: ");
        this->append(compuesto, nombre_comp);
        this->append(compuesto, " ===\n");
        
        // MUY IMPORTANTE: Liberar la memoria de la cadena devuelta por getnombre()
        // porque ya la copiamos en 'compuesto'.
        delete[] nombre_comp; 

        // 2. Obtener los ingredientes del componente (Polimorfismo para getingrediente())
        // Esta llamada funciona tanto si el componente es simple (devuelve sus ingredientes) 
        // o si es otro compuesto (devuelve los de sus sub-componentes).
        char* ing_comp = productocomp_v[i]->getingrediente();
        
        // Añadir la lista de ingredientes del componente a la cadena resultante.
        this->append(compuesto, ing_comp);
        
        // NOTA: No liberamos 'ing_comp' aquí, ya que sigue la regla de la consigna 
        // de no liberar la cadena que retorna getingrediente().
    }
    
    // Retorna la cadena dinámica final.
    return compuesto;
}

class Gestor {
private:
    // Almacena punteros a la clase base, lo que permite guardar cualquier tipo de producto.
    Vector<Producto_base*> productos_v; 
public:
    Gestor() {}
    
    // NOTA: Se omite el destructor ~Gestor() para seguir la consigna,
    // pero en un entorno real, sería necesario para liberar la memoria de los punteros guardados.

    // Método para agregar cualquier producto de la jerarquía.
    void addProducto(Producto_base * newProd); 

    // Consigna b: Muestra todos los productos_v guardados.
    void mostrarproductos_v(); 
};

void Gestor::addProducto(Producto_base * newProd) {
    // Simplemente guarda el puntero al nuevo producto en el vector.
    productos_v.push_back(newProd); 
}

void Gestor::mostrarproductos_v(){
    size_t tam = productos_v.size();
    std::cout << "\n--- LISTADO DE productos_v (GESTIÓN) ---\n";
    
    // Itera sobre el vector
    for (size_t i = 0; i < tam; i++) {
        // Usa el operador<< sobrecargado. El polimorfismo garantiza
        // que se llama al getingrediente() correcto (simple o compuesto).
        cout << *productos_v[i] <<endl;
    }
    std::cout << "-------------------------------------\n";
}

int main()
{

return 0;
}