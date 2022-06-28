/**
 * Implementación del TAD Set utilizando árboles de búsqueda.
 * Añade comparador
 * (c) Marco Antonio Gómez Martín y Miguel Gómez-Zamalloa, 2018
 * Adaptado por Ignacio Fábregas, 2022
*/
#ifndef __TREESETC_H
#define __TREESETC_H

#include "Exceptions.h"
#include "Stack.h" // Usado internamente por los iteradores

/**
 * Implementación dinámica del TAD Set utilizando árboles de búsqueda (no auto-balanceados).
 * Se añade un comparador: objeto función que acepta dos valores de tipo T y devuelve si el
 *          primero es menor que el segundo. Por defecto se toma el < T si está definido
 * Como no necesitamos compartir subestructuras no es necesario usar punteros inteligentes
 * Las operaciones son:
 *    - TreeSetVacio: operación generadora que construye un conjunto vacío (árbol de búsqueda vacío).
 *    - Insert(elem): generadora que añade un nuevo elem al conjunto. Si elem ya estaba no se hace nada.
 *    - erase(elem): operación modificadora. Elimina elem del conjunto.  Si elem no está la operación no tiene efecto.
 *    - contains(elem): operación observadora. Determina si elem pertenece conjunto.
 *    - empty(): operación observadora que indica si el conjunto es vacío.
 *    - size(): operación observadora que devuelve el tamaño del conjunto
 */

template <class T, class Comparador = std::less<T>>
class TreeSetC {
private:
	/**
	 Clase nodo que almacena internamente el dato
	 y los punteros al hijo izquierdo y al hijo derecho.
	 */
	class Nodo {
	public:
		Nodo() : iz(nullptr), dr(nullptr) {}
		Nodo(const T &elem)
			: elem(elem), iz(nullptr), dr(nullptr) {}
		Nodo(Nodo *iz, const T &elem, Nodo *dr)
			: elem(elem), iz(iz), dr(dr) {}

		T elem;
		Nodo *iz;
		Nodo *dr;
	};

public:

	/** Constructor; operacion EmptyTreeSet. O(1) */
	TreeSetC() : ra(nullptr) { numElems = 0;}

	/** Destructor; elimina la estructura jerárquica de nodos. O(n) */
	~TreeSetC() {
		libera();
        ra = nullptr;
        numElems = 0;
	}

	/** Operación generadora que añade un nuevo elemento al conjunto. O(log n) */
	void insert(const T &elem) {
        ra = insertaAux(elem, ra);
	}

	/**
	 * Operación modificadora que elimina un elemento del conjunto.
	 * Si elem no existía la operación no tiene efecto.
	 * O(log n)
	 */
	void erase(const T &elem) {
        ra = borraAux(ra, elem);
	}

	/** Operación observadora que comprueba si elem pertenece al conjunto.*/
	bool contains(const T &elem) const {
		return (buscaAux(ra, elem) != nullptr) ? true : false;
	}

	/** Operación observadora que comprueba si el conjunto es vacío. */
	bool empty() const {
		return ra == nullptr;
	}

    /** Operación observadora que que indica el tamaño del conjunto. */
    int size() const{
        return numElems;
    }

    /** Dibujo del árbol: Uso únicamente para debugear durante clase */
    friend std::ostream& operator<<(std::ostream& o, const TreeSetC<T, Comparador>& t){
        o  << "==== Tree =====" << std::endl;
        graph_rec(o, 0, t.ra);
        o << "===============" << std::endl;
        return o;
    }


	// //
	// ITERADOR CONSTANTE Y FUNCIONES RELACIONADAS
	// //

	/** Clase interna que implementa un iterador que permite
	 * recorrer el árbol pero no  modificarlo.
	 */
	class ConstIterator {
	public:
		ConstIterator() : act(nullptr) {}

        /** O(log n) */
		void next() {
			if (act == nullptr)
                throw InvalidAccessException();
			// Si hay hijo derecho, saltamos al primero en inorden del hijo derecho
			if (act->dr != nullptr)
                act = primeroInOrden(act->dr);
			else {
				// Si no, vamos al primer ascendiente no visitado
				if (ascendientes.empty()) //Ya no hay por visitar
                    act = nullptr;
				else {
                    act = ascendientes.top();
					ascendientes.pop();
				}
			}
		}

