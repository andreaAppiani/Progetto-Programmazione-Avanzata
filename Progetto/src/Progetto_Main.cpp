
#include <iostream>
#include <cstring>
#include "Progetto.hpp"

using namespace std;

int main() {
	Casino* cp = Casino::getCasino();

	while(true){
		cp->addCiclo();
		cp->listaGiocatori();
		cp->gioca();
	}
}
