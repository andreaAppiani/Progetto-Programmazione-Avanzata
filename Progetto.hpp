
#ifndef PROGETTO_HPP_
#define PROGETTO_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <ctime>
#include <algorithm>
using namespace std;

class Persona {
private:
	int eta;
	string nome;
	string cognome;

public:
	Persona(string, string, int);
	Persona(const Persona&);
	Persona();

	void virtual print();
	virtual ~Persona() = default;

	string getNome() const {return nome;}
	string getCognome() const {return cognome;}
	int getEta() const {return eta;}
};

class GiocatoreAbstract{
private:
	int vittorie;
	int sconfitte;
	static int counter;
	int ID;

public:
	friend class ComparatorByID;
	friend class ComparatorByWinrate;
	friend class Casino;
	friend class Giocatore;

	virtual float getWinrate() const = 0; // Dichiarazione per le classi astratte
	virtual ~GiocatoreAbstract() = default;
};

class Giocatore: public Persona, public GiocatoreAbstract{
public:
	Giocatore(string, string, int);
	Giocatore(const Giocatore&);
	Giocatore();

	//Override dei metodi ereditati
	void print();
	float getWinrate() const;
	~Giocatore() = default;
};


class ComparatorByID{
public: bool operator() (const Giocatore&, const Giocatore&) const;
};
class ComparatorByWinrate{
public: bool operator() (const Giocatore&, const Giocatore&) const;
};


class Casino{
private:
	vector<Giocatore> giocatori; // lista dei partecipanti alla sessione corrente
	map<Giocatore, int, ComparatorByID> previsioni;

	Casino(){};
	static Casino* casino;
	void inserisciPrevisioni();
	void trovaVincitori(int);
	bool giocaAncora() const;
	Giocatore* trovaGiocatore(int);

public:
	static Casino* getCasino();
	void addCiclo();
	void addGiocatore(string, string, int);
	void listaGiocatori();
	void listaPrevisioni();
	void classificaWinrates();
	void gioca();
	~Casino() = default;
};

#endif /* PROGETTO_HPP_ */
