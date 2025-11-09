#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

// ===================================================
// BASE CLASS: PRODUCTOS (Corregida con virtual destructor y print)
// ===================================================
class Productos {
public:
	Productos(float precio, char * codigo) {
		this->precio = precio;
		this->codigo = setNewString(codigo);
	}
	// IMPORTANTE: Destructor virtual para asegurar la limpieza de memoria de las clases hijas
	virtual ~Productos() {
		delete[] codigo;
	}
	
	virtual float getPrecio() const { // Agregado const
		return this->precio;
	}
	char * getCodigo() const {
		return this->codigo;
	}
	
	// Función virtual pura para impresión polimórfica (Punto 2)
	virtual void imprimir(ostream& os) const = 0;
	
	// Función auxiliar para copiar strings (copia profunda)
	char * setNewString(const char * base) {
		char * newString = new char[strlen(base) + 1];
		strcpy(newString, base);
		return newString;
	}
	
protected:
		float precio;
		char * codigo;
};

// Sobrecarga Global del Operador << (Punto 2)
ostream& operator<<(ostream& os, const Productos& p) {
	p.imprimir(os);
	return os;
}

// ===================================================
// HIJA 1: PRODUCTONACIONAL
// ===================================================
class ProductoNacional : public Productos {
public:
	ProductoNacional(float newPrecio, char *codigo) :
		Productos(newPrecio, codigo) {}
	
	void actualizarPrecio(float porcentaje) {
		// precio actual = (precio anterior * (100 + porcentaje)) / 100
		this->precio = (this->precio * (100 + porcentaje)) / 100.0;
	}
	
	void imprimir(ostream& os) const override {
		os << "Producto Nacional (Cod: " << this->codigo << ") | Precio: $" << fixed << setprecision(2) << this->precio;
	}
};

// ===================================================
// HIJA 2: PRODUCTOEXTRANJERO
// ===================================================
class ProductoExtranjero : public Productos {
public:
	ProductoExtranjero(float precio, char * codigo) : Productos(precio, codigo) {}
	
	void actualizarPrecio(float cotizacionAnterior, float cotizacionActual) {
		// precio actual = (precio anterior / cotización anterior) * cotización actual
		this->precio = (this->precio / cotizacionAnterior) * cotizacionActual;
	}
	
	void imprimir(ostream& os) const override {
		os << "Producto Extranjero (Cod: " << this->codigo << ") | Precio: $" << fixed << setprecision(2) << this->precio;
	}
};

// ===================================================
// HIJA 3: PRODUCTOOFERTA (Corregida la lógica interna)
// ===================================================
class ProductoOferta : public Productos {
private:
	Productos ** productos;
	int maxProductos;
	int indice; // Representa el tamaño actual del vector
	
	// CORRECCIÓN: Lógica para hacer el vector crecer
	void modificarVector(int max) {
		Productos ** aux = this->productos;
		int oldSize = this->indice; // Usamos el índice actual como tamaño
		this->maxProductos = max;
		this->productos = new Productos*[this->maxProductos];
		for (int i = 0; i < oldSize; i++)
			this->productos[i] = aux[i];
		delete[] aux;
	}
	
	// CORRECCIÓN: Se utiliza la lógica de desplazamiento, no reasignación completa
	void eliminarProductoPorIndice(int indice_a_eliminar) {
		if (indice_a_eliminar < 0 || indice_a_eliminar >= this->indice) return;
		
		// Desplazar los elementos posteriores hacia adelante
		for (int i = indice_a_eliminar; i < this->indice - 1; i++) {
			this->productos[i] = this->productos[i+1];
		}
		this->indice--; // Disminuir el tamaño
	}
	
public:
		ProductoOferta(char * newCodigo) : Productos(0.0, newCodigo) {
			this->maxProductos = 2;
			this->productos = new Productos*[this->maxProductos];
			this->indice = 0;
		}
		
		// Destructor para limpiar la memoria interna
		~ProductoOferta() override {
			// Importante: La oferta solo guarda punteros, la responsabilidad de la liberación
			// de los productos individuales recae en el Gestor, pero la oferta debe liberar su propio array
			delete[] productos;
		}
		
