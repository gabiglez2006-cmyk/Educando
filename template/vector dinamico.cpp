template <typename T>
class VectorDinamico {

private:
    T* data;
    size_t tamano;
    size_t capacidad;

    /*funcion para redimensionar */
    void redimensionar() {
        // 1. Calculamos la nueva capacidad (normalmente el doble)
        size_t nuevaCapacidad = (capacidad == 0) ? 2 : capacidad * 2;

        // 2. Creamos un nuevo array con más espacio
        T* nuevoArray = new T[nuevaCapacidad];

        // 3. Copiamos los elementos del array viejo al nuevo
        for (size_t i = 0; i < tamano; i++) {
            nuevoArray[i] = data[i];
        }

        // 4. Liberamos la memoria del array viejo (¡importante!)
        delete[] data;

        // 5. Apuntamos 'data' al nuevo array y actualizamos la capacidad
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
};