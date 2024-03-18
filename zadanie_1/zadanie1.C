#include<iostream>
using namespace std;

Bool_t zadanie1(string name="wave_0.dat"){

	TFile *file = new TFile("widmo.root","UPDATE");

	if(!file->IsOpen()){
		cout << "plik nie działa" <<endl;
		return kFALSE;
	}

	fstream mydat;
	mydat.open(name,ios::in | ios::binary);

	if(!mydat.is_open()){
		cout << "dat nie działa" <<endl;
		return kFALSE;
	}
	else cout << "dat działa" <<endl;


	Float_t line;
	Int_t nbin = 0;
	Int_t nbins = 1024;
	//Float_t counts[nbins];

	cout << "dupa ";

	while (!mydat.eof() ){
		++nbin;
		mydat >> line;
	//	cout << line << " "; 
		//counts[nbin] = line
		if(nbin%10000 == 0) cout << nbin << endl;
	}
	
	cout << nbin;
	//cout << nbins;
	Float_t counts[nbins];

	TH1F *myhist = new TH1F("hWidmo","hWidmo",nbins,0,nbins);

	for(int i=0; i<nbins; i++){
		myhist->SetBinContent(i+1,counts[i]);
	}

	myhist->Draw();


	mydat.close();

	return kTRUE;
}
