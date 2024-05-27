#include<TMath>

#define ndet 16
Float_t xscale = 17.5;

Float_t R = 89;
Float_t getx(Float_t X, Int_t det){
    Float_t alpha = 0.3927*(det);
    return R*TMath::Sin(alpha) + X*TMath::Cos(alpha);
}
Float_t gety(Float_t X, Int_t det){
    Float_t alpha = 0.3927*(det);
    return R*TMath::Cos(alpha) - X*TMath::Sin(alpha);
}

void lor(void){ 

Int_t Qthr=400; // ustawienie progu dyskryminacji impulsow
Int_t QX1[ndet], QX2[ndet], QZ1[ndet], QZ2[ndet], QX[ndet], QZ[ndet]; 
Float_t X[ndet], Z[ndet];
Float_t x[2], y[2];

// otworz plik z danymi w formacie root
TFile *hfile = TFile::Open("QDC.root"); 
TTree *tree = (TTree*)hfile->Get("tree");
tree->SetBranchAddress("QX1",&QX1);
tree->SetBranchAddress("QX2",&QX2);
tree->SetBranchAddress("QZ1",&QZ1);
tree->SetBranchAddress("QZ2",&QZ2);

// utworzenie histogramow
TH1F *hQX[ndet], *hQZ[ndet];
TH2F *hQXZ[ndet];

// definicja histogramow
for(Int_t ii=0;ii<ndet ;ii++)
{
	TString name1;
	name1="X for detector";   name1+=ii+1;
	hQX[ii]=new TH1F(name1,name1,100, -xscale,xscale);
	name1="Z for detector";   name1+=ii+1;
	hQZ[ii]=new TH1F(name1,name1,100, -xscale,xscale);

	name1="X vs Z for detector";   name1+=ii+1;
	hQXZ[ii]=new TH2F(name1,name1,100, -xscale,xscale, 100, -xscale,xscale);
}


TString name2;
name2="x";
TH1F *hx=new TH1F(name2,name2,100, -R*1.2,R*1.2);
name2="y";
TH1F *hy=new TH1F(name2,name2,100, -R*1.2,R*1.2);
name2="x vs y";
TH2F *hxy=new TH2F(name2,name2,200,-100,100,200,-100,100);

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
// tutaj można wstawić wypelnianie roznych histogramow			

		//}
        //else{
            //continue;
            //break;
        //}
        X[ii] = ((Float_t)QX2[ii]-(Float_t)QX1[ii])/((Float_t)QX1[ii]+(Float_t)QX2[ii])*xscale;
		hQX[ii]->Fill(X[ii]);
        Z[ii] = ((Float_t)QZ2[ii]-(Float_t)QZ1[ii])/((Float_t)QZ1[ii]+(Float_t)QZ2[ii])*xscale;
		hQZ[ii]->Fill(Z[ii]);
		hQXZ[ii]->Fill(X[ii],Z[ii]);
       
        x[mult] = getx(X[ii],ii);
        y[mult] = gety(X[ii],ii);

		mult++;
        if(mult==2){
            hx->Fill(x[0]);
            hy->Fill(y[0]);
            hxy->Fill(x[0],y[0]);
            hx->Fill(x[1]);
            hy->Fill(y[1]);
            hxy->Fill(x[1],y[1]);
            break;
        }
        
        }
	}

} // koniec petli po zdarzeniach



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

TCanvas* d4 = new TCanvas("d4", "Histograms", 200,160, 800, 800); //utworz kanwe
d4->Divide(2,2);	
d4->cd(1);
hx->Draw();
d4->cd(2);
hy->Draw();
d4->cd(3);
hxy->Draw();
d4->Update();
d4->WaitPrimitive();

return;
}
