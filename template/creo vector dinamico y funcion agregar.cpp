template <typename T>
class VectorDinamico {
private:
	T* data;          // Puntero al array dinámico
	size_t tamano;    // Número actual de elementos
	size_t capacidad; // Capacidad máxima actual
	
	void redimensionar() {
		size_t nuevaCapacidad = (capacidad == 0) ? 2 : capacidad * 2;
		T* nuevoArray = new T[nuevaCapacidad];
		for (size_t i = 0; i < tamano; i++) {
			nuevoArray[i] = data[i];
		}
		delete[] data;
		data = nuevoArray;
		capacidad = nuevaCapacidad;
	}
	
public:
		VectorDinamico() {
			tamano = 0;
			capacidad = 2;
			data = new T[capacidad];
		}
		
		~VectorDinamico() {
			delete[] data;
		}
		
		// a. Declaración: Agregar un elemento
		void agregar(const T& elemento); 
};

// Definición de la función 'agregar' fuera de la clase (usa '::')
template <typename T>
void VectorDinamico<T>::agregar(const T& elemento) {
	// 1. Verificar si está lleno y redimensionar si es necesario
	if (tamano == capacidad) {
		redimensionar();
	}
	
	// 2. Insertar en la posición actual de 'tamano' y luego incrementar 'tamano'.
	data[tamano++] = elemento;
}
