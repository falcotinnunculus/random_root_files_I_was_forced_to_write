#include<iostream>
#include<fstream>
using namespace std;

Double_t myfunc(Double_t *x, Double_t *par){ //TODO to na pewno da sie zrobic latwiej nie wierze

	if(x[0]<175 || x[0]>350) TF1::RejectPoint();

	return TMath::Exp(par[0]+par[1]*x[0])+par[2]*TMath::Exp(-0.5*TMath::Power(((x[0]-par[3])/par[4]),2));

}

Bool_t Fitowanie(string filename = "widmo.root", string histname = "hWidmo") {
	cout << filename;
	TFile *file = new TFile("widmo.root", "READ"); //TODO naprawić żeby czytało filename
	if(!file->IsOpen()){
		cout << "Could not open the file!" << endl;
		return kFALSE;
	}

	TH1F *hist = (TH1F*)file->Get("hWidmo");
	hist->Draw();

	TF1 *fun = new TF1("fun",myfunc,175,350,5);
//	if(x[0]>175 && x[0]<350)
//		TF1::RejectPoint();
	fun-> SetParameters(1.23039e+01,-3.42148e-02,1.20100e+03,2.56884e+02,1.25688e+01);

	TFitResultPtr results = hist->Fit(fun,"S,R");

	return kTRUE;

}
