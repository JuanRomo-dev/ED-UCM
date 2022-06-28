/**
 * Implementación dinámica del TAD Arbol Binario con punteros inteligentes
 * Por Ignacio Fábregas, 2022
 * Basado en clases originales de Marco Antonio Gómez Martín y Enrique Martín Martín
*/
#ifndef __ARBIN_SMART_H
#define __ARBIN_SMART_H

#include "Exceptions.h"
#include "List.h" // Tipo devuelto por los recorridos
#include "Queue.h" // Tipo auxiliar para implementar el recorrido por niveles
#include <iomanip>   // setw
#include <iostream>  // endl
#include <memory> //shared_ptr

/**
 * Implementación dinámica del TAD Arbin utilizando
 * nodos con un puntero al hijo izquierdo y otro al
 * hijo derecho. La implementación permite compartición
 * de estructura, manteniendola bajo control mediante
 * conteo de referencias. 
 * 
 * Es como la clase Arbin pero usando punteros inteligentes, por lo que NO necesitamos un destructor.
 * Tampoco necesitamos un constructor por copia ni modificar la asignación.
 *
 * Las operaciones son:
 * - ArbolVacio: -> Arbin. Generadora implementada en el constructor sin parámetros.
 * - Cons: Arbin, Elem, Arbin -> Arbin. Generadora implementada en un constructor con tres parámetros.
 * - hijoIz, hijoDr: Arbin - -> Arbin. Observadoras que devuelven el hijo izquiero o derecho de un árbol.
 * - esVacio: Arbin -> Bool. Observadora que devuelve si un árbol binario es vacío.
 */

template <typename T>
class ArbinS {
public:

	/** Constructor; operacion ArbolVacio */
	ArbinS() : ra(nullptr) {
	}

	/** Constructor; operacion Cons */
	ArbinS(const ArbinS &iz, const T &elem, const ArbinS &dr) :
            ra(std::make_shared<Nodo>(Nodo(iz.ra, elem, dr.ra))) {}

	ArbinS(const T &elem) :
            ra(std::make_shared<Nodo>(Nodo(nullptr, elem, nullptr))) {}

	/**
	 * Devuelve el elemento almacenado en la raiz
	 * error raiz(ArbolVacio)
	 * @return Elemento en la raíz.
	 */
	const T &raiz() const {
		if (esVacio())
			throw EArbolVacio();
		return ra->elem;
	}

	/**
	 * Devuelve el subárbol izquierdo compartiendo memoria
	 * Es una operación parcial (falla con el árbol vacío).
	*/
	ArbinS hijoIz() const {
		if (esVacio())
			throw EArbolVacio();
		return ArbinS(ra->iz);
	}

    /**
     * Devuelve el subárbol derecho compartiendo memoria
     * Es una operación parcial (falla con el árbol vacío).
    */
	ArbinS hijoDr() const {
		if (esVacio())
			throw EArbolVacio();
		return ArbinS(ra->dr);
	}

	/** Operación observadora; devuelve si el árbol es vacío. */
	bool esVacio() const {
		return ra == nullptr;
	}

	// //
	// RECORRIDOS SOBRE EL ÁRBOL
	// //

    /** Devolvemos un puntero a lista */
	List<T>* preorden() const {
		List<T>* ret = new List<T>();
		preordenAcu(ra, *ret);
		return ret;
	}

	List<T>* inorden() const {
		List<T>* ret = new List<T>();
		inordenAcu(ra, *ret);
		return ret;
	}

	List<T>* postorden() const {
		List<T>* ret = new List<T>();
		postordenAcu(ra, *ret);
		return ret;
	}

	List<T>* niveles() const {
		List<T>* ret = new List<T>();
		if (!esVacio()){
			Queue<Link> porProcesar;
			porProcesar.push_back(ra);

			while (!porProcesar.empty()) {
				Link visita = porProcesar.front();
				porProcesar.pop_front();
				ret->push_back(visita->elem);
				if (visita->iz != NULL)
					porProcesar.push_back(visita->iz);
				if (visita->dr != NULL)
					porProcesar.push_back(visita->dr);
			}
		}
		return ret;
	}

	// //
	// OTRAS OPERACIONES OBSERVADORAS
	// //

	/** Devuelve el número de nodos de un árbol. */
	unsigned int numNodos() const {
		return numNodosAux(ra);
	}

	/** Devuelve la talla del árbol. */
	unsigned int talla() const {
		return tallaAux(ra);
	}

	/** Devuelve el número de hojas de un árbol. */
	unsigned int numHojas() const {
		return numHojasAux(ra);
	}

	// //
	// MÉTODOS DE "FONTANERÍA" DE C++ QUE HACEN VERSÁTIL A LA CLASE
	// //

	/** Operadores de comparación. */
	bool operator==(const ArbinS<T> &rhs) const {
		return comparaAux(ra, rhs.ra);
	}

	bool operator!=(const ArbinS<T> &rhs) const {
		return !(*this == rhs);
	}

