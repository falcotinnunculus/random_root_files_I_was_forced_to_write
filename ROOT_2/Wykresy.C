#include<iostream>
#include<fstream>
using namespace std;

Double_t energie[4] = {12,20,40,65};
//TString modes[4] = {"APC","PC","PC","PC"};

TGraph* Rysuj(Double_t energia = 12){

	const int n=91;
	TFile *file = new TFile("mojwykres.root","UPDATE");

        if(!file->IsOpen()){
                cout << "plik nie działa" <<endl;
                //return kFALSE;
		return nullptr;
        }

	TString nazwa = Form("pp0%.3fang.L01",energia);

	fstream dane;
	dane.open(nazwa, ios::in);

	if(!dane.is_open()){
		cout << "Nie mozna otworzyc pliku tekstowego " << nazwa << endl;
 //Sprawdzanie czy plik został poprawnie otwarty
		//return kFALSE;
		return nullptr;
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

//	graph->Draw(mode);
	graph->Write();
	file->Close();


	return graph;
	

}

Bool_t Wykresy(){
	
	TMultiGraph *mg = new TMultiGraph();

	TLegend *leg = new TLegend(0,0,0.5,0.5,"legemda");

	for(Int_t i=0; i<4; i++){
		TGraph *graph=Rysuj(energie[i]);
		mg->Add(graph);
		leg->AddEntry(graph,to_string(energie[i]).c_str());
	}

	mg->Draw("ALP");
	leg->Draw();

	return kTRUE;


}