		const T &elem() const {
			if (act == nullptr)
                throw InvalidAccessException();
			return act->elem;
		}

		const T& operator*() const {
			return elem();
		}

		bool operator==(const ConstIterator &other) const {
			return act == other.act;
		}

		bool operator!=(const ConstIterator &other) const {
			return !(this->operator==(other));
		}

        /** O(log n) */
		ConstIterator &operator++() {
			next();
			return *this;
		}

        /** O(log n) */
		ConstIterator operator++(int) {
			ConstIterator ret(*this);
			operator++();
			return ret;
		}

	protected:
		friend class TreeSetC;

		ConstIterator(Nodo *act) {
            this->act = primeroInOrden(act);
		}

		/**
		 *  Busca el primer elemento en inorden de la estructura jerárquica de nodos.
		 *  Va apilando sus ascendientes para poder "ir hacia atrás" cuando sea necesario.
		 */
		Nodo *primeroInOrden(Nodo *p) {
			if (p == nullptr)
				return nullptr;
			while (p->iz != nullptr) {
				ascendientes.push(p);
				p = p->iz;
			}
			return p;
		}

        /** Puntero al nodo actual del recorrido */
		Nodo *act;

		/** Ascendientes del nodo actual aún por visitar */
		Stack<Nodo*> ascendientes;
	};

	/**
	 * Devuelve el iterador constante al principio del recorrido inorden.
	 * Es decir, empieza por el elemento más pequeño.
	 * O(log n)
	 */
	ConstIterator cbegin() const {
		return ConstIterator(ra);
	}

	/** Devuelve un iterador constante al final del recorrido (fuera de éste). O(1) */
	ConstIterator cend() const {
		return ConstIterator(nullptr);
	}

    /**
     * Devuelve un iterador constante al nodo con elemento c.
     * Usa el comparador
     * Devuelve el iterador cend si no está
     */
	ConstIterator find(const T &c) const {
		Stack<Nodo*> ascendientes;
		Nodo *p = ra;
		while ((p != nullptr) && cless(p->elem, c) && cless(c, p->elem)) {
			if (cless(c, p->elem)){
				ascendientes.push(p);
				p = p->iz;
			} else
				p = p->dr;
		}
		ConstIterator ret;
		ret.act = p;
		if (p != nullptr)
			ret.ascendientes = ascendientes;
		return ret;
	}

	// //
	// ITERADOR NO CONSTANTE Y FUNCIONES RELACIONADAS
	// //

	/**
	 * Clase interna que implementa un iterador sobre el árbol de búsqueda que permite recorrerlo.
	 * Este iterador tampoco permite modificaciones de la estructura (es idéntico a ConstIterator).
	 * Se mantienen ambos por consistencia con el resto de contenedores.
	 */
	class Iterator {
	public:
		Iterator() : act(nullptr) {}

        /** O(log n) */
        void next() {
            if (act == nullptr)
                throw InvalidAccessException();
            // Si hay hijo derecho, saltamos al primero en inorden del hijo derecho
            if (act->dr != nullptr)
                act = primeroInOrden(act->dr);
            else {
                // Si no, vamos al primer ascendiente no visitado
                if (ascendientes.empty()) //Ya no hay por visitar
                    act = nullptr;
                else {
                    act = ascendientes.top();
                    ascendientes.pop();
                }
            }
        }

		const T &elem() const {
			if (act == nullptr)
                throw InvalidAccessException();
			return act->elem;
		}

		const T& operator*() const {
			return elem();
		}

		bool operator==(const Iterator &other) const {
			return act == other.act;
		}

		bool operator!=(const Iterator &other) const {
			return !(this->operator==(other));
		}

        /** O(log n) */
		Iterator &operator++() {
			next();
			return *this;
		}

        /** O(log n) */
		Iterator operator++(int) {
			Iterator ret(*this);
			operator++();
			return ret;
		}

	protected:
		friend class TreeSetC;

