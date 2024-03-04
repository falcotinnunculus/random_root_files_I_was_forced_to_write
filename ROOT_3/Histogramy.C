void Histogramy1(void){
 
    const Int_t events = 5000; // liczba zdarzeń w histogramie
    
    TCanvas *can = new TCanvas("can", "can", 800, 800); // tworzę canvas

    TH1F *hGauss1 = new TH1F("hGauss1", "hGauss1", 50, -10, 10); // tworzę histogram

    for(Int_t i=0; i<events; i++){ // wypełniam histogram
      hGauss1->Fill(gRandom->Gaus(0, 1));
    }
    
    hGauss1->GetXaxis()->SetTitle("x axis"); // ustawiam atrybuty histogramu
    hGauss1->GetYaxis()->SetTitle("y axis");
    hGauss1->SetLineColor(kRed);
    hGauss1->Draw();

    TH1F *hGauss2 = new TH1F("hGauss2", "hGauss2", 50, -10, 10); // tworzę kolejny histogram

    for(Int_t i=0; i<events; i++){ // wypełniam histogram
      hGauss2->Fill(gRandom->Gaus(0, 3));
    }

    hGauss2->GetXaxis()->SetTitle("x axis"); // ustawiam atrybuty histogramu
    hGauss2->GetYaxis()->SetTitle("y axis");
    hGauss2->SetLineColor(kBlue);
    hGauss2->Draw("same");

    TH1F *hGauss3 = new TH1F("hGauss3", "hGauss3",50, -10, 10); // tworzę kolejny histogram

    for(Int_t i=0; i<events; i++){ // wypełniam histogram
      hGauss3->Fill(gRandom->Gaus(0, 5)); 
    }
    
    hGauss3->GetXaxis()->SetTitle("x axis"); // ustawiam atrybuty histogramu
    hGauss3->GetYaxis()->SetTitle("y axis");
    hGauss3->SetLineColor(kGreen);
    hGauss3->Draw("same");
    
    return;
}


void Histogramy2(void){
 
    const Int_t events = 5000; // liczba zdarzeń w histogramie
    const Int_t nhist = 3; // liczba histogramów do stworzenia
    
    Int_t colors[nhist] = {kRed, kBlue, kGreen}; 
    Double_t sigmas[nhist] = {1, 3, 5};
    TH1F *hGauss[nhist] = {nullptr}; // tablica wskaźników do histogramów
    
    TCanvas *can = new TCanvas("can", "can", 800, 800); // canvas
    
    for(Int_t i=0; i<nhist; i++){ // pętla
        
        hGauss[i] = new TH1F(Form("hGauss%i", i), Form("hGauss%i", i), 50, -10, 10); // tworzę kolejne histogramy...

        for(Int_t j=0; j<events; j++){ // ... wypełniam je ...
          hGauss[i]->Fill(gRandom->Gaus(0, sigmas[i]));
        }    
        
        hGauss[i]->GetXaxis()->SetTitle("x axis"); // ... i ustalam im atrybuty
        hGauss[i]->GetYaxis()->SetTitle("y axis");
        hGauss[i]->SetLineColor(colors[i]);
        if(i==0)
          hGauss[i]->Draw(); // na koniec rysowanie
        else 
            hGauss[i]->Draw("same");
    }
    
    return;
}


// funkcja, która tworzy histogramy wypełnioone rozkładem Gaussa o zadanych parametrach
TH1F* DrawHistogram(Int_t sigma, Int_t events){ 
    
    TH1F* hGauss = new TH1F(Form("hGaussSig%i", sigma), Form("hGaussSig%i", sigma), 50, -10, 10);
        
    for(Int_t j=0; j<events; j++){
      hGauss->Fill(gRandom->Gaus(0, sigma));
    } 
    
    return hGauss;
}


void Histogramy3(void){
    
    const Int_t events = 5000; // liczba zdarzeń w histogramie
    const Int_t nhist = 3; // liczba histogramów
    
    Int_t colors[nhist] = {kRed, kBlue, kGreen};
    Double_t sigmas[nhist] = {1, 3, 5};
    TH1F *hGauss[nhist] = {nullptr}; // tablica wskaźników do histogramów
    
    TCanvas *can = new TCanvas("can", "can", 800, 800); // canvas
    
    for(Int_t i=0; i<nhist; i++){ // pętla
        
        hGauss[i] = DrawHistogram(sigmas[i], events); // wywołuję funkcję tworzącą histogramy...   
        
        hGauss[i]->GetXaxis()->SetTitle("x axis"); //... ustalam atrybuty histogramów
        hGauss[i]->GetYaxis()->SetTitle("y axis");
        hGauss[i]->SetLineColor(colors[i]);
        if(i==0)
          hGauss[i]->Draw(); // na koniec rysowanie
        else 
            hGauss[i]->Draw("same");
    }
    
    return;  
}
