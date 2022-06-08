#include "rand.h"
#include "array.h"
#include <sstream>
#include <string>
#include <Python.h>

using namespace std;
PyObject *pName, *pModule, *pArgs, *pValue, *pFunc;

//passar isto a double implementar limaites nas variáveis do cromossoma
int ffitness(Array<float> &);

/*
float noIntervalo(float r,float a,float b){
	return (1-r)*a+r*b;
}*/

class Cromossoma{
	Array <float> string;
	//Array 
	friend ostream  &operator <<(ostream &out,Cromossoma in);

	public:
	Cromossoma(int tamanho){string.setDim(tamanho+1);}
	Cromossoma(){}
	Cromossoma(Cromossoma &in){string=in.string;}

	int  	getDim()		{return string.getDim()-1;}
	float   getFitness()		{return string[string.getDim()-1];}
	float   &Fitness()  		{return string[string.getDim()-1];}
	void 	calculaFitness()	{ffitness(string);}
	void 	setDim(int tamanho)	{string.setDim(tamanho+1);}
	void 	setCromossoma(Random &r);
	void 	mutacaoUniforme(float prob,Random &r);
	void 	mutacaoDeFronteira(float prob1,float prob2,Random &r);
	void 	mutacaoNaoUnif(int greacao,int geracao_max,float p_forma,float prob,Random &r);
	void    mutacaoInversa(float prob,Random &r);
	void 	mutacaoReplica(Random &r);

	float 		&operator [](int i)	   {return string[i];}
	Cromossoma	operator   =(Cromossoma in){string=in.string; return *this;}
};

void Cromossoma::setCromossoma(Random &r){

	for(int i=0;i<string.getDim()-1;i++) string[i]=r.rand();
	calculaFitness();
}

void Cromossoma::mutacaoUniforme(float prob,Random &r){

	for(int i=0;i<string.getDim()-1;i++) if(r.flip(prob)) string[i]=r.rand();
	calculaFitness();
}

void Cromossoma::mutacaoDeFronteira(float prob1,float prob2,Random &r){

	for(int i=0;i<string.getDim()-1;i++) if(r.flip(prob1)) string[i]=r.flip(prob2);
	calculaFitness();
}

void Cromossoma::mutacaoNaoUnif(int geracao,int geracao_max,float p_forma,float prob,Random &r){

	if(prob>0.5)
		for(int i=0;i<string.getDim()-1;i++)
			string[i]+=(1-string[i])*pow(r.rand()*(1-(float)geracao/geracao_max),p_forma);
	else
		for(int i=0;i<string.getDim()-1;i++)
			string[i]-=string[i]*pow(r.rand()*(1-(float)geracao/geracao_max),p_forma);
	calculaFitness();
}

void Cromossoma::mutacaoInversa(float prob,Random &r){

	for(int i=0;i<string.getDim()-1;i++) if(r.flip(prob)) string[i]=1-string[i];
	calculaFitness();
}

void Cromossoma::mutacaoReplica(Random &r){
	int a=r.randArray(string.getDim()-1);

	if(a<string.getDim()-2) string[a+1]=string[a];
	calculaFitness();
}