    /** Escritura de un árbol
     *  Adaptado de "ADTs, DataStructures, and Problem Solving with C++", 
     *  Larry Nyhoff, Person, 2015
     */
    friend std::ostream& operator<<(std::ostream& o, const ArbinS<T>& t){
        o  << "==== Tree =====" << std::endl;
        graph_rec(o, 0, t.ra);
        o << "===============" << std::endl;
        return o;
    }

    /** Lectura de árboles en preorden */
	static ArbinS<T> leerArbol(const T& repVacio) {
		T elem;
		std::cin >> elem;
		if (elem == repVacio)
			return ArbinS<T>();
		else {
			ArbinS<T> hi = leerArbol(repVacio);
			ArbinS<T> hd = leerArbol(repVacio);
			return ArbinS<T>(hi, elem, hd);
		}
	}

	 /** Lectura de árboles en inorden */
	static ArbinS<T> leerArbolInorden(const T& repVacio) {
		T c;
		std::cin >> c;
		if (c == repVacio) 
			return ArbinS<T>(); 
		else {
			assert (c == '(');
			ArbinS<T> left = leerArbolInorden(repVacio);
			T elem;
    		std::cin >> elem;
    		ArbinS<T> right = leerArbolInorden(repVacio);
    		std::cin >> c;
    		assert (c == ')');
    		ArbinS<T> result(left, elem, right);
    		return result;
    	}
	}


protected:
    /** para la escritura del árbol */
    static const int TREE_INDENTATION = 4;

	/**
	 * Clase nodo que almacena internamente el elemento (de tipo T),
	 * y punteros inteligentes al hijo izquierdo y al hijo derecho
	 */
    class Nodo; // Declaración adelantada para poder definir Link
    using Link = std::shared_ptr<Nodo>; // Alias de tipo

	class Nodo {
	public:
		Nodo() : iz(nullptr), dr(nullptr) {}
		Nodo(Link iz, const T &elem, Link dr) : elem(elem), iz(iz), dr(dr) {}

		T elem;
		Link iz;
		Link dr;
	};

	/**
	 * Constructor protegido que crea un árbol a partir de una estructura jerárquica existente.
	 */
	ArbinS(Link raiz) : ra(raiz) {
	}

	// //
	// MÉTODOS AUXILIARES PARA LOS RECORRIDOS
	// //
	
	static void preordenAcu(Link ra, List<T> &acu) {
		if (ra != nullptr){
            acu.push_back(ra->elem);
            preordenAcu(ra->iz, acu);
            preordenAcu(ra->dr, acu);
        }
	}

	static void inordenAcu(Link ra, List<T> &acu) {
        if (ra != nullptr) {
            inordenAcu(ra->iz, acu);
            acu.push_back(ra->elem);
            inordenAcu(ra->dr, acu);
        }
	}

	static void postordenAcu(Link ra, List<T> &acu) {
        if (ra != nullptr) {
            postordenAcu(ra->iz, acu);
            postordenAcu(ra->dr, acu);
            acu.push_back(ra->elem);
        }
	}

    static void graph_rec(std::ostream & out, int indent, Link raiz){
        if (raiz != nullptr) {
            graph_rec(out, indent + TREE_INDENTATION, raiz->dr);
            out << std::setw(indent) << " " << raiz->elem << std::endl;
            graph_rec(out, indent + TREE_INDENTATION, raiz->iz);
        }
    }

	// //
	// MÉTODOS AUXILIARES (RECURSIVOS) DE OTRAS OPERACIONES OBSERVADORAS
	// //

	static unsigned int numNodosAux(Link ra) {
		if (ra == nullptr)
			return 0;
		return 1 + numNodosAux(ra->iz) + numNodosAux(ra->dr);
	}

	static unsigned int tallaAux(Link ra) {
		if (ra == nullptr)
			return 0;
		int tallaiz = tallaAux(ra->iz);
		int talladr = tallaAux(ra->dr);
		if (tallaiz > talladr)
			return 1 + tallaiz;
		else
			return 1 + talladr;
	}

	static unsigned int numHojasAux(Link ra) {
		if (ra == nullptr)
			return 0;

		if ((ra->iz == NULL) && (ra->dr == NULL))
			return 1;

		return numHojasAux(ra->iz) + numHojasAux(ra->dr);
	}

private:

	/**
	 * Compara dos estructuras jerárquicas de nodos, dadas sus raices.
	 */
	static bool comparaAux(Link r1, Link r2) {
		if (r1 == r2)
			return true;
		else if ((r1 == nullptr) || (r2 == nullptr))
			// En el if anterior nos aseguramos de que r1 != r2. Si uno es NULL, el
			// otro entonces no lo será, luego son distintos.
			return false;
		else {
			return (r1->elem == r2->elem) &&
                   comparaAux(r1->iz, r2->iz) &&
                   comparaAux(r1->dr, r2->dr);
		}
	}

protected:
	/**
	 * Puntero inteligente a la raíz de la estructura jerárquica de nodos.
	 */
	Link ra;
};

#endif // __ARBIN_SMART_H
