#define ndet 4
#define npom 7

Float_t x[npom] = {1,10,20,23,30,40,46};
Float_t L = 48;
Float_t xerr[npom] = {1,1,1,1,1,1,1};
Float_t dt12[npom], dA12[npom], sigmadt12[npom];
Float_t dt12err[npom], dA12err[npom], sigmadt12err[npom];

Int_t iter = 0;

void an2(Int_t length, Bool_t save = 0){ 

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
hdt12=new TH1F(name1,name1,100, -10,10);

name1="dA12";
hdA12=new TH1F(name1,name1,200, -1, 1);

name1="dt12 vs dA12";  
hdt12_dA12=new TH2F(name1,name1,200, -1, 1, 100, -10,10);

name1="A1 vs A2";  
hA1_A2=new TH2F(name1,name1,200, -4000, 0, 200, -4000,0);

TF1 *funs1 = new TF1("funs","gaus",-20,20);
TF1 *funs2 = new TF1("funs","gaus",-1,1);


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
        hdA12->Fill(delta_A12);
		hdt12_dA12->Fill(delta_A12, delta_tpocz12); // wypelnienie hdt12_dA12
	}

	if(ampl[0]<0 && ampl[1]<0) 
	{
        hA1_A2->Fill(ampl[0],ampl[1]);
    }

} // koniec petli po zdarzeniach


TCanvas* a = new TCanvas("a", Form("Histograms length=%icm",length), 200,100,1500, 1000); //utworz kanwe
a->Divide(2,2);	
a->cd(1);
hdt12->Draw();
hdt12->Fit(funs1,"S,R");
a->cd(2);
hdt12_dA12->Draw("colz");
a->cd(3);
hdA12->Draw();
hdA12->Fit(funs2,"S,R");
a->cd(4);
hA1_A2->Draw("colz");
a->Update();

Float_t mean1 = funs1->GetParameter(1);
Float_t meanerr1 = funs1->GetParError(1);
Float_t sigma1 = funs1->GetParameter(2);
Float_t sigmaerr1 = funs1->GetParError(2);
Float_t mean2 = funs2->GetParameter(1);
Float_t meanerr2 = funs2->GetParError(1);

//Float_t output[6] = {mean1, meanerr1, sigma1, sigmaerr1, mean2, meanerr2};

//Float_t dt12[npom], dA12[npom], sigmadt12[npom];
//Float_t dt12err[npom], dA12err[npom], sigmadt12err[npom];
if(save){
    dt12[iter] = mean1;
    dt12err[iter] = meanerr1;
    sigmadt12[iter] = sigma1;
    sigmadt12err[iter] = sigmaerr1;
    dA12[iter] = mean2;
    dA12err[iter] = meanerr2;
}
//hfile->Close();

return ;
}

void loopn2(){
    Float_t*  input;

    Float_t mean1[npom];
    Float_t meanerr1[npom];
    Float_t sigma1[npom];
    Float_t sigmaerr1[npom];
    Float_t mean2[npom];
    Float_t meanerr2[npom];

    for(Int_t i = 0; i<npom; i++){
    iter = i;
    an2(x[i],true);
        

    }

    TGraphErrors *graph1 = new TGraphErrors(npom,x,dt12,xerr,dt12err);
    TGraphErrors *graph2 = new TGraphErrors(npom,x,dA12,xerr,dA12err);
    TGraphErrors *graph3 = new TGraphErrors(npom,x,sigmadt12,xerr,sigmadt12err);
    
    graph1->GetXaxis()->SetTitle("x");
    graph1->GetYaxis()->SetTitle("dt12 [ns]");
    graph1->SetTitle("dt12(x)");
    graph2->GetXaxis()->SetTitle("x");
    graph2->GetYaxis()->SetTitle("dA12 [normalized]");
    graph2->SetTitle("dA12(x)");
    graph3->GetXaxis()->SetTitle("x");
    graph3->GetYaxis()->SetTitle("sigmadt12 [ns]");
    graph3->SetTitle("sigmadt12(x)");

    TCanvas *can = new TCanvas("gc","gc",1200,800);
    can->Divide(2,2);

    TF1 *funl = new TF1("funl","pol1",0,50);

    can->cd(1);
    graph1->Fit(funl,"S,R");
    graph1->Draw();
    can->cd(2);
    graph2->Draw();
    can->cd(3);
    graph3->Draw();
    can->Update();

    Float_t A = funl->GetParameter(1);
    Float_t B = funl->GetParameter(0);
    cout << "c = " << -2/A << "cm/ns" << endl;
    cout << "c = " << L/B << "cm/ns" << endl;



return;
}
