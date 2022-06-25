/**
 * Implementación del TAD Pila utilizando una lista enlazada de nodos.
 * (c) Marco Antonio Gómez Martín, 2012
 * Modificada por Ignacio Fábregas, 2022
*/
#ifndef __PILA_LISTA_ENLAZADA_H
#define __PILA_LISTA_ENLAZADA_H

#include "Exceptions.h"
#include <iostream>
#include <iomanip>

/**
 * Implementación del TAD Pila utilizando una lista enlazada simple de nodos.
 * Las operaciones son:
 *   - EmptyStack: -> Stack. Generadora implementada en el constructor sin parámetros.
 *   - push: Stack, Elem -> Stack. Generadora
 *   - pop: Stack - -> Stack. Modificadora parcial.
 *   - top: Stack - -> Elem. Observadora parcial.
 *   - empty: Stack -> Bool. Observadora.
 *   - size: Stack -> Entero. Observadora.
 */
template <class T>
class LinkedListStack {
public:

	/** Constructor; operación PilaVacia. O(1) */
	LinkedListStack() : cima(nullptr), numElems(0) {
	}

	/** Destructor; elimina la lista enlazada. O(n) */
	~LinkedListStack() {
		libera();
        cima = nullptr; //dejamos la pila como vacía
	}

	/** Apila un elemento. Operación generadora. O(1) */
	void push(const T &elem) {
        cima = new Nodo(elem, cima);
		numElems++;
	}
	
	/**
	 * Desapila un elemento. Operación modificadora parcial, que falla si la pila está vacía.
	 * O(1)
	 */
	const T& pop() {
		if (empty())
			throw EmptyStackException("Cannot pop. The stack is empty");
		Nodo *aBorrar = cima;
		T dato = aBorrar->elem;
        cima = cima->sig;
		delete aBorrar;
		--numElems;
		return dato;
	}

	/**
	 * Devuelve el elemento en la cima de la pila. Operación
	 * observadora parcial, que falla si la pila está vacía.
	 * O(1)
	 */
	const T &top() const {
		if (empty())
			throw EmptyStackException("Cannot get top. The stack is empty");
		return cima->elem;
	}

	/** Devuelve true si, y solo si, la pila no tiene ningún elemento. O(1) */
	bool empty() const {
		return cima == nullptr;
	}

	/** Devuelve el número de elementos que hay en la pila. O(1) */
	int size() const {
		return numElems;
	}

	// //
	// MÉTODOS DE "FONTANERÍA" DE C++ QUE HACEN VERSÁTIL A LA CLASE
	// //

	/** Constructor por copia. O(n) */
	LinkedListStack(const LinkedListStack<T> &other) : cima(nullptr) {
		copia(other);
	}

	/** Operador de asignación. O(n) */
	LinkedListStack<T> &operator=(const LinkedListStack<T> &other) {
		if (this != &other) {
			libera();
			copia(other);
		}
		return *this;
	}

	/** Operadores de comparación. O(n) */
	bool operator==(const LinkedListStack<T> &rhs) const {
		if (numElems != rhs.numElems)
			return false;
		bool iguales = true;
		Nodo *cima1 = cima;
		Nodo *cima2 = rhs.cima;
		while ((cima1 != nullptr) && (cima2 != nullptr) && iguales) {
			if (cima1->elem != cima2->elem)
				iguales = false;
			else {
				cima1 = cima1->sig;
				cima2 = cima2->sig;
			}
		}
		return iguales;
	}

    /** O(n) */
	bool operator!=(const LinkedListStack<T> &rhs) const {
		return !(*this == rhs);
	}

	/** Escribe la pila en el flujo. Se usa desde operator<< */
	void write(std::ostream& sOut) {
		Nodo* aux = cima;
		while (aux != nullptr){
			sOut << "| " << std::setw(2) << std::left << aux->elem << "|" << std::endl;
			aux = aux->sig;
		}
		sOut << "|---|" << std::endl;
	}

protected:

	void libera() {
		libera(cima);
	}

	void copia(const LinkedListStack &other) {
		if (other.empty()) {
            cima = nullptr;
            numElems = 0;
		} else {
			Nodo *act = other.cima;
			Nodo *ant;
            cima = new Nodo(act->elem);
			ant = cima;
			while (act->sig != nullptr) {
				act = act->sig;
				ant->sig = new Nodo(act->elem);
				ant = ant->sig;
			}
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
	static void libera(Nodo *n) {
		while (n != nullptr) {
			Nodo *aux = n;
			n = n->sig;
			delete aux;
		}
	}

	/** Puntero al primer elemento */
	Nodo *cima;

	/** Número de elementos */
	int numElems;
};

/** Operador de escritura */
template<class T>
std::ostream& operator<<(std::ostream& sOut, LinkedListStack<T>& s) {
	s.write(sOut);
	return sOut;
}

#endif // __PILA_LISTA_ENLAZADA_H
