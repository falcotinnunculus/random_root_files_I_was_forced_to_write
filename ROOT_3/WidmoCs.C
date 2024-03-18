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

	Float_t line;
	Int_t nbin = 0;
	Int_t nbins = 1024;
	Float_t counts[nbins];

	while (!mydat.eof() ){
		++nbin;
		mydat >> line;
		cout << line << " "; 
		counts[nbins] = line;
	}
	

	//cout << nbins;

	TH1F *myhist = new TH1F("hWidmo","hWidmo",nbins,0,nbins);

	for(int i=0; i<nbins; i++){
		myhist->SetBinContent(i+1,counts[i]);
	}

	myhist->Draw();


	mydat.close();
	return kTRUE;


}
