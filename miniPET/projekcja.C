#include<TMath>

#define ndet 16
#define nbin 200
Float_t xscale = 17.5;

Float_t R = 89;
Float_t getx(Float_t X, Int_t det)
{
    Float_t alpha = 0.3927 * (det);
    return R * TMath::Sin(alpha) + X * TMath::Cos(alpha);
}
Float_t gety(Float_t X, Int_t det)
{
    Float_t alpha = 0.3927 * (det);
    return R * TMath::Cos(alpha) - X * TMath::Sin(alpha);
}

float π = TMath::Pi;

void projekcja(void)
{

    Int_t Qthr = 400; // ustawienie progu dyskryminacji impulsow
    Int_t QX1[ndet], QX2[ndet], QZ1[ndet], QZ2[ndet], QX[ndet], QZ[ndet];
    Float_t X[ndet], Z[ndet];
    Float_t x[2], y[2];
    Float_t xx, yy, x0, y0;
    Float_t A, B, s, phi, ds, dphi;
    Int_t zlicz;

    Double_t pro[nbin]; // projekcja przy ustalonym kącie (uwaga: na początku skryptu umieszczono: #define nbin 200)
    Double_t P_re[nbin], P_im[nbin]; // jednowymiarowa transformata Fouriera projekcji (część rzeczywista i urojona)
    Double_t Q_re[nbin], Q_im[nbin]; // przefiltrowana transformata Fouriera projekcji (część rzeczywista i urojona)
    Double_t q_re[nbin], q_im[nbin]; // odwrotna transformata Fouriera (część rzeczywista i urojona)

    // otworz plik z danymi w formacie root
    TFile *hfile = TFile::Open("QDC.root");
    TTree *tree = (TTree*)hfile->Get("tree");
    tree->SetBranchAddress("QX1", &QX1);
    tree->SetBranchAddress("QX2", &QX2);
    tree->SetBranchAddress("QZ1", &QZ1);
    tree->SetBranchAddress("QZ2", &QZ2);

    // utworzenie histogramow
    TH1F *hQX[ndet], *hQZ[ndet];
    TH2F *hQXZ[ndet];

    // definicja histogramow
    for (Int_t ii = 0; ii < ndet ; ii++)
    {
        TString name1;
        name1 = "X for detector";
        name1 += ii + 1;
        hQX[ii] = new TH1F(name1, name1, 100, -xscale, xscale);
        name1 = "Z for detector";
        name1 += ii + 1;
        hQZ[ii] = new TH1F(name1, name1, 100, -xscale, xscale);

        name1 = "X vs Z for detector";
        name1 += ii + 1;
        hQXZ[ii] = new TH2F(name1, name1, 100, -xscale, xscale, 100, -xscale, xscale);
    }


    TString name2;
    name2 = "x";
    TH1F *hx = new TH1F(name2, name2, 100, -R * 1.2, R * 1.2);
    name2 = "y";
    TH1F *hy = new TH1F(name2, name2, 100, -R * 1.2, R * 1.2);
    name2 = "old x vs y";
    TH2F *holdxy = new TH2F(name2, name2, 200, -100, 100, 200, -100, 100);
    name2 = "s vs phi";
    TH2F *hsphi = new TH2F(name2, name2, 200, -100, 100, 200, -2, 2);
    name2 = "x vs y";
    TH2F *hxy = new TH2F(name2, name2, 200, -100, 100, 200, -100, 100);
    name2 = "s vs phi filtr";
    TH2F *hsphi_filtr = new TH2F(name2, name2, 200, -100, 100, 200, -2, 2);

    // petla po zdarzeniach
    Long64_t nentries = tree->GetEntries(); // wczytuje liczbe zdarzen
    for (Long64_t i = 0; i < nentries; i++)
    {
        tree->GetEntry(i); // wczytuje zdarzenie

        Int_t mult = 0; // liczba detektorow z ktorych sygnaly przekroczyly odpowiednie progi
        for (Int_t ii = 0; ii < ndet ; ii++) // petla po detektorach
        {
            // sprawdz czy sygnaly przekraczaja prog
            if ((QX1[ii] > Qthr) && (QX2[ii] > Qthr) && (QZ1[ii] > Qthr) && (QZ2[ii] > Qthr))
            {
                // tutaj można wstawić wypelnianie roznych histogramow

                X[ii] = ((Float_t)QX2[ii] - (Float_t)QX1[ii]) / ((Float_t)QX1[ii] + (Float_t)QX2[ii]) * xscale;
                hQX[ii]->Fill(X[ii]);
                Z[ii] = ((Float_t)QZ2[ii] - (Float_t)QZ1[ii]) / ((Float_t)QZ1[ii] + (Float_t)QZ2[ii]) * xscale;
                hQZ[ii]->Fill(Z[ii]);
                hQXZ[ii]->Fill(X[ii], Z[ii]);

                x[mult] = getx(X[ii], ii);
                y[mult] = gety(X[ii], ii);

                mult++;
                if (mult == 2)
                {
                    hx->Fill(x[0]);
                    hy->Fill(y[0]);
                    holdxy->Fill(x[0], y[0]);
                    hx->Fill(x[1]);
                    hy->Fill(y[1]);
                    holdxy->Fill(x[1], y[1]);

                    //for(int iii=0; iii<400; iii++)
                    //{
                    //    xx=x[1]+(float)iii/400.0*(x[2]-x[1]); yy=y[1]+(float)iii/400.0*(y[2]-y[1]);
                    //    holdxy->Fill(xx,yy);
                    //}

                    A = (y[1] - y[0]) / (x[1] - x[0]);
                    B = (y[0] * x[1] - y[1] * x[0]) / (x[1] - x[0]);
                    s = (B) / (TMath::Sqrt(A * A + 1));
                    phi = TMath::ATan(-1 / A);

                    hsphi->Fill(s, phi);

                    break;
                } //koniec ifa po mult
            } //koniec ifa po thr
        } //koniec pętli po detektorach

    } // koniec petli po zdarzeniach

    //projekcja wsteczna
    dphi = 4. / 200.;
    phi = -2 - dphi / 2.;
    ds = 200. / 200.;

    for (int varphi = 0; varphi < nbin; varphi++) //pętla po φ
    {
        phi += dphi;
        s = -100 - ds / 2.;
        for (int σ = 0; σ < nbin; σ++) //pętla po s
        {
            s += ds;
            zlicz = hsphi->GetBinContent(σ +1, varphi + 1);
            if (zlicz == 0) continue;
            x0 = s * TMath::Cos(phi);
            y0 = s * TMath::Sin(phi);
            A = -x0 / y0;
            B = y0 - A * x0;
            if (TMath::Abs(A) <= 1) for (float ι = 0; ι < nbin; ι++)
                {
                    xx = -100 + 1. / 2. + ι;
                    yy = A * xx + B;
                    hxy->Fill(xx, yy, zlicz);
                }
            if (TMath::Abs(A) > 1) for (float ι = 0; ι < nbin; ι++)
                {
                    yy = -100 + 1. / 2. + ι;
                    xx = (yy - B) / A;
                    hxy->Fill(xx, yy, zlicz);
                }
        } //koniec pętli po s
    } //koniec pętli po φ


    // filtracja sinogramu


    for (int iphi = 0; iphi < nbin; iphi++) // pętla po przedziałach kata
    {
        for (int is = 0; is < nbin; is++) pro[is] = hsphi->GetBinContent(is + 1, iphi + 1); // wczytaj projekcję dla danego kata
        // transformata Fouriera projekcji
        for (int m = 0; m < nbin; m++)
        {
            //
            // tutaj wstawić odpowiedni kod wyliczający P_re[m ], P_im[m ]

            P_re[m] = 0;
            P_im[m] = 0;
            for (int k = 0; k < nbin; k++)
            {
                P_re[m] += pro[k] * TMath::Cos(-2 * π*(float)k * (float)m / (float)nbin);
                P_im[m] -= pro[k] * TMath::Sin(-2 * π*(float)k * (float)m / (float)nbin);

            }
            cout << "iphi m Pre[m] Pim[m]" << iphi << " " << m << " " << P_re[m] << " " << P_im[m] << endl;
        } //koniec pierwszej pętli po ω
        // filtr
        for (int m = 0; m < nbin; m++)
        {
            if (m < nbin / 2)
            {
                Q_re[m] = P_re[m] * (double)m;
                Q_im[m] = P_im[m] * (double)m;
            }
            else
            {
                Q_re[m] = 0.;
                Q_im[m] = 0.;
            }
            cout << "iphi m Qre[m] Qim[m]" << iphi << " " << m << " " << Q_re[m] << " " << Q_im[m] << endl;
        }
        // odwrotna transformata Fouriera
        for (int k = 0; k < nbin; k++)
        {
            //
            // tutaj wstawić odpowiedni kod wyliczający q_re[k ], q_im[k ]
            q_re[k] = 0;
            q_im[k] = 0;
            for (int m = 0; m < nbin; m++)
            {
                q_re[k] += Q_re[m] * TMath::Cos(2 * π*(float)k * (float)m / (float)nbin) + Q_im[m] * TMath::Sin(2 * π*(float)k * (float)m / (float)nbin);
                q_im[k] += -Q_re[m] * TMath::Sin(2 * π*(float)k * (float)m / (float)nbin) + Q_im[m] * TMath::Cos(2 * π*(float)k * (float)m / (float)nbin);
            }
            cout << "iphi k qre[k] qim[k]" << iphi << " " << k << " " << q_re[k] << " " << q_im[k] << endl;
            hsphi_filtr->SetBinContent(k + 1, iphi + 1, q_re[k]); // wypełnianie przefiltrowanego sinogramu
        } //koniec pętli po k
    } //koniec pętli po iφ

    TCanvas* d4 = new TCanvas("d4", "Histograms", 200, 160, 1200, 1200); //utworz kanwe
    TCanvas* e4 = new TCanvas("e4", "Histograms_del", 200, 160, 1200, 1200); //utworz kanwe
    d4->Divide(2, 1);
    //    d4->cd(1);
    //    hx->Draw();
    //    d4->cd(2);
    //    hy->Draw();
    d4->cd(1);
    //    holdxy->Draw();
    //    d4->cd(4);
    hsphi->Draw("colz");
    d4->cd(2);
    hsphi_filtr->Draw("colz");
    d4->Update();
    d4->WaitPrimitive();
    e4->Divide(1, 1);
    e4->cd(1);
    hxy->Draw("colz");
    e4->WaitPrimitive();

    return;
}
