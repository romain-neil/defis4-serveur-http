//
// Created by Romain Neil on 28/03/2021.
//

#include "Compteur.h"

#include <utility>

Compteur::Compteur(std::string nom, int initialValue, bool isSpecial) : m_nom(std::move(nom)), m_val(initialValue), m_isSpecial(isSpecial) {}

void Compteur::inc(int val) {
	if(!m_isSpecial) {
		m_val += val;
	}
}

void Compteur::setVal(int newVal) {
	m_val = newVal;
}
