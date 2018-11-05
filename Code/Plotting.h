#include "CommonHeader.h"

void Plot1DHistos(std::vector<TH1D*> vTH1D, std::vector<TString> vLegEntries,
                  std::vector<TString> vLeglORp, TString orientation,
                  std::vector<Int_t> vColor, std::vector<Int_t> vMarker,
                  std::vector<TString> vAxisTitles, TString RangeStr){

  int nHistos = vTH1D.size();
  int nLegEntries = vLegEntries.size();

  if(nLegEntries < nHistos - 1 && nHistos != 1){

      std::cerr << "Less LegEntries then Histograms" << '\n';
      exit(2);
  }

  TCanvas* c1 = NULL;

  if(orientation.contains("horizontal") ){
    c1 = new TCanvas("c1", "",1000, 1200);
  }
  else if(orientation.contains("vertical") ){
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
      leg->AddEntry(vTH1D, vLegEntries, vLeglORp);
    }
  }
  else{
    leg->SetHeader(vLegEntries[nLegEntries-1]);
    for (int i = 0; i < nLegEntries; i++){
      leg->AddEntry(vTH1D, vLegEntries, vLeglORp);
    }
  }

  vTH1D[0]->SetXTitle(vAxisTitles[0]);
  vTH1D[0]->SetYTitle(vAxisTitles[1]);
  if(vAxisTitles.size() == 2){
    vTH1D[0]->SetZTitle("");
  }
  else{
    vTH1D[0]->SetZTitle(vAxisTitles[2]);
  }

  if(orientation.contains("horizontal") ){
    for (int i = 0; i < nHistos; i++) {
      SetHistoSettings(vTH1D[i], 1.2, 1., 40, 43, (Int_t) vColor[i], vMarker[i], 3);
    }
  }
  else if(orientation.contains("vertical") ){
    for (int i = 0; i < nHistos; i++) {
      SetHistoSettings(vTH1D[i], 1.2, 1., 40, 43, (Int_t) vColor[i], vMarker[i], 3);
    }
  }
  else{
    for (int i = 0; i < nHistos; i++) {
      SetHistoSettings(vTH1D[i], 1.2, 1., 40, 43, (Int_t) vColor[i], vMarker[i], 3);
    }
  }

  vTH1D[i]->Draw("AXIS");
  for (int i = 0; i < nHistos; i++) {
    vTH1D[i]->DrawCopy("SAME" + vLeglORp);
  }
  leg->Draw("SAME");
  DrawLabelALICE(0.6, 0.9, 0.02, 40, RangeStr);

  c1->Update();
  c1->SaveAs("Bild.png");
  c1->Clear();

  delete leg;
  delete c1;


}
