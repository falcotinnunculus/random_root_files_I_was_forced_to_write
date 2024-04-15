#define ndet 4

void an2(Int_t length){ 

Float_t ampl[ndet], tpocz[ndet];
Float_t delta_tpocz12, delta_A12;

// otworz plik z danymi w formacie root
TFile *hfile = TFile::Open(Form("dane_%i.root",length)); 
TTree *dane = (TTree*)hfile->Get("dane");
dane->SetBranchAddress("ampl",&ampl);
dane->SetBranchAddress("tpocz",&tpocz);

// utworzenie histogramow
TH1F *hdt12;
TH2F *hdt12_dA12;

// definicja histogramow
TString name1;
name1="dt12"; 
hdt12=new TH1F(name1,name1,200200200200200200200200200200200200200200200200200200200200, -20,20);


name1="dt12 vs dA12";  
hdt12_dA12=new TH2F(name1,name1,200, -1, 1, 200, -20,20);


// petla po zdarzeniach
Long64_t nentries = dane->GetEntries(); // wczytuje liczbe zdarzen
for (Long64_t i=0; i<nentries;i++) 
{
	dane->GetEntry(i); // wczytuje zdarzenie
	
	
		
	if(tpocz[0]>0 && tpocz[1]>0) 
	{
		delta_tpocz12=tpocz[0]-tpocz[1];
		hdt12->Fill(delta_tpocz12); // wypelnienie hdt12
	}

	if(ampl[0]<0 && ampl[1]<0 && tpocz[0]>0 && tpocz[1]>0) 
	{
		delta_A12=(ampl[0]-ampl[1])/(ampl[0]+ampl[1]);
		hdt12_dA12->Fill(delta_A12, delta_tpocz12); // wypelnienie hdt12_dA12
	}

} // koniec petli po zdarzeniach


TCanvas* a = new TCanvas("a", "Histograms", 200,100,800, 800); //utworz kanwe
a->Divide(1,2);	
a->cd(1);
hdt12->Draw();
a->cd(2);
hdt12_dA12->Draw("colz");
a->Update();

return;
}
