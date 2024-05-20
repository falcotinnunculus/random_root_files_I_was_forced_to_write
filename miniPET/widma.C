#define ndet 16

void widma(void){ 

Int_t Qthr=400; // ustawienie progu dyskryminacji impulsow
Int_t QX1[ndet], QX2[ndet], QZ1[ndet], QZ2[ndet], QX[ndet], QZ[ndet]; 
Float_t X[ndet], Z[ndet];

// otworz plik z danymi w formacie root
TFile *hfile = TFile::Open("QDC.root"); 
TTree *tree = (TTree*)hfile->Get("tree");
tree->SetBranchAddress("QX1",&QX1);
tree->SetBranchAddress("QX2",&QX2);
tree->SetBranchAddress("QZ1",&QZ1);
tree->SetBranchAddress("QZ2",&QZ2);

// utworzenie histogramow
TH1F *hQX1[ndet], *hQX2[ndet];
TH2F *hQX12[ndet];

// definicja histogramow
for(Int_t ii=0;ii<ndet ;ii++)
{
	TString name1;
	name1="QX1 for detector";   name1+=ii+1;
	hQX1[ii]=new TH1F(name1,name1,100, 0,4100);
	name1="QX2 for detector";   name1+=ii+1;
	hQX2[ii]=new TH1F(name1,name1,100, 0,4100);

	name1="QX2 vs QX1 for detector";   name1+=ii+1;
	hQX12[ii]=new TH2F(name1,name1,100, 0,8200, 100, 0,8200);
}
TH1* hmult = new TH1F("hmult", "hmult", 16, 0, 16);


// petla po zdarzeniach
Long64_t nentries = tree->GetEntries(); // wczytuje liczbe zdarzen
for (Long64_t i=0; i<nentries;i++) 
{
	tree->GetEntry(i); // wczytuje zdarzenie
	
	Int_t mult =0; // liczba detektorow z ktorych sygnaly przekroczyly odpowiednie progi
	for(Int_t ii=0;ii<ndet ;ii++) // petla po detektorach
	{		
		hQX1[ii]->Fill(QX1[ii]); // wypelnienie histogramu hQX1 dla detektora ii
		hQX2[ii]->Fill(QX2[ii]); // wypelnienie histogramu hQX2
		hQX12[ii]->Fill(QX1[ii],QX2[ii]); // wypelnienie histogramu hQX12
// sprawdz czy sygnaly przekraczaja prog
		if((QX1[ii]>Qthr) && (QX2[ii]>Qthr) && (QZ1[ii]>Qthr) && (QZ2[ii]>Qthr)) 
		{  
			mult++;
// tutaj można wstawić wypelnianie roznych histogramow			

		}
	}
	hmult->Fill(mult); // wypelnienie histogramu z krotnoscia odpowiedzi detektorow

} // koniec petli po zdarzeniach


TCanvas* a = new TCanvas("a", "Histograms", 200,100,800, 800); //utworz kanwe
a->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	a->cd(ii+1);
	hQX1[ii]->Draw();
	gPad->SetLogy();
	a->Update();
}
a->WaitPrimitive(); // program czeka w tym miejscu na klikniecie myszka na kanwe

TCanvas* b = new TCanvas("b", "Histograms", 200,100,800, 800); //utworz kanwe
b->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	b->cd(ii+1);
	hQX2[ii]->Draw();
	gPad->SetLogy();
	b->Update();
}
b->WaitPrimitive();

TCanvas* c = new TCanvas("c", "Histograms", 200,160, 800, 800); //utworz kanwe
c->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	c->cd(ii+1);
	hQX12[ii]->Draw("colz");
	c->Update();
}
c->WaitPrimitive(); 

TCanvas* m = new TCanvas("m", "Histograms", 200,200, 500, 500); //utworz kanwe
m->Divide(1,1);	
m->cd(1);
hmult->Draw();
m->Update();	

return;
}
