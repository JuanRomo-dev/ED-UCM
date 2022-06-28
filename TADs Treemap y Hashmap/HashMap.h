/**
 * Implementación dinámica del TAD Diccionario usando tablas de búsqueda.
 * (c) Antonio Sánchez Ruiz-Granados, 2012
 * Modificado por Ignacio Fábregas, 2022
 */
#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <iostream>
#include "Exceptions.h"

/**
 * Implementación dinámica del TAD Diccionario usando una tabla hash abierta.
 * La tabla hash se redimensiona según lo necesite.
 * Se añade la opción de usar cualquier comparador hash
 * Las operaciones son:
 *    - HashMapVacio: operación generadora que construye una tabla hash vacía
 *    - Insert(clave, valor): generadora que añade una nueva pareja (clave, valor)
 *       a la tabla. Si la clave ya estaba se sustituye el valor.
 *    - erase(clave): operación modificadora. Elimina la clave de la tabla.
 *       Si la clave no está, la operación no tiene efecto.
 *    - at(clave): operación observadora que devuelve el valor asociado a una clave.
 *       Es un error preguntar por una clave que no existe.
 *    - contains(clave): operación observadora. Sirve para averiguar si una clave
 *       está presente en la tabla.
 *    - empty(): operación observadora que indica si la tabla tiene alguna clave introducida.
 *    - size(): operación observadora que indica el tamaño del diccionario.
 */
template <typename Clave, typename Valor, typename Hash = std::hash<Clave>>
class HashMap {
private:
	/**
	 * La tabla contiene un array dinámico de punteros a nodos. 
	 * Cada nodo contiene una clave, un valor y un puntero al siguiente nodo.
	 */
	class Nodo {
	public:
		/* Constructores. */
		Nodo(const Clave &clave, const Valor &valor) :
                clave(clave), valor(valor), sig(nullptr) {};
		
		Nodo(const Clave &clave, const Valor &valor, Nodo *sig) :
                clave(clave), valor(valor), sig(sig) {};

		/** Clave */
		Clave clave;

        /** Valor */
		Valor valor;

        /** Puntero al siguiente nodo */
		Nodo *sig;
	};  
	
public:
	
	/** Tamaño inicial de la tabla. */
	static const int TAM_INICIAL = 8;    
	
	/** Constructor por defecto que implementa HashMapVacio. O(1) */
	HashMap() : v(new Nodo*[TAM_INICIAL]), tam(TAM_INICIAL), numElems(0) {
		for (unsigned int i=0; i < tam; ++i)
            v[i] = nullptr;
	}
	
	/** Destructor; elimina las listas enlazadas */
	~HashMap() {
		libera();
	}
	
	/**
	 * Operación generadora que añade una nueva clave/valor a la tabla.
	 * Si la clave ya estaba, se sustituye el valor.
	 * O(k) amortizado (pocas veces vamos a ampliar)
	 *   donde k es el número de colisiones en el hash.
	 */
	void insert(const Clave &clave, const Valor &valor) {        
		// Si la ocupación es muy alta ampliamos la tabla
		float ocupacion = 100 * ((float) numElems) / tam;
		if (ocupacion > MAX_OCUPACION)
			amplia();
		// Obtenemos el índice asociado a la clave.
		unsigned int ind = hash(clave) % tam;
		// Si la clave ya existía, actualizamos su valor
		Nodo *nodo = buscaNodo(clave, v[ind]);
		if (nodo != nullptr) {
			nodo->valor = valor;
		} else { //si la clave es nueva, creamos un nuevo nodo y lo insertamos.
			v[ind] = new Nodo(clave, valor, v[ind]);
			numElems++;
		}
	}
	
	/**
	 * Operación modificadora que elimina una clave de la tabla.
	 * Si la clave no existía la operación no tiene efecto.
	 *  O(k) donde k es el número de colisiones en el hash.
	 */
	void erase(const Clave &clave) {
		// Obtenemos el índice asociado a la clave.
		unsigned int ind = hash(clave) % tam;
		// Buscamos el nodo que contiene esa clave y el nodo anterior.
		Nodo *act = v[ind];
		Nodo *ant = nullptr;
        buscaNodoConAnterior(clave, act, ant);
		if (act != nullptr) { //si está
			// Sacamos el nodo de la secuencia de nodos.
			if (ant != nullptr)
				ant->sig = act->sig;
			else
                v[ind] = act->sig;
			// Borramos el nodo.
			delete act;
			numElems--;
		}        
	}
	