		void cargarProducto(Productos *newProducto) {
			if (dynamic_cast<ProductoOferta*>(newProducto))
				return; // No se puede guardar una oferta dentro de otra oferta.
			
			if (this->indice == this->maxProductos)
				modificarVector(this->maxProductos * 2);
			
			this->productos[indice++] = newProducto;
		}
		
		// CORRECCIÓN: Implementación para eliminar por código
		void eliminarProducto(char *buscarcodigo) {
			for (int i = 0; i < this->indice; i++) {
				if (strcmp(this->productos[i]->getCodigo(), buscarcodigo) == 0) {
					eliminarProductoPorIndice(i);
					i--; // Vuelve un paso para revisar si hay más ocurrencias
				}
			}
		}
		
		// CORRECCIÓN: Lógica correcta para calcular el precio (suma de componentes - 10%)
		float getPrecio() const override {
			float precioAcumulado = 0.0;
			for (int i = 0; i < this->indice; i++)
				precioAcumulado += this->productos[i]->getPrecio();
			
			// Descuento del 10% (multiplicar por 0.90)
			return precioAcumulado * 0.90;
		}
		
		void imprimir(ostream& os) const override {
			os << "OFERTA (Cod: " << this->codigo << ") | Precio C/DCTO: $" << fixed << setprecision(2) << this->getPrecio() << " | Compuesto por: ";
			for (int i = 0; i < this->indice; i++) {
				os << this->productos[i]->getCodigo();
				if (i < this->indice - 1) os << ", ";
			}
		}
};

// ===================================================
// GESTOR DE PRODUCTOS (Para cargar y gestionar hasta 100)
// ===================================================
class GestorProductos {
private:
	Productos *productos[100];
	int numProductos;
	float precioTotalInicial;
	float precioTotalFinal;
	
public:
	GestorProductos() : numProductos(0), precioTotalInicial(0.0), precioTotalFinal(0.0) {}
	
	~GestorProductos() {
		for (int i = 0; i < numProductos; ++i) {
			delete productos[i];
		}
	}
	
	void addProducto(Productos *newProducto) {
		if (numProductos < 100) {
			productos[numProductos++] = newProducto;
		}
	}
	
	void calcularPrecioTotalInicial() {
		this->precioTotalInicial = 0.0;
		for (int i = 0; i < numProductos; ++i) {
			this->precioTotalInicial += productos[i]->getPrecio();
		}
	}
	
	void actualizarPrecios(float porcentajeNacional, float cotizacionAnterior, float cotizacionActual) {
		calcularPrecioTotalInicial();
		
		for (int i = 0; i < numProductos; ++i) {
			// Uso de dynamic_cast para aplicar polimorfismo de actualización
			if (ProductoNacional* pn = dynamic_cast<ProductoNacional*>(productos[i])) {
				pn->actualizarPrecio(porcentajeNacional);
			} else if (ProductoExtranjero* pe = dynamic_cast<ProductoExtranjero*>(productos[i])) {
				pe->actualizarPrecio(cotizacionAnterior, cotizacionActual);
			}
			// Las ofertas no tienen una función de actualización propia, ya que su precio depende de sus componentes actualizados.
		}
		
		this->precioTotalFinal = 0.0;
		for (int i = 0; i < numProductos; ++i) {
			this->precioTotalFinal += productos[i]->getPrecio();
		}
	}
	
	// Muestra todos los productos y el aumento total (Punto 2)
	void mostrarInforme() const {
		cout << "\n--- INFORME DE PRODUCTOS CARGADOS ---" << endl;
		for (int i = 0; i < numProductos; ++i) {
			cout << i + 1 << ". " << *productos[i] << endl; // Uso de operador <<
		}
		cout << "\n--- RESUMEN DE AUMENTOS ---" << endl;
		cout << "Precio Total Antes de la Actualización: $" << fixed << setprecision(2) << precioTotalInicial << endl;
		cout << "Precio Total Después de la Actualización: $" << fixed << setprecision(2) << precioTotalFinal << endl;
		cout << "Aumento Total: $" << fixed << setprecision(2) << (precioTotalFinal - precioTotalInicial) << endl;
	}
};
int main() {

	return 0;
}
