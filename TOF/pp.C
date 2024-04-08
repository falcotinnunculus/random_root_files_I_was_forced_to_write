#include <fstream>
#include <iostream>

#define NS 1024 // liczba probek
#define NCHMAX 8 // liczba kanalow digitizera
#define NCH 4  // liczba wykorzystanych kanalow digitizera
#define NADC 4096 // zakres ADC (2^12)
#define DT 0.2 // okres probkowania w ns (0.2 ns odpowiada czestotliwosci probkowania 5 GHz)

Double_t czas[NS];
Double_t kanal_sygnal[NCH][NS]; // tablica z sygnalami 
Float_t ampl[NCH], tpocz[NCH], baseline[NCH]; // tablice z amplitudami, czasami poczatku impulsu i liniami bazowymi

ifstream fp[NCH];


// funkcja do wyznaczebnia linii bazowej impulsu
int bsl()
{
	for(Int_t ii=0;ii<NCH ;ii++)
	{
		baseline[ii]=100.;
	}
	return 1;
}


// funkcja do wyznaczenia amplitudy impulsu
int amplituda()
{
	for(Int_t ii=0;ii<NCH ;ii++)
	{
		ampl[ii]=100.; 
	}
	return 1;
}


// funkcja wyznaczajaca czas poczatku impulsu tpocz
int poczatek()
{
	for(Int_t ii=0;ii<NCH ;ii++)
	{
		tpocz[ii]=100.;

	}
	return 1;
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
"./data/wave_0.dat",
"./data/wave_1.dat",
"./data/wave_2.dat",
"./data/wave_3.dat",
"./data/wave_4.dat",
"./data/wave_5.dat",
"./data/wave_6.dat",
"./data/wave_7.dat"
};


// otworz pliki z danymi
for(Int_t ii=0;ii<NCH ;ii++)
{
	fp[ii].open(nazwa_pliku[ii], ios::binary); 
}


// utworz drzewo
TString filename = "dane.root";
hfile = TFile::Open(filename,"RECREATE");
TTree *dane = new TTree("dane","dane");
dane->Branch("ampl",ampl,"ampl[4]/F");
dane->Branch("tpocz",tpocz,"tpocz[4]/F");




// Petla po zdarzeniach
iz=0;
do
{

// czytaj impulsy
  float binary_number;
	for(Int_t ii=0;ii<NCH ;ii++)
	{
		for (Int_t i=0;i<NS; i++)
		{
			if(!fp[ii].eof())
			{
			fp[ii].read((char*)&binary_number,sizeof(binary_number));
			kanal_sygnal[ii][i]=binary_number;
			czas[i]=i*DT;
			}
			else end_file=1;
		}
	}

	bsl(); // wyznacz linie bazowe

// odejmij linie bazowe od sygralow
	for(Int_t ii=0;ii<NCH ;ii++)
	{
		for (Int_t i=0;i<NS; i++)
		{
			kanal_sygnal[ii][i]=kanal_sygnal[ii][i]-baseline[ii];
		}
	}
	amplituda(); // wyznacz amplitude
        poczatek(); // wyznacz poczatek impulsu

	dane->Fill();	// zapisz zdarzenie w pliku dane.root


	iz++;
}while(end_file==0);


// zamknij pliki z impulsami
for(Int_t k=0;k<NCH ;k++)  fp[k].close();
printf("data file closed \n");

// zapisz drzewo na pliku dyskowym
dane->Write();

}
