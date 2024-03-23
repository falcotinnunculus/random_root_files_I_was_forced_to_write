#include<iostream>
#include<fstream>
#include<string>
using namespace std;

Bool_t zadanie1(string name="wave_0.txt"){

	//TFile *file = new TFile("widmo.root","UPDATE");

	//if(!file->IsOpen()){
	//	cout << "plik nie działa" <<endl;
		//return kFALSE;
	//}
	//
	cout << "dupa blada";
	ifstream mydat;
	mydat.open(name, ios::in );

	cout << "dupa blada";
	if(!mydat.is_open()){
		cout << "dat nie działa" <<endl;
		return kFALSE;
	}
	else {cout << "dat działa" <<endl;}

	cout << "dupa blada";
	Float_t liczba;
	cout << "dupa blada";
	string line;
	cout << "dupa blada";
	Int_t nbin = 0;
	cout << "dupa blada";
	Int_t nbins = 1024;
	cout << "dupa blada";
	//Float_t counts[nbins];
	Float_t temp;
	//Int_t maxline = 55048191;
	Int_t maxline = 55048191;
	Float_t lines[nbins];
	for(int i=0; i<nbins; i++) lines[i] = 0;
	cout << "dupa";
	while (nbin < maxline ){
	//	if(nbin%100000==0) cout << "chuj";
		mydat >> temp;
		lines[nbin%1024] += temp;
	//	if(nbin%1000000==0) cout << lines[nbin] << " ";
	//	if(nbin%100 == 0) break;
		nbin++;
	}
	for(int i=0; i<nbins; i++) cout << lines[i] << " ";
	

	cout << nbin;
	//cout << nbins;
	Float_t counts[nbins];

	TH1F *myhist = new TH1F("hWidmo","hWidmo",nbins,0,nbins);

	for(int i=0; i<nbins; i++){
		myhist->SetBinContent(i+1,counts[i]);
	}

	//myhist->Draw();


	//mydat.close();

	return kTRUE;
}
