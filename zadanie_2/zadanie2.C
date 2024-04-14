#include<iostream>
#include<fstream>
#include<string>
using namespace std;

Bool_t zadanie2(string name="wave_0.dat", int baseline=0){

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

	Int_t maxhist = 5000;

	for(int i=0; i<nbins; i++) lines[i] = 0;
	while ((!mydat.eof()) && nhist < maxhist){
		peak = 0;
		threshold = 0;
		t0 = 0;
		integral = 0;
		
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

		nhist++;
		cout << peak << "\t" << t0 << "\t" << integral <<endl;
		tree->Fill();
	}

	tree->Write();
	file->Close();

	return kTRUE;
}

Bool_t Rysuj(){

	TFile *file = new TFile("widmo.root","UPDATE");
	
	if(!file->IsOpen()){
		cout << "plik nie działa" <<endl;
		return kFALSE;
	}

	TTree *tree = (TTree*)file->Get("events");
	Int_t ntree = tree->GetEntries();

	Float_t peak = 0;
	Float_t t0 = 0;
	Float_t integral = 0;

	tree->SetBranchAddress("peak",&peak);
	tree->SetBranchAddress("t0",&t0);
	tree->SetBranchAddress("integral",&integral);

	TCanvas *can = new TCanvas("can","histogramy window",1500,1000);
	can->Divide(3,2);

	TH1F* h_peak = new TH1F("h_peak", "h_peak", 100, -3000, 0);
	h_peak->GetXaxis()->SetTitle("peak amplitude");
	h_peak->GetYaxis()->SetTitle("count");
	TH1F* h_t0 = new TH1F("h_t0", "h_t0", 60, 230, 290);
	h_t0->GetXaxis()->SetTitle("t0");
	h_t0->GetYaxis()->SetTitle("count");
	TH1F* h_int = new TH1F("h_int", "h_int", 100, -200000, 0);
	h_int->GetXaxis()->SetTitle("integrated charge");
	h_int->GetYaxis()->SetTitle("count");
	TH2F* h_peakint = new TH2F("h_peakint", "h_peakint", 100, -3000, 0, 100, -200000, 0);
	h_peakint->GetXaxis()->SetTitle("peak amplitude");
	h_peakint->GetYaxis()->SetTitle("integrated charge");
	TH2F* h_intt0 = new TH2F("h_intt0", "h_intt0", 100, -200000, 0, 60, 230, 290);
	h_intt0->GetXaxis()->SetTitle("integrated charge");
	h_intt0->GetYaxis()->SetTitle("t0");

	for(Int_t i=0; i<ntree; i++){
		tree->GetEntry(i);
		h_peak->Fill(peak);
		h_t0->Fill(t0);
		h_int->Fill(integral);
		h_peakint->Fill(peak,integral);
		h_intt0->Fill(integral,t0);
	}

	can->cd(1);
	h_peak->Draw();
	can->cd(2);
	h_t0->Draw();
	can->cd(3);
	h_int->Draw();
	can->cd(4);
	h_peakint->Draw();
	can->cd(5);
	h_intt0->Draw();

	return kTRUE;
}
