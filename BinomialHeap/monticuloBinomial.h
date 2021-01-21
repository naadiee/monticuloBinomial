//
// Created by Marlon on 20/1/21.
//
#include <cstddef>

#ifndef MONTICULOBINOMIAL_MONTICULOBINOMIAL_H
#define MONTICULOBINOMIAL_MONTICULOBINOMIAL_H


template <class T>
class Nodo {
public:

	/* ===================================================================== *\
	*                      CONSTRUCTORES Y DESTRUCTOR                      *
	\* ===================================================================== */

	/*
		4 posibles constructores:
			1. Por defecto, nos devuelve un nodo "vacio", con los atributos de tipo Nodo a NULL y el grado a 0
			2. Creamos un Nodo al cual le pasamos un valor (la clave), los atributos de tipo nodo a NULL
			3. Creamos un Nodo al cual le pasamos una Clave y además un padre.
			4. Creamos un nodo dado dos atributos de tipo nodo y la clave. Por un lado el "hijo izquierdo" y un  "hermnao".
			   Nos sirve para los nodos que estan en el centro del monticulo.
	*/

	//Cosntructores:

	Nodo() : _p(nullptr), _degree(0), _child(nullptr), _sibling(nullptr) {}
	Nodo(const T &key) : _p(nullptr), _key(key), _degree(0), _child(nullptr), _sibling(nullptr) {}
	Nodo(const T &key, Nodo *p) : _p(p), _key(key), _degree(0), _child(nullptr), _sibling(nullptr) {}
	//Nodo(const T &key, Nodo *hijo, Nodo *hermano) : _p(nullptr), _key(key), _degree(2), _child(hijo), _sibling(hermano) {}

	//Destructor:

	~Nodo() {
		delete _p;
		delete _child;
		delete _sibling;


	}

	/* ===================================================================== *\
	 *                          GETTERS AND SETTERS                          *
	\* ===================================================================== */

	// Observadora que nos devuelve la clave de un nodo
	T getValor() {
		return _key;
	}

	//Observadora que nos devuelve el grado del nodo


	Nodo<T>* getHijo() {
		return _child;
	}

	Nodo<T>* getHermanos() {
		return _sibling;
	}

	Nodo<T>* getPadre() {
		return _p;
	}

	//Setter que cambia el valor de la clave
	void setClave(T key) {
		this->_key = key;
	}

	//Setter que cambia el valor del grado
	void setGrado(int grado) {
		this->_degree = grado;
	}

	//Setter que cambia la referencia al hijo izquierdo
	void setChild(Nodo<T> *child) {
		this->_child = child;
	}

	//Setter que cambia la referencia al hermano
	void setSibling(Nodo<T> *sibling) {
		this->_sibling = sibling;
	}

	void setPadre(Nodo<T>* padre) {
		this->_p = padre;
	}

	int _degree; 
private:

	Nodo<T>* _p; // referencia al padre
	T _key;    // valor del nodo
	//int _degree; // grados del nodo
	Nodo<T>* _child; // referencia al hijo izquierdo
	Nodo<T>* _sibling; // referencia a los demas descendientes

	void linkTwo(Nodo<T>* t) {
		t->_p = this;
		t->_sibling = this->_child;
		_child = t;
		_degree = _degree + 1;
	}


};


template <class T>
class MonticuloBin {
private:

	Nodo<T>* _head; //Puntero que apunta al primer arbol del monticulo
	Nodo<T>* _minimum; // Puntero al Nodo cuyo valor es el más pequeño del monticulo
	int _numNodos; // Nos dice cuantos arboles cuelgan del monticulo


	/*
	 * El nodo z es el padre y el nodo y es el nuevo hijo de z
	 *
	 * Este metodo es estatico para poder llamar a esta funcion sin tener un objeto de tipo nodo creado.
	 * Es privado dado que va a ser usado por otras clases mas externas.
	 * */

	static void binomialLink(Nodo<T>* y, Nodo<T>* z) {
		y->setPadre(z); // y es ahora hijo de z, por lo que le le indicamos la referencia a su padre
		y->setSibling(z->getHijo()); // Los hermanos de z son los hijos de y
		z->setChild(y); // indicamos que y es el hijo de z
		z->setGrado(z->_degree + 1); // incrementamos en uno el grado de z
	}


