#include <iostream>               
#include <fstream>
#include "list_linked_single_plus.h"
using namespace std;

/*
 * No necesitas editar nada a partir de aquí hacia abajo salvo, quizás, el nombre del fichero de datos
 */

template<typename T>
ListLinkedSingle<T> leerLista() {
    ListLinkedSingle<T> lista;
    T n;
    cin >> n;
    while (n != 0) {
        lista.push_back(n);
        cin >> n;
    }
    return lista;
}

bool resuelveCaso() {
    //lectura
    ListLinkedSingle<int> lista1 = leerLista<int>();
    ListLinkedSingle<int> lista2 = leerLista<int>();

    //llamada a la función que debes hacer
    lista1.interseccion(lista2);

    //escritura
    lista1.display();
    cout << endl;

    return true;
}


int main() {
#ifndef DOMJUDGE
    std::ifstream in("sample1.txt");
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
