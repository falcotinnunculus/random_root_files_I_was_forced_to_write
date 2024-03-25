#include <iostream>
#include <fstream>
#include "TTimeStamp.h"
using namespace std;

//funkcja generująca "zdarzenia" i zapisujące ja do pliku
//tekstowego i do drzewa

Bool_t GenerateEvents(void){
    
 Int_t nev = 5E7; // liczba zdarzeń do wygenerowania
 Double_t px, py;
 
 fstream txtfile("events.txt", ios::out); // otwieram plik tekstowy do zapisu
 
 TFile *rootfile = new TFile("events.root","RECREATE"); // otwieram plik ROOT do zapisu
 TTree *tree = new TTree("events","events"); // tworzę drzewo
 tree->Branch("px",&px); // tworzę gałęzie
 tree->Branch("py",&py);
 
 for(Int_t i=0; i<nev; i++){ // pętla wypełniająca drzewo i plik tekstowy
     if(i%(nev/50)==0) cout << "Processing event " << i << endl;
     gRandom->Rannor(px,py); // funkcja zwraca 2 liczby wylosowane zgodnie z rozkładem Gauss: mean=0, sigma=1
     txtfile << px << "\t" << py << endl;
     tree->Fill();
 }
    
    tree->Write(); // zapisuje drzewo do pliku ROOT
    rootfile->Close(); // zamykanie pliku ROOT i pliku tekstowego
    txtfile.close();
    
 return kTRUE;   
}

//funkcja odczytująca drzewo oraz plik tekstowy
//odczytane dane są przedstawiane na histogramie
//porównanie czasu operacji na drzewie i na pliku tekstowym

Bool_t TreesExample(void){
 
//----- drzewo
 cout << "Reading tree and drawing px*px + py*py" << endl;
 
 TTimeStamp tree_start; // zaczynam liczenie czasu
 gSystem->Exec("date");
    
 TFile *rootfile = new TFile("events.root","READ"); // otwieram plik ROOT do odczytu
 TTree *tree = (TTree*)rootfile->Get("events"); // pobieram drzewo z pliku ROOT
 
//rożne metody czytania drzew:
//aby przetestować daną metodę odkomentuj blok 
//kodu i zakomentuj pozostałe dwie metody 
 
//----- metoda 1 
 
// TCanvas *can = new TCanvas("can","can",1000,600); //tworzę canvas. Uwaga na kolejność: canvas, cd, histogram
// can->Divide(2,1);
// 
// TH1F* h_tree = new TH1F("h_tree", "h_tree", 100, 0, 30); // tworzę histogram
// 
// Int_t ntree = tree->GetEntries();
// Double_t px_tree, py_tree;
// tree->SetBranchAddress("px",&px_tree); // "łączę się" z gałeziami z drzewa
// tree->SetBranchAddress("py",&py_tree);
// 
// for(Int_t i=0; i<ntree; i++){ // iteruję po zdarzeniach w drzewie i wypełniam histogram
//     tree->GetEntry(i);
//     h_tree->Fill(px_tree*px_tree + py_tree*py_tree);
// }
// 
// can->cd(1); // rysowanie histogramu
// h_tree->Draw();

//----- koniec metody 1
 
//----- metoda 2   
/*
 TCanvas *can = new TCanvas("can","can",1000,600); // tworzę canvas
 can->Divide(2,1);
 can->cd(1); 
 
 tree->Draw("px*px + py*py>>htmp(100,0,30)","",""); // Wypełniam tymczasowy histogram danymi z drzewa. Uwaga na kolejność: canvavas, cd, histogram
 TH1F* h_tree = (TH1F*)gROOT->FindObjectAny("htmp"); // tworzę wskaźnik do histogramu tymczasowego

 //can->cd(1);  //nie tutaj!
 h_tree->Draw();
*/
//----- koniec metody 2
 
//----- metoda 3 

 TCanvas *can = new TCanvas("can","can",1000,600); //Tworzę canvas. Uwaga na kolejność: canvas, cd, histogram
 can->Divide(2,1); 
 can->cd(1);
 tree->Draw("px*px+py*py","",""); //Wypełniam domyślny histogram wprost z drzewa. Najprostsza metoda, kiedy wskaźnik jest niepotrzebny

//----- koniec metody 3

//rootfile->Close(); //Ważne - zamykanie pliku z drzewem przy rysowaniu!
 
 TTimeStamp tree_stop;
 
 cout << "This took " << tree_stop.GetSec()-tree_start.GetSec() << " seconds\n\n" << endl; // kończę liczyć czas
 
//-----plik tekstowy
 cout << "Reading text file and drawing px*px + py*py" << endl; 
 
 TTimeStamp txt_start; // znów zaczynam liczyć czas
 gSystem->Exec("date");
 
 fstream txtfile("events.txt",ios::in); // otwieram plik tekstowy
 Int_t nev = 5E7; 
 Double_t px,py;
 
 TH1F *h_txt = new TH1F("h_txt","h_txt",100,0,30); // tworzę histogram
 
 for(Int_t i=0; i<nev; i++){ // czytanie pliku i wypełnianie histogramu
    txtfile >> px >> py;
    h_txt->Fill( px*px + py*py);
 }
 
 can->cd(2); // rysowanie histogramu
 h_txt->Draw();
 
 txtfile.close(); // zamykanie pliku tekstowego
 
 TTimeStamp txt_stop; // kończę liczyć czas
 
 cout << "This took " << txt_stop.GetSec()-txt_start.GetSec() << " seconds\n\n" << endl;

 return kTRUE;
}

