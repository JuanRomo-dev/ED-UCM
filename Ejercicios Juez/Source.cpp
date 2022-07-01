#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <list>
using namespace std;

void presidente(int nSobres, int nDar) {
    int i = 0;
    int num;
    queue<int> cola;
    list<int> lista;

    while (cola.size() < nDar) {
        cin >> num;

        while (!lista.empty() && lista.back() < num) {
            lista.pop_back();
        }
        lista.push_back(num);
        cola.push(num);
        i++;
    }

    while (i < nSobres) {
        cout << lista.front() << " ";
        if (cola.front() == lista.front()) {
            lista.pop_front();
        }
        cola.pop();

        cin >> num;
        cola.push(num);
        while (!lista.empty() && lista.back() < num) {
            lista.pop_back();
        }
        lista.push_back(num);
        i++;
    }

    cout << lista.front() << " ";
}

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // leer los datos de la entrada
    int nSobres, nDar;

    cin >> nSobres >> nDar;
    
    if (nSobres == 0)
        return false;
    
    presidente(nSobres, nDar);

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