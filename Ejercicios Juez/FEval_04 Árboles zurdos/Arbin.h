/**
 * Implementación dinámica del TAD Arbol Binario con conteo manual de referencias
 * (c) Marco Antonio Gómez Martín, 2012
 * Modificado por Ignacio Fábregas, 2022
*/
#ifndef __ARBIN_H
#define __ARBIN_H

#include "Exceptions.h"
#include "List.h" // Tipo devuelto por los recorridos
#include "Queue.h" // Tipo auxiliar para implementar el recorrido por niveles
#include <iomanip>   // setw
#include <iostream>  // endl 

/**
 * Implementación dinámica del TAD Arbin utilizando
 * nodos con un puntero al hijo izquierdo y otro al
 * hijo derecho. La implementación permite compartición
 * de estructura, manteniendola bajo control mediante
 * conteo de referencias. La implementación, sin embargo,
 * es bastante artesanal, pues para no complicar el código
 * excesivamente no se ha hecho uso de punteros inteligentes
 * que incrementen y decrementen automáticamente esas
 * referencias.
 *
 * Las operaciones son:
 * - ArbolVacio: -> Arbin. Generadora implementada en el constructor sin parámetros.
 * - Cons: Arbin, Elem, Arbin -> Arbin. Generadora implementada en un constructor con tres parámetros.
 * - hijoIz, hijoDr: Arbin - -> Arbin. Observadoras que devuelven el hijo izquiero o derecho de un árbol.
 * - esVacio: Arbin -> Bool. Observadora que devuelve si un árbol binario es vacío.
 */

template <typename T>
class Arbin {
public:

	/** Constructor; operacion ArbolVacio */
	Arbin() : ra(nullptr) {
	}

	/** Constructor; operacion Cons */
	Arbin(const Arbin& iz, const T& elem, const Arbin& dr) :
		ra(new Nodo(iz.ra, elem, dr.ra)) {
		ra->addRef();
	}

	Arbin(const T& elem) :
		ra(new Nodo(nullptr, elem, nullptr)) {
		ra->addRef();
	}

	/** Destructor; elimina la estructura jerárquica de nodos. */
	~Arbin() {
		libera();
		ra = nullptr;
	}

	/**
	 * Devuelve el elemento almacenado en la raiz
	 * error raiz(ArbolVacio)
	 * @return Elemento en la raíz.
	 */
	const T& raiz() const {
		if (esVacio())
			throw EArbolVacio();
		return ra->elem;
	}

	/**
	 * Devuelve el subárbol izquierdo compartiendo memoria
	 * Es una operación parcial (falla con el árbol vacío).
	*/
	Arbin hijoIz() const {
		if (esVacio())
			throw EArbolVacio();
		return Arbin(ra->iz);
	}

