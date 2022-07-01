//-------------------------------------------------------
// Ejercicio 1: Listas equilibradas. Mayo 2022
// Escribe tu nombre y respuestas en las zonas indicadas
//-------------------------------------------------------
//@ <answer>
// Nombre :
// Usiario del Juez de Clase :
// Usuario del Juez de Exámenes :
//@ </answer>

#include <iostream>
#include <fstream>
#include <string>
#include <list>
//Añade las librerías que necesites:
//@ <answer>
#include <stack>
//@ </answer>


using namespace std;


//-------------------------------------------------------
//Rellena adecuadamente la función y RAZONA el coste
//@ <answer>

void reconstruir(list<int>& lista) {
    stack<int> pila;

    auto it = lista.begin();
    while (it != lista.end()) {
        if (*it > 0) {          // Si el valor es positivo, se apila como negativo.
            pila.push(-*it);
            ++it;
        }
        else if (*it == 0) {    // Si el valor es igual a 0, quiere decir que un negativo ha sido remplazado.
            *it = pila.top();   // Cogemos el valor de la pila.
            pila.pop();
            ++it;
        }
        else {                  // Si el valor es negativo.
            if (!pila.empty() && pila.top() == -(*it)) {        // Es correcto
                pila.pop();
                ++it;
            }
            else {              // Si es incorrecto se elimina.
                it = lista.erase(it);
            }
        }
    }

    while (!pila.empty()) {     // Vaciamos la pila.
        int valor = pila.top();
        pila.pop();
        lista.push_back(valor);     // Añadimos todos los valores negativos.
    }
}


//@ </answer>
//
// ¡No modifiques nada debajo de esta línea!
// ----------------------------------------------------------------

bool resuelveCaso() {
    int n;
    list<int> lista;
    cin >> n;
    if (!cin)
        return false;

    for (int i = 0; i < n; i++) {
        int elem;
        cin >> elem;
        lista.push_back(elem);
    }

    reconstruir(lista);

    bool primero = true;
    for (int x : lista) {
        if (primero) {
            primero = false;
            cout << x;
        }
        else {
            cout << " " << x;
        }
    }
    cout << endl;

    return true;
}

int main() {

#ifndef DOMJUDGE
    ifstream in("sample1.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    while (resuelveCaso())
        ;

#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif

    return 0;

} // main
