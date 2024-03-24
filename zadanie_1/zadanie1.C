#include<iostream>
#include<fstream>
#include<string>
using namespace std;

Bool_t zadanie1(string name="wave_0.dat", int baseline=0){

	gROOT->Reset();
	
	ifstream mydat;
	mydat.open(name, ios::binary | ios::in );

	if(!mydat.is_open()){
		cout << "dat nie działa" <<endl;
		return kFALSE;
	}
	else {cout << "dat działa" <<endl;}

	Int_t nhist = 0;
	Int_t nbins = 1024;
	Float_t tempx;
	Float_t mean;
	Float_t lines[nbins];
	Float_t adc = 4.096;
	Int_t mean_denom = 50;


	TH1F *myhist = new TH1F("histo","histogram from scintillator",nbins,0,nbins);
	myhist->GetXaxis()->SetTitle("channle number");
	myhist->GetYaxis()->SetTitle("amplitude [mV]");
	TLatex text;

	for(int i=0; i<nbins; i++) lines[i] = 0;
	while (!mydat.eof()){
		cout << Form("printing histogram %i", nhist) << endl; 
		for(int i=0; i<nbins; i++){
			mydat.read((char*)&tempx, sizeof(float));
			lines[i] = tempx;
		}

		mean = 0;
		if(baseline==1){
			for(int i=0; i<50; i++){
				mean+= lines[i]/mean_denom;
			}
		}



		for(int i=0; i<nbins; i++){
			myhist->SetBinContent(i+1,(lines[i]-mean)/adc);
		}
		TCanvas *can = new TCanvas("can","histogramy window",600,600);
		
		if(baseline==1){
		       	text.DrawLatex(-100,100,Form("removed baseline %f",mean/adc));
			cout << Form("removed baseline %f",mean/adc) << endl;
		}

		myhist->SetTitle(Form("%i histogram from scintillator",nhist));
		nhist++;
		myhist->Draw();
		gPad->WaitPrimitive();
	}

	return kTRUE;
}