	MonticuloBin<T>* merge(MonticuloBin<T>* h2) {

		/*
		 * Necesitamos tres punteros que van desde la posicion i hasta la i + 2, se van actualizando al siguiente nodo del monticulo, hasta recorrerlo.
		 * Esto es de utilidad ya que si queremos descolgar el nodo i+1 necesitmaos posteriormente unir el nodo i con el nodo i+2.
		 *
		 *
		 */

		Nodo<T>* ant = nullptr;
		Nodo<T>* act = nullptr;
		Nodo<T>* sig = nullptr;

		Nodo<T>* aux = nullptr;



		MonticuloBin<T>* monticulo_merge = new MonticuloBin(); // referencia al monticulo resultante de hacer el merge


		if (this->_head == nullptr && h2->_head != nullptr) {
			return h2; 
		}
		else if (this->_head != nullptr && h2->_head == nullptr) {
			return this; 
		}
		else if (this->_head == nullptr && h2->_head == nullptr) {
			return monticulo_merge; 
		}


		//Vemos que monticulo tiene el menor de los dos para asignarselo al "nuevo Monticulo"

		if (this->_minimum <= h2->_minimum) {
			monticulo_merge->_minimum = this->_minimum;
		}
		else {
			monticulo_merge->_minimum = h2->_minimum;
		}

		/*
		 * Ahora vamos a comprobar los grados de las dos cabezas para elegir cual de las dos es la menor y establecerla como cabeza del nuevo monticulo
		 * */

		if (this->_head->_degree >= h2->_head->_degree) {
			monticulo_merge->_head = h2->_head; // Inicio del nuevo monticulo
			sig = this->_head; // nodo a analizar para ver si es condidato posible como  _sibling del nodo monticulo_merge, es decir, siguiente arbol binomial
			//monticulo_merge->_head->setSibling(sig)

			act = monticulo_merge->_head; //establecemos el elemento actual, ya aceptado para el nuevo monticulo
		}
		else {//idem
			monticulo_merge->_head = this->_head;
			sig = h2->_head;
			act = monticulo_merge->_head;
		}

		/*
		 * Este algoritmo se va implentar como una especie de ventana deslizante, donde las variables ant (anterio), act (actual) son referencias a nodos los cuales ya han sido añadidos al
		 * nuevo monticulo, mientras que la variable sig (siguiente), es una referencia al posible candidato a ser añadido como sibling de actual en caso de que el grado de actual sea menor o igual a siguiente.
		 * En caso contrario siguiente ha de ser añadido entre medias de ant y de act ya que su grado (el de siguiente) es menor que el de actual.
		 * Asi podemos crear un nuevo monticulo el cual este ordendo en orden creciente de grados.
		 * */

		 /*
		  * Haremos un bucle while cuya condicion sea que sig != null, esto se puede traducir como "Mientras haya un posible condidato iteramos". Cuando ya no haya mas candidatos para puesto que
		  * habremos analizado todos los nodos.
		  * */

		while (sig != nullptr) {

			// Caso en donde siguiente hay que meterlo entre medias de ant y act
			if (act->_degree > sig->_degree) {

				/*
				 * Dos posibles casos:
				 * 1. Que no haya anterior, el nodo sig, pasa a estar antes de act(es la cabeza del monticulo) y como no hay anterior previo siguiente pasa a ser la nueva cabeza del monticulo
				 * asi que se actualiza esta referencia y actualizamos punteros para seguir explorando
				 *
				 * 2. Que haya anterio. Aqui habría que hay que unir a sig con ant (el sibling de ant apunta al nodo sig )
				 *
				 * Igual, actualizamos ant, sig y act para seguir.
				 * */

				if (ant == nullptr) {// no hay anterior, actualizamos cabeza
					monticulo_merge->_head = sig;
				}
				else {// hay anterior, cogamos a sig de ant
					ant->setSibling(sig);
				}

				aux = sig->getHermanos(); // guardamos los hermanos de sig, futuros candidatos a formar el monticulo
				ant = sig; // como siguiente ya ha sido aceptado pasa a formar parte del monticulo, le asignamos ant
				sig->setSibling(act); // unimos el nodo siguiente con actual
				sig = aux; // actualizamos para seguir explorando.

			}
			else {
				//Actual es menor o igual que sig, unimos a sig con act y actualizamos variables
				//guardamos act._sibling porque lo perderemos al juntarlo con sig

				aux = act->getHermanos();
				//metemos al nodo sig en el monticulo
				act->setSibling(sig);
				//deslizamos la ventana
				ant = act;
				act = sig;
				sig = aux;

			}

		}
		return monticulo_merge;
	}

	void setHead(Nodo<T>* head) {
		this->_head = head; 
	}



public:


	/*
	  2 Constructores posibles:
		1. Constructor por defecto, vacio
		2. Constructor que crea un monticulo con un unico arbol Binomial

	 */

