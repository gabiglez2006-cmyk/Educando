#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath> // Para usar floor()

using namespace std;

// ===================================================
// PUNTO 30: VECTOR DINÁMICO (UTILIZANDO TEMPLATES)
// ===================================================
template <typename T>
class VectorDinamico {
private:
	T* data;
	int capacity;
	int size;
	
	void resize(int newCapacity) {
		T* newData = new T[newCapacity];
		for (int i = 0; i < size; ++i) {
			newData[i] = data[i];
		}
		delete[] data;
		data = newData;
		capacity = newCapacity;
	}
	
public:
		VectorDinamico() : capacity(1), size(0) {
			data = new T[capacity];
		}
		~VectorDinamico() {
			delete[] data;
		}
		
		// Agregar un elemento
		void agregar(const T& element) {
			if (size == capacity) {
				resize(capacity * 2);
			}
			data[size++] = element;
		}
		
		// Obtener un elemento dado un índice
		T& obtener(int index) {
			if (index >= 0 && index < size) {
				return data[index];
			}
			throw out_of_range("Índice fuera de rango");
		}
		
		// Obtener tamaño (añadido para la composición)
		int getSize() const {
			return size;
		}
		
		// Eliminar todas las ocurrencias de un elemento determinado
		void eliminarOcurrencias(const T& element) {
			int newSize = 0;
			// Solo necesitamos reescribir el vector, manteniendo los elementos NO iguales a 'element'
			for (int i = 0; i < size; ++i) {
				// NOTA: Para que esto funcione, el tipo T debe tener sobreescrito el operador ==.
				if (!(data[i] == element)) {
					data[newSize++] = data[i];
				}
			}
			size = newSize;
			
			// Opcional: reducir la capacidad si es mucho mayor que el tamaño
			if (size > 0 && size < capacity / 4) {
				resize(capacity / 2);
			}
		}
		
		// Iterador para simplificar el recorrido (aunque no es estrictamente obligatorio)
		T* begin() { return data; }
		T* end() { return data + size; }
};

// Pregunta del Punto 30: ¿Qué operador se debe sobreescribir para utilizar este vector dinámico?
// R: Se debe sobreescribir el **operador de igualdad (==)** para el tipo `T` si queremos usar la función `eliminarOcurrencias`.


// ===================================================
// PUNTO 35: DISEÑO Y PROGRAMACIÓN DE CLASES (POLIMORFISMO)
// ===================================================

/**
* Clase Base Abstracta para el Polimorfismo.
* Permite que un vector pueda contener Partes, Bicicletas y Ofertas de forma homogénea.
*/
class ItemVendible {
protected:
	int numero;
	string descripcion;
	
public:
	ItemVendible(int num, const string& desc) : numero(num), descripcion(desc) {}
	virtual ~ItemVendible() = default;
	
	// Función virtual pura para calcular el precio
	virtual double getPrecio() const = 0;
	
	// Función virtual para obtener la descripción, usada para la impresión
	virtual string getDescripcion() const { return descripcion; }
	
	// Función virtual pura para imprimir detalles (utilizada por el operador <<)
	virtual void imprimirDetalle(ostream& os) const = 0;
};

// ---------------------------------------------------
// Clase 1: Parte (Hija de ItemVendible)
// ---------------------------------------------------
class Parte : public ItemVendible {
private:
	double precioBase;
	
public:
	Parte(int num, const string& desc, double precio)
		: ItemVendible(num, desc), precioBase(precio) {}
	
	double getPrecio() const override {
		return precioBase;
	}
	
	// Sobrecarga del operador == para cumplir con el requisito de VectorDinamico::eliminarOcurrencias
	bool operator==(const Parte& other) const {
		return this->numero == other.numero;
	}
	
	void imprimirDetalle(ostream& os) const override {
		os << this->descripcion << " (Parte, No: " << this->numero << ", Precio: $" << this->getPrecio() << ")";
	}
};

// ---------------------------------------------------
// Clase 2: Bicicleta Armada (Hija de ItemVendible)
// Usa composición: contiene un vector de Partes.
// ---------------------------------------------------
class BicicletaArmada : public ItemVendible {
private:
	// La bicicleta armada está compuesta por un conjunto de Partes
	VectorDinamico<Parte> partes;
	
public:
	BicicletaArmada(int num, const string& desc)
		: ItemVendible(num, desc) {}
	
	void agregarParte(const Parte& p) {
		partes.agregar(p);
	}
	
