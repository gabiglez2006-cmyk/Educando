#include <iostream>
#include <cstring>
#include "template_producto_al.h"

using namespace std;

//esta seria la clase abstracta 
class Producto_base {
protected:
	int codigo;
	char* nombre;
	char* copystr(const char* str) {
		char* newstr = new char[strlen(str) + 1];
		strcpy(newstr, str);
		return newstr;
	}
	
public:
		//este seria el constructor profe
		Producto_base(int codigo, const char* nombre) {
			this->codigo = codigo;
			this->nombre = copystr(nombre);
		}
		//este es el metodo virtual que retorna un vector de ingredientes (lo hice asi para no usar el append)
		virtual Vector<char*> getIngredientes() const = 0;
		
		const char* getNombre() const { return nombre; }
		int getCodigo() const { return codigo; }
		//para la sobrecarga del operador 
		friend ostream& operator<<(ostream& os, const Producto_base& p);
};
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
class Producto_simple : public Producto_base {
private:
	Vector<char*> ingredientes_v;
	
public:
	Producto_simple(int codigo, const char* nombre)
		: Producto_base(codigo, nombre) {}
	
	void addIngrediente(const char* ingrediente) {
		ingredientes_v.push_back(copystr(ingrediente));
	}
	// aca retorno un vector de ingredientes mi pregunta aca podria retornar directamente el vector ingredientes_v en vez de la copia
	Vector<char*> getIngredientes() const override {
		Vector<char*> copia;
		for (size_t i = 0; i < ingredientes_v.size(); i++) {
			copia.push_back(copystr(ingredientes_v[i]));
		}
		return copia;
	}
};

class Producto_compuesto : public Producto_base {
private:
	Vector<Producto_base*> productos_comp;
	
public:
	Producto_compuesto(int codigo, const char* nombre)
		: Producto_base(codigo, nombre) {}
	
	void addProducto(Producto_base* p) {
		//aca deveria hacer un  if (dynamic_cast<Producto_compuesto*>(b)) {
		//	return; 
		//}
		//productocomp_v.push_back(b); o asi esta bien??
		productos_comp.push_back(p);
	}
	// este lo hice pero si usted me podria dar una forma mas sensilla de hacerlo se lo agradezco , 
	//esta funcion agarra los ingredientes de todos losproductos del producto compuesto
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
					todos.push_back(copystr(ingredientes_componente[j]));
				}
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
		for (size_t i = 0; i < productos_v.size(); i++) {
			cout << *productos_v[i] << endl;
		}
	}
};


int main() {

	return 0;
}