ostream &operator <<(ostream &out,Cromossoma in){

	for(int i=0;i<in.string.getDim();i++)out<<in.string[i]<<" ";
	out<<endl;
	return out;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class Populacao{
	Array<Cromossoma> array;
	Random a;

	public:
	//Construtores
			Populacao(){}
			Populacao(int tamanho,int n_vars);
			Populacao(Populacao &in);//C¢pia do objecto
	//M‚todos
	inline int	getDim() {return array.getDim();}
	//inline int 	getBits(){return n_bits;}
	Cromossoma      &maiorFitness();
	Cromossoma 	&menorFitness();
	void		setDim(int tamanho,int n_vars);
	void 		Torneio(int n_torneios);
	float 		calculaSemelhanca(int i,int j);//incesto d  merda
	void		Crossover();
	void		CrossoverArit();
	void 		Mutacao1();
	void 		Mutacao2();
	void 		Mutacao3(int geracao,int g_max);
	void 		Mutacao4(int geracao,int g_max,float p_forma);
	void 		Mutacao5();
	void            Mutacao6();
	//operadores
	inline Cromossoma	&operator [](int indice){return array[indice];}
	Populacao		operator =(Populacao in);
};

Cromossoma &Populacao::maiorFitness(){
	float fit=array[0].getFitness();
	int i;

	for(i=1;i<array.getDim();i++)
		if(array[i].getFitness()>fit) fit=array[i].getFitness();

	for(i=0;i<array.getDim();i++) if(fit==array[i].getFitness()) break;
	return array[i];
}

Cromossoma &Populacao::menorFitness(){
	float fit=array[0].getFitness();
	int i;

	for(i=1;i<array.getDim();i++)
		if(array[i].getFitness()<fit) fit=array[i].getFitness();

	for(i=0;i<array.getDim();i++) if(fit==array[i].getFitness()) break;
	return array[i];
}

void  Populacao::setDim(int tamanho,int n_vars){

	array.setDim(tamanho);
	for(int i=0;i<tamanho;i++){
		array[i].setDim(n_vars);
		array[i].setCromossoma(a);
	}

}

void Populacao::Torneio(int n_torneios){
	int i,j,n_gladiadores=(array.getDim())/2;
	Array<Populacao> temp(n_gladiadores);

	if(n_torneios > array.getDim()) n_torneios=array.getDim();
	Array<int> indice(n_torneios);
	//escalonamento para o torneio

	for(i=0;i<indice.getDim();i++) indice[i]=a.randArray(array.getDim());
	temp[0].setDim(n_torneios,array[0].getDim());
	for(i=0;i<temp[0].getDim();i++)temp[0][i]=array[indice[i]];


	for(i=1;i<temp.getDim();i++){
		temp[i].setDim(n_torneios,array[0].getDim());
		for(j=0;j<temp[i].getDim();j++) temp[i][j]=array[a.randArray(array.getDim())];
	}
	//torneio
	for(i=1;i<n_gladiadores;i++)
		for(j=0;j<n_torneios;j++)
			if(temp[i][j].getFitness()<temp[0][j].getFitness())
				temp[0][j]=temp[i][j];

	for(i=0;i<n_torneios;i++) array[indice[i]]=temp[0][i];
}

void Populacao::Mutacao1(){
     int i=a.randArray(array.getDim());

     array[i].mutacaoDeFronteira(a.rand(),a.rand(),a);
}

void Populacao::Mutacao2(){
     int i=a.randArray(array.getDim());

     array[i].mutacaoUniforme(a.rand(),a);
}

void Populacao::Mutacao3(int geracao,int g_max){
     int i=a.randArray(array.getDim());

     array[i].mutacaoNaoUnif(geracao,g_max,(float)(g_max-geracao)/g_max,a.rand(),a);
}

void Populacao::Mutacao4(int geracao,int g_max,float p_forma){
     int i=a.randArray(array.getDim());

     array[i].mutacaoNaoUnif(geracao,g_max,p_forma,a.rand(),a);
}

void Populacao::Mutacao5(){
     int i=a.randArray(array.getDim());

     array[i].mutacaoReplica(a);
}

void Populacao::Mutacao6(){
     int i=a.randArray(array.getDim());

     array[i].mutacaoInversa(a.rand(),a);
}



float Populacao::calculaSemelhanca(int i,int j){
	int coincide=0;

	for(int k=0;k<array[0].getDim();k++)if(array[i][k]==array[j][k]) coincide++;
	return (float)coincide/array.getDim();
}

void Populacao::Crossover(){
	Array <int>     corte(array[0].getDim());
	Populacao	Filho(2,array[0].getDim());
	int 		pai=a.randArray(array.getDim()),mae=a.randArray(array.getDim()),i;

	for(i=0;i<corte.getDim();i++)corte[i]=a.flip(a.randn(0.5,0.005));

	for(i=0;i<corte.getDim();i++)
		if(corte[i]){
			Filho[0][i]=array[pai][i];
			Filho[1][i]=array[mae][i];
		} else{
			Filho[1][i]=array[pai][i];
			Filho[0][i]=array[mae][i];
		}

	for(i=0;i<Filho.getDim();i++){ //alterar
		Filho[i].mutacaoUniforme(calculaSemelhanca(pai,mae)*0.1,a);
		Filho[i].calculaFitness();
		array[a.randArray(array.getDim())] = Filho[i];
	}
}

void Populacao::CrossoverArit(){
	int pai=a.randArray(array.getDim()),mae=a.randArray(array.getDim());
	Cromossoma Filho(array[0].getDim());
	float r;

	for(int i=0;i<array[pai].getDim();i++){
		r=a.rand();
		Filho[i]=(1-r)*array[pai][i] + r*array[mae][i];
	}
	Filho.calculaFitness();
	array[a.randArray(array.getDim())]=Filho;
}
/*
void Populacao::CrossoverArit2(){
	int pai=a.RandArray(array.getDim()),mae=a.RandArray(array.getDim());
	Cromossoma Filho(array[0].getDim());
	float r;

	for(int i=0;i<array[pai].getDim();i++){
		if(a.Flip(0.8))Filho[i]=0.5*fabs(array[pai][i] + a.rand()*(array[pai][i] - array[mae][i]));
		else Filho[i]=(array[pai][i]+array[mae][i])/2;
	}
	Filho.calculaFitness();
	array[a.RandArray(array.getDim())]=Filho;
} */
//-Construtores------------------------------------------


Populacao::Populacao(Populacao &in){

	array.setDim(in.array.getDim());
	for(int i=0;i<array.getDim();i++) array[i]=in.array[i];
}

Populacao::Populacao(int tamanho,int n_vars){

	array.setDim(tamanho);
	for(int i=0;i<tamanho;i++){
		array[i].setDim(n_vars);
		array[i].setCromossoma(a);
	}

}
//-operadores---------------------------------------------------
Populacao  Populacao::operator =(Populacao in){

	   array.setDim(in.array.getDim());
	   for(int i=0;i<array.getDim();i++) array[i]=in.array[i];
	   return *this;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//Solu‡Æo do float ga (Matlab) 0.8917 -15.1644        0.141918*2pi=0.8917

#define TAMANHO_POP  10
#define N_TORNEIOS   4
#define N_MUTACOES   2
#define N_CROSSOVERS 10
#define N_VARS 1
#define GERACOES 100


int ffitness(Array<float> &var){
	//float x=noIntervalo(var[0],0,2*M_PI);
	//cout << x <<endl;
	float x = var[0];
	
    	PyRun_SimpleString("import sys");
    	PyRun_SimpleString("sys.path.append('')");
    	pName = PyUnicode_FromString("atest");
    	pModule = PyImport_Import(pName);
    	Py_DECREF(pName);

    	if(pModule == NULL)
    	{
        	printf("PMod is null\n");
        	PyErr_Print();
        	return 127;
    	}

    	pFunc = PyObject_GetAttrString(pModule, "doStuff");
    	pArgs = PyTuple_New(1);
    	pValue = PyFloat_FromDouble(x);
    	PyTuple_SetItem(pArgs, 0, pValue);

    	pValue = PyObject_CallObject(pFunc, pArgs);
    	Py_DECREF(pArgs);
        //printf("Returned val: %ld\n", PyLong_AsLong(pValue));
    	var[var.getDim()-1]=PyFloat_AsDouble(pValue);

	
        return 0;	
}

double algoritmo(int geracoes){
	Populacao pop(TAMANHO_POP,N_VARS); //falta implementar os limites das variáveis ainda não sei como
	Cromossoma aux;
	int i;
        Py_Initialize();
       
        
	for(int j=0;j<GERACOES;j++){
		aux=pop.menorFitness();
		for(i=0;i<pop.getDim();i++) cout<<pop[i];

		for(i=0;i<N_MUTACOES;i++){
			//pop.Mutacao4(i,GERACOES,5);
			pop.Mutacao1();
		}
		for(i=0;i<N_CROSSOVERS;i++) pop.CrossoverArit();
		pop.Mutacao6();
		pop.Torneio(N_TORNEIOS);
		pop.maiorFitness()=aux; //elitismo
	}
	cout<<"--------------------------------------------------------"<<endl;
	cout<<aux<<endl;
	//*a = aux[0];
	return aux[0];


       /*Py_DECREF(pValue);
    	Py_XDECREF(pFunc);
    	Py_DECREF(pModule);

    	Py_Finalize();*/
    	
    	
}