	MonticuloBin() : _head(nullptr), _minimum(nullptr), _numNodos(0) {}

	MonticuloBin(Nodo<T>* arbol) : _head(arbol), _minimum(arbol) {}


	//Destructor
	~MonticuloBin() {
		delete _head;
		delete _minimum;

		_head = nullptr;
		_minimum = nullptr;
	}


	static MonticuloBin<T>* join(MonticuloBin<T>* h1, MonticuloBin<T>* h2) {
		//Primero fusionamos h1 y h2 para hacer un solo monticulo, ordenado por grados

		MonticuloBin<T>* mergeMon = h1->merge(h2);

		//Puede que la referencia devuelta por merge sea una posicion de memoria null, en caso de que esto seceda terminamos devolvindo la referencia null sin necesidad de seguir con la union
		if (mergeMon->_head == nullptr) {
			return mergeMon; 
		}

		//revisamos que nos ha dado una referencia valida

		if (mergeMon) {
			//declaramos tres variables de tipo Nodo ya que usaremos una especie de ventana deslizante de 3 elementos

			Nodo<T>* ant = nullptr;
			Nodo<T>* act = mergeMon->getCabeza();
			Nodo<T>* sig = nullptr;

			//Tambien usare dos variables para almacenar los grados de los arbolres, facilitando la lectura del codigo
			int grado1, grado2;

			while (act->getHermanos() != nullptr) {

				//asignamos los respectivos valores de los dos arbolres a analizar
				sig = act->getHermanos();
				grado1 = act->_degree;
				grado2 = sig->_degree;

				/* En la teoria hay 4 posibes casos
				 *  1. Si no hay arbol Bk se avanza
				 *  2. si hay dos arboles juntos con grados distintos, se avanza a k + 1
				 *  3. Hay dos arboles con mismo grado, se unen para formar un arbol Bk + 1 y se avanza
				 *  4. Tres arbolres con mismo grado, se deja el de la izquierda y juntamos los dos siguientes
				 *
				 *  el csao 1, 2 y tres se puede englobal en una rama del if y el 2 en otra
				 *
				 *  veamos...
				 *
				 * */

				if ((grado1 == grado2) && (grado2 != sig->getHermanos()->_degree)) {
					/*
					 * esta condicion nos dice: que dados dos arboles consecutivos de mismo grado cuyo tercer elemento tiene un grado distinto a los dos anteriores, si true podremos fusionarlps
					 * dentro de estaa rama nos surgen otros dos casos mas, quien es el padre y quien es el hijo.
					 * Para determinar esto usaremos la clave el arbol cuyo nodo raiz sea menor sera el padre y el otro su hijo.
					 *
					 * */

					if (act->getValor() <= sig->getValor()) {
						//padre es actual
						binomialLink(sig, act);
						//actualizamos variables
						ant = act;
						act = act->getHermanos();
					}
					else {

						// posibles dos casos: que no haya anterior, esto significa que anterior es la cabeza del monticulo por lo que hay que actualizar esta refenrencia.
						if (ant) {
							ant->setSibling(sig);
							binomialLink(act, sig);

						}
						else {
							mergeMon->_head = sig;
							binomialLink(act, sig);

						}
						ant = sig;
						act = sig->getHermanos(); // CUIDADO

					}
				}
				else {
					//lo preparamos para que en la siguiente iteracion apunte a los correspondientes elementos
					if (ant) {
						ant = act;
						act = sig;
					}
					else {
						//mergeMon. = act;

						mergeMon->setHead(act); 
					}
				}



			}
		}
		return mergeMon;
	}


	Nodo<T>* getMinimo() {
		return _minimum;
	}

	Nodo<T>* getCabeza() {
		return _head;
	}


	void insertar(T key) {

		//creamos una hoja que solo contiene la clave, no tiene ni padres ni hijos ni hermanos
		Nodo<T>* nuevo = new Nodo<T>(key);

		//creamos un monticulo unitario con la hoja creada

		MonticuloBin<T>* nuevoMonticulo = new MonticuloBin<T>(nuevo);

		//hacemos la union de ambos monticulos:

		MonticuloBin<T>* monticulo_resul = join(this, nuevoMonticulo);

		//Nos devuelve un monticulo con el elemento insertado, hay que actualizar los atributos de nuestro monticulo "this"

		this->_head = monticulo_resul->getCabeza();
		this->_minimum = monticulo_resul->getMinimo();
	}

};









#endif //MONTICULOBINOMIAL_MONTICULOBINOMIAL_H
