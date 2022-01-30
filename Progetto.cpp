
#include "Progetto.hpp"
using namespace std;


//------------PERSONA------------
Persona::Persona(string s1, string s2, int i): eta(i), nome(s1), cognome(s2) {};
Persona::Persona(const Persona& p): eta(p.eta), nome(p.nome), cognome(p.cognome)  { };
Persona::Persona(): Persona("","",30) {}; //OVERLOADING

void Persona::print(){ cout << "Nome: " << nome << ", Cognome: " << cognome << ", Età: " << eta << endl; };



//------------GIOCATORE ABSTRACT------------
int GiocatoreAbstract::counter = 1;



//------------GIOCATORE------------
Giocatore::Giocatore(string s1, string s2, int i): Persona(s1,s2,i) {vittorie=sconfitte=0; ID = counter++;};
Giocatore::Giocatore(const Giocatore& g): Persona(g.getNome(), g.getCognome(), g.getEta()) { vittorie=g.vittorie; sconfitte=g.sconfitte; ID = g.ID;};
Giocatore::Giocatore(): Persona() {vittorie=sconfitte=0; ID = counter++;};

void Giocatore::print(){ cout << "Giocatore n°" << ID << " | "; Persona::print();};
float Giocatore::getWinrate() const {
	return (vittorie==0 && sconfitte==0) ? 0 : (float) vittorie/(vittorie+sconfitte);};


//------------ComparatorByID------------
bool ComparatorByID::operator() (const Giocatore& g1, const Giocatore& g2) const {return g1.ID < g2.ID;};

//------------ComparatorByWinrate--------
bool ComparatorByWinrate::operator() (const Giocatore& g1, const Giocatore& g2) const {return g1.getWinrate() > g2.getWinrate();};



//------------CASINO'------------
Casino* Casino::casino;

Casino* Casino::getCasino(){
	if(casino == 0) casino = new Casino;
	return casino;
}

void Casino::addGiocatore(string s1,  string s2, int i) {giocatori.push_back(Giocatore(s1,s2,i));};

void Casino::addCiclo(){
	char risposta = 'y';
	string nome; string cognome; int eta;

	while(tolower(risposta)=='y'){
		cout << "Inserire nome giocatore: "; cin >> nome;
		cout << "Inserire cognome giocatore: "; cin >> cognome;
		cout << "Inserire età giocatore: "; cin >> eta;
		if(eta > 17) { addGiocatore(nome,cognome,eta); }
		else cout << "Non sono accettati giocatori minorenni\n";
		cout << "Aggiungere un nuovo giocatore? y/n "; cin >> risposta;
	}
}

void Casino::listaGiocatori(){
	cout<<"\n--------LISTA GIOCATORI PARTECIPANTI-------\n";
	for(vector<Giocatore>::iterator i=giocatori.begin();i!=giocatori.end();++i){ i->print(); }
	cout<<"-------------------------------------------\n";
};
void Casino::listaPrevisioni(){
	cout<<"\n--------LISTA PREVISIONI-------\n";
	for(map<Giocatore,int>::const_iterator i=previsioni.begin();i!=previsioni.end();++i){
		cout << "Giocatore n°" << i->first.ID << ": " << i->second << endl;}
	cout<<"-------------------------------\n\n";
};
void Casino::inserisciPrevisioni(){
	previsioni.clear();
	for(vector<Giocatore>::const_iterator i=giocatori.begin();i!=giocatori.end();++i){
		int n; bool flag=false;
		do{
			cout << "Giocatore n°" << i->ID << " prevede il numero: "; cin >> n;
			if(cin.fail() || n<0 || n>100) {cout << " Inserire un numero valido tra 0 e 100\n"; cin.clear(); cin.ignore(); flag=true;}
			else flag=false;
		}
		while(flag);
		cout << endl;
		previsioni[*i] = n;
	}
};

void Casino::trovaVincitori(int numero){
	int distanza_min = 100;
	for(map<Giocatore,int>::const_iterator i=previsioni.begin();i!=previsioni.end();++i){
			if(abs(i->second - numero) < distanza_min) {distanza_min = abs(i->second - numero);}
	}

	for(map<Giocatore,int>::const_iterator j=previsioni.begin();j!=previsioni.end();++j){
		Giocatore* G = trovaGiocatore(j->first.ID);
		if(abs(j->second - numero) == distanza_min) {
			G->vittorie++;
			cout<<"\n-------->   VINCITORE: Giocatore n°" << G->ID << " | Winrate: " << G->getWinrate()*100 << "%";
			}
		else {G->sconfitte++;}
	}
	cout << endl << endl;
};

Giocatore* Casino::trovaGiocatore(int id){
	int index=0;
	for(vector<Giocatore>::iterator i=giocatori.begin();i!=giocatori.end();++i){
		if(id == i->ID) {return &(*i);}
		index++;
	}
	return NULL;
};

bool Casino::giocaAncora() const {char s;
	cout << "Giocare ancora? y/n: "; cin >> s; cout << endl;
	return (tolower(s)=='y') ? true : false;
};
void Casino::gioca(){
	if(giocatori.empty()) {cout << "Non ci sono partecipanti.\n"; return;}
	bool flag = true;
	while(flag){
		srand(time(0));
		int numero = rand()%100;  //intero random in [0,100]
		inserisciPrevisioni();
		listaPrevisioni();
		cout<<"Numero estratto: " << numero;
		trovaVincitori(numero);
		flag = giocaAncora();
	}
	cout<<"---------GIOCO CONCLUSO--------\n";
	classificaWinrates();
	cout<<"-------------------------------\n\n";
	giocatori.clear(); //Chiama il distruttore di ogni oggetto contenuto
};

void Casino::classificaWinrates(){
	sort(giocatori.begin(), giocatori.end(), ComparatorByWinrate());
	for(vector<Giocatore>::iterator i=giocatori.begin();i!=giocatori.end();++i){
		cout << "Giocatore n°" << i->ID << " | Winrate: " << i->getWinrate()*100 << "%\n";
	}
}
