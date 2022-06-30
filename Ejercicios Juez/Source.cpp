#include <iostream>
#include <iomanip>
#include <fstream>
#include "List.h"
using namespace std;

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
void resuelveCaso() {
    // leer los datos de la entrada
    int aux, n;
    List<int> lista;

    cin >> aux;
    while (aux != -1) {
        lista.push_back(aux);
        cin >> aux;
    }

    cin >> n;

    // escribir sol
    for (auto it = lista.begin(); it != lista.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    lista.elimianrElem(n);

    for (auto it = lista.begin(); it != lista.end(); ++it) {
        cout << *it << " ";
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