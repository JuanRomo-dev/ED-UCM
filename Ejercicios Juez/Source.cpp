#include <iostream>
#include <list>
#include <iomanip>
#include <algorithm>
#include <fstream>
using namespace std;

int calcularRecesion(const list<int>& lista, int& pos) {
    int res = 0;
    int aux = lista.front();
    int cont = 0;
    int i = 0;
    pos = 0;

    for (auto it = lista.begin(); it != lista.end(); ++it) {
        if (aux >= *it) {
            cont++;
        }
        else {
            aux = *it;
            if (cont >= res) {
                pos = i - cont;
                res = cont;
            }
            cont = 1;
        }
        i++;
    }

    if (cont >= res) {
        pos = i - cont;
        res = cont;
    }
    cont = 1;

    return res;
}


// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // leer los datos de la entrada
    int n;

    cin >> n;
    if (!std::cin)
        return false;

    list<int> lista;

    int num;
    for (int i = 0; i < n; i++) {
        cin >> num;
        lista.push_back(num);
    }

    // escribir sol
    int pos = 0;
    int sol = calcularRecesion(lista, pos);

    cout << sol << " " << pos << endl;

    return true;
}

int main() {
    // Para la entrada por fichero.
    // Comentar para acepta el reto
#ifndef DOMJUDGE
    std::ifstream in("casos.txt");
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