	/**
	 * Operación observadora que devuelve el valor asociado a una clave.
	 * Si no existe se lanza una excepción.
	 *  O(k) donde k es el número de colisiones en el hash.
	 */
	const Valor &at(const Clave &clave) const {
		// Obtenemos el índice asociado a la clave.
		unsigned int ind = hash(clave) % tam;
		// Buscamos un nodo que contenga esa clave.
		Nodo *nodo = buscaNodo(clave, v[ind]);
		if (nodo == nullptr)
			throw EClaveErronea();
		return nodo->valor;
	}
	
	/** Operación observadora que indica si una clave aparece. */
	bool contains(const Clave &clave) const {
		// Obtenemos el índice asociado a la clave.
		unsigned int ind = hash(clave) % tam;
		// Buscamos un nodo que contenga esa clave.
		Nodo *nodo = buscaNodo(clave, v[ind]);
		return nodo != nullptr;
	}
	
	/** Operación observadora que devuelve si el diccionario es vacío. O(1) */
	bool empty() const {
		return numElems == 0;
	}

    /** Operación observadora que devuelve el tamaño del diccionario. O(1) */
    int size() const {
        return numElems;
    }
	
	/**
	 * Sobrecarga del operador [] que permite acceder al valor asociado
	 * a una clave y modificarlo. Si el elemento buscado no estaba, se inserta uno
	 * con el valor por defecto del tipo Valor.
	 * O(k) donde k es el número de colisiones en el hash.
	 */
	Valor &operator[](const Clave &clave) {
		// Obtenemos el índice asociado a la clave.
		unsigned int ind = hash(clave) % tam;
		// Buscamos un nodo que contenga esa clave.
		Nodo *nodo = buscaNodo(clave, v[ind]);
		if (nodo == nullptr) { // No está, se añade
			insert(clave, Valor());
			// ¡Ojo, ind puede cambiar si al insertar hubo expansion!
			ind = hash(clave) % tam;
			nodo = buscaNodo(clave, v[ind]);
		}        
		return nodo->valor;
	}
	
	// //
	// ITERADOR CONSTANTE Y FUNCIONES RELACIONADAS
	// //
	
	/**
	 * Clase interna que implementa un iterador sobre la lista
	 * que permite recorrer la tabla pero no permite modificarla.
	 */
	class ConstIterator {
	public:
		ConstIterator() : tabla(nullptr), act(nullptr), ind(0) {}

		void next() {
			if (act == nullptr)
                throw InvalidAccessException();
			// Buscamos el siguiente nodo de la lista de nodos.
			act = act->sig;
			// Si hemos llegado al final de la lista de nodos,
			// pasamos al siguiente índice
			while ((act == nullptr) && (ind < tabla->tam - 1)) {
				++ind;
                act = tabla->v[ind];
			}
		}
		
		const Clave &key() const {
			if (act == nullptr)
                throw InvalidAccessException();
			return act->clave;
		}
		
		const Valor &value() const {
			if (act == nullptr)
                throw InvalidAccessException();
			return act->valor;
		}
		
		bool operator==(const ConstIterator &other) const {
			return act == other.act;
		}
		
		bool operator!=(const ConstIterator &other) const {
			return !(this->operator==(other));
		}
		
		ConstIterator &operator++() {
			next();
			return *this;
		}
		
		ConstIterator operator++(int) {
			ConstIterator ret(*this);
			operator++();
			return ret;
		}
		
	protected:
		friend class HashMap;
		
		ConstIterator(const HashMap* tabla, Nodo* act, unsigned int ind) 
			: tabla(tabla), act(act), ind(ind) { }

        /** Puntero a la tabla que se está recorriendo */
		const HashMap *tabla;

        /** Puntero al nodo actual del recorrido */
        Nodo* act;

        /** Índice actual en el vector v */
		unsigned int ind;
	};
	
	/** Devuelve un iterador constante al principio del diccionario. */
	ConstIterator cbegin() const {
		unsigned int ind = 0;
		Nodo *act = v[0];
		while (ind < tam - 1 && act == nullptr) {
			ind++;
			act = v[ind];
		}
		return ConstIterator(this, act, ind);
	}
	
