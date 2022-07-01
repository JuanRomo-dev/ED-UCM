#include <iostream>
#include <iomanip>
#include <fstream>
#include <stack>
#include <vector>
using namespace std;

// función que resuelve el problema
bool equilibrado(vector<char> caracteres) {
    stack<char> pila;
    bool esEq = true;
    int i = 0;

    if (caracteres.size() > 0) {
        while (i < caracteres.size() && esEq) {
            switch (caracteres[i])
            {
            case '(': case '{':  case '[':
                pila.push(caracteres[i]);
                break;
            case ')': case '}': case ']':
                if (!pila.empty() && caracteres[i] == ')' && caracteres[i] - 1 == pila.top()) {
                    pila.pop();
                }
                else if (!pila.empty() && caracteres[i] - 2 == pila.top()) {
                    pila.pop();
                }
                else {
                    esEq = false;
                }
            default:
                break;
            }
            i++;
        }
    }

    return esEq && pila.empty();
}

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // leer los datos de la entrada
    vector<char> caracteres;
    char c;

    cin.get(c);

    if (!std::cin)
        return false;

    while (c != '\n') {
        caracteres.push_back(c);
        cin.get(c);
    }

    bool sol = equilibrado(caracteres);

    // escribir sol
    if (sol) {
        cout << "SI" << endl;
    }
    else {
        cout << "NO" << endl;
    }

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
