/**
 * Ejemplo ilustrativo de la declaración e implementación de 
 * funciones de localización (hash( para tipos básicos y 
 * objeto función.
 * (c) Antonio Sánchez Ruiz-Granados, 2012
 * Modificado por Ignacio Fábregas, 2022
 */

#ifndef __HASH_H
#define __HASH_H

#include <string>

// ----------------------------------------------------
//
// Funciones hash para distintos tipos de datos básicos
//
// ----------------------------------------------------


inline unsigned int mihash(unsigned int clave) {
	return clave;
}

inline unsigned int mihash(int clave) {
	return (unsigned int) clave;
}

inline unsigned int hash(char clave) {
	return clave;
}

// Fowler/Noll/Vo (FNV) -- adaptada de http://bretmulvey.com/hash/6.html 
inline unsigned int mihash(std::string clave) {
	const unsigned int p = 16777619; // primo grande
	unsigned int hash = 2166136261;  // valor inicial
	for (unsigned int i=0; i<clave.size(); i++)
		hash = (hash ^ clave[i]) * p; // ^ es xor binario
	// mezcla final
	hash += hash << 13;
	hash ^= hash >> 7;
	hash += hash << 3;
	hash ^= hash >> 17;
	hash += hash << 5;
	return hash;
}

/**
 * Objeto función para hash .
 */
template<class C>
class Hash{
public:
    int operator()(const C& c)  const{
        return mihash(c);
    }

};


#endif // __HASH_H
