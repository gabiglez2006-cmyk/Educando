template <typename T>
class VectorDinamico {
private:
		T* data; 
		size_t tamano;
		size_t capacidad;
		
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
			
			void agregar(T elemento);  
};
template <typename T>
void VectorDinamico<T>::agregar(T elemento) {
	if (tamano == capacidad) {
		redimensionar();
	}
	
	data[tamano] = elemento;
	tamano++;
}
