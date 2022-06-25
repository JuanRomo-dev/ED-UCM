/**
 * Implementación del TAD Cola utilizando una lista enlazada simple de nodos.
 * (c) Marco Antonio Gómez Martín, 2012
 * Modificado por Ignacio Fábregas, 2022
*/
#ifndef __LINKED_LIST_QUEUE_H
#define __LINKED_LIST_QUEUE_H

#include "Exceptions.h"
#include <iostream>

/**
 * Implementación del TAD Cola utilizando una lista simple enlazada.
 * Las operaciones son:
 *  - EmptyQueue: -> Queue. Generadora implementada en el constructor sin parámetros.
 *  - push_back: Queue, Elem -> Queue. Generadora
 *  - pop_front: Queue - -> Queue. Modificadora parcial.
 *  - front: Queue - -> Elem. Observadora parcial.
 *  - empty: Queue -> Bool. Observadora.
 *  - size: Queue -> Entero. Observadora.
 */
template <class T>
class Queue {
public:

	/** Constructor; operación EmptyQueue. O(1) */
	Queue() : prim(nullptr), ult(nullptr), numElems(0) {
	}

	/** Destructor; elimina la lista enlazada. O(n) */
	~Queue() {
		libera();
        prim = ult = nullptr;
	}

	/**
	 * Añade un elemento en la parte trasera de la cola. O(1)
	 */
	void push_back(const T &elem) {
		Nodo *nuevo = new Nodo(elem, nullptr);
		if (ult != nullptr)
            ult->sig = nuevo;
        ult = nuevo;
		// Si la cola estaba vacía, el primer elemento es el que acabamos de añadir
		if (prim == nullptr)
            prim = nuevo;
		numElems++;
	}

	/**
	 * Elimina el primer elemento de la cola.
	 * Operación modificadora parcial, que falla si la cola está vacía.
	 * O(1)
	 */
	void pop_front() {
		if (empty())
			throw EmptyQueueException("Cannot pop: Queue is empty");
		Nodo *aBorrar = prim;
        prim = prim->sig;
		delete aBorrar;
		--numElems;
		if (prim == nullptr) //si la cola queda vacía, no hay último
            ult = nullptr;
	}

	/**
	 * Devuelve el primer elemento de la cola.
	 * Operación observadora parcial, que falla si la cola está vacía.
	 * O(1)
	 */
	const T &front() const {
		if (empty())
			throw EmptyQueueException("Cannot get front: Queue is empty");
		return prim->elem;
	}

	/** Devuelve true si la cola no tiene ningún elemento. O(1) */
	bool empty() const {
		return prim == nullptr;
	}

	/** Devuelve el número de elementos que hay en la cola. O(1) */
	int size() const {
		return numElems;
	}

	// //
	// MÉTODOS DE "FONTANERÍA" DE C++ QUE HACEN VERSÁTIL A LA CLASE
	// //

	/** Constructor copia. O(n) */
	Queue(const Queue<T> &other) : prim(nullptr), ult(nullptr) {
		copia(other);
	}

	/** Operador de asignación. O(n) */
	Queue<T> &operator=(const Queue<T> &other) {
		if (this != &other) {
			libera();
			copia(other);
		}
		return *this;
	}

	/** Operador de comparación. O(n) */
	bool operator==(const Queue<T> &rhs) const {
		if (numElems != rhs.numElems)
			return false;
		bool iguales = true;
		Nodo *p1 = prim;
		Nodo *p2 = rhs.prim;
		while ((p1 != nullptr) && (p2 != nullptr) && iguales) {
			if (p1->elem != p2->elem)
				iguales = false;
			else {
				p1 = p1->sig;
				p2 = p2->sig;
			}
		}
		return iguales;
	}

	bool operator!=(const Queue<T> &rhs) const {
		return !(*this == rhs);
	}

	/** Escribe la cola en el flujo. Se usa desde operator<< */
	void write(std::ostream& sOut) {
		Nodo* aux = prim;
		while (aux != nullptr){
			sOut << aux->elem;
			aux = aux->sig;
			if (aux != nullptr) sOut << " ";
		}
	}

protected:

	void libera() {
		libera(prim);
	}

	void copia(const Queue &other) {
		if (other.empty()) {
            prim = ult = nullptr;
			numElems = 0;
		} else {
			Nodo *act = other.prim;
			Nodo *ant;
            prim = new Nodo(act->elem);
			ant = prim;
			while (act->sig != nullptr) {
				act = act->sig;
				ant->sig = new Nodo(act->elem);
				ant = ant->sig;
			}
            ult = ant;
			numElems = other.numElems;
		}
	}

private:

    /**
     * Clase nodo que almacena internamente el elemento (de tipo T),
     * y un puntero al nodo siguiente, que podría ser nullptr so
     * el nodo es el último de la lista enlazada.
     */
	class Nodo {
	public:
		Nodo() : sig(nullptr) {}
		Nodo(const T &elem) : elem(elem), sig(nullptr) {}
		Nodo(const T &elem, Nodo *sig) : elem(elem), sig(sig) {}

		T elem;
		Nodo *sig;
	};

    /**
     * Elimina todos los nodos de la lista enlazada cuyo primer nodo se pasa como parámetro.
     * Se admite que el nodo sea nullptr (no habría nada que liberar).
     */
	static void libera(Nodo *prim) {
		while (prim != nullptr) {
			Nodo *aux = prim;
			prim = prim->sig;
			delete aux;
		}
	}

	/** Puntero al primer elemento. */
	Nodo *prim;

	/** Puntero al último elemento. */
	Nodo *ult;

	/** Número de elementos */
	int numElems;
};

/** Operador de escritura */
template<class T>
std::ostream& operator<<(std::ostream& sOut, Queue<T>& q) {
	q.write(sOut);
	return sOut;
}

#endif // __LINKED_LIST_QUEUE_H
