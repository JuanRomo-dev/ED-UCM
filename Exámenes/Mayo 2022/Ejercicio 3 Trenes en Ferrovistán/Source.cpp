//-------------------------------------------------------
// Ejercicio 3: Ferrovistán. Mayo 2022
// Escribe tu nombre y respuestas en las zonas indicadas
//-------------------------------------------------------
//@ <answer>
// Nombre :
// Usiario del Juez de Clase :
// Usuario del Juez de Exámenes :
//@ </answer>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
//Añade las librerías que necesites:
//@ <answer>
#include <unordered_map>
#include <map>
//@ </answer>

using namespace std;

// Elige el tipo representante adecuado para el TAD Ferrovistan e
// implementa sus operaciones. Puedes añadir métodos o funciones privadas
// si lo consideras conveniente. Justifica la elección de los tipos y el
// coste de todas las operaciones.
//@ <answer>

using tLinea = string;          //Nombres de líneas
using tEstacion = string;       //Nombres de estaciones

class Ferrovistan {

private:
    // escribe aquí los atributos y funciones privadass
    using datoLinea = map<int, tEstacion>; // Diccionario ordenado para guardar la posicion de una estacion con su nombre, contenida en una línea.
    using datoEstacion = map<tLinea, int>;   // Diccionario ordenado donde se guarda el nombre de una linea con la posicion de la estación en esa línea, para así poder saber a partir del
                                     // nombre de una estación, en que líneas está y en qué posiciones.
    unordered_map<tLinea, datoLinea> lineas;      // Diccionario sin ordenar que contiene clave el nombre de la línea y valor sus datos.
    unordered_map<tEstacion, datoEstacion> estaciones;  // Diccionario sin ordenar que contiene clave el nombre de la estación y valor sus datos.

public:

    // Coste O(1) Constante.
    // Añadimos nueva linea. Los datos de la línea serán vacíos al momento de añadirlos, pues aún no tiene estaciones.
    void nueva_linea(const tLinea& nombre) {    
        auto it = lineas.find(nombre);
        if (it != lineas.end()) {
            throw domain_error("Linea existente");
        }
        lineas[nombre] = datoLinea();
    }

    // Coste O(log L) + O(log E). Logarítimico, dependiendo del cuál sea el máximo, si el de líneas o el de estaciones.
    //  Añade una nueva estación que de servicio a una línea dada. Para mantener la consistencia de los datos, habrá que añadir también la estación a los datos de la línea.
    void nueva_estacion(const tLinea& linea, const tEstacion& nombre, int posicion) {
        auto it = lineas.find(linea);       // Comprobamos si la línea existe o no.
        if (it == lineas.end()) {
            throw domain_error("Linea no existente");
        }
        datoLinea& datoL = it->second;      // Si existe, guardamos una referencia a sus datos sobre estaciones.

        // Ahora, vamos a ver si la estación ya está contenida en la línea o no.
        auto itE = estaciones.find(nombre);
        if (itE != estaciones.end()) {      // Si la estación existe.
            datoEstacion& datoE = itE->second;
            auto itL = datoE.find(linea);   // Buscamos la estación en la línea (la posicion).
            if (itL != datoE.end()) {       // Comprobamos si la estación está duplicada en la línea.
                throw domain_error("Estacion duplicada en linea");
            }
        }

        auto lin = datoL.find(posicion);        // Comprobamos si la linea ya tenia una estación en la posición dada.
        if (lin != datoL.end()) {
            throw domain_error("Posicion duplicada");
        }

        datoL[posicion] = nombre;               // Añadimos la estacion a los datos de la linea en la posicion dada.
        estaciones[nombre][linea] = posicion;   // Añadimos la estacion, guardando los datos de la estación con el nombre de la línea y la posición.
    }
       
    // Coste O(N). Lineal, siendo N el número de líneas que pasan por la estación.
    // Elimina una estación. Para mantener la consistencia de los datos, hay que eliminar la estación tanto en el diccionario de estaciones como de los datos de una línea.
    void eliminar_estacion(const tEstacion& estacion) {
        auto it = estaciones.find(estacion);        // Comprobamos primero si la estación es existente.
        if (it == estaciones.end()) {
            throw domain_error("Estacion no existente");
        }
        datoEstacion& datoE = it->second;       // Guardamos una referencia a los datos de la estación, es decir las líneas que pasan por la estación y la posición donde están en cada línea.
        for (auto itL = datoE.begin(); itL != datoE.end(); ++itL) {     
            lineas[itL->first].erase(itL->second);      // Eliminamos la estación de todas las líneas que pasan por ella.
        }   
        estaciones.erase(estacion);         // Finalmente eliminamos la estación.
    }

