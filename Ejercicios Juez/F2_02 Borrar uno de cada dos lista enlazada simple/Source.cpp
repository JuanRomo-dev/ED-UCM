#include <iostream>
#include <iomanip>
#include <fstream>
#include "list_linked_single_plus.h"
#include "horas.h"
using namespace std;

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // leer los datos de la entrada
    int n;
    cin >> n;

    if (n < 1)
        return false;
   
    ListLinkedSingle<horas> horario;
    horas h;

    for (int i = 0; i < n; i++) {
        cin >> h;
        horario.push_back(h);
    }

    horario.eliminarPares();

    // escribir sol
    horario.display();
    cout << endl;

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
