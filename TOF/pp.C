#include <fstream>
#include <iostream>
#include <algorithm>
#include "TMath.h"

#define NS 1024 // liczba probek
#define NCHMAX 8 // liczba kanalow digitizera
#define NCH 4  // liczba wykorzystanych kanalow digitizera
#define NADC 4096 // zakres ADC (2^12)
#define DT 0.2 // okres probkowania w ns (0.2 ns odpowiada czestotliwosci probkowania 5 GHz)
#define NS_BSL 200 // liczba probek do wyznaczania linii bazowej

Double_t czas[NS];
Double_t kanal_sygnal[NCH][NS]; // tablica z sygnalami 
Float_t ampl[NCH], tpocz[NCH], baseline[NCH]; // tablice z amplitudami, czasami poczatku impulsu i liniami bazowymi

ifstream fp[NCH];


// funkcja do wyznaczenia linii bazowej impulsu
int bsl()
{
	
	for(Int_t ii=0;ii<NCH ;ii++)
	{
		baseline[ii] = 0;
		for(Int_t i=0; i<NS_BSL; i++){
			baseline[ii] += (kanal_sygnal[ii][i]/NS_BSL);
		}
	}
	return 1;

}


// funkcja do wyznaczenia amplitudy impulsu
int amplituda()
{
	for(Int_t ii=0;ii<NCH ;ii++)
	{
		ampl[ii] = 2137;
		for(Int_t i=0; i<NS; i++){
			ampl[ii] = TMath::Min(ampl[ii],kanal_sygnal[ii][i]);
		}

	}
	return 1;
}


// funkcja wyznaczajaca czas poczatku impulsu tpocz
int poczatek()
{
    Double_t thr = -20;
	for(Int_t ii=0;ii<NCH ;ii++)
	{
		for(Int_t i=0; i<NS; i++){
            if(kanal_sygnal[ii][i]<thr){
			    tpocz[ii] = czas[i];
                cout <<"ii tpocz:" <<ii << " " << tpocz[ii] << endl;
                break;
            }
		}

	}
	return 0;
}


int poczatek2()
{
    Double_t thr1 = -20;
    Double_t thr2 = -47;
    Int_t isthr; // sprawdza ktory prog jest teraz wyznaczany
    Double_t t1, t2;
	for(Int_t ii=0;ii<NCH ;ii++)
	{
        isthr = 0;
		for(Int_t i=0; i<NS; i++){
            if(kanal_sygnal[ii][i]<thr1 && isthr == 0){
			    t1 = czas[i];
                isthr = 1;
            }
            if(kanal_sygnal[ii][i]<thr2 && isthr == 1){
			    t2 = czas[i];
                isthr = 2;
                //break;
            }
		}

        tpocz[ii] = (thr2 * t1 - thr1 * t2)/(thr2 - thr1);

        //cout <<"ii t1 t2 tpocz:" <<ii << " " << " " << t1 << " " << t2 << " " << tpocz[ii] << endl;
	}
	return 0;
}

