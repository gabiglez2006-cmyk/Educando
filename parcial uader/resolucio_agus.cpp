#include <iostream>
#include <ostream>
#include <cstring>

using namespace std;


template<class T>
class vector {
private:
    T* data;
    size_t size;
    size_t capacity;
    void resize(size_t newcapacity);
public:
    vector();
void add(T datos);
void remove(size_t pos);
void removeData(T datos);
size_t getsize(){return this->size;}
void reemplazar(T datoviejo, T datonuevo);
T& operator[](size_t pos);
};

template<class T>
vector<T>::vector(){
    this->size = 0;//cantidad de elementos del vector
    this->capacity = 2; // capacidad maxima(temporalmente) del vector
    this->data = new T[2];// vector dinamico
}

template<class T>
void vector<T>::add(T datos) {
    if (this->size == this->capacity) {
        this->resize(this->capacity2);
    }
    this->data[this->size++] = datos;// se incrementa post asignacion
}

template<class T>
void vector<T>::resize(size_t newcapacity){
    T temp= new T[newcapacity];
    for (size_t i=0; i<this->size; i++){
        temp[i]=this->data[i];
    }
    delete[] this->data;
    this->data=temp;
    this->capacity= newcapacity;
}

template<class T>
void vector<T>::remove(size_t pos) {

    for(size_t i = pos; i < this->size - 1; i++) {
        this->data[i] = this->data[i+1]; // Mueve todos los elementos hacia la izquierda
    }
    // Disminuir el tamaño (después de eliminar el elemento lógico)
    this->size--;
}

template<class T>
void vector<T>::removeData(T datos){
    for(size_t i=0; i<this->size; i++)
        if(this->data[i]==datos)//remueve solo el primer T dato que encuentre en el vector
            remove(i);
}
template<class T>
void vector<T>::reemplazar(T datoviejo,T datonuevo){
    for (size_t i=0; i<this->size; i++)
        if(this->data[i]==datoviejo)
            this->data[i]=datonuevo;
}


template<class T>
T& vector<T>::operator[](size_t pos){// es lo mismo que un getter

    return this->data[pos];
}




char* copystr(const char* str){
    char* newstr=new char[strlen(str)+1];
    strcpy(newstr,str);
    return newstr;
}
class Examen{
private:
    int id;//nombre del parcial//1,2,3,
    double nota;
public:
    Examen(int i, double n){id=i,nota=n;}
    int get_id(){return id;}
    double getnota(){return nota;}//pasa valor nota
    void setnota(double nota){this->nota=nota;}///para pasar varibale
};

class Alumno{
private:
    char* nya;
    long dni;
    vector<Examen>notas;


 public:
    Alumno(char* nya, long dni){
        this->nya=nya;
        this->dni=dni;
    }
    void add_nota(Examen& e){
        notas.add(e);
    }
    char* getnya(){return this-> nya;}
    void reemplazar_nota(int id,double nota_recu);//recuperatorio
    char*  getestado();
    double getpromedio();

    friend ostream& operator<<(ostream& os, Alumno& a);
};




void Alumno:: reemplazar_nota(int id,double nota_recu){
   for(size_t i=0;i<notas.getsize();i++){
        if(notas[i].get_id()==id){
            notas[i].setnota(nota_recu);
        }
   }
}
double Alumno ::getpromedio(){
    double acum=0;
    for(size_t i=0;i<notas.getsize();i++){
        acum+=notas[i].getnota();
    }
    return acum/notas.getsize();
  
}

char* Alumno:: getestado(){
   if(notas.getsize() == 0){
   return "libre ausente";
   } 
    bool algunaMenor50 = false;
    bool algunaMenor70 = false;

    for (int i = 0; i < notas.getsize(); i++) {
        if (notas[i].getnota()< 50) algunaMenor50 = true;
        if (notas[i].getnota()< 70) algunaMenor70 = true;
    }

    if (getpromedio()< 55 || algunaMenor50) {
        return "libre";
    }
    else if (getpromedio() >= 55 && getpromedio() < 75) {
        return "regular";
    }
    else if (getpromedio() >= 75 && !algunaMenor70) {
        return "promocionado";
    }
}


ostream& operator<<(ostream& os, Alumno& a){
    os<<"nombre: "<<a.getnya()<<"estado: "<<a.getestado()<<endl;
}

class Curso{
    private:
        vector<Alumno>alumnos;
    public:
        void add_alumno(Alumno & a);
        void mostrar();
};
void Curso :: add_alumno(Alumno & a){
    if(alumnos.getsize()<50){
        alumnos.add(a);
    }
    
}
void Curso::mostrar(){
    for(size_t i=0;i<alumnos.getsize();i++){
        cout<<alumnos[i]<<endl;
    }
}