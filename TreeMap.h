/**
 * Implementación del TAD Dictionary utilizando árboles de búsqueda.
 * (c) Marco Antonio Gómez Martín, 2012
 * Adaptada por Ignacio Fábregas, 2022
*/

#ifndef __TREEMAP_H
#define __TREEMAP_H

#include <iostream>
#include "Exceptions.h"
#include "Stack.h" // Usado internamente por los iteradores

/**
 * Implementación dinámica del TAD Dictionary utilizando  árboles de búsqueda (no auto-balanceados).
 * Se añade un comparador entre claves: objeto función que acepta dos valores de tipo T y devuelve si el
 *          primero es menor que el segundo. Por defecto se toma el "<" en T si está definido
 * Las operaciones son:
 *    - TreeMapVacio: operación generadora que construye un árbol de búsqueda vacío.
 *    - Insert(clave, valor): generadora que añade una nueva pareja (clave, valor) al árbol.
 *          Si la clave ya estaba se sustituye el valor.
 *    - erase(clave): operación modificadora. Elimina la clave del árbol de búsqueda.
 *          Si la clave no está la operación no tiene efecto.
 *    - at(clave): operación observadora que devuelve el valor asociado a una clave.
 *          Es un error preguntar por una clave que no existe.
 *    - contains(clave): operación observadora. Sirve para averiguar si se ha introducido una
 *          clave en el árbol
 *    - empty(): operación observadora que indica si el árbol de búsqueda tiene alguna clave introducida.
 *    - size(): operación observadora que indica el tamaño del diccionario.
 */

template <typename Clave, typename Valor, typename Comparador = std::less<Clave>>
class TreeMap {
private:
	/**
	 * Clase nodo que almacena internamente la pareja (clave, valor)
	 * y los punteros al hijo izquierdo y al hijo derecho.
	 */
	class Nodo {
	public:
		Nodo() : iz(nullptr), dr(nullptr) {}
		Nodo(const Clave &clave, const Valor &valor) 
			: clave(clave), valor(valor), iz(nullptr), dr(nullptr) {}
		Nodo(Nodo *iz, const Clave &clave, const Valor &valor, Nodo *dr)
			: clave(clave), valor(valor), iz(iz), dr(dr) {}

		Clave clave;
		Valor valor;
		Nodo* iz;
		Nodo* dr;
	};

public:

	/** Constructor; operación EmptyTreeMap */
	TreeMap() : ra(nullptr) { numElems =0;}

	/** Destructor; elimina la estructura de nodos. */
	~TreeMap() {
		libera();
        ra = nullptr;
        numElems = 0;
	}

	/**
	 * Operación generadora que añade una nueva clave/valor a un árbol de búsqueda.
	 * Si la clave ya existía, sustituimos el valor viejo por el nuevo.
	 * El aumento en el número de elementos se maneja en insertaAux
	 * O(log n)
	 */
	void insert(const Clave &clave, const Valor &valor) {
        ra = insertaAux(clave, valor, ra);
	}

	/**
	 * Operación modificadora que elimina una clave del árbol.
	 * Si la clave no existía la operación no tiene efecto.
	 * El decremento en el número de elementos se maneja en insertaAux
	 * O(log n)
	 */
	void erase(const Clave &clave) {
        ra = borraAux(ra, clave);
	}

	/**
	 * Operación observadora que devuelve el valor asociado
	 a una clave dada. O(log n)
	 */
	const Valor &at(const Clave &clave) const {
		Nodo *p = buscaAux(ra, clave);
		if (p == nullptr)
			throw EClaveErronea();
		return p->valor;
	}

	/**
	 * Operación observadora que permite averiguar si una clave
	 * determinada está en el árbol de búsqueda. O(log n)
	 */
	bool contains(const Clave &clave) const {
		return (buscaAux(ra, clave) != nullptr) ? true : false;
	}

	/** Operación observadora que devuelve si un diccionario es vacío */
	bool empty() const {
		return ra == nullptr;
	}

    /** Operación observadora que devuelve el número de elementos del diccionario */
    int size() const{
        return numElems;
    }

	/**
	 * Sobrecarga del operador [] que permite acceder al valor asociado a una clave y modificarlo.
	 * Si el elemento buscado no estaba, se inserta uno con el valor por defecto del tipo Valor.
	 */
	Valor &operator[](const Clave &clave) {
        bool nuevo;
		Nodo* ret = busca_inserta(ra, clave, nuevo); //busca o inserta el elemento.
		if (nuevo)// se añade un nuevo valor
            ++numElems;
		return ret->valor; //ret es donde está el valor asociado a la clave.
	}

    /** Dibujo del diccionario: Uso únicamente para debugear durante clase */
    friend std::ostream& operator<<(std::ostream& o, const TreeMap<Clave, Valor, Comparador>& t){
        o<<"{";
        muestra(t.ra, o);
        o<<"}";
        return o;
    }


    // //
	// ITERADOR CONSTANTE Y FUNCIONES RELACIONADAS
	// //

	/**
	 * Clase interna que implementa un iterador que permite
	 * recorrer el diccionario pero no modificarlo
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

        /** O(1) */
		const Clave &key() const {
			if (act == nullptr) throw InvalidAccessException();
			return act->clave;
		}

        /** O(1) */
		const Valor &value() const {
			if (act == nullptr) throw InvalidAccessException();
			return act->valor;
		}

        /** O(1) */
		bool operator==(const ConstIterator &other) const {
			return act == other.act;
		}

