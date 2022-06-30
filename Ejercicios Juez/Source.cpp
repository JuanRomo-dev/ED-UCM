#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;
//Añade las librerías que necesites:
//@ <answer>
#include <set>
#include <unordered_map>
#include <map>
#include <deque>
//@ </answer>

using tPaciente = string;
using tGravedad = int;

class Urgencias {

private:
	unordered_map<tPaciente, tGravedad> pacientes;
	map<tGravedad, deque<tPaciente>, greater<tGravedad>> espera;
	set<tPaciente> pacientesRecuperados;

public:
	void nuevo_paciente(const tPaciente& paciente, const tGravedad& gravedad) {
		auto it = pacientes.find(paciente);
		if (it != pacientes.end()) {
			throw domain_error("Paciente repetido");
		}

		if (gravedad < 0 || gravedad > 3) {
			throw domain_error("Gravedad incorrecta");
		}

		pacientes[paciente] = gravedad;
		espera[gravedad].push_back(paciente);
	}

	int gravedad_actual(const tPaciente& paciente) {
		auto it = pacientes.find(paciente);
		if (it == pacientes.end()) {
			throw domain_error("Paciente inexistente");
		}

		return it->second;
	}

	tPaciente siguiente() {
		bool vacio = true;
		tPaciente p;

		auto itA = espera.begin();
		while (itA != espera.end() && itA->second.empty()) {
			++itA;
		}

		while (itA != espera.end() && !itA->second.empty() && itA->first != pacientes[itA->second.front()]) {
			itA->second.pop_front();
			if (itA->second.empty()) {
				++itA;
			}
		}

		if (itA == espera.end() || itA->second.empty()) {
			throw domain_error("No hay pacientes");
		}

		p = itA->second.front();
		pacientes.erase(p);
		itA->second.pop_front();
		
		return p;	
	}

	void mejora(const tPaciente& paciente) {
		auto it = pacientes.find(paciente);
		if (it == pacientes.end()) {
			throw domain_error("Paciente inexistente");
		}

		if (it->second == 1) {
			pacientesRecuperados.insert(paciente);
			pacientes.erase(paciente);
		}
		else {
			pacientes[paciente]--;
			espera[pacientes[paciente]].push_front(paciente);
		}
	}

	set<tPaciente> recuperados() {
		return pacientesRecuperados;
	}

};

bool resuelveCaso() {
	Urgencias urgencias;
	string operacion;
	cin >> operacion;

	if (!cin) {
		return false;
	}

	while (operacion != "FIN") {
		try {
			if (operacion == "nuevo_paciente") {
				string nombre;
				int gravedad;

				cin >> nombre >> gravedad;
				urgencias.nuevo_paciente(nombre, gravedad);
			}
			else if (operacion == "gravedad_actual") {
				string nombre;
				cin >> nombre;
				cout << "La gravedad de " << nombre << " es " << urgencias.gravedad_actual(nombre) << endl;
			}
			else if (operacion == "siguiente") {
				cout << "Siguiente paciente: " << urgencias.siguiente() << endl;
			}
			else if (operacion == "mejora") {
				string nombre;
				cin >> nombre;
				urgencias.mejora(nombre);
			}
			else if (operacion == "recuperados") {
				set<string> recup = urgencias.recuperados();
				cout << "Lista de recuperados: ";
				for (auto it = recup.begin(); it != recup.end(); ++it) {
					cout << *it << " ";
				}
				cout << endl;
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
	ifstream in("datos.txt");
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