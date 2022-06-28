//-------------------------------------------------------
// Ejercicio 2: Nodos intermedios. Mayo 2022
// Escribe tu nombre y respuestas en las zonas indicadas
//-------------------------------------------------------
//@ <answer>
// Nombre :
// Usiario del Juez de Clase :
// Usuario del Juez de Exámenes :
//@ </answer>

#include <iostream>
#include <fstream>
#include "Arbin_Smart.h"

//Añade las librerías que necesites
//@ <answer>


//@ </answer>

using namespace std;

//-------------------------------------------------------
//Escribe la solución. Puedes definir las funciones auxiliares
// y tipos de datos que necesites.
//@ <answer>

bool esIntermedio(const int nodo, int sumaIzq, int sumaDer, int padre) {
    return (padre != 0) && (nodo == abs(sumaIzq - sumaDer) % padre);        // El nodo es intermedio si el padre no es la raíz y el valor absoluto de la resta de las sumas módulo valor del padre es igual al nodo.
}

int nodos_intermedio(const BinTree<int> arbol, int& suma, int padre) {       // Función que devuelve el número de nodos intermedios de un árbol.
    if (arbol.empty()) {      // Caso base: Si el árbol es vacío, la suma es 0 y hay 0 nodos intermedios en él.
        suma = 0;           
        return 0;
    }
    else {
        int nIntermediosIzq, nIntermediosDer, sumaIzq, sumaDer, nIntermedios;
        nIntermediosIzq = nodos_intermedio(arbol.left(), sumaIzq, arbol.root());        //  Recursión por la izquierda 
        nIntermediosDer = nodos_intermedio(arbol.right(), sumaDer, arbol.root());       //  Recursión por la derecha
        suma = sumaIzq + sumaDer + arbol.root();            // Calculamos la suma
        nIntermedios = nIntermediosIzq + nIntermediosDer;   // Calculamos el número de nodos intermedios.
        if (esIntermedio(arbol.root(), sumaIzq, sumaDer, padre)) {      // Si el nodo es intermedio
            nIntermedios++;     // Aumentamos el contador.
        }
        return nIntermedios;
    }
}

int nodos_intermedio(const BinTree<int>& arbol) {
    int padre = 0;      // Valor del nodo padre.
    int suma;           // Suma de los nodos hijos.
    return nodos_intermedio(arbol, suma, padre);
}

//@ </answer>
//
// ¡No modifiques nada debajo de esta línea!
// ----------------------------------------------------------------
void resuelveCaso() {
    BinTree<int> arbol = read_tree<int>(cin);
    cout << nodos_intermedio(arbol) << endl;
}


int main() {
#ifndef DOMJUDGE
    ifstream in("sample2.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif
    int num_casos;
    cin >> num_casos;
    for (int i = 0; i < num_casos; i++) {
        resuelveCaso();
    }
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif

    return 0;

} // main