void pp2(Int_t length, Int_t debug = 0)
{ 
Int_t end_file=0, iz;
Double_t sy;
char odp;
TString nazwa1;
float par1[NCH], par2[NCH], par3[NCH];

char *nazwa_pliku[NCHMAX] =
{
"./data/wave_0.txt",
"./data/wave_1.txt",
"./data/wave_2.txt",
"./data/wave_3.txt",
"./data/wave_4.txt",
"./data/wave_5.txt",
"./data/wave_6.txt",
"./data/wave_7.txt"
};

cout << "otwieranie plików" << endl;
// otworz pliki z danymi
for(Int_t ii=0;ii<NCH ;ii++)
{
	fp[ii].open(nazwa_pliku[ii], ios::in);
    cout << "otwarto plik " << ii << endl;
}

cout << "tworzenie drzewa" << endl;
// utworz drzewo
TString filename = Form("dane_%i.root",length);
hfile = TFile::Open(filename,"RECREATE");
TTree *dane = new TTree("dane","dane");
dane->Branch("ampl",ampl,"ampl[4]/F");
dane->Branch("tpocz",tpocz,"tpocz[4]/F");

//tworzenie histgramów
TH1F *histb[NCH];  
TH1F *hista[NCH];  
TH1F *histt[NCH]; 
TH1F *hists[NCH];  
for(Int_t ii=0;ii<NCH ;ii++){
    histb[ii] = new TH1F(Form("hist_bsl %i",ii),Form("hist_bsl %i",ii) ,NADC,0,NADC);
    hista[ii] = new TH1F(Form("hist_amp %i",ii),Form("hist_amp %i",ii) ,NADC/10,-NADC,0);
    histt[ii] = new TH1F(Form("hist_tp  %i",ii),Form("hist_tp  %i",ii) ,NS,0,NS*DT);
    hists[ii] = new TH1F(Form("hist_szu %i",ii),Form("hist_szu %i",ii) ,NADC/25,-NADC/100,NADC/100);
}

TCanvas *can = new TCanvas("can","can", 1800, 1200);
can->Divide(4,4);

TF1 *funs = new TF1("funs","gaus",-NADC/100,NADC/100);


cout << "pętla po zdarzeniach" << endl;

// Petla po zdarzeniach
iz=0;
do
{
if(iz%100==0) cout << endl << "zdarzenie " << iz << ": ";

// czytaj impulsy
  Double_t binary_number;
	for(Int_t ii=0;ii<NCH ;ii++)
	{
		for (Int_t i=0;i<NS; i++)
		{
			if(!fp[ii].eof())
			{
			fp[ii] >> binary_number;
			kanal_sygnal[ii][i]=binary_number;
			czas[i]=(float) i*DT;
            //cout << czas[i] << "\t";
			}
			else end_file=1;
		}
		if(iz%100==0) cout << kanal_sygnal[ii][0] << " ";
	}

	bsl(); // wyznacz linie bazowe
	if(iz%100==0) cout << "bsl2: " << baseline[debug] << " ";

// odejmij linie bazowe od sygralow
	for(Int_t ii=0;ii<NCH ;ii++)
	{
		for (Int_t i=0;i<NS; i++)
		{
			kanal_sygnal[ii][i]=kanal_sygnal[ii][i]-baseline[ii];
		}
	}
	amplituda(); // wyznacz amplitude
	if(iz%100==0) cout << "amp2: " << ampl[debug] << " ";

    poczatek2(); // wyznacz poczatek impulsu
	if(iz%100==0) cout << "tp2: " << tpocz[debug] << " ";
    //if(iz%1000==0) for(Int_t i=0; i<NS; i++) cout << kanal_sygnal[debug][i] << "\n";


	for(Int_t ii=0;ii<NCH ;ii++){
        histb[ii]->Fill(baseline[ii]);
        hista[ii]->Fill(ampl[ii]);
        //cout << tpocz[ii] << " ";
        histt[ii]->Fill(tpocz[ii]);
        for(Int_t i=0; i<NS_BSL; i++) hists[ii]->Fill(kanal_sygnal[ii][i]);
    }





    dane->Fill();	// zapisz zdarzenie w pliku dane.root

	iz++;
}while(end_file==0);

Int_t length = iz;
cout << length << endl;


for(Int_t ii=0; ii<NCH; ii++){
    cout << "histogram " << ii << endl;
    can->cd(4*ii+1);
    histb[ii]->Draw();
    can->cd(4*ii+2);
    hista[ii]->Draw();
    can->cd(4*ii+3);
    histt[ii]->Draw();
    can->cd(4*ii+4);
    hists[ii]->Draw();
    hists[ii]->Fit(funs,"S,R");
    can->Update();
}

gPad->WaitPrimitive();
// zamknij pliki z impulsami
for(Int_t k=0;k<NCH ;k++)  fp[k].close();
printf("data file closed \n");

// zapisz drzewo na pliku dyskowym
dane->Write();
hfile->Close();
//return kTRUE;

}
