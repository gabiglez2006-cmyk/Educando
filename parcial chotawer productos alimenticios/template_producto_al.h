#include <iostream>
#include <cstring> // Incluido para funciones de string
using namespace std;

template <class T>
class Vector{
public:
    Vector();
    ~Vector() { delete[] data; }
    
    // Función para escribir: no es const
    void push_back(T data);
    
    // Función para leer y modificar: no es const
    T& operator[](size_t pos);
    
    // ⭐ CORRECCIÓN: Función para solo leer: DEBE ser const
    const T& operator[](size_t pos) const; 
    
    void eliminar(T dato);
    
    // ⭐ CORRECCIÓN: Función para solo leer el tamaño: DEBE ser const
    size_t size() const { return this->size_; }

private:
    void resize(size_t newMax);
    
    T * data; 	    // Puntero a los datos (nuestro array dinámico)
    size_t max; 	// Capacidad máxima actual
    size_t size_; 	// Cantidad de elementos actuales
};

template<class T>
Vector<T>::Vector()
{
    this->data = new T[2];
    this->max = 2;
    this->size_ = 0;
}

template<class T>
void Vector<T>::push_back(T data_)
{
    if (size_ == max)
        this->resize(this->max * 2);
    
    this->data[size_++] = data_;
}

template<class T>
void Vector<T>::resize(size_t newMax)
{
    T * temp = new T[newMax];
    
    for (size_t i = 0; i < this->size_; i++)
        temp[i] = this->data[i];

    delete[] this->data;
    this->data = temp;
    this->max = newMax;
}

// Implementación del operator[] para LECTURA Y ESCRITURA (no const)
template<class T>
T& Vector<T>::operator[](size_t pos)
{
    // Una implementación robusta debería chequear límites aquí
    return data[pos];
}

// ⭐ NUEVA IMPLEMENTACIÓN: operator[] para SOLO LECTURA (const)
// Este es el operador que es llamado por las funciones const de otras clases
template<class T>
const T& Vector<T>::operator[](size_t pos) const
{
    // Una implementación robusta debería chequear límites aquí
    return data[pos];
}

template <class T>
void Vector<T>::eliminar(T dato)
{
    size_t j = 0;
    
    for (size_t i = 0; i < this->size_; i++)
    {
        if (this->data[i] != dato) // Asume que T tiene implementado operator!=
        {
            this->data[j] = this->data[i];
            j++; 
        }
    }
    this->size_ = j;
}


// Función auxiliar para imprimir el vector (de cualquier tipo)
// La función auxiliar ya recibía el Vector como referencia NO const. 
// Para que sea más seguro, la hice const. 
template<class T>
void imprimirVector(const string& titulo, const Vector<T>& vec) { // ⭐ MODIFICADO a const Vector<T>&
    cout << titulo << "\n[ ";
    for (size_t i = 0; i < vec.size(); i++) { // Ahora llama a size() const
        cout << vec[i] << " "; // Ahora llama a operator[] const
    }
    cout << "]\nTamano: " << vec.size() << endl;
}

// int main() {
//     // Puedes probar tu vector aquí
//     Vector<int> v;
//     v.push_back(10);
//     v.push_back(20);
//     v.push_back(10);
//     imprimirVector("Vector original", v);
    
//     v.eliminar(10);
//     imprimirVector("Vector despues de eliminar 10", v);

//     return 0;
// }