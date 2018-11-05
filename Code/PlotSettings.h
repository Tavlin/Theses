#ifndef plotsettings
#define plotsettings
/**
 * Functions to set Standard settings on the histograms
 * They probably all need some work done on them.
 */

/**
 * Sets the Standard Canvas Settings up.
 * @param cCanv pointer to the TCanvas you want set up.
 */
void SetCanvasSettings(TCanvas *cCanv, int tm, int bm, int lf, int rm,
                      int tickx, int ticky, int tickz, int Logx, int Logy,
                      int Logz){
  gStyle->SetOptStat(kFALSE); // das hier macht dies box rechts oben weg
  TGaxis::SetMaxDigits(3);    // maximale Anzahl an Digits an den Achsen
  cCanv->SetTopMargin(tm);
  cCanv->SetBottomMargin(bm);
  cCanv->SetRightMargin(rm);
  cCanv->SetLeftMargin(lm);
  cCanv->SetTickx(tickx);
  cCanv->SetTicky(ticky);
  cCanv->SetTicky(tickz);
  cCanv->SetLogx(Logx);
  cCanv->SetLogy(Logy);
  cCanv->SetLogz(Logz);
}

/**
 * Sets the Standard TH1 Settings up.
 * @param histo    pointer to TH1
 * @param XOffset  X-Title Offset
 * @param YOffset  Y-Title Offset
 * @param textSize Text size
 */
void SetHistoSettings(TH1* histo, Double_t XOffset = 1.2, Double_t YOffset = 1.,
                      Double_t textSize = 35, int TextFont = 43,
                      Int_t color = (Int_t) kBlack, int mStyle = 20,
                      Double_t lw = 3){

  histo->SetStats(0);
  histo->GetXaxis()->SetTitleOffset(XOffset);
  histo->GetYaxis()->SetTitleOffset(YOffset);
  histo->GetXaxis()->SetTitleSize(textSize);
  histo->GetYaxis()->SetTitleSize(textSize);
  histo->GetXaxis()->SetLabelSize(textSize);
  histo->GetYaxis()->SetLabelSize(textSize);
  histo->GetXaxis()->SetLabelFont(TextFont);
  histo->GetYaxis()->SetLabelFont(TextFont);
  histo->GetYaxis()->SetTitleFont(TextFont);
  histo->GetXaxis()->SetTitleFont(TextFont);

  histo->SetTitle("");
  histo->Sumw2();
  histo->SetMarkerStyle(mStyle);
  histo->SetMarkerSize(1.5);
  histo->SetLineWidth(lw);
  histo->SetLineColor(color);
  histo->SetMarkerColor(color);
}



void SetHistoSettings2(TH1* histo, Double_t XOffset = 1.2,
                               Double_t YOffset = 1., Double_t ZOffset = 1.,
                               Double_t textSize = 35, int TextFont = 43,
                               Int_t color = (Int_t) kBlack, int mStyle = 20,
                               Double_t lw = 3){
  histo->SetStats(0);
  histo->SetTitle("");
  histo->GetXaxis()->SetTitleOffset(XOffset);
  histo->GetYaxis()->SetTitleOffset(YOffset);
  histo->GetXaxis()->SetTitleSize(textSize);
  histo->GetYaxis()->SetTitleSize(textSize);
  histo->GetXaxis()->SetLabelSize(textSize);
  histo->GetYaxis()->SetLabelSize(textSize);
  histo->GetXaxis()->SetLabelFont(TextFont);
  histo->GetYaxis()->SetLabelFont(TextFont);
  histo->GetYaxis()->SetTitleFont(TextFont);
  histo->GetXaxis()->SetTitleFont(TextFont);

  histo->GetZaxis()->SetTitleOffset(ZOffset);
  histo->GetZaxis()->SetLabelSize(textSize);
  histo->GetZaxis()->SetLabelFont(TextFont);
  histo->GetZaxis()->SetTitleFont(TextFont);

  histo->Sumw2();
}


void SetLegendSettings(TLegend* leg, Double_t textSize = 35, int textFont = 43){
  leg->SetTextFont(textFont);
  leg->SetTextSize(textSize);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetLineWidth(0);
  leg->SetLineColor(0);
  leg->SetMargin(0.15);
  leg->SetBorderSize(0);
}

void SetLatexSettings(TLatex* tex, Double_t textSize = 35, int textFont = 43){
  tex->SetTextSize(textSize);
  tex->SetTextFont(textFont);
  }

#endif
