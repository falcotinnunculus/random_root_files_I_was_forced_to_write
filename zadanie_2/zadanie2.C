#include<iostream>
#include<fstream>
#include<string>
using namespace std;

Bool_t zadanie2(string name="wave_0.dat", int baseline=1){

	gROOT->Reset();

	TFile *file = new TFile("widmo.root","UPDATE");
	
	if(!file->IsOpen()){
		cout << "plik nie działa" <<endl;
		return kFALSE;
	}

	TTree *tree = new TTree("events","events");
	
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
	Float_t peak = 0;
	Float_t threshold = 0;
	Float_t t0 = 0;
	Float_t integral = 0;

	tree->Branch("peak",&peak);
	tree->Branch("t0",&t0);
	tree->Branch("integral",&integral);


	TH1F *myhist = new TH1F("histo","histogram from scintillator",nbins,0,nbins);
	myhist->GetXaxis()->SetTitle("channle number");
	myhist->GetYaxis()->SetTitle("amplitude [mV]");
	TLatex text;

	Int_t maxhist = 5000;

	Float_t peaks[maxhist];
	Float_t t0s[maxhist];
	Float_t integrals[maxhist];

	for(int i=0; i<nbins; i++) lines[i] = 0;
	while ((!mydat.eof()) && nhist < maxhist){
		peak = 0;
		threshold = 0;
		t0 = 0;
		integral = 0;
		
		cout << Form("printing histogram %i", nhist) << endl; 
		for(int i=0; i<nbins; i++){
			mydat.read((char*)&tempx, sizeof(float));
			lines[i] = tempx;
		}

		mean = 0;
		if(baseline==1){
			for(int i=0; i<mean_denom; i++){
				mean+= lines[i]/mean_denom;
			}
		}

		for(int i=0; i<nbins; i++){
			if(lines[i] - mean < peak){
				peak = lines[i]-mean;
			}
		}

		threshold = peak * 0.15;

		for(int i=0; i<nbins; i++){
			if(lines[i] - mean < threshold){
				integral += lines[i]-mean;
				if(t0 == 0) t0 = i;	
			}
		}

		cout << "peak\tthres\tintegr\tt0" <<endl;
		cout << peak << "\t" << threshold << "\t" << integral << "\t" << t0 <<endl;
		peaks[nhist] = peak;
		t0s[nhist] = t0;
		integrals[nhist] = integral;

		//for(int i=0; i<nbins; i++){
		//	myhist->SetBinContent(i+1,(lines[i]-mean)/adc);
		//}
		//TCanvas *can = new TCanvas("can","histogramy window",600,600);
		
		//if(baseline==1){
		//       	text.DrawLatex(-100,100,Form("removed baseline %f",mean/adc));
		//	cout << Form("removed baseline %f",mean/adc) << endl;
		//}

		//myhist->SetTitle(Form("%i histogram from scintillator",nhist));
		nhist++;
		//myhist->Draw();
		//gPad->WaitPrimitive();
		tree->Fill();
	}

	tree->Write();
	file->Close();

	return kTRUE;
}
