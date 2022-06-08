//Class Array
#include <iostream>
#include <cstdlib>

//using namespace std;
//--------------------------------------------------------------------------------------------------------
//defini��o da classe Array <Tipo de dados> gen�rico
//--------------------------------------------------------------------------------------------------------
template <typename Tipo> class Array;

template <typename Tipo> std::ostream &operator<<(std::ostream &,const Array<Tipo> &);

template <typename Tipo> class Array{
		Tipo *array;
		int tamanho;

        protected:   
		void aloca(int);
		void remove();
	public:
		//Construtores
		//nulo
		Array(){array = NULL;tamanho=0;}
		//normal
		Array(int);
		
		Array(Tipo [],int);
		
		//c�pia do objecto
		Array(const Array<Tipo> &);
		//destrutor
	       ~Array(){remove();}

		//m�todos
		//para variar a dimens�o durante o programa (depois de declarado)
		
		//modificar		
		void		setDim   (int);
		void		meteAZero(int); 
		void		copiaPara(Array <Tipo> &);
		//devolve a dimens�o
		inline  int	getDim   (){return tamanho;}
		inline  Tipo * 	&cArray  (){return array;}
		
		//operadores reescritos
		//permite acesso aos elementos atraves do operador deslocamento (a[i])
		Tipo 		   &operator [](int i){return array[i];}
		Array<Tipo> 	operator  =(Array <Tipo> in);
		friend std::ostream &operator<< <>(std::ostream &,const Array<Tipo> &); 

};
//--------------------------------------------------------------------------------------------------------
//M�todos Privados
//--------------------------------------------------------------------------------------------------------
template <typename Tipo> void Array<Tipo>::aloca(int tamanho){

	if(!(array = new Tipo[tamanho])){ //refazer para windows
		std::cout <<"Erro na Mem�ria"<<std::endl;
		exit(1);
	}
	this->tamanho=tamanho;
}

template <typename Tipo> void Array<Tipo>::remove(){
		if(array){ 
			delete[] array; 
			array = NULL; //acho que isto n�o � necess�rio
		}
}
//--------------------------------------------------------------------------------------------------------
//Construtores
//--------------------------------------------------------------------------------------------------------

template <typename Tipo> Array<Tipo>::Array(int tamanho){
	aloca(tamanho);
}

template <typename Tipo> Array<Tipo>::Array(Tipo in[],int tamanho){	
	aloca(tamanho);
	
	for(register int i=0;i < tamanho;i++) *(array+i) = *(in+i);
	this->tamanho=tamanho;
}
//c�pia do objecto
template <typename Tipo> Array<Tipo>::Array(const Array<Tipo> &in){
	
       			aloca(in.tamanho);
       			for(register int i = 0;i < tamanho;i++) *(array+i) = *(in.array+i);
}
//--------------------------------------------------------------------------------------------------------
//Operadores
//--------------------------------------------------------------------------------------------------------
template <typename Tipo> Array<Tipo> Array<Tipo>::operator  = (Array <Tipo> in){
			register Tipo *ptr1 = in.array;
			
			if(this != &in){//n�o modifica a c�pia dele mesmo			
				remove();
				aloca(in.tamanho); //fica com o tamanho do outro
				for(register Tipo *ptr = array;(ptr - array) -  tamanho;) *ptr ++ = *ptr1 ++;
			}
			return *this;
}
//--------------------------------------------------------------------------------------------------------
//Outros M�todos
//--------------------------------------------------------------------------------------------------------
template <typename Tipo> void Array<Tipo>::meteAZero(int dim){
			register Tipo *ptr = array; 

			for(;(ptr - array) -  dim;) *ptr ++  =  (Tipo)NULL;
}
//--------------------------------------------------------------------------------------------------------
template <typename Tipo> void Array<Tipo>::copiaPara(Array <Tipo> &in){
			register int i=0;
			
			for(;in.tamanho-i&&tamanho-i;i++) *(in.array+i) = *(array+i);
			for(;i<in.tamanho;) *(in.array + i++) = (Tipo)NULL;
}

template <typename Tipo> void Array<Tipo>::setDim(int tamanho){
	remove();
	aloca(tamanho);
}
//---------------------------------------------------------------------------------------------------------
//Fun��es friend
//---------------------------------------------------------------------------------------------------------
template <typename Tipo> std::ostream &operator <<(std::ostream & out, const Array<Tipo> &in){
	out <<'(';
	for(int i=0;i<in.tamanho-1;i++) out<<in.array[i]<<',';
	out<<in.array[in.tamanho-1]<<')'<<std::endl;
	
	return out;
}
