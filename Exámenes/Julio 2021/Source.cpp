// Problema Sumar nodos
//
//
// IMPORTANTE
//
// Nombre y apellidos del alumno:
// Usuario del juez de clase:
// Usuario que has utilizado en esta prueba:

/////////////////////////////////////////////////////////
// de aquí hacia abajo NO se puede modificar nada

#include <iostream>               
#include <fstream>
using namespace std;

#include "List.h"


List<int> leerLista(int N) {
    List<int> lista;
    int n, i;
    i = 0;
    while (i != N) {
        cin >> n;
        lista.push_back(n);
        i++;
    }
    return lista;
}

bool resuelveCaso() {
    int N, pos, m;
    cin >> N;
    cin >> pos >> m;
    List<int> lista = leerLista(N);

    lista.add_to(pos, m);

    cout << lista << endl;
    return true;
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
