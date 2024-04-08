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
		ampl[ii] = 0;
		for(Int_t i=0; i<NS; i++){
			ampl[ii] = TMath::Min(ampl[ii],kanal_sygnal[ii][i]);
		}

	}
	return 1;
}


// funkcja wyznaczajaca czas poczatku impulsu tpocz
int poczatek()
{
    Double_t thr = -50;
	for(Int_t ii=0;ii<NCH ;ii++)
	{
		for(Int_t i=0; i<NS; i++){
            if(kanal_sygnal[ii][i]<thr){
			    tpocz[ii] = i;
                return 1;
            }
		}

	}
	return 0;
}


void pp(void)
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
TString filename = "dane.root";
hfile = TFile::Open(filename,"RECREATE");
TTree *dane = new TTree("dane","dane");
dane->Branch("ampl",ampl,"ampl[4]/F");
dane->Branch("tpocz",tpocz,"tpocz[4]/F");


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
			czas[i]=i*DT;
			}
			else end_file=1;
		}
		if(iz%100==0) cout << kanal_sygnal[ii][0] << " ";
	}

	bsl(); // wyznacz linie bazowe
	if(iz%100==0) cout << "bsl0: " << baseline[0] << " ";

// odejmij linie bazowe od sygralow
	for(Int_t ii=0;ii<NCH ;ii++)
	{
		for (Int_t i=0;i<NS; i++)
		{
			kanal_sygnal[ii][i]=kanal_sygnal[ii][i]-baseline[ii];
		}
	}
	amplituda(); // wyznacz amplitude
	if(iz%100==0) cout << "amp0: " << ampl[0] << " ";

    poczatek(); // wyznacz poczatek impulsu
	if(iz%100==0) cout << "tp0: " << tpocz[0] << " ";

	dane->Fill();	// zapisz zdarzenie w pliku dane.root


	iz++;
}while(end_file==0);


// zamknij pliki z impulsami
for(Int_t k=0;k<NCH ;k++)  fp[k].close();
printf("data file closed \n");

// zapisz drzewo na pliku dyskowym
dane->Write();

}
