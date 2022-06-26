#ifndef HORAS
#define HORAS 

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <iomanip>
using namespace std;

class horas {
private:
	int nhoras, nminutos, nsegundos;

public:
	horas(int h = 0, int m = 0, int s = 0) {		// Constructora.
		if (h < 0 || h > 23 || m < 0 || m > 59 || s < 0 || s > 59) {		// Comprobamos la hora de entrada.
			throw invalid_argument("ERROR");
		}
		else {
			nhoras = h;
			nminutos = m;
			nsegundos = s;
		}
	}

	int hora() const {
		return nhoras;
	}

	int minutos() const {
		return nminutos;
	}

	int segundos() const {
		return nsegundos;
	}

	bool operator <(const horas& h) const {				// Sobrecarga operador < para hora.
		bool b = false;
		if (nhoras < h.nhoras) {
			b = true;
		}
		else if (nhoras == h.nhoras && nminutos < h.nminutos) {
			b = true;
		}
		else if (nhoras == h.nhoras && nminutos == h.nminutos && nsegundos <= h.nsegundos) {
			b = true;
		}
		return b;
	}

	void print(ostream& o) {							// Escribir hora.
		o << setfill('0') << setw(2) << nhoras << ':'
			<< setfill('0') << setw(2) << nminutos << ':'
			<< setfill('0') << setw(2) << nsegundos;
	}

	horas read(istream& i) {			// Leer hora.
		int h, min, seg;
		char aux;
		i >> h >> aux >> min >> aux >> seg;
		horas hora = horas(h, min, seg);
		return hora;
	}
};

ostream& operator <<(ostream& insercion, horas& hora) {			// Sobrecarga operador <<.
	hora.print(insercion);
	return insercion;
}

istream& operator >>(istream& extraccion, horas& hora) {		// Sobrecarga operador >>.
	hora = hora.read(extraccion);
	return extraccion;
}

#endif