    // Coste O(N). Lineal, siendo N el número de líneas que pasan por esa estación..
    //  Devuelve una lista de las líneas que pasan por la estación dada.
    vector<string> lineas_de(const tEstacion& estacion) const {
        vector<string> v;
        auto it = estaciones.find(estacion);
        if (it == estaciones.end()) {       // Comprobamos si la estación existe.
            throw domain_error("Estacion no existente");
        }
        const datoEstacion datoE = it->second;      // Guardamos una referencia a los datos de la estación, es decir a las lineas que contiene esa estacion
        for (auto itL = datoE.begin(); itL != datoE.end(); ++itL) {     // Recorremos las lineas con un iterador para guardarlas en el vector de salida.
            v.push_back(itL->first);
        }
        return v;
    }

    // Coste O(log L) + O(log E). Logarítmico dependiendo de lo anteriormente dicho.
    //  Devuelve el nombre de la siguiente estación a la pasada por parámetro en la línea pasada por parámetro.
    string proxima_estacion(const tLinea& linea, const tEstacion& estacion) const {
        auto it = lineas.find(linea);       // Comprobamos si la línea existe.
        if (it == lineas.end()) {
            throw domain_error("Linea no existente");
        }
        const datoLinea datoL = it->second;     // Guardamos una referencia a los datos de la la linea, es decir a la posición de cada estación con su nombre en la línea.
        auto itE = estaciones.find(estacion);
        if (itE == estaciones.end()) {              // Comprobamos si la estación existe.
            throw domain_error("Estacion no existente");
        }
        const datoEstacion datoE = itE->second;
        auto itLAux = datoE.find(linea);        // Guardamos una referencia a la linea pasada como parámetro en los datos de la estación.
        if (itLAux == datoE.end()) {            // Comprobamos si la linea contiene o no la estación con el nombre pasado por parámetro.
            throw domain_error("Estacion no existente");
        }
        auto itEAux = datoL.find(itLAux->second);       // Obtenemos la posición de la estación pasada por parámetro dentro de la línea pasada por parámetro.
        ++itEAux;                           // Aumentamos en 1 la posición para poder acceder a la próxima estación.
        if (itEAux == datoL.end()) {                // Comprobamos si la posición entra en el rango de la línea, si no, entonces es el fin del trayecto.
            throw domain_error("Fin de trayecto");
        }
        return itEAux->second;          // Devolvemos el nombre de la próxima estación a la pasada por parámetro.
    }

};

//@ </answer>
//
// ¡No modifiques nada debajo de esta línea!
// ----------------------------------------------------------------

bool resuelveCaso() {
    Ferrovistan trenes;
    string operacion;
    cin >> operacion;

    if (!cin)
        return false;

    while (operacion != "FIN") {
        try {
            if (operacion == "nueva_linea") {
                string nombre;
                cin >> nombre;
                trenes.nueva_linea(nombre);
            }
            else if (operacion == "nueva_estacion") {
                string linea, nombre;
                int posicion;
                cin >> linea;
                cin >> nombre;
                cin >> posicion;
                trenes.nueva_estacion(linea, nombre, posicion);
            }
            else if (operacion == "eliminar_estacion") {
                string estacion;
                cin >> estacion;
                trenes.eliminar_estacion(estacion);
            }
            else if (operacion == "lineas_de") {
                string estacion;
                cin >> estacion;
                vector<string> lineas = trenes.lineas_de(estacion);
                cout << "Lineas de " << estacion << ":";
                for (const string& linea : lineas)
                    cout << " " << linea;
                cout << endl;
            }
            else if (operacion == "proxima_estacion") {
                string linea, estacion;
                cin >> linea;
                cin >> estacion;
                string proxima = trenes.proxima_estacion(linea, estacion);
                cout << proxima << endl;
            }
        }
        catch (exception& e) {
            cout << "ERROR: " << e.what() << endl;
        }
        cin >> operacion;
    }
    cout << "---" << endl;

    return true;
}

int main() {
#ifndef DOMJUDGE
    ifstream in("sample3.txt");
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
