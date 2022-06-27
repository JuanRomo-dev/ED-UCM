#include <iostream>
#include <iomanip>
#include <fstream>
#include "Queue.h"
#include "deque.h"
using namespace std;


// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // leer los datos de la entrada
    int n;
    
    cin >> n;
    if (n == 0)
        return false;

    Queue<int> cola;

    int aux;
    for (int i = 0; i < n; i++) {
        cin >> aux;
        cola.push_back(aux);
    }
       
    Queue<int> positivos;
    deque<int> negativos;

    int size = cola.size();
    for (int i = 0; i < size; i++) {
        int num = cola.front();
        if (num < 0) {
            negativos.push_front(num);
        }
        else {
            positivos.push_back(num);
        }
        cola.pop_front();
    }

    int sizeNeg = negativos.size();
    for (int i = 0; i < sizeNeg; i++) {
        cola.push_back(negativos.front());
        negativos.pop_front();
    }

    int posSize = positivos.size();
    for (int i = 0; i < posSize; i++) {
        cola.push_back(positivos.front());
        positivos.pop_front();
    }

    // escribir sol
    cout << cola << endl;

    return true;
}

int main() {
    // Para la entrada por fichero.
    // Comentar para acepta el reto
#ifndef DOMJUDGE
    std::ifstream in("datos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf()); //save old buf and redirect std::cin to casos.txt
#endif 


    while (resuelveCaso())
        ;


    // Para restablecer entrada. Comentar para acepta el reto
#ifndef DOMJUDGE // para dejar todo como estaba al principio
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif

    return 0;
}