	/**
	 * Devuelve el subárbol derecho compartiendo memoria
	 * Es una operación parcial (falla con el árbol vacío).
	*/
	Arbin hijoDr() const {
		if (esVacio())
			throw EArbolVacio();
		return Arbin(ra->dr);
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
		if (!esVacio()) {
			Queue<Nodo*> porProcesar;
			porProcesar.push_back(ra);

			while (!porProcesar.empty()) {
				Nodo* visita = porProcesar.front();
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
	// MÉTODOS DE "FONTANERÍA" DE C++ QUE HACEN VERSÁTIL
	// A LA CLASE
	// //

	/** Constructor copia */
	Arbin(const Arbin<T>& other) : ra(NULL) {
		copia(other);
	}

	/** Operador de asignación */
	Arbin<T>& operator=(const Arbin<T>& other) {
		if (this != &other) {
			libera();
			copia(other);
		}
		return *this;
	}

	/** Operadores de comparación. */
	bool operator==(const Arbin<T>& rhs) const {
		return comparaAux(ra, rhs.ra);
	}

	bool operator!=(const Arbin<T>& rhs) const {
		return !(*this == rhs);
	}

	/** Escritura de un árbol
	 *  Adaptado de "ADTs, DataStructures, and Problem Solving with C++",
	 *  Larry Nyhoff, Person, 2015
	 */
	friend std::ostream& operator<<(std::ostream& o, const Arbin<T>& t) {
		o << "==== Tree =====" << std::endl;
		graph_rec(o, 0, t.ra);
		o << "===============" << std::endl;
		return o;
	}

	/** Lectura de árboles en preorden */
	static Arbin<T> leerArbol(const T& repVacio) {
		T elem;
		std::cin >> elem;
		if (elem == repVacio)
			return Arbin<T>();
		else {
			Arbin<T> hi = leerArbol(repVacio);
			Arbin<T> hd = leerArbol(repVacio);
			return Arbin<T>(hi, elem, hd);
		}
	}

	/** Lectura de árboles en inorden */
	static Arbin<T> leerArbolInorden(const T& repVacio) {
		T c;
		std::cin >> c;
		if (c == repVacio)
			return Arbin<T>();
		else {
			assert(c == '(');
			Arbin<T> left = leerArbolInorden(repVacio);
			T elem;
			std::cin >> elem;
			Arbin<T> right = leerArbolInorden(repVacio);
			std::cin >> c;
			assert(c == ')');
			Arbin<T> result(left, elem, right);
			return result;
		}
	}


protected:
	/** para la escritura del árbol */
	static const int TREE_INDENTATION = 4;

	/**
	 * Clase nodo que almacena internamente el elemento (de tipo T),
	 * y los punteros al hijo izquierdo y al hijo derecho, así
	 * como el número de referencias que hay.
	 */
	class Nodo {
	public:
		Nodo() : iz(nullptr), dr(nullptr), numRefs(0) {}
		Nodo(Nodo* iz, const T& elem, Nodo* dr) :
			elem(elem), iz(iz), dr(dr), numRefs(0) {
			if (iz != nullptr)
				iz->addRef();
			if (dr != nullptr)
				dr->addRef();
		}

		void addRef() { assert(numRefs >= 0); numRefs++; }
		void remRef() { assert(numRefs > 0); numRefs--; }

		T elem;
		Nodo* iz;
		Nodo* dr;

		int numRefs;
	};

	/**
	 * Constructor protegido que crea un árbol a partir de una estructura jerárquica existente.
	 * Esa estructura jerárquica SE COMPARTE, por lo que se añade la referencia.
	 * Se utiliza en hijoIz e hijoDr.
	 */
	Arbin(Nodo* raiz) : ra(raiz) {
		if (ra != nullptr)
			ra->addRef();
	}

	/** Operación auxiliar para liberar la memoria */
	void libera() {
		libera(ra);
	}

	/** copia */
	void copia(const Arbin& other) {
		assert(this != &other);
		ra = other.ra;
		if (ra != nullptr)
			ra->addRef();
	}

	// //
	// MÉTODOS AUXILIARES PARA LOS RECORRIDOS
	// //

	static void preordenAcu(Nodo* ra, List<T>& acu) {
		if (ra != nullptr) {
			acu.push_back(ra->elem);
			preordenAcu(ra->iz, acu);
			preordenAcu(ra->dr, acu);
		}
	}

	static void inordenAcu(Nodo* ra, List<T>& acu) {
		if (ra != nullptr) {
			inordenAcu(ra->iz, acu);
			acu.push_back(ra->elem);
			inordenAcu(ra->dr, acu);
		}
	}

	static void postordenAcu(Nodo* ra, List<T>& acu) {
		if (ra != nullptr) {
			postordenAcu(ra->iz, acu);
			postordenAcu(ra->dr, acu);
			acu.push_back(ra->elem);
		}
	}

	static void graph_rec(std::ostream& out, int indent, Nodo* raiz) {
		if (raiz != nullptr) {
			graph_rec(out, indent + TREE_INDENTATION, raiz->dr);
			out << std::setw(indent) << " " << raiz->elem << std::endl;
			graph_rec(out, indent + TREE_INDENTATION, raiz->iz);
		}
	}

	// //
	// MÉTODOS AUXILIARES (RECURSIVOS) DE OTRAS OPERACIONES
	// OBSERVADORAS
	// //

	static unsigned int numNodosAux(Nodo* ra) {
		if (ra == nullptr)
			return 0;
		return 1 + numNodosAux(ra->iz) + numNodosAux(ra->dr);
	}

	static unsigned int tallaAux(Nodo* ra) {
		if (ra == nullptr)
			return 0;
		int tallaiz = tallaAux(ra->iz);
		int talladr = tallaAux(ra->dr);
		if (tallaiz > talladr)
			return 1 + tallaiz;
		else
			return 1 + talladr;
	}

	static unsigned int numHojasAux(Nodo* ra) {
		if (ra == nullptr)
			return 0;

		if ((ra->iz == NULL) && (ra->dr == NULL))
			return 1;

		return numHojasAux(ra->iz) + numHojasAux(ra->dr);
	}

private:

	/**
	 * Elimina todos los nodos de una estructura arbórea que comienza con el puntero ra.
	 * Se admite que el nodo sea nullptr
	 */
	static void libera(Nodo* ra) {
		if (ra != nullptr) {
			ra->remRef();
			if (ra->numRefs == 0) {
				libera(ra->iz);
				libera(ra->dr);
				delete ra;
			}
		}
	}

	/**
	 * Compara dos estructuras jerárquicas de nodos, dadas sus raices.
	 */
	static bool comparaAux(Nodo* r1, Nodo* r2) {
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
	 * Puntero a la raíz de la estructura jerárquica de nodos.
	 */
	Nodo* ra;
};

#endif // __ARBIN_H
