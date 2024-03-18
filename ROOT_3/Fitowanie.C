#include<iostream>
#include<fstream>
using namespace std;


Bool_t Fitowanie(string filename = "widmo.root", string histname = "hWidmo") {
	cout << filename;
	TFile *file = new TFile("widmo.root", "READ"); //TODO naprawić żeby czytało filename
	if(!file->IsOpen()){
		cout << "Could not open the file!" << endl;
		return kFALSE;
	}

	TH1F *hist = (TH1F*)file->Get("hWidmo");
	hist->Draw();

	TF1 *fun = new TF1("fun","expo(0)+gaus(2)",175,350);
//	if(x[0]>175 && x[0]<350)
//		TF1::RejectPoint();
	fun-> SetParameters(1.23039e+01,-3.42148e-02,1.20100e+03,2.56884e+02,1.25688e+01);

	TFitResultPtr results = hist->Fit(fun,"S,R");

	Double_t fwhm = fun->GetParameter(4) / fun->GetParameter(3);
	TLatex text;
	text.DrawLatex(400,1000,Form("fwhm=%f%%",fwhm*100));

	fstream myfile;
	myfile.open("fitowanieresults.txt",ios::out);
	for (int i=0; i<5; i++) myfile << fun->GetParameter(i) << " ";
	myfile << endl;
	for (int i=0; i<5; i++) myfile << fun->GetParError(i) << " ";
	myfile << endl;
	myfile << results->Chi2() << endl;
	myfile << results->Ndf() << endl;
	myfile << results->Edm() << endl;
	myfile << fwhm << endl;
	myfile.close();

	return kTRUE;

}
