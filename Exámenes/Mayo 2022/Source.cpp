//-------------------------------------------------------
// Ejercicio 3: Ferrovist�n. Mayo 2022
// Escribe tu nombre y respuestas en las zonas indicadas
//-------------------------------------------------------
//@ <answer>
// Nombre :
// Usiario del Juez de Clase :
// Usuario del Juez de Ex�menes :
//@ </answer>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
//A�ade las librer�as que necesites:
//@ <answer>
#include <unordered_map>
#include <map>
//@ </answer>

using namespace std;

// Elige el tipo representante adecuado para el TAD Ferrovistan e
// implementa sus operaciones. Puedes a�adir m�todos o funciones privadas
// si lo consideras conveniente. Justifica la elecci�n de los tipos y el
// coste de todas las operaciones.
//@ <answer>

using tLinea = string;          //Nombres de l�neas
using tEstacion = string;       //Nombres de estaciones

class Ferrovistan {

private:
    // escribe aqu� los atributos y funciones privadass
    using datoLinea = map<int, tEstacion>; // Diccionario ordenado para guardar la posicion de una estacion con su nombre, contenida en una l�nea.
    using datoEstacion = map<tLinea, int>;   // Diccionario ordenado donde se guarda el nombre de una linea con la posicion de la estaci�n en esa l�nea, para as� poder saber a partir del
                                     // nombre de una estaci�n, en que l�neas est� y en qu� posiciones.
    unordered_map<tLinea, datoLinea> lineas;      // Diccionario sin ordenar que contiene clave el nombre de la l�nea y valor sus datos.
    unordered_map<tEstacion, datoEstacion> estaciones;  // Diccionario sin ordenar que contiene clave el nombre de la estaci�n y valor sus datos.

public:

    // Coste O(1) Constante.
    // A�adimos nueva linea. Los datos de la l�nea ser�n vac�os al momento de a�adirlos, pues a�n no tiene estaciones.
    void nueva_linea(const tLinea& nombre) {    
        auto it = lineas.find(nombre);
        if (it != lineas.end()) {
            throw domain_error("Linea existente");
        }
        lineas[nombre] = datoLinea();
    }

    // Coste O(log L) + O(log E). Logar�timico, dependiendo del cu�l sea el m�ximo, si el de l�neas o el de estaciones.
    //  A�ade una nueva estaci�n que de servicio a una l�nea dada. Para mantener la consistencia de los datos, habr� que a�adir tambi�n la estaci�n a los datos de la l�nea.
    void nueva_estacion(const tLinea& linea, const tEstacion& nombre, int posicion) {
        auto it = lineas.find(linea);       // Comprobamos si la l�nea existe o no.
        if (it == lineas.end()) {
            throw domain_error("Linea no existente");
        }
        datoLinea& datoL = it->second;      // Si existe, guardamos una referencia a sus datos sobre estaciones.

        // Ahora, vamos a ver si la estaci�n ya est� contenida en la l�nea o no.
        auto itE = estaciones.find(nombre);
        if (itE != estaciones.end()) {      // Si la estaci�n existe.
            datoEstacion& datoE = itE->second;
            auto itL = datoE.find(linea);   // Buscamos la estaci�n en la l�nea (la posicion).
            if (itL != datoE.end()) {       // Comprobamos si la estaci�n est� duplicada en la l�nea.
                throw domain_error("Estacion duplicada en linea");
            }
        }

        auto lin = datoL.find(posicion);        // Comprobamos si la linea ya tenia una estaci�n en la posici�n dada.
        if (lin != datoL.end()) {
            throw domain_error("Posicion duplicada");
        }

        datoL[posicion] = nombre;               // A�adimos la estacion a los datos de la linea en la posicion dada.
        estaciones[nombre][linea] = posicion;   // A�adimos la estacion, guardando los datos de la estaci�n con el nombre de la l�nea y la posici�n.
    }
       
    // Coste O(N). Lineal, siendo N el n�mero de l�neas que pasan por la estaci�n.
    // Elimina una estaci�n. Para mantener la consistencia de los datos, hay que eliminar la estaci�n tanto en el diccionario de estaciones como de los datos de una l�nea.
    void eliminar_estacion(const tEstacion& estacion) {
        auto it = estaciones.find(estacion);        // Comprobamos primero si la estaci�n es existente.
        if (it == estaciones.end()) {
            throw domain_error("Estacion no existente");
        }
        datoEstacion& datoE = it->second;       // Guardamos una referencia a los datos de la estaci�n, es decir las l�neas que pasan por la estaci�n y la posici�n donde est�n en cada l�nea.
        for (auto itL = datoE.begin(); itL != datoE.end(); ++itL) {     
            lineas[itL->first].erase(itL->second);      // Eliminamos la estaci�n de todas las l�neas que pasan por ella.
        }   
        estaciones.erase(estacion);         // Finalmente eliminamos la estaci�n.
    }

    // Coste O(N). Lineal, siendo N el n�mero de l�neas que pasan por esa estaci�n..
    //  Devuelve una lista de las l�neas que pasan por la estaci�n dada.
    vector<string> lineas_de(const tEstacion& estacion) const {
        vector<string> v;
        auto it = estaciones.find(estacion);
        if (it == estaciones.end()) {       // Comprobamos si la estaci�n existe.
            throw domain_error("Estacion no existente");
        }
        const datoEstacion datoE = it->second;      // Guardamos una referencia a los datos de la estaci�n, es decir a las lineas que contiene esa estacion
        for (auto itL = datoE.begin(); itL != datoE.end(); ++itL) {     // Recorremos las lineas con un iterador para guardarlas en el vector de salida.
            v.push_back(itL->first);
        }
        return v;
    }

    // Coste O(log L) + O(log E). Logar�tmico dependiendo de lo anteriormente dicho.
    //  Devuelve el nombre de la siguiente estaci�n a la pasada por par�metro en la l�nea pasada por par�metro.
    string proxima_estacion(const tLinea& linea, const tEstacion& estacion) const {
        auto it = lineas.find(linea);       // Comprobamos si la l�nea existe.
        if (it == lineas.end()) {
            throw domain_error("Linea no existente");
        }
        const datoLinea datoL = it->second;     // Guardamos una referencia a los datos de la la linea, es decir a la posici�n de cada estaci�n con su nombre en la l�nea.
        auto itE = estaciones.find(estacion);
        if (itE == estaciones.end()) {              // Comprobamos si la estaci�n existe.
            throw domain_error("Estacion no existente");
        }
        const datoEstacion datoE = itE->second;
        auto itLAux = datoE.find(linea);        // Guardamos una referencia a la linea pasada como par�metro en los datos de la estaci�n.
        if (itLAux == datoE.end()) {            // Comprobamos si la linea contiene o no la estaci�n con el nombre pasado por par�metro.
            throw domain_error("Estacion no existente");
        }
        auto itEAux = datoL.find(itLAux->second);       // Obtenemos la posici�n de la estaci�n pasada por par�metro dentro de la l�nea pasada por par�metro.
        ++itEAux;                           // Aumentamos en 1 la posici�n para poder acceder a la pr�xima estaci�n.
        if (itEAux == datoL.end()) {                // Comprobamos si la posici�n entra en el rango de la l�nea, si no, entonces es el fin del trayecto.
            throw domain_error("Fin de trayecto");
        }
        return itEAux->second;          // Devolvemos el nombre de la pr�xima estaci�n a la pasada por par�metro.
    }

};

//@ </answer>
//
// �No modifiques nada debajo de esta l�nea!
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
