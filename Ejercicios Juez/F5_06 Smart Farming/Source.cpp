// Problema Smart farming

// IMPORTANTE

// Nombre y apellidos del alumno
// Usuario del juez de clase
// Usuario que has utilizado en esta prueba

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <unordered_map>
#include <map>
#include <queue>

using namespace std;


// Explicación de la representación elegida para resolver el problema.






// No olvides poner comentarios en el codigo para que se puedan entender bien
// las funciones.
// No olvides poner el coste de cada una de las funciones y explicarlo.

class smartFarming {
private:
    using tsector = string;
    using tplaga = string;

    struct TipoSector {
        int plantas;                         // Número de plantas que contiene un sector.
        map<tplaga, int> datosSector;        // Diccionario ordenado que contiene los datos de un sector, es decir, las plagas junto con el número de plantas que han infectado. 

        TipoSector() : plantas(0), datosSector(map<tplaga, int>()) {};
        TipoSector(int n) : plantas(n) { datosSector = map<tplaga, int>(); };
    };

    unordered_map<tsector, TipoSector> sectores;  // Diccionario sin ordenar que contiene los sectores junto con sus datos.

    queue<pair<tsector, tplaga>> cola;            // Cola que contiene un par con el nombre del sector junto con el nombre de la plaga para poder saber el orden en el que se fue infectando
                                                  // cada sector por cada plaga, en la que sólo guardaremos aquellas plagas que infecten a más de 1/4 de las plantas del sector.

public:
    smartFarming() {}

    // Coste de la función y justificación
    // Coste O(1). Constante. --> Inserción en un unordered_map.   
    // Función que da de alta un nuevo sector id con n plantas. Si el sector ya estaba dado de alta se incrementa el valor del número de plantas.
    void alta(tsector const& id, int n) {
        sectores[id].plantas += n;      // Se incrementa el número de plantas en ese sector con el valor n recibido por parámetro. Es irrelevante si el sector ya estaba dado de alta o no.
    }

    // Coste de la función y justificación
    // Coste O(log n), siendo n el número de plagas actual al estar haciendo una inserción en un mapa ordenado.
    // Función que recibe una notificiación de plaga p en el sector id con n plantas afectadas. Hay que tener en cuenta que las plantas n pasadas por parámetro incluyen también las que
    // estaban afectadas antes de este aviso.
    void datos(tsector const& id, tplaga const& p, int n) {
        auto it = sectores.find(id);                    // Comprobamos si el sector existe.
        if (it == sectores.end()) {
            throw domain_error("Sector no existente");
        }
        TipoSector& tipoS = it->second;                 // Guardamos una referencia a los datos del sector.

        if (n <= sectores[id].plantas) {                // Si el número de plantas a añadir es menor que el de las plantas totales del sector.
            sectores[id].datosSector[p] += n;           // Añadimos las plantas afectadas a los datos del sector.
            if (n >= (sectores[id].plantas / 4)) {      // Si el número de plantas afectadas supera al cuarto de las totales del sector.
                cola.push(pair<tsector, tplaga>(id, p));        // Entonces añadimos el sector y la plaga a la cola para la función de fumigar.
            }
        }
        else {
            throw domain_error("Numero de plantas incorrecto");     // Si se supera el número de plantas, se lanza  domain_error.
        }
    }

    // Coste de la función y justificación
    // Coste  O(n(log m)), donde n es el número de plantas que se van a fumigar y m el número total de plantas en cada sector.
    // n Porque es una inserción a un vector y log m porque es un acceso a un diccionario ordenado.
    // Función que realiza dos acciones. Devuelve un vector con los sectores que tienen 1/4 o más de sus plantas afectadas por una plaga y por otro lado fumiga esos sectores
    // (elimina las plagas). Sectores deben guardarse en el orden en el que fueron alcanzando el cuarto de sus plantas afectadas, es decir, usando la cola.
    vector<pair<tsector, tplaga>> fumigar() {
        vector<pair<tsector, tplaga>> v;
        int infectadas = 0;
        
        while (!cola.empty()) {
            pair<tsector, tplaga> elem = cola.front();
            cola.pop();
            v.push_back(elem);
            sectores[elem.first].datosSector[elem.second] = 0;
        }

        return v;
    }


    // Coste de la función y justificación
    // Coste O(n) según el número de plagas en el sector id, pues estamos introduciendo datos en un vector.
    // Función que obtiene un vector con todas las plagas que ha sufrido el sector id junto con las plantas afectadas.
    vector<pair<tplaga, int>> plagas(tsector const& id) {
        vector<pair<tplaga, int>> v;
        
        auto it = sectores.find(id);
        if (it == sectores.end()) {
            throw domain_error("Sector no existente");
        }

        TipoSector& tipoSec = it->second;
        for (auto itP = tipoSec.datosSector.begin(); itP != tipoSec.datosSector.end(); ++itP) {
            v.push_back({ itP->first, itP->second });
        }

        return v;
    }
};

//////////////////////////////////////////////////////////////////////////////////////
// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    std::string op;
    smartFarming sf;
    std::cin >> op;  // operacion a realizar
    if (!std::cin)
        return false;
    std::string id; std::string plaga; int n;
    while (op != "FIN") {
        try {
            if (op == "alta") {
                std::cin >> id >> n;  // sector - numero de plantas
                sf.alta(id, n);
            }
            else if (op == "datos") {
                std::cin >> id >> plaga >> n; // sector, plaga, numero de plantas afectadas
                sf.datos(id, plaga, n);
            }
            else if (op == "fumigar") {
                std::vector<std::pair<std::string, std::string>> v = sf.fumigar();
                // Escribir el resultado
                for (auto const& s : v) {
                    std::cout << "Fumigar la plaga " << s.second << " del sector";
                    std::cout << ' ' << s.first << '\n';
                }
            }
            else if (op == "plagas") {
                std::cin >> id; // sector
                std::vector<std::pair<std::string, int>> v = sf.plagas(id);
                // Escribir el resultado
                std::cout << "Plagas del sector " << id << " :";
                for (auto const& s : v)
                    std::cout << ' ' << s.first << ' ' << s.second;
                std::cout << '\n';
            }
        }
        catch (std::domain_error& d) {
            std::cout << "ERROR: " << d.what() << '\n';
        }
        std::cin >> op;
    }
    std::cout << "---" << '\n';

    return true;

}


int main() {
#ifndef DOMJUDGE
    std::ifstream in("sample3.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf()); //save old buf and redirect std::cin to casos.txt
#endif

    while (resuelveCaso())
        ;

#ifndef DOMJUDGE // para dejar todo como estaba al principio
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif

    return 0;
}
