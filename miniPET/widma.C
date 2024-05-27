#define ndet 16
Float_t xscale = 17.5;

void widma2(void){ 

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
TH1F *hQZ1[ndet], *hQZ2[ndet];
TH2F *hQZ12[ndet];
TH1F *hQX1P2[ndet], *hQZ1P2[ndet];
TH2F *hQXPZP[ndet];
TH1F *hQX[ndet], *hQZ[ndet];
TH2F *hQXZ[ndet];

// definicja histogramow
for(Int_t ii=0;ii<ndet ;ii++)
{
	TString name1;
	name1="QX1 for detector";   name1+=ii+1;
	hQX1[ii]=new TH1F(name1,name1,100, 0,4100);
	name1="QX2 for detector";   name1+=ii+1;
	hQX2[ii]=new TH1F(name1,name1,100, 0,4100);
	name1="QZ1 for detector";   name1+=ii+1;
	hQZ1[ii]=new TH1F(name1,name1,100, 0,4100);
	name1="QZ2 for detector";   name1+=ii+1;
	hQZ2[ii]=new TH1F(name1,name1,100, 0,4100);
	name1="QX1+QX2 for detector";   name1+=ii+1;
	hQX1P2[ii]=new TH1F(name1,name1,100, 0,8200);
	name1="QZ1+QZ2 for detector";   name1+=ii+1;
	hQZ1P2[ii]=new TH1F(name1,name1,100, 0,8200);
	name1="X for detector";   name1+=ii+1;
	hQX[ii]=new TH1F(name1,name1,100, -xscale,xscale);
	name1="Z for detector";   name1+=ii+1;
	hQZ[ii]=new TH1F(name1,name1,100, -xscale,xscale);

	name1="QX2 vs QX1 for detector";   name1+=ii+1;
	hQX12[ii]=new TH2F(name1,name1,100, 0,8200, 100, 0,8200);
	name1="QZ2 vs QZ1 for detector";   name1+=ii+1;
	hQZ12[ii]=new TH2F(name1,name1,100, 0,8200, 100, 0,8200);
	name1="QX1+QX2 vs QZ1+QZ2 for detector";   name1+=ii+1;
	hQXPZP[ii]=new TH2F(name1,name1,100, 0,8200, 100, 0,8200);
	name1="X vs Z for detector";   name1+=ii+1;
	hQXZ[ii]=new TH2F(name1,name1,100, -xscale,xscale, 100, -xscale,xscale);
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
// sprawdz czy sygnaly przekraczaja prog
		if((QX1[ii]>Qthr) && (QX2[ii]>Qthr) && (QZ1[ii]>Qthr) && (QZ2[ii]>Qthr)) 
		{  
			mult++;
// tutaj można wstawić wypelnianie roznych histogramow			

		//}
        //else{
            //continue;
            //break;
        //}
		hQX1[ii]->Fill(QX1[ii]); // wypelnienie histogramu hQX1 dla detektora ii
		hQX2[ii]->Fill(QX2[ii]); // wypelnienie histogramu hQX2
		hQX12[ii]->Fill(QX1[ii],QX2[ii]); // wypelnienie histogramu hQX12
		hQZ1[ii]->Fill(QZ1[ii]); // wypelnienie histogramu hQZ1 dla detektora ii
		hQZ2[ii]->Fill(QZ2[ii]); // wypelnienie histogramu hQZ2
		hQZ12[ii]->Fill(QZ1[ii],QZ2[ii]); // wypelnienie histogramu hQZ12
		hQX1P2[ii]->Fill(QX1[ii]+QX2[ii]); 
		hQZ1P2[ii]->Fill(QZ1[ii]+QZ2[ii]);
		hQXPZP[ii]->Fill(QX1[ii]+QX2[ii],QZ1[ii]+QZ2[ii]);
        X[ii] = ((Float_t)QX2[ii]-(Float_t)QX1[ii])/((Float_t)QX1[ii]+(Float_t)QX2[ii]);
		hQX[ii]->Fill(X[ii]*xscale);
        Z[ii] = ((Float_t)QZ2[ii]-(Float_t)QZ1[ii])/((Float_t)QZ1[ii]+(Float_t)QZ2[ii]);
		hQZ[ii]->Fill(Z[ii]*xscale);
		hQXZ[ii]->Fill(X[ii]*xscale,Z[ii]*xscale);
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
	//gPad->SetLogy();
	a->Update();
}
a->WaitPrimitive(); // program czeka w tym miejscu na klikniecie myszka na kanwe

TCanvas* b = new TCanvas("b", "Histograms", 200,100,800, 800); //utworz kanwe
b->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	b->cd(ii+1);
	hQX2[ii]->Draw();
	//gPad->SetLogy();
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

TCanvas* a2 = new TCanvas("a2", "Histograms", 200,100,800, 800); //utworz kanwe
a2->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	a2->cd(ii+1);
	hQZ1[ii]->Draw();
	//gPad->SetLogy();
	a2->Update();
}
a2->WaitPrimitive(); // program czeka w tym miejscu na klikniecie myszka na kanwe

TCanvas* b2 = new TCanvas("b2", "Histograms", 200,100,800, 800); //utworz kanwe
b2->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	b2->cd(ii+1);
	hQZ2[ii]->Draw();
	//gPad->SetLogy();
	b2->Update();
}
b2->WaitPrimitive();

