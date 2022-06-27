#include <iostream>
#include <iomanip>
#include <fstream>
#include "list_linked_single_plus.h"
using namespace std;

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
void resuelveCaso() {
    // leer los datos de la entrada
    ListLinkedSingle<int> lista1;
    ListLinkedSingle<int> lista2;

    int aux;
    cin >> aux;
    while(aux != 0) {
        lista1.push_back(aux);
        cin >> aux;
    }

    cin >> aux;
    while (aux != 0) {
        lista2.push_back(aux);
        cin >> aux;
    }

        
    // escribir sol
    if (lista2.empty()) {
        lista1.display();
    }
    else {
        lista1.mezclar(lista2);
        lista1.display();
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
