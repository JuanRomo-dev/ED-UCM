#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "Arbin.h"
using namespace std;

void calcularFrontera(ArbinS<int>arbol, std::vector<int>& frontera) {
    if (!arbol.esVacio()) {
        if (arbol.hijoIz().esVacio() && arbol.hijoDr().esVacio()) {     // Si los hijos del nodo son vacios, entonces es una frontera.
            frontera.push_back(arbol.raiz());
        }
        else {
            calcularFrontera(arbol.hijoIz(), frontera);     // Avanzamos por el hijo izquierdo.
            calcularFrontera(arbol.hijoDr(), frontera);     // Avanzamos por el hijo derecho.
        }
    }
}

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
void resuelveCaso() {
    // leer los datos de la entrada
    ArbinS<int> arbol;

    arbol = ArbinS<int>::leerArbol(-1);

    vector<int> frontera;
    calcularFrontera(arbol, frontera);

    for (int i = 0; i < frontera.size(); i++) {
        cout << frontera[i] << " ";
    }
    cout << endl;
}

int main() {
    // Para la entrada por fichero.
    // Comentar para acepta el reto
#ifndef DOMJUDGE
    std::ifstream in("datos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf()); //save old buf and redirect std::cin to casos.txt
#endif 


    int numCasos;
    std::cin >> numCasos;
    for (int i = 0; i < numCasos; ++i)
        resuelveCaso();


    // Para restablecer entrada. Comentar para acepta el reto
#ifndef DOMJUDGE // para dejar todo como estaba al principio
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif

    return 0;
}