	// El precio es la suma del precio de las partes
	double getPrecio() const override {
		double total = 0;
		for (int i = 0; i < partes.getSize(); ++i) {
			total += partes.obtener(i).getPrecio();
		}
		return total;
	}
	
	void imprimirDetalle(ostream& os) const override {
		os << this->descripcion << " (Bicicleta Armada, No: " << this->numero << ", Precio Total: $" << this->getPrecio() << ")";
	}
};

// ---------------------------------------------------
// Clase 3: Oferta (Hija de ItemVendible)
// Usa composición: contiene un vector de ItemVendible* (Polimorfismo).
// ---------------------------------------------------
class Oferta : public ItemVendible {
private:
	// La oferta contiene Partes y/o Bicicletas (ItemVendible*)
	VectorDinamico<ItemVendible*> elementos;
	const double DESCUENTO = 0.20;
	
public:
	Oferta(int num, const string& desc)
		: ItemVendible(num, desc) {}
	
	void agregarElemento(ItemVendible* item) {
		elementos.agregar(item);
	}
	
	// El precio es la suma de los elementos menos un 20%
	double getPrecio() const override {
		double totalSinDescuento = 0;
		for (int i = 0; i < elementos.getSize(); ++i) {
			totalSinDescuento += elementos.obtener(i)->getPrecio();
		}
		return totalSinDescuento * (1.0 - DESCUENTO);
	}
	
	// ===================================================
	// PUNTO 20: SOBREESCRITURA DEL OPERADOR <<
	// ===================================================
	
	// La función principal es esta, que implementa la lógica de impresión.
	void imprimirDetalle(ostream& os) const override {
		os << "OFERTA (" << this->descripcion << ", No: " << this->numero << ")";
	}
	
	// Sobrecarga del operador << como función amiga
	friend ostream& operator<<(ostream& os, const Oferta& o) {
		os << o.getDescripcion() << " (Oferta No: " << o.numero << ") - Elementos que la componen: ";
		
		for (int i = 0; i < o.elementos.getSize(); ++i) {
			// Usamos la descripción del elemento
			os << o.elementos.obtener(i)->getDescripcion();
			if (i < o.elementos.getSize() - 1) {
				os << ", ";
			}
		}
		// Se debe redondear a dos decimales o usar floor() para la impresión
		os.precision(2);
		os << fixed << " | Precio Total CON 20% DCTO: $" << o.getPrecio();
		return os;
	}
};

// ---------------------------------------------------
// Implementación del Main
// ---------------------------------------------------
int main() {
	cout << "--- Sistema de Control de Precios Bicicletería Oro Verde ---" << endl;
	
	// 1. Creación de Partes
	Parte *cuadro = new Parte(101, "Cuadro de Aluminio", 5000.0);
	Parte *rueda = new Parte(102, "Rueda R26", 1500.0);
	Parte *manubrio = new Parte(103, "Manubrio Ergonomico", 800.0);
	
	// 2. Creación de una Bicicleta Armada (usa composición de Partes)
	BicicletaArmada *biciMontana = new BicicletaArmada(201, "Mountain Bike Deluxe");
	biciMontana->agregarParte(*cuadro);
	biciMontana->agregarParte(*rueda);
	biciMontana->agregarParte(*rueda); // Dos ruedas
	biciMontana->agregarParte(*manubrio);
	
	cout << "\n> Detalle de Bicicleta Armada (Suma de partes):" << endl;
	biciMontana->imprimirDetalle(cout);
	cout << endl;
	
	// 3. Creación de otra Parte para la Oferta
	Parte *casco = new Parte(104, "Casco de Seguridad", 1000.0);
	
	// 4. Creación de la Oferta (usa composición de ItemVendible*)
	Oferta ofertaFamiliar(301, "Pack Aventura");
	ofertaFamiliar.agregarElemento(biciMontana); // Agrega la Bicicleta
	ofertaFamiliar.agregarElemento(casco);       // Agrega una Parte
	
	cout << "\n> Detalle de Oferta (Usa operador << sobreescrito):" << endl;
	cout << ofertaFamiliar << endl;
	
	cout << "\n--- FIN DE DEMOSTRACIÓN ---" << endl;
	
	// 5. Limpieza de memoria
	delete cuadro;
	delete rueda;
	delete manubrio;
	delete casco;
	delete biciMontana;
	// La Oferta no es dueña de los punteros que contiene, pero el vector dinámico de punteros de Oferta debe ser liberado.
	// Para este simple ejemplo, no liberamos los elementos de la oferta para evitar doble free, ya que fueron liberados antes.
	
	return 0;
}
