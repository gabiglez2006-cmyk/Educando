///vector dinamico 
public:
	///para poder acceder a los datos con corchetes como en funpro 
	/// y poder acceder a los datos con el indice
	T&  operator[](size_t pos){return data[pos];}
	
	///caso de uso en el ejercicio  de aplicacion de clase
	for(size_t i=0;i<ingredientes.size();i++){
		cout<<ingredientes[i]<<endl;///podes usar [0],[1] o lo que sea
	}
	
