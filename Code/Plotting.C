#include "Plotting_Patrick.h"
#include "CommonHeader.h"

// Start of the Main
void Plotting(std::string current_path){

  //////////////////////////////////////////////////////////////////////////////
  // setting up the canvas to draw on. Will later be changed for the chi2 pic
  TCanvas* c1 = new TCanvas("c1","",1540,1417);
  c1->cd();
  c1->SetTopMargin(0.04);
  c1->SetBottomMargin(0.08);
  c1->SetRightMargin(0.10);
  c1->SetLeftMargin(0.08);
  c1->SetTicky();
  c1->SetTickx();
  c1->SetLogz(1);

  //////////////////////////////////////////////////////////////////////////////
  // setting up the canvas to draw on. Will later be changed for the chi2 pic
  TCanvas* c2 = new TCanvas("c2","",1540,1417);
  c2->SetTopMargin(0.04);
  c2->SetBottomMargin(0.08);
  c2->SetRightMargin(0.05);
  c2->SetLeftMargin(0.08);
  c2->SetTicky();
  c2->SetTickx();
  c2->SetLogz(1);

  TGaxis::SetMaxDigits(3);
  gStyle->SetOptStat(0);

  TString sPath = gDirectory->GetPath();            // retrieve neutral path

  //////////////////////////////////////////////////////////////////////////////
  // open ESD histo which is inside TLists inside a rootfile
  // There get the Histo with Number of Events (minimum Bias)

  int k                           = 10;
  TFile* ESDFile_data             = NULL;
  TList* lGammaCalo_data          = NULL;
  TList* lCutNumber_data          = NULL;
  TList* lESD_data                = NULL;
  TList* lGammaCalo_MC            = NULL;
  TList* lCutNumber_MC            = NULL;
  TList* lESD_MC                  = NULL;
  TList* lMC_MC                   = NULL;
  TList* lTrue_MC                 = NULL;
  TH1D*  hNEvents                 = NULL;
  TH1D*  hMC_Pi0InAcc_Pt          = NULL;
  TH2D*  hTrueDoubleCounting_Pi0  = NULL;
  TH2D*  hInvMass_pT_Signal       = NULL; // Signal + both bkg. TH2
  TH2D*  hInvMass_pT_Bkg          = NULL; // mixed event  bkg. TH2
  TH1D*  hInvMass                 = NULL; // Signal + both bkg TH1
  TH1D*  hInvMass_Bkg             = NULL; // mixed event  bkg. TH1
  TH1D*  hSignalInvMassMC         = NULL;
  TH1D*  hTrueMesonInvMassMC      = NULL;
  TH1D*  hSignalPlusBkg           = NULL;
  TH1D*  hCorrBckMC               = NULL;
  TH1D*  hUncorrBkgNorm           = NULL;

  TFile* ESDFile_MC       = SafelyOpenRootfile("/data4/mhemmer/Documents/BachelorArbeit/Daten/" + current_path + ".root");
  if (ESDFile_MC->IsOpen() ) printf("ESDFile_MC opened successfully\n");

  ESDFile_data          = SafelyOpenRootfile("/data4/mhemmer/Documents/BachelorArbeit/Daten/GammaCalo-data_503.root");
  if (ESDFile_data->IsOpen() ) printf("ESDFile_data opened successfully\n");

  lGammaCalo_data         = (TList*) ESDFile_data->Get("GammaCalo_503");
  lCutNumber_data         = (TList*) lGammaCalo_data->FindObject("Cut Number 00010113_1111112067032220000_01631031000000d0");
  lESD_data               = (TList*) lCutNumber_data->FindObject("00010113_1111112067032220000_01631031000000d0 ESD histograms");

  lGammaCalo_MC           = (TList*) ESDFile_MC->Get("GammaCalo_503");
  lCutNumber_MC           = (TList*) lGammaCalo_MC->FindObject("Cut Number 00010113_1111112067032220000_01631031000000d0");
  lESD_MC                 = (TList*) lCutNumber_MC->FindObject("00010113_1111112067032220000_01631031000000d0 ESD histograms");
  lMC_MC                  = (TList*) lCutNumber_MC->FindObject("00010113_1111112067032220000_01631031000000d0 MC histograms");
  lTrue_MC                = (TList*) lCutNumber_MC->FindObject("00010113_1111112067032220000_01631031000000d0 True histograms");
  hNEvents                = (TH1D*)  lESD_data->FindObject("NEvents");
  hInvMass_pT_Signal      = (TH2D*)  lESD_data->FindObject("ESD_Mother_InvMass_Pt");
  hInvMass_pT_Bkg         = (TH2D*)  lESD_data->FindObject("ESD_Background_InvMass_Pt");
  hMC_Pi0InAcc_Pt         = (TH1D*)  lMC_MC->FindObject("MC_Pi0InAcc_Pt");
  hTrueDoubleCounting_Pi0 = (TH2D*)  lTrue_MC->FindObject("ESD_TrueDoubleCountPi0_InvMass_Pt");
  //////////////////////////////////////////////////////////////////////////////

  SetHistoStandardSettings2(hInvMass_pT_Signal, 1.1, 1., 40, 43);
  hInvMass_pT_Signal->GetXaxis()->SetNdivisions(505);
  SetHistoStandardSettings2(hInvMass_pT_Bkg, 1.1, 1., 40, 43);
  hInvMass_pT_Bkg->GetXaxis()->SetNdivisions(505);

  TLine* lPi0_mass = new TLine(0.135, 1.4, 0.135, 12.0);
  lPi0_mass->SetLineWidth(3);
  lPi0_mass->SetLineColor(kBlack);

  c1->cd();
  hInvMass_pT_Signal->GetXaxis()->SetRangeUser(0.0, 0.3);
  hInvMass_pT_Signal->GetYaxis()->SetRangeUser(0.0, 12.0);
  hInvMass_pT_Signal->SetXTitle(minv_str);
  hInvMass_pT_Signal->SetYTitle(pt_str);


  hInvMass_pT_Signal->Draw("AXIS");
  hInvMass_pT_Signal->DrawClone("SAME colz");
  lPi0_mass->Draw("SAME");
  c1->Update();
  c1->SaveAs(Form("../BachelorArbeit/hInvMass_pT_Signal.pdf"));
  c1->Clear();

  hInvMass_pT_Bkg->GetXaxis()->SetRangeUser(0.0, 0.3);
  hInvMass_pT_Bkg->GetYaxis()->SetRangeUser(0.0, 12.0);
  hInvMass_pT_Bkg->SetXTitle(minv_str);
  hInvMass_pT_Bkg->SetYTitle(pt_str);

  hInvMass_pT_Bkg->Draw("AXIS");
  hInvMass_pT_Bkg->Draw("SAME colz");
  c1->Update();
  c1->SaveAs(Form("../BachelorArbeit/hInvMass_pT_Bkg.pdf"));
  c1->Clear();




  //////////////////////////////////////////////////////////////////////////////


  gDirectory->Cd(sPath.Data());                     // for saftey resetting path

  //////////////////////////////////////////////////////////////////////////////
  // declaring global variables

  TString str;                                      // contains the pT range
  //////////////////////////////////////////////////////////////////////////////
  // open True Yield Path

  TFile* FData_corrected = SafelyOpenRootfile("/data4/mhemmer/Documents/BachelorArbeit/GammaCalo-All_503_normal_and_extra/00010113_1111112067032220000_01631031000000d0/13TeV/Pi0_data_GammaConvV1Correction_00010113_1111112067032220000_01631031000000d0.root");
  if (FData_corrected->IsOpen() ) printf("FData_corrected opened successfully\n");


  TFile* MCFile = NULL;
  TFile* DataFile = NULL;

  ////////////////////////////////////////////////////////////////////////////
  // open MC histo path
  MCFile = SafelyOpenRootfile("/data4/mhemmer/Documents/BachelorArbeit/GammaCalo-All_503_normal_and_extra/00010113_1111112067032220000_01631031000000d0/13TeV/Pi0_MC_GammaConvV1WithoutCorrection_00010113_1111112067032220000_01631031000000d0.root");
  if (MCFile->IsOpen() ) printf("MCFile opened successfully\n");


  ////////////////////////////////////////////////////////////////////////////
  // retrieve MC histograms
  hSignalInvMassMC = (TH1D*) MCFile->Get(Form("fHistoMappingSignalInvMass_in_Pt_Bin%02i",k)); //fHistoMappingSignalInvMass_in_Pt_Bin
  hTrueMesonInvMassMC = (TH1D*) MCFile->Get(Form("Mapping_TrueMeson_InvMass_in_Pt_Bin%02i",k));


  DataFile = SafelyOpenRootfile("/data4/mhemmer/Documents/BachelorArbeit/GammaCalo-All_503_normal_and_extra/00010113_1111112067032220000_01631031000000d0/13TeV/Pi0_data_GammaConvV1WithoutCorrection_00010113_1111112067032220000_01631031000000d0.root");
  if (DataFile->IsOpen() ) printf("DataFile opened successfully\n");
  hSignalPlusBkg  = (TH1D*) DataFile->Get(Form("Mapping_GG_InvMass_in_Pt_Bin%02d",k));
  hUncorrBkgNorm  = (TH1D*) DataFile->Get(Form("Mapping_BckNorm_InvMass_in_Pt_Bin%02d",k));

  ////////////////////////////////////////////////////////////////////////////
  // Getting the purposed corr Background
  hCorrBckMC = (TH1D*) hSignalInvMassMC->Clone("hCorrBckMC");
  hCorrBckMC->Add(hTrueMesonInvMassMC,-1);


  //////////////////////////////////////////////////////////////////////////
  // Fix! Changes < in TLatex to #leq
  str = hSignalInvMassMC->GetTitle();
  TString str_copy = str.Copy();
  str_copy.ReplaceAll("<","#leq");
  str.Replace(0,20,str_copy,23);
  str = Form("%.1lf #leq #it{p}_{T} (GeV/#it{c}) < %.1lf", fBinsPi013TeVEMCPt[k], fBinsPi013TeVEMCPt[k+1]);


  c2->cd();
  TLegend* legSignalPlusBkg = new TLegend(0.6, 0.7, 0.9, 0.9);
  SetLegendSettigns(legSignalPlusBkg,40, 43);
  legSignalPlusBkg->AddEntry(hSignalPlusBkg, "Signal", "lp");
  legSignalPlusBkg->AddEntry((TObject*) 0x0, "+ korr. Untergrund", "");
  legSignalPlusBkg->AddEntry((TObject*) 0x0, "+ unkorr. Untergrund", "");
  SetHistoStandardSettings(hSignalPlusBkg, 1.2, 1.2, 40, 43);
  hSignalPlusBkg->SetXTitle(minv_str);
  hSignalPlusBkg->SetYTitle("counts");
  hSignalPlusBkg->SetMarkerSize(2.5);

  hSignalPlusBkg->DrawCopy("AXIS");
  hSignalPlusBkg->DrawCopy("SAME");
  legSignalPlusBkg->Draw("SAME");
  DrawLabelALICEwoTemp(0.18, 0.85, 0.03, 40, 43, str);
  c2->Update();
  c2->SaveAs(Form("../BachelorArbeit/hSignalPlusBkg.pdf"));
  c2->Clear();

  delete legSignalPlusBkg;


  // uncorrelated Background Making and Drawing
  TH1D* hUncorrBkg = hInvMass_pT_Bkg->ProjectionX("hUncorrBkg", hInvMass_pT_Bkg->GetYaxis()->FindBin(3.2),  hInvMass_pT_Bkg->GetYaxis()->FindBin(3.4));
  hUncorrBkg->Rebin(4);
  c2->cd();
  TLegend* legUncorrBkg = new TLegend(0.5, 0.3, 0.8, 0.5);
  SetLegendSettigns(legUncorrBkg,40, 43);
  legUncorrBkg->AddEntry(hUncorrBkg, "#it{mixed event} Rekombinationen", "lp");
  SetHistoStandardSettings(hUncorrBkg, 1.2, 1.2, 40, 43);
  hUncorrBkg->SetMarkerColor(kBlue+2);
  hUncorrBkg->SetLineColor(kBlue+2);
  hUncorrBkg->SetXTitle(minv_str);
  hUncorrBkg->SetYTitle("counts");
  hUncorrBkg->SetMarkerSize(2.5);

  hUncorrBkg->DrawCopy("AXIS");
  hUncorrBkg->DrawCopy("SAME");
  legUncorrBkg->Draw("SAME");
  DrawLabelALICEwoTemp(0.18, 0.85, 0.03, 40, 43, str);
  c2->Update();
  c2->SaveAs(Form("../BachelorArbeit/hUncorrBkg.pdf"));
  c2->Clear();

  delete legUncorrBkg;

  TObjArray* histArray = new TObjArray(2);
  histArray->Add(hSignalPlusBkg);
  histArray->Add(hUncorrBkgNorm);

  TObjArray* ratioArray = NULL;

  const char* controlstring = "ThickHorizontal";
  Short_t* colorArray = NULL;
  Short_t* markerArray = NULL;
  TCanvas* cPatrick = makeCanvas(histArray, ratioArray,controlstring, colorArray, markerArray);
  cPatrick->SaveAs("../BachelorArbeit/Patrick.pdf");
  // scaled uncorrelated with Signal + both Backgrunds
  c2->cd();
  TLegend* legUncorrBkgNorm = new TLegend(0.55, 0.7, 0.85, 0.9);
  SetLegendSettigns(legUncorrBkgNorm, 40, 43);
  legUncorrBkgNorm->AddEntry(hSignalPlusBkg, "Signal", "lp");
  legUncorrBkgNorm->AddEntry((TObject*) 0x0, "+ korr. Untergrund", "");
  legUncorrBkgNorm->AddEntry((TObject*) 0x0, "+ unkorr. Untergrund", "");
  legUncorrBkgNorm->AddEntry(hUncorrBkgNorm, "skalierte #it{mixed event}", "f");
  legUncorrBkgNorm->AddEntry((TObject*) 0x0, "Rekombinationen", "");
  SetHistoStandardSettings(hUncorrBkgNorm, 1.2, 1.2, 40, 43);
  hUncorrBkgNorm->SetFillColor(kBlue+2);
  hUncorrBkgNorm->SetLineWidth(2);
  hUncorrBkgNorm->SetFillStyle(3344);
  hUncorrBkgNorm->SetMarkerColor(kBlue+2);
  hUncorrBkgNorm->SetLineColor(kBlue+2);
  hUncorrBkgNorm->SetXTitle(minv_str);
  hUncorrBkgNorm->SetYTitle("counts");
  hUncorrBkgNorm->SetMarkerSize(2.5);

  hSignalPlusBkg->DrawCopy("AXIS");
  hSignalPlusBkg->DrawCopy("SAME");
  hUncorrBkgNorm->Draw("HIST SAME");
  DrawLabelALICEwoTemp(0.18, 0.85, 0.03, 40, 43, str);
  c2->Update();
  TLine* lBkgFitRange = new TLine(0.19, gPad->GetUymax()*0.995, 0.30, gPad->GetUymax()*0.995);
  lBkgFitRange->SetLineColor(kCyan+2);
  lBkgFitRange->SetLineWidth(5);
  legUncorrBkgNorm->AddEntry(lBkgFitRange, "Parametrisierungsbereich", "l");
  legUncorrBkgNorm->Draw("SAME");
  lBkgFitRange->Draw("SAME");

  c2->SaveAs(Form("../BachelorArbeit/hUncorrBkgNorm.pdf"));
  c2->Clear();

  delete legUncorrBkgNorm;

  delete hNEvents;
  delete hMC_Pi0InAcc_Pt;
  delete hTrueDoubleCounting_Pi0;
  delete hInvMass_pT_Signal;
  delete hInvMass_pT_Bkg;
  delete hInvMass;
  delete hInvMass_Bkg;
  delete hSignalInvMassMC;
  delete hTrueMesonInvMassMC;
  delete hSignalPlusBkg;
  delete hCorrBckMC;
  delete lPi0_mass;
  ESDFile_MC->Close();
  ESDFile_data->Close();
  MCFile->Close();
  DataFile->Close();
  delete c1;

}
