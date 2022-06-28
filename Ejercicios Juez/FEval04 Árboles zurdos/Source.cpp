#include <iostream>               
#include <fstream>
#include "Arbin.h"
using namespace std;

/*
 * Implementa la función pedida. Puedes usar funciones auxiliares
 */


bool esZurdo(const Arbin<char>& arbol, int& numHijos) {
    if (arbol.esVacio()) {      // Si el árbol es vacío entonces es zurdo.
        numHijos = 0;
        return true;
    }
    else if(arbol.hijoIz().esVacio() && arbol.hijoDr().esVacio()) {     // Si es una hoja entonces es zurdo y el número de hijos es 1.
        numHijos = 1;
        return true;
    }
    else {      
        int nIzq, nDer;
        bool izq, der;
        izq = esZurdo(arbol.hijoIz(), nIzq);        // Comprobamos si el hijo izquierdo es zurdo
        if (izq) {          // Para evitar llamadas innecesarias.
            der = esZurdo(arbol.hijoDr(), nDer);        // Comprobamos si el hijo derecho es zurdo.
            numHijos = nIzq + nDer + 1;         // Actualizamos numHijos.
        }
        return izq && der && nIzq > nDer;       // Comprobamos si los hijos son zurdos y si mas de la mitad de los descendientes están en el hijo izquierdo.
    }
}


 /*
  * No necesitas editar nada a partir de aquí hacia abajo salvo la LLLAMADA a la función
  * o el nombre del fichero de datos
  */
bool esZurdo(const Arbin<char>& arbol) {
    int numHijos = 0;
    return esZurdo(arbol, numHijos);
}


void resuelveCaso() {
    Arbin<char> arbol = Arbin<char>::leerArbolInorden('.');
    if (esZurdo(arbol))
        cout << "SI" << endl;
    else
        cout << "NO" << endl;
}


int main() {
#ifndef DOMJUDGE
    std::ifstream in("datos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif
    int casos;
    cin >> casos;
    while (casos--) {
        resuelveCaso();
    }
#ifndef DOMJUDGE 
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif
    return 0;
}
