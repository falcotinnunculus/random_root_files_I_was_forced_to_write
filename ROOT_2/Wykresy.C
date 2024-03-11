#include<iostream>
#include<fstream>
using namespace std;



Bool_t Wykresy(Double_t energia){

	const int n=91;
	TFile *file = new TFile("mojwykres.root","UPDATE");

        if(!file->IsOpen()){
                cout << "plik nie działa" <<endl;
                return kFALSE;
        }

	TString nazwa = Form("pp0%.3fang.L01",energia);

	fstream dane;
	dane.open(nazwa, ios::in);

	if(!dane.is_open()){
		cout << "Nie mozna otworzyc pliku tekstowego " << nazwa << endl;
 //Sprawdzanie czy plik został poprawnie otwarty
		return kFALSE;
	}

	Double_t katy[n], pczynny[n], xd;
	TString naglowek;

	for(Int_t i=0; i<5; i++) {naglowek.ReadLine(dane);}

	for(Int_t j=0; j<n; j++) {dane >> katy[j] >> pczynny[j] >> xd >> xd;}

	TGraph *graph = new TGraph(n,katy,pczynny);

	graph->SetMarkerStyle(8);
	graph->SetMarkerColor(kRed);
	graph->SetMarkerSize(0.3);
	//graph->SetLineColor(kBlue);
	//graph->SetLineWidth();

	graph->Draw("AP");
	graph->Write();
	file->Close();


	return kTRUE;
	


}
