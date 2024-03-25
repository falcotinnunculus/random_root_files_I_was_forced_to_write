#include<iostream>
using namespace std;

Bool_t GenerateProblem(void){

	Int_t nev = 1E6; // liczba zdarzeń do wygenerowania
	Double_t ex, ey, ez, er, etheta, ephi;
	Double_t prob;
	TVector3 start(1,1,1), versor(1,1,1);
	Float_t energy;
	Float_t E1 = 1.1732;
	Float_t E2 = 1.3325;
	Float_t thr = 0.9;

	fstream txtfile("emission.txt", ios::out);

	TFile *rootfile = new TFile("emission.root","RECREATE"); // otwieram plik ROOT do zapisu
	TTree *tree = new TTree("emission","emission"); // tworzę drzewo
	tree->Branch("start",&start);
	tree->Branch("versor",&versor);
	tree->Branch("energy",&energy);

	for(Int_t i=0; i<nev; i++){ // pętla wypełniająca drzewo i plik tekstowy
		if(i%(nev/50)==0) cout << "Processing event " << i << endl;
		
		ex = gRandom->Gaus(0,1);
		ey = gRandom->Gaus(0,1);
		ez = 0;
		ephi = gRandom->Uniform(-M_PI,M_PI);
		etheta = acos(gRandom->Uniform(-1,1));
		er = 1;
		prob = gRandom->Rndm();
		if(prob<thr) energy = E1; else energy = E2;

		start.SetX(ex);
		start.SetY(ey);
		start.SetZ(ez);
		versor.SetMag(er);
		versor.SetTheta(etheta);
		versor.SetPhi(ephi);

		txtfile << er << "\t" << etheta << "\t" << ephi << endl;
		tree->Fill();
	}

	tree->Write();
	//rootfile->Close();
	txtfile.close();


	return kTRUE;

}

Bool_t ReadProblem(void){

	TFile *rootfile = new TFile("emission.root","READ"); // otwieram plik ROOT do odczytu
	TTree *tree = (TTree*)rootfile->Get("emission");

	TCanvas *can = new TCanvas("can","can",1000,1000); //tworzę canvas. Uwaga na kolejność: canvas, cd, histogram
	can->Divide(2,2);

	Int_t nev = 1E6; // liczba zdarzeń do wygenerowania
	Double_t ex, ey, ez, er, etheta, ephi;
	Double_t prob;
	TVector3* start = new TVector3;
	TVector3* versor = new TVector3;
	Float_t energy;
	
	TH1F* h1 = new TH1F("h1", "h1", 100, 1, 2); // tworzę histogram
	TH2F* h2 = new TH2F("h2", "h2", 100, -5, 5, 100, -5, 5); // tworzę histogram
	TH2F* h3 = new TH2F("h3", "h3", 100, -1.1, 1.1, 100, -1.1, 1.1); // tworzę histogram
	TH2F* h4 = new TH2F("h4", "h4", 100, -0.1, M_PI+0.1, 100, -M_PI-0.1, M_PI+0.1); // tworzę histogram
	
	Int_t ntree = tree->GetEntries();
	tree->SetBranchAddress("start",&start);
	tree->SetBranchAddress("versor",&versor);
	tree->SetBranchAddress("energy",&energy);
	
	for(Int_t i=0; i<ntree; i++){ // iteruję po zdarzeniach w drzewie i wypełniam histogram
	    tree->GetEntry(i);
	    h1->Fill(energy);
	    h2->Fill(start->X(),start->Y());
	    h3->Fill(versor->X(),versor->Y());
	    h4->Fill(versor->Theta(),versor->Phi());

	}
	
	can->cd(1); // rysowanie histogramu
	h1->Draw();
	can->cd(2); // rysowanie histogramu
	h2->Draw("colz");
	can->cd(3); // rysowanie histogramu
	h3->Draw("colz");
	can->cd(4); // rysowanie histogramu
	h4->Draw("colz");


	return kTRUE;

}