	/** Devuelve un iterador constante al final del recorrido */
	ConstIterator cend() const {
		return ConstIterator(this, nullptr, 0);
	}
	
	/**
	 * Devuelve un iterador a la posición de la clave.
	 * Si no existe la clave devuelve un iterador al final.
	 */
	ConstIterator find(const Clave &clave) const {
		// Obtenemos el índice asociado a la clave.
		unsigned int ind = hash(clave) % tam;
		// Buscamos un nodo que contenga esa clave.
		Nodo *nodo = buscaNodo(clave, v[ind]);
		return ConstIterator(this, nodo, ind); //si nodo == nullptr se devuelve cend
	}
	
	// //
	// ITERADOR NO CONSTANTE Y FUNCIONES RELACIONADAS
	// //

    /**
	 * Clase interna que implementa un iterador sobre la lista
	 * que permite recorrer la tabla e incluso modificarla.
	 */
	class Iterator {
	public:
		Iterator() : tabla(nullptr), act(nullptr), ind(0) {}

		void next() {
			if (act == nullptr)
                throw InvalidAccessException();
			// Buscamos el siguiente nodo de la lista de nodos.
			act = act->sig;
            // Si hemos llegado al final de la lista de nodos,
            // pasamos al siguiente índice
			while ((act == nullptr) && (ind < tabla->tam - 1)) {
				++ind;
                act = tabla->v[ind];
			}
		}
		
		const Clave &key() const {
			if (act == nullptr)
                throw InvalidAccessException();
			return act->clave;
		}
		
		Valor &value() const {
			if (act == nullptr)
                throw InvalidAccessException();
			return act->valor;
		}
		
		bool operator==(const Iterator &other) const {
			return act == other.act;
		}
		
		bool operator!=(const Iterator &other) const {
			return !(this->operator==(other));
		}
		
		Iterator &operator++() {
			next();
			return *this;
		}
		
		Iterator operator++(int) {
			Iterator ret(*this);
			operator++();
			return ret;
		}
		
	protected:
		friend class HashMap;
		
		Iterator(const HashMap* tabla, Nodo* act, unsigned int ind) 
		: tabla(tabla), act(act), ind(ind) { }

        /** Puntero a la tabla que se está recorriendo */
        const HashMap *tabla;

        /** Puntero al nodo actual del recorrido */
        Nodo* act;

        /** Índice actual en el vector v */
        unsigned int ind;
	};

    /** Devuelve un iterador al principio del diccionario. */
	Iterator begin() {
		unsigned int ind = 0;
		Nodo *act = v[0];
		while (ind < tam - 1 && act == nullptr) {
			ind++;
			act = v[ind];
		}
		return Iterator(this, act, ind);        
	}
	
	/** Devuelve un iterador al final del recorrido. */
	Iterator end() const {
		return Iterator(this, nullptr, 0);
	}

    /**
     * Devuelve un iterador a la posición de la clave.
     * Si no existe la clave devuelve un iterador al final.
     */
	Iterator find(const Clave &clave) {
		// Obtenemos el índice asociado a la clave.
		unsigned int ind = hash(clave) % tam;
		// Buscamos un nodo que contenga esa clave.
		Nodo *nodo = buscaNodo(clave, v[ind]);
		return Iterator(this, nodo, ind); //si nodo == nullptr se devuelve cend
	}
	
	
	// //
	// MÉTODOS DE "FONTANERÍA" DE C++ QUE HACEN VERSÁTIL A LA CLASE
	// //

    /** Dibujo del diccionario: Uso únicamente para debugear durante clase */
    friend std::ostream& operator<<(std::ostream& o, const HashMap& t){
        o<<"{";
        t.muestra(o);
        o<<"}";
        return o;
    }
	
	/** Constructor copia */
	HashMap(const HashMap<Clave, Valor, Hash> &other) {
		copia(other);
	}
	
	/** Operador de asignación */
	HashMap<Clave, Valor, Hash> &operator=(const HashMap<Clave, Valor, Hash> &other) {
		if (this != &other) {
			libera();
			copia(other);
		}
		return *this;
	}
	
private:
	
	/** Libera toda la memoria dinámica reservada para la tabla. */
	void libera() {
		// Liberamos las listas de nodos.
		for (unsigned int i=0; i < tam; i++)
			liberaNodos(v[i]);
		// Liberamos el array de punteros a nodos.
		if (v != nullptr) {
			delete[] v;
            v = nullptr;
		}
	}
	
