#include <iostream>
#include <iomanip>
#include <fstream>
#include "List.h"
using namespace std;

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
void resuelveCaso() {
    List<int> lista1;
    List<int> lista2;

    int aux;
    cin >> aux;
    while (aux != 0) {
        lista1.push_back(aux);
        cin >> aux;
    }
    cin >> aux;
    while (aux != 0) {
        lista2.push_back(aux);
        cin >> aux;
    }

    lista1.interseccion(lista2);

    // escribir sol
    auto it = lista1.begin();
    while (it != lista1.end()) {
        cout << *it << " ";
        it++;
    }
    cout << endl;

}

int main() {
    // Para la entrada por fichero.
    // Comentar para acepta el reto
#ifndef DOMJUDGE
    std::ifstream in("sample1.txt");
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
