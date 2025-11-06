#include <iostream>
#include <cstring>
#include <algorithm>
#include "template_producto_al.h"
// === CLASE BASE ===
class Producto_base {
protected:
	int codigo;
	char* nombre;
	
public:
	Producto_base(int codigo, const char* nombre) {
		this->codigo = codigo;
		this->nombre = new char[strlen(nombre) + 1];
		strcpy(this->nombre, nombre);
	}
	
	// Constructor copia
	Producto_base(const Producto_base& other) {
		codigo = other.codigo;
		nombre = new char[strlen(other.nombre) + 1];
		strcpy(nombre, other.nombre);
	}

	virtual Vector<char*> getIngredientes() const = 0;
	
	const char* getNombre() const { return nombre; }
	int getCodigo() const { return codigo; }
	
	friend ostream& operator<<(ostream& os, const Producto_base& p);
};

// Función auxiliar para comparar char*
bool compararCadenas(const char* a, const char* b) {
	return strcmp(a, b) == 0;
}

// === OPERADOR << ===
ostream& operator<<(ostream& os, const Producto_base& p) {
	os << "Producto: " << p.getNombre() << " (Código: " << p.getCodigo() << ")";
	
	Vector<char*> ingredientes = p.getIngredientes();
	if (ingredientes.size() == 0) {
		os << " - Sin ingredientes";
	} else {
		os << " - Ingredientes: ";
		for (size_t i = 0; i < ingredientes.size(); i++) {
			os << ingredientes[i];
			if (i < ingredientes.size() - 1) {
				os << ", ";
			}
		}
	}
	return os;
}

// === CLASE PRODUCTO SIMPLE ===
class Producto_simple : public Producto_base {
private:
	Vector<char*> ingredientes_v;
	
public:
	Producto_simple(int codigo, const char* nombre)
		: Producto_base(codigo, nombre) {}
	
	~Producto_simple() {
		// Liberar memoria de los ingredientes
		for (size_t i = 0; i < ingredientes_v.size(); i++) {
			delete[] ingredientes_v[i];
		}
	}
	
	void addIngrediente(const char* ingrediente) {
		char* copia = new char[strlen(ingrediente) + 1];
		strcpy(copia, ingrediente);
		ingredientes_v.push_back(copia);
	}
	
	Vector<char*> getIngredientes() const override {
		return ingredientes_v;
	}
};

// === CLASE PRODUCTO COMPUESTO ===
class Producto_compuesto : public Producto_base {
private:
	Vector<Producto_base*> productos_comp;
	
public:
	Producto_compuesto(int codigo, const char* nombre)
		: Producto_base(codigo, nombre) {}
	
	~Producto_compuesto() {
		// No liberamos los productos_comp porque son manejados por el Gestor
	}
	
	void addProducto(Producto_base* p) {
		productos_comp.push_back(p);
	}
	
	Vector<char*> getIngredientes() const override {

		Vector<char*> todos;
		
		for (size_t i = 0; i < productos_comp.size(); i++) {
			
			Vector<char*> ingredientes_componente = productos_comp[i]->getIngredientes();
			
			for (size_t j = 0; j < ingredientes_componente.size(); j++) {
				
				bool existe = false;
				for (size_t k = 0; k < todos.size(); k++) {
					if (strcmp(todos[k], ingredientes_componente[j]) == 0) {
						existe = true;
						break;
					}
				}
				if (!existe) {
					char* copia = new char[strlen(ingredientes_componente[j]) + 1];
					strcpy(copia, ingredientes_componente[j]);
					todos.push_back(copia);
				}
			}
			
			for (size_t l = 0; l < ingredientes_componente.size(); l++) {
				delete[] ingredientes_componente[l];
			}
			
		}
		return todos;
	}
};
class Gestor {
private:
	Vector<Producto_base*> productos_v;
	
public:
	void addProducto(Producto_base* p) {
		productos_v.push_back(p);
	}
	
	void mostrarProductos() {
		cout << "\n--- LISTADO DE PRODUCTOS ---\n";
		for (size_t i = 0; i < productos_v.size(); i++) {
			cout << *productos_v[i] << endl;
		}
		cout << "-----------------------------\n";
	}
};


int main() {

	return 0;
}
