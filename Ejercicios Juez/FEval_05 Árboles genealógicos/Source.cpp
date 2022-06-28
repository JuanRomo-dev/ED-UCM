/**
 * Plantilla Grupo F
 *
 * Nombre y apellidos:
 *
 * Usuario del juez
 */

#include <iostream>
#include <fstream>
#include "Arbin_Smart.h"

using namespace std;

//Escrbe la función que resuelve el problema

bool sonFamilia(const ArbinS<int>& padre, const ArbinS<int>& hijoI, const ArbinS<int>& hijoD) {
    return (!hijoI.esVacio() && padre.raiz() - hijoI.raiz() >= 18) && (hijoD.esVacio() || hijoI.raiz() - hijoD.raiz() >= 2);
}

bool esGenealogico(const ArbinS<int> &arbol, int& altura) {
    if (arbol.esVacio()) {      // Caso base, si el árbol es vacío, entonces es genealógico con altura = 0.
        altura = 0;
        return true;
    }
    else if (arbol.hijoIz().esVacio() && arbol.hijoDr().esVacio()) {
        altura = 1;
        return true;
    }
    else {
        int altura1, altura2;
        bool izq, der;

        izq = esGenealogico(arbol.hijoIz(), altura1);
        if (izq) {
            der = esGenealogico(arbol.hijoDr(), altura2);
        }
        altura = max(altura1, altura2) + 1;
        return izq && der && sonFamilia(arbol, arbol.hijoIz(), arbol.hijoDr());
    }
}


//No puedes cambiar nada salvo la llamada a la función y el nombre del fichero

void resuelveCaso() {
    ArbinS<int> arbol = ArbinS<int>::leerArbol(-1);
    int altura;
    bool sol = esGenealogico(arbol, altura);
    if (sol)
        cout << "SI " << altura << endl;
    else
        cout << "NO" << endl;
}

int main() {
#ifndef DOMJUDGE
    std::ifstream in("datos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif
    int numCasos;
    std::cin >> numCasos;
    for (int i = 0; i < numCasos; ++i)
        resuelveCaso();
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif
        return 0;
}
