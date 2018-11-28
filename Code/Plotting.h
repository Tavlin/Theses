#include "CommonHeader.h"

Int_t GetNiceColor(Int_t i)
{//
  Int_t niceColors[] = {kRed+1, kGreen-3, kBlue+1, kViolet-4, kOrange-3, kBlack, kCyan-2, kGray+2, kOrange+2, kBlack, kYellow+3};
  return niceColors[i%11];
}

Int_t GetNiceFilledMarker(Int_t i){
  Int_t niceMarkers[] = {20 , 21, 22, 33, 34};
  return niceColors[i%5];
}

Int_t GetNiceEmptyMarker(Int_t i){
  Int_t niceMarkers[] = {24 , 25, 26, 27, 28};
  return niceColors[i%5];
}

void Plot1DHistos(TObjArray* (TH1D*) histArray, TString* LegEntries,
                  TString* LeglORp, TString option,
                  TString* AxisTitles, TString RangeStr,
                  TString safePath){

  int nHistos = histArray.GetEntries();
  int nLegEntries = sizeof(LegEntries);
  nLegEntries /= sizeof(TString);

  if(nLegEntries < nHistos - 1 && nHistos != 1){

      std::cerr << "Less LegEntries then Histograms" << '\n';
      exit(2);
  }

  TCanvas* c1 = NULL;

  if(option.contains("horizontal") || option.contains("HORIZONTAL") || option.contains("Horizontal")){
    c1 = new TCanvas("c1", "",1000, 1200);
  }
  else if(option.contains("vertical") || option.contains("VERTICAL") || option.contains("Vertical")){
    c1 = new TCanvas("c1", "",1200, 1000);
  }
  else{
    c1 = new TCanvas("c1", "",1200, 1200);
  }
  c1->cd();


  TLegend* leg = new TLegend(0.6, 0.3, 0.9, 0.6);
  SetLegendSettings(leg, 40, 43);
  if(nLegEntries == nHistos){
    for (int i = 0; i < nLegEntries; i++) {
      leg->AddEntry((TH1D*) histArray[i], LegEntries[i], LeglORp);
    }
  }
  else{
    leg->SetHeader(LegEntries[nLegEntries-1]);
    for (int i = 0; i < nLegEntries; i++){
      leg->AddEntry((TH1D*) histArray[i], LegEntries[i], LeglORp);
    }
  }

  (TH1D*) histArray[0]->SetXTitle(AxisTitles[0]);
  (TH1D*) histArray[0]->SetYTitle(AxisTitles[1]);
  if(AxisTitles.size() == 2){
    (TH1D*) histArray[0]->SetZTitle("");
  }
  else{
    (TH1D*) histArray[0]->SetZTitle(AxisTitles[2]);
  }

  if(option.contains("horizontal") || option.contains("HORIZONTAL") || option.contains("Horizontal")){
    for (int i = 0; i < nHistos; i++) {
      SetHistoSettings((TH1D*) histArray[i], 1.2, 1., 40, 43, (Int_t) GetNiceColor(i), GetNiceFilledMarker(i), 3);
    }
  }
  else if(option.contains("vertical") || option.contains("VERTICAL") || option.contains("Vertical")){
    for (int i = 0; i < nHistos; i++) {
      SetHistoSettings((TH1D*) histArray[i], 1.2, 1., 40, 43, (Int_t) GetNiceColor(i), GetNiceFilledMarker(i), 3);
    }
  }
  else{
    for (int i = 0; i < nHistos; i++) {
      SetHistoSettings((TH1D*) histArray[i], 1.2, 1., 40, 43, (Int_t) GetNiceColor(i), GetNiceFilledMarker(i), 3);
    }
  }

  (TH1D*) histArray[i]->Draw("AXIS");
  for (int i = 0; i < nHistos; i++) {
    (TH1D*) histArray[i]->DrawCopy("SAME" + LeglORp);
  }
  leg->Draw("SAME");
  DrawLabelALICE(0.6, 0.9, 0.02, 40, RangeStr);

  c1->Update();
  c1->SaveAs("Bild.png");
  // c1->SaveAs(safePath + ".png");
  c1->Clear();

  delete leg;
  delete c1;


}
