#include<iostream>
#include<fstream>
#include<string>
using namespace std;

bool zadanie1(string name="wave_0.dat"){

//	TFile *file = new TFile("widmo.root","UPDATE");

//	if(!file->IsOpen()){
//		cout << "plik nie działa" <<endl;
//		return kFALSE;
//	}

	ifstream mydat;
	mydat.open(name, ios::binary | ios::in);

	if(!mydat.is_open()){
		cout << "dat nie działa" <<endl;
		return false;
	}
	else cout << "dat działa" <<endl;


	string line;
	int nbin = 0;
	int nbins = 1024;
	//Float_t counts[nbins];
	int temp;
	int maxline = 2816387;

	while (nbin < maxline ){
		++nbin;
		mydat >> line;
		//temp += stoi(line);
	//	cout << line << " "; 
		//counts[nbin] = line
		//cout << line << " ";
		if(nbin%100 == 0) break;
	}
	
	cout << nbin;
	//cout << nbins;

	mydat.close();

	return true;
}

int main(){
	zadanie1();

	return 0;

}
