#include<iostream>
#include<fstream>
using namespace std;

Bool_t WidmoCs(string name = "Cs-137.dat") {
	TFile *file = new TFile("widmo.root","UPDATE");

	if(!file->IsOpen()){
		cout << "plik nie działa" <<endl;
		return kFALSE;
	}

	fstream mydat;
	mydat.open(name,ios::in);

	if(!mydat.is_open()){
		cout << "dat nie działa" <<endl;
		return kFALSE;
	}
	else cout << "dat działa" <<endl;

	string line;
	while (!mydat.eof() ){
		mydat >> line;
		cout << line << " "; 
	}
	
	mydat.close();
	return kTRUE;

}