		Iterator(Nodo *act) {
            this->act = primeroInOrden(act);
		}

        /**
         *  Busca el primer elemento en inorden de la estructura jerárquica de nodos.
         *  Va apilando sus ascendientes para poder "ir hacia atrás" cuando sea necesario.
         *  O(log n)
         */
		Nodo *primeroInOrden(Nodo *p) {
			if (p == nullptr)
				return nullptr;

			while (p->iz != nullptr) {
				ascendientes.push(p);
				p = p->iz;
			}
			return p;
		}
        /** Puntero al nodo actual del recorrido */
        Nodo *act;

        /** Ascendientes del nodo actual aún por visitar */
        Stack<Nodo*> ascendientes;
	};

    /**
     * Devuelve el iterador al principio del recorrido inorden.
     * Es decir, empieza por el elemento más pequeño.
     * O(log n)
     */
	Iterator begin() {
		return Iterator(ra);
	}

    /** Devuelve un iterador constante al final del recorrido (fuera de éste). O(1) */
	Iterator end() const {
		return Iterator(nullptr);
    }

    /**
     * Devuelve un iterador constante al nodo con elemento c.
     * Usa el comparador
     * Devuelve el iterador end si no está.
     * O(log n)
     */
	Iterator find(const T &c) {
		Stack<Nodo*> ascendientes;
		Nodo *p = ra;
		while ((p != nullptr) && cless(p->elem, c) && cless(c, p->elem)) {
			if (cless(c, p->elem)) {
				ascendientes.push(p);
				p = p->iz;
			} else
				p = p->dr;
		}
		Iterator ret;
		ret.act = p;
		if (p != nullptr)
			ret.ascendientes = ascendientes;
		return ret;
	}


	// //
	// MÉTODOS DE "FONTANERÍA" DE C++ QUE HACEN VERSÁTIL  A LA CLASE
	// //

	/** Constructor copia. O(n) */
	TreeSetC(const TreeSetC<T, Comparador> &other) : ra(nullptr) {
		copia(other);
	}

	/** Operador de asignación. O(n) */
	TreeSetC<T, Comparador> &operator=(const TreeSetC<T, Comparador> &other) {
		if (this != &other) {
			libera();
			copia(other);
		}
		return *this;
	}

protected:

	void libera() {
		libera(ra);
	}

	void copia(const TreeSetC &other) {
        ra = copiaAux(other.ra);
        numElems = other.numElems;
	}

private:
    /** para el dibujo del árbol */
    static const int TREE_INDENTATION = 4;

	/**
	 * Elimina todos los nodos de una estructura arbórea que comienza con el puntero ra.
	 * O(n)
	 */
	static void libera(Nodo *ra) {
		if (ra != nullptr) {
			libera(ra->iz);
			libera(ra->dr);
			delete ra;
		}
	}

	/**
	 * Copia la estructura jerárquica de nodos pasada como parámetro (puntero a su raiz).
	 * Devuelve un puntero a una nueva estructura jerárquica, copia de la anterior
	 * O(n)
	 */
	static Nodo* copiaAux(Nodo *raiz) {
		if (raiz == nullptr)
			return nullptr;
		return new Nodo(copiaAux(raiz->iz), raiz->elem, copiaAux(raiz->dr));
	}

	/**
	 * Inserta un elem en la estructura jerárquica que comienza ebn el puntero pasado como parámetro.
	 * Usa el comparador.
	 * EL puntero puede ser nullptr, por lo que se creará un nuevo nodo (la nueva raíz).
	 * El método devuelve un puntero a la raíz de la estructura modificada.
	 * En condiciones normales coincidirá con el parámetro redibido; sólo cambiará cuando era vacía.
	 * O(log n)
	 */
    Nodo* insertaAux(const T &elem, Nodo *p) {
        if (p == nullptr) {
            ++numElems; //hay un nuevo elemento
            return new Nodo(elem);
        }
        else if (cless(elem, p->elem)) { // (elem < p->elem)
            p->iz = insertaAux(elem, p->iz);
            return p;
        } else if (cless(p->elem, elem)) { // (p-> elem < elem)
            p->dr = insertaAux(elem, p->dr);
            return p;
        } else // (elem === p->elem)
            return p;
    }