        /** O(1) */
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
        friend class TreeMap;

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
    * Devuelve el iterador cend si no está
    */
	ConstIterator find(const Clave &c) const {
		Stack<Nodo*> ascendientes;
		Nodo *p = ra;
		while ((p != nullptr) && cless(p->clave, c) && cless(c, p->clave)) {
			if (p->clave > c) {
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
	 * Clase interna que implementa un iterador sobre el árbol de búsqueda
	 * que permite recorrer la lista e incluso alterar el valor de sus elementos.
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

        /** O(1) */
		const Clave &key() const {
			if (act == nullptr) throw InvalidAccessException();
			return act->clave;
		}

        /** O(1) */
		Valor &value() const {
			if (act == nullptr) throw InvalidAccessException();
			return act->valor;
		}

        /** O(1) */
		bool operator==(const Iterator &other) const {
			return act == other.act;
		}

        /** O(1) */
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
		friend class TreeMap;

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
    * Devuelve el iterador end si no está.
    * O(log n)
    */
	Iterator find(const Clave &c) {
		Stack<Nodo*> ascendientes;
		Nodo *p = ra;
		while ((p != nullptr) && cless(p->clave, c) && cless(c, p->clave)) {
			if (p->clave > c) {
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
	// MÉTODOS DE "FONTANERÍA" DE C++ QUE HACEN VERSÁTIL A LA CLASE
	// //

	/** Constructor copia */
	TreeMap(const TreeMap<Clave, Valor, Comparador> &other) : ra(nullptr) {
		copia(other);
	}

	/** Operador de asignación. O(n) */
	TreeMap<Clave, Valor, Comparador> &operator=(const TreeMap<Clave, Valor, Comparador> &other) {
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

	void copia(const TreeMap &other) {
        ra = copiaAux(other.ra);
        numElems = other.numElems;
        cless = other.cless;
	}

private:

	/**
	 * Elimina todos los nodos de una estructura  que comienza con el puntero n.
	 * O(n)
	 */
	static void libera(Nodo *n) {
		if (n != nullptr) {
			libera(n->iz);
			libera(n->dr);
			delete n;
		}
	}

	/**
	 * Copia la estructura jerárquica de nodos pasada como parámetro
	 */
	static Nodo *copiaAux(Nodo *n) {
		if (n == nullptr)
			return nullptr;
		return new Nodo(copiaAux(n->iz), n->clave, n->valor, copiaAux(n->dr));
	}

	/**
	 * Inserta una pareja (clave, valor) en la estructura que comienza en el puntero
	 * pasado como parámetro. El método devuelve un puntero a la raíz de la estructura
	 * modificada. En condiciones normales coincidirá con el parámetro recibido;
	 * sólo cambiará si la estructura era vacía.
	 */
	Nodo *insertaAux(const Clave &clave, const Valor &valor, Nodo *p) {
		if (p == nullptr) {//se inserta
            numElems++;
			return new Nodo(clave, valor);
		} else if (cless(clave, p->clave)) { // clave < p->clave
            p->iz = insertaAux(clave, valor, p->iz);
            return p;
        } else if(cless(p->clave, clave)) { // (clave > p->clave)
            p->dr = insertaAux(clave, valor, p->dr);
            return p;
        } else { // p->clave == clave
            p->valor = valor;
            return p;
        }
	}

    /**
     * Busca un elemento en la estructura de nodos cuya raíz se pasa como parámetro.
     * Devuelve un puntero al nodo si lo encuentra (o nullptr si no está).
     * O(log n)
     */
	Nodo *buscaAux(Nodo *p, const Clave &clave) const {
		if (p == nullptr)
			return nullptr;
        if (cless(clave, p->clave)) //clave < p->clave
            return buscaAux(p->iz, clave);
        else if (cless(p->clave, clave)) //clave > p->clave
            return buscaAux(p->dr, clave);
		else // clave == p->clave
			return p;
	}


    /**
	 * Busca o inserta una clave en el diccionario. Si la clave es nueva, se
     * inserta un valor por defecto. Si ya estaba no se inserta nada.
     * Devuelve el nodo con la clave (el nuevo o el que ya estaba).
     * Se usa en el operator[]. insertado indica si la clave era nueva
     * O(log n)
	 */
    Nodo *busca_inserta(Nodo* & raiz, const Clave &clave, bool& insertado) {
        if (raiz == nullptr) { //La clave es nueva
            insertado = true;
            raiz = new Nodo(clave, Valor()); //se cambia el árbol añadiendo el nuevo valor
            return raiz; //devolvemos el recientemente modificado
        } else if (cless(clave, raiz->clave)) { // clave < raiz->clave
            return busca_inserta(raiz->iz, clave, insertado);
        } else if(cless(raiz->clave, clave)) { // (clave > raiz->clave)
            return busca_inserta(raiz->dr, clave, insertado);
        } else { // raiz->clave == clave. La clave aparecía
            insertado = false;
            return raiz;  //devolvemos el nodo sin modificar
        }
    }

    /**
     * Elimina (si existe) un elemento de la estructura de nodos apuntada por p.
     * Si el elemento estaba en la propia raíz, éste cambiará. Si no cambia se devuelve p.
     * O(log n)
    */
	Nodo *borraAux(Nodo *p, const Clave &clave) {
		if (p == nullptr)
			return nullptr;
        else if (cless(clave, p->clave)) { // clave < p->clave
            p->iz = borraAux(p->iz, clave);
            return p;
        } else if(cless(p->clave, clave)) { // (clave > p->clave)
            p->dr = borraAux(p->dr, clave);
            return p;
        } else {  // p->clave == clave
            numElems--;
            return borraRaiz(p);
        }
    }

    /**
     * Borra la raíz de la estructura de nodos y devuelve un puntero a la nueva raíz.
     * Ésta raíz nueva garantiza que la estructura sigue siendo un árbol de búsqueda correcto.
     * O(log n) (si hay que buscar el mínimo)
     */
	static Nodo *borraRaiz(Nodo *p) {
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
		} else {
            //hay hijo derecho e izquierdo
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


    /**
     * Método para dibujar el diccionario.
     * Usado únicamente para debuguear.
     */
    static void muestra(Nodo *n, std::ostream &out) {
        if (n != nullptr) {
            if (n->iz != nullptr) {
                muestra(n->iz, out);
                out << ", ";
            }
            out << n->clave << " -> " << n->valor;
            if (n->dr != nullptr) {
                out << ", ";
                muestra(n->dr, out);
            }
        }
    }

	/** Puntero a la raíz de la estructura jerárquica de nodos. */
	Nodo *ra;

    /** Comparador: menor estricto. */
    Comparador cless;

    /** número de elementos en el conjunto */
    int numElems;
};

#endif // __TREEMAP_H
