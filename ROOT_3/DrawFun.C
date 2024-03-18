#include <iostream>
#include <fstream>
using namespace std;

void DrawFun(void){
	TCanvas *can = new TCanvas("can","can",800,800);
	can->Divide(2,2);
	
	TF1 *fun1 = new TF1("fun1","gaus",-10,10);
	fun1->SetParameter(0,2);
	fun1->SetParameter(1,0);
	fun1->SetParameter(2,1);
	can->cd(1);
	fun1->Draw();
	
	TF1 *fun2 = new TF1("fun2","gaus(0)+gaus(3)+gaus(6)+gaus(9)",-10,10);
	fun2->SetParameters(2,0,0.1,2,1,0.1,2,2,0.1,2,3);
	can->cd(2);
	fun2->Draw();
	
	//TF1 *fun3 = new TF1("fun3","sin(x)/x",0,15);
	//can->cd(3);
	//fun3->Draw();
	
	TF1 *fun3 = new TF1("fun3","pol2",-5,5);
	fun3->SetParameters(0,0,1);
	can->cd(3);
	fun3->Draw();
	//TF1 *fun4 = new TF1("fun4","[0]+cos(x*[1])",0,15);
	//fun4->SetParameters(2,0.7);
	//can->cd(4);
	//fun4->Draw();
	
	TF1 *fun5 = new TF1("fun5","pol17",-8,8);
	//fun5->SetParameters(0,1,0,-1./6,0,1./120,0,-1./5040,0,1./362880,0);
	fun5->SetParameter(1,1);
	fun5->SetParameter(3,-1./6);
	fun5->SetParameter(5,1./120);	
	fun5->SetParameter(7,-1./5040);	
	fun5->SetParameter(9,1./362880);	
	fun5->SetParameter(11,-1./39916800);	
	fun5->SetParameter(13,1./6227020800);	
	fun5->SetParameter(15,-1./1307674380000);	
	fun5->SetParameter(17,1./355687428096000);	

	can->cd(4);
	fun5->Draw();

	return;
}