TCanvas* c2 = new TCanvas("c2", "Histograms", 200,160, 800, 800); //utworz kanwe
c2->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	c2->cd(ii+1);
	hQZ12[ii]->Draw("colz");
	c2->Update();
}
c2->WaitPrimitive(); 


TCanvas* a3 = new TCanvas("a3", "Histograms", 200,100,800, 800); //utworz kanwe
a3->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	a3->cd(ii+1);
	hQX1P2[ii]->Draw();
	//gPad->SetLogy();
	a3->Update();
}
a3->WaitPrimitive(); // program czeka w tym miejscu na klikniecie myszka na kanwe

TCanvas* b3 = new TCanvas("b3", "Histograms", 200,100,800, 800); //utworz kanwe
b3->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	b3->cd(ii+1);
	hQZ1P2[ii]->Draw();
	//gPad->SetLogy();
	b3->Update();
}
b3->WaitPrimitive();

TCanvas* c3 = new TCanvas("c3", "Histograms", 200,160, 800, 800); //utworz kanwe
c3->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	c3->cd(ii+1);
	hQXPZP[ii]->Draw("colz");
	c3->Update();
}
c3->WaitPrimitive();


TCanvas* a4 = new TCanvas("a4", "Histograms", 200,100,800, 800); //utworz kanwe
a4->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	a4->cd(ii+1);
	hQX[ii]->Draw();
	////gPad->SetLogy();
	a4->Update();
}
a4->WaitPrimitive(); // program czeka w tym miejscu na klikniecie myszka na kanwe

TCanvas* b4 = new TCanvas("b4", "Histograms", 200,100,800, 800); //utworz kanwe
b4->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	b4->cd(ii+1);
	hQZ[ii]->Draw();
	////gPad->SetLogy();
	b4->Update();
}
b4->WaitPrimitive();

TCanvas* c4 = new TCanvas("c4", "Histograms", 200,160, 800, 800); //utworz kanwe
c4->Divide(4,4);	
for(Int_t ii=0;ii<ndet ;ii++)
{
	c4->cd(ii+1);
	hQXZ[ii]->Draw("colz");
	c4->Update();
}
c4->WaitPrimitive();

TCanvas* m = new TCanvas("m", "Histograms", 200,200, 500, 500); //utworz kanwe
m->Divide(1,1);	
m->cd(1);
hmult->Draw();
m->Update();	

return;
}