    /**
	 * Busca un elemento en la estructura de nodos cuya raíz se pasa como parámetro.
     * Usa el comparador.
	 * Devuelve un puntero al nodo si lo encuentra (o nullptr si no está).
	 * O(log n)
	 */
	Nodo* buscaAux(Nodo *p, const T &elem) const {
		if (p == nullptr)
			return nullptr;
        else if (cless(elem, p->elem))  // (elem < p->elem)
            return buscaAux(p->iz, elem);
         else if (cless(p->elem, elem)) // (p-> elem < elem)
            return buscaAux(p->dr, elem);
         else  // (elem === p->elem)
            return p;
	}

	/**
	 * Elimina (si existe) un elemento de la estructura de nodos apuntada por p.
	 * Usa el comparador
	 * Si el elemento estaba en la propia raíz, éste cambiará. Si no cambia se devuelve p.
	 * O(log n)
	*/
	Nodo* borraAux(Nodo *p, const T &elem) {
		if (p == nullptr)
			return nullptr;
        else if (cless(elem, p->elem)) { // (elem < p->elem)
            p->iz = borraAux(p->iz, elem);
            return p;
        } else if (cless(p->elem, elem)) { // (p-> elem < elem)
            p->dr = borraAux(p->dr, elem);
            return p;
        } else { // (elem === p->elem)
            --numElems; //eliminamos un elemento
            return borraRaiz(p);
        }
	}

	/**
	 * Borra la raíz de la estructura de nodos y devuelve un puntero a la nueva raíz.
	 * Ésta raíz nueva garantiza que la estructura sigue siendo un árbol de búsqueda correcto.
	 * O(log n) (si hay que buscar el mínimo)
	 */
	static Nodo* borraRaiz(Nodo *p) {
		Nodo *aux;
		if (p->iz == nullptr) {// Si no hay hijo izquierdo, la raíz pasa a ser el hijo derecho
			aux = p->dr;
			delete p;
			return aux;
		} else
		if (p->dr == nullptr) {// Si no hay hijo derecho, la raíz pasa a ser el hijo izquierdo
			aux = p->iz;
			delete p;
			return aux;
		} else { //hay hijo derecho e izquierdo
                 //Convertimos el elemento más pequeño del hijo derecho en la raíz
			return mueveMinYBorra(p);
		}
	}

	/**
	 * Método auxiliar para el borrado. Busca el elemento más pequeóo del hijo derecho
	 * que pasa a ser la nueva raíz (que devolverá), borra la antigua raíz y cose los punteros:
	 *   - El mínimo pasa a ser la raíz.
	 *   - El hijo izquierdo del padre del mínimo pasa a ser el antiguo hijo derecho de ese mínimo.
	 * O(log n)
	 */
	static Nodo *mueveMinYBorra(Nodo *p) {
        /*
         * Vamos bajando hasta encontrar el elemento más pequeño (aquel sin hijo izquierdo).
         * También guardamos el padre.
         */
		Nodo *padre = nullptr;
		Nodo *aux = p->dr;
		while (aux->iz != nullptr) {
			padre = aux;
			aux = aux->iz;
		}
		if (padre != nullptr) { //hay un padre, se cambia
			padre->iz = aux->dr;
			aux->iz = p->iz;
			aux->dr = p->dr;
		} else { //si es la raíz directamente
			aux->iz = p->iz;
		}

		delete p;
		return aux;
	}

    /** Dubujo del árbol interno */
    static void graph_rec(std::ostream & out, int indent, Nodo* raiz) {
        if (raiz != nullptr) {
            graph_rec(out, indent + TREE_INDENTATION, raiz->dr);
            out << std::setw(indent) << " " << raiz->elem << std::endl;
            graph_rec(out, indent + TREE_INDENTATION, raiz->iz);
        }
    }

	/** Puntero a la raíz de la estructura jerárquica de nodos. */
	Nodo *ra;

    /** Comparador: menor estricto. */
    Comparador cless;

    /** número de elementos en el conjunto */
    int numElems;

};

#endif // __TREESETC_H

