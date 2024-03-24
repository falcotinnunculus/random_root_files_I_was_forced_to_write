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
	gROOT->Reset();
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

	Float_t liczba;
	string line;
	Int_t nbin = 0;
	Int_t nbins = 1024;
	Float_t temp;
	Int_t maxline = 55048192;
	Float_t lines[nbins];
	Float_t adc = 4.096;
	for(int i=0; i<nbins; i++) lines[i] = 0;
	while (!mydat.eof()){
		for(int i=0; i<nbins; i++){
			mydat >> temp;
			lines[i] = temp;
		nbin++;
		}

		TH1F *myhist = new TH1F("hWidmo","hWidmo",nbins,0,nbins);
	
		for(int i=0; i<nbins; i++){
			myhist->SetBinContent(i+1,lines[i]/adc);
		}
		TCanvas *can = new TCanvas("can","can",600,600);

		myhist->Draw();
		gPad->WaitPrimitive();
	}
	for(int i=0; i<nbins; i++) cout << lines[i] << " ";
	

	cout << nbin;
	//cout << nbins;
	Float_t counts[nbins];




	//mydat.close();

	return kTRUE;
}