	/** Libera un nodo y todos los siguientes. */
	static void liberaNodos(Nodo *prim) {
		while (prim != nullptr) {
			Nodo *aux = prim;
			prim = prim->sig;
			delete aux;
		}       
	}
	
	/**
	 * Hace una copia de la tabla que recibe como parámetro.
	 * Antes de llamar a este método se debe invocar al método "libera".
	 */
	void copia(const HashMap<Clave, Valor, Hash> &other) {
        tam = other.tam;
        numElems = other.numElems;
		// Reservar memoria para el array de punteros a nodos.
		v = new Nodo*[tam];
		for (unsigned int i=0; i < tam; ++i) {
            v[i] = nullptr;
			// Copiar la lista de nodos de other.v[i] a v[i].
			// La lista de nodos queda invertida con respecto a la original.
			Nodo *act = other.v[i];
			while (act != nullptr) {
                v[i] = new Nodo(act->clave, act->valor, v[i]);
				act = act->sig;
			}
		}
	}
	
	/** Duplica la capacidad del array de punteros a Nodos */
	void amplia() {
		// Creamos un puntero al array actual y anotamos su tamaño.
		Nodo **vAnt = v;
		unsigned int tamAnt = tam;
		// Duplicamos el array en otra posición de memoria.
		tam *= 2;
        v = new Nodo*[tam];
		for (unsigned int i=0; i < tam; ++i)
            v[i] = nullptr;
		
		// Recorremos el array original
		for (unsigned int i=0; i<tamAnt; ++i) {
            /* IMPORTANTE: al modificar el tamaño también se modifica el índice.
             * Por eficiencia NO copiamos Nodos, sino que los movemos.
             */
			Nodo *nodo = vAnt[i];
			while (nodo != nullptr) {
				Nodo *aux = nodo;
				nodo = nodo->sig;
				unsigned int ind = hash(aux->clave) % tam; //el nuevo índice
				aux->sig = v[ind];
                v[ind] = aux;
			}
		}
		// Borramos el array antiguo (ya no contiene ningún nodo).
		delete[] vAnt;
	}
	
	/**
	 * Busca un nodo a partir del nodo "act" que contenga la clave dada. Si lo 
	 * encuentra, "act" quedará apuntando a dicho nodo y "ant" al nodo anterior.
	 * Si no lo encuentra "act" quedará apuntando a nullptr.
	 * O(k) donde k es el número de colisiones que haya
	 */
	static void buscaNodoConAnterior(const Clave &clave, Nodo* &act, Nodo* &ant) {
		ant = nullptr;
		bool encontrado = false;
		while ((act != nullptr) && !encontrado) {
			// Comprobar si el nodo actual contiene la clave buscada
			if (act->clave == clave) {
				encontrado = true;
			} else {
				ant = act;
				act = act->sig;
			}
		}
	}
	
	/**
	 * Busca un nodo a partir de "n" que contenga la clave dada.
	 * Devuelve el nodo si lo encuentra, pero no el anterior.
	 * Se usa como función auxiliar
	 */
	static Nodo* buscaNodo(const Clave &clave, Nodo* n) {
		Nodo *act = n;
		Nodo *ant = nullptr;
        buscaNodoConAnterior(clave, act, ant);
		return act;
	}

    /**
     * Método para dibujar el diccionario. (Usado únicamente paa debuguear).
     */
    void muestra(std::ostream &out) const {
        bool primero = true;
        for (int i = 0; i < tam; i++) {
            Nodo * nodo = v[i];
            while(nodo != nullptr){ //si hay algo en esta posición recorremos la lista
                out << (primero ? " " : ", ");
                primero = false;
                out << nodo->clave << " -> " << nodo->valor;
                nodo = nodo->sig;
            }
        }
    }

	
	/**
	 * Ocupación máxima permitida antes de ampliar la tabla en tanto por cientos.
	 */
	static const unsigned int MAX_OCUPACION = 80;

    /** Array de punteros a Nodo */
	Nodo **v;

    /** Función hash usada */
    Hash hash;

    /** Tamaño del array v */
	unsigned int tam;

    /** Número de elementos en la tabla */
	unsigned int numElems;
};

#endif // __HASHMAP_H
