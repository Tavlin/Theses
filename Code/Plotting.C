#include "Plotting_Patrick.h"
#include "CommonHeader.h"

// Start of the Main
void Plotting(std::string current_path){

  TGaxis::SetMaxDigits(3);
  gStyle->SetOptStat(0);

  TString sPath = gDirectory->GetPath();            // retrieve neutral path

  //////////////////////////////////////////////////////////////////////////////
  // open ESD histo which is inside TLists inside a rootfile
  // There get the Histo with Number of Events (minimum Bias)

  int k                           = 10;
  auto OAhists                    = new TObjArray();
  auto OAratios                   = new TObjArray();
  TCanvas* cPatrick               = NULL;
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
  TH1D*  hInvMass_Data            = NULL;
  TH1D*  hGG                      = NULL;
  TH1D*  hGC                      = NULL;
  TH1D*  hCC                      = NULL;
  TH1D*  hSignalTemp              = NULL;
  TF1*   fSignalPlusBkg           = NULL;
  TF1*   fGauss                   = NULL;
  TF1*   fTail                    = NULL;
  TF1*   fBkg                     = NULL;

  TLegend* legpT                   = NULL;

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

  TLegend* legSystem = new TLegend(0.1, 0.94, 0.7, 0.98);
  legSystem->AddEntry((TObject*) 0x0, "ALICE, pp #sqrt{#it{s}} = 13 TeV, #pi^{0} #rightarrow #gamma#gamma with EMCal", "");

  /**
   *plotting the minv, pt Signal TH2 with bin as TLines
   */
  hInvMass_pT_Signal->GetXaxis()->SetNdivisions(505);
  hInvMass_pT_Bkg->GetXaxis()->SetNdivisions(505);

  TLine* lPi0_mass = new TLine(0.134977, 1.4, 0.134977, 12.0);
  lPi0_mass->SetLineWidth(3);
  lPi0_mass->SetLineColor(kRed+1);

  TLine* line_0 = new TLine(0.0, 0.0, 0.3, 0.0);
  line_0->SetLineWidth(3);
  line_0->SetLineColor(kBlack);
  line_0->SetLineStyle(1);

  TLine* lpTLines[39];

  for(int k = 1; k <= 39; k++){
    lpTLines[k-1] = new TLine(0.0, fBinsPi013TeVEMCPt[k], 0.3, fBinsPi013TeVEMCPt[k]);
    lpTLines[k-1]->SetLineWidth(1);
    lpTLines[k-1]->SetLineColor(kBlack);
  }

  hInvMass_pT_Signal->GetXaxis()->SetRangeUser(0.0, 0.3);
  hInvMass_pT_Signal->GetYaxis()->SetRangeUser(0.0, 12.0);
  hInvMass_pT_Signal->SetXTitle(minv_str);
  hInvMass_pT_Signal->SetYTitle(pt_str);

  OAhists->Add(hInvMass_pT_Signal);
  for(int k = 1; k <= 39; k++){
    OAhists->Add(lpTLines[k-1]);
  }
  OAhists->Add(legSystem);
  OAhists->Add(lPi0_mass);

  cPatrick = makeCanvas(OAhists, 0, "notimeSquare", 0, 0);
  cPatrick->Update();
  cPatrick->SaveAs(Form("../BachelorArbeit/hInvMass_pT_Signal.eps"));
  cPatrick->Clear();

  OAhists->Clear();

  /**
   * Drawing minv, pt Bkg with TLines as Bins
   */

  hInvMass_pT_Bkg->GetXaxis()->SetRangeUser(0.0, 0.3);
  hInvMass_pT_Bkg->GetYaxis()->SetRangeUser(0.0, 12.0);
  hInvMass_pT_Bkg->SetXTitle(minv_str);
  hInvMass_pT_Bkg->SetYTitle(pt_str);

  OAhists->Add(hInvMass_pT_Bkg);
  OAhists->Add(legSystem);
  for(int k = 1; k <= 39; k++){
    OAhists->Add(lpTLines[k-1]);
  }

  cPatrick = makeCanvas(OAhists, 0, "notimeSquare", 0, 0);
  cPatrick->Update();
  cPatrick->SaveAs(Form("../BachelorArbeit/hInvMass_pT_Bkg.eps"));
  cPatrick->Clear();

  OAhists->Clear();




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

  /**
  * Histogram from the data which was created through the normal analysis
  * method: same events - scaled mixed events.
   */
  hInvMass_Data = (TH1D*) DataFile->Get(Form("fHistoMappingSignalInvMass_in_Pt_Bin%02d",k));

  ////////////////////////////////////////////////////////////////////////////
  // Getting the purposed corr Background
  hCorrBckMC = (TH1D*) hSignalInvMassMC->Clone("hCorrBckMC");
  hCorrBckMC->Add(hTrueMesonInvMassMC,-1);

  /**
   * open Output 3to8 File and get Signal
   */

  TFile* OutputFile = SafelyOpenRootfile("/data4/mhemmer/Documents/BachelorArbeit/GammaCalo-All_503_normal_and_extra/OutputFileBetterBkg3to8.root");
  if (OutputFile->IsOpen() ) printf("DataFile opened successfully\n");

  hGG                      = (TH1D*) MCFile->Get(Form("Mapping_TrueMesonCaloPhoton_InvMass_in_Pt_Bin%02d", k));
  SetHistogramProperties(hGG, "minv", count_str, 2, 0.0, 0.3);

  hGC                      = (TH1D*) MCFile->Get(Form("Mapping_TrueMesonMixedCaloConvPhoton_InvMass_in_Pt_Bin%02d", k));
  SetHistogramProperties(hGC, "minv", count_str, 4, 0.0, 0.3);

  hCC                      = (TH1D*) MCFile->Get(Form("Mapping_TrueMesonCaloConvPhoton_InvMass_in_Pt_Bin%02d", k));
  SetHistogramProperties(hCC, "minv", count_str, 7, 0.0, 0.3);

  hSignalTemp              = (TH1D*) OutputFile->Get(Form("hSignal_bin%02d",k));
  SetHistogramProperties(hSignalTemp, "minv", count_str, 1, 0.0, 0.3);

  //////////////////////////////////////////////////////////////////////////
  // Fix! Changes < in TLatex to #leq
  // str = hSignalInvMassMC->GetTitle();
  // TString str_copy = str.Copy();
  // str_copy.ReplaceAll("<","#leq");
  // str.Replace(0,20,str_copy,23);
  str = Form("%.1lf #leq #it{p}_{T} /(GeV/#it{c}) < %.1lf", fBinsPi013TeVEMCPt[k], fBinsPi013TeVEMCPt[k+1]);
  legpT = new TLegend(0.07, 0.85, 0.42, 0.9);
  legpT->AddEntry((TObject*) 0x0, str, "");


  cPatrick->cd();
  TLegend* legSignalPlusBkg = new TLegend(0.55, 0.7, 0.85, 0.9);
  legSignalPlusBkg->AddEntry(hSignalPlusBkg, "signal", "p");
  legSignalPlusBkg->AddEntry((TObject*) 0x0, "+ corr. background", "");
  legSignalPlusBkg->AddEntry((TObject*) 0x0, "+ uncorr. background", "");
  SetHistogramProperties(hSignalPlusBkg, "minv", count_str, 5, 0.0, 0.3);

  OAhists->Add(hSignalPlusBkg);
  OAhists->Add(legSignalPlusBkg);
  OAhists->Add(legSystem);
  OAhists->Add(legpT);

  cPatrick = makeCanvas(OAhists, 0, "notimeThickHorizontal", 0, 0);
  // DrawLabelALICE(0.18, 0.85, 0.03, 30, str);
  cPatrick->Update();
  cPatrick->SaveAs(Form("../BachelorArbeit/hSignalPlusBkg.eps"));
  cPatrick->Clear();

  OAhists->Clear();

  delete legSignalPlusBkg;


  // uncorrelated Background Making and Drawing
  TH1D* hUncorrBkg = hInvMass_pT_Bkg->ProjectionX("hUncorrBkg", hInvMass_pT_Bkg->GetYaxis()->FindBin(3.2),  hInvMass_pT_Bkg->GetYaxis()->FindBin(3.4));
  hUncorrBkg->Rebin(4);
  cPatrick->cd();
  TLegend* legUncorrBkg = new TLegend(0.5, 0.3, 0.8, 0.5);
  legUncorrBkg->AddEntry(hUncorrBkg, "#it{mixed event} combinations", "p");
  SetHistogramProperties(hUncorrBkg, "minv", count_str, 2, 0.0, 0.3);

  OAhists->Add(hUncorrBkg);
  OAhists->Add(legSystem);
  OAhists->Add(legpT);
  // OAhists->Add(legUncorrBkg);

  cPatrick = makeCanvas(OAhists, 0, "notimeThickHorizontal", 0, 0);
  // DrawLabelALICE(0.18, 0.85, 0.03, 30, str);
  cPatrick->Update();
  cPatrick->SaveAs(Form("../BachelorArbeit/hUncorrBkg.eps"));
  cPatrick->Clear();

  OAhists->Clear();

  delete legUncorrBkg;

  OAhists->Add(hSignalPlusBkg);
  OAhists->Add(hUncorrBkgNorm);
  OAhists->Add(legSystem);
  OAhists->Add(legpT);

  OAratios = NULL;

  cPatrick = makeCanvas(OAhists, 0, "notimeThickHorizontal", 0, 0);
  cPatrick->SaveAs("../BachelorArbeit/Patrick.eps");
  // scaled uncorrelated with Signal + both Backgrunds
  cPatrick->Clear();

  TLegend* legUncorrBkgNorm = new TLegend(0.55, 0.57, 0.8, 0.9);
  legUncorrBkgNorm->AddEntry(hSignalPlusBkg, "signal", "p");
  legUncorrBkgNorm->AddEntry((TObject*) 0x0, "+ corr. background", "");
  legUncorrBkgNorm->AddEntry((TObject*) 0x0, "+ uncorr. background", "");
  legUncorrBkgNorm->AddEntry(hUncorrBkgNorm, "scaled #it{mixed event}", "p");
  legUncorrBkgNorm->AddEntry((TObject*) 0x0, "combinations", "");
  SetHistogramProperties(hUncorrBkgNorm, "minv", count_str, 2, 0.0, 0.3);

  OAhists->Add(hSignalPlusBkg);
  OAhists->Add(hUncorrBkgNorm);
  OAhists->Add(legUncorrBkgNorm);
  OAhists->Add(legSystem);
  OAhists->Add(legpT);

  cPatrick = makeCanvas(OAhists, 0, "notimeThickHorizontal", 0, 0);

  OAhists->Clear();

  // DrawLabelALICE(0.18, 0.85, 0.03, 30, str);
  cPatrick->Update();
  TLine* lBkgFitRange = new TLine(0.19, gPad->GetUymax()*0.995, 0.30, gPad->GetUymax()*0.995);
  lBkgFitRange->SetLineColor(kCyan+2);
  lBkgFitRange->SetLineWidth(5);
  legUncorrBkgNorm->AddEntry(lBkgFitRange, "param. range", "l");

  cPatrick->Clear();

  SetHistogramProperties(hSignalPlusBkg, "minv", count_str, 5, 0.0, 0.3);
  SetHistogramProperties(hUncorrBkgNorm, "minv", count_str, 2, 0.0, 0.3);

  OAhists->Add(hSignalPlusBkg);
  OAhists->Add(hUncorrBkgNorm);
  OAhists->Add(legUncorrBkgNorm);
  OAhists->Add(legSystem);
  OAhists->Add(lBkgFitRange);
  OAhists->Add(legpT);

  cPatrick = makeCanvas(OAhists, 0, "notimeThickHorizontal", 0, 0);

  cPatrick->SaveAs(Form("../BachelorArbeit/hUncorrBkgNorm.eps"));
  OAhists->Clear();
  cPatrick->Clear();

  /**
   * Signal = korr. Background aus Daten Plot
   */

  SetHistogramProperties(hInvMass_Data, "minv", count_str, 5, 0.0, 0.3);
  TLegend* legInvMass_Data = new TLegend(0.55, 0.7, 0.85, 0.9);
  legInvMass_Data->AddEntry(hInvMass_Data, "signal", "lp");
  legInvMass_Data->AddEntry((TObject*) 0x0, "+ corr. background", "");

  OAhists->Add(hInvMass_Data);
  OAhists->Add(legInvMass_Data);
  OAhists->Add(legSystem);
  OAhists->Add(legpT);

  cPatrick = makeCanvas(OAhists, 0, "notimeThickHorizontal", 0, 0);

  cPatrick->SaveAs(Form("../BachelorArbeit/hInvMass_Data.eps"));
  cPatrick->Clear();
  OAhists->Clear();

  delete legInvMass_Data;

  /**
   * Drawing Standardmethod parametrization with components
   */

  fSignalPlusBkg  = (TF1*) DataFile->Get(Form("Signal_InvMassFit_in_Pt_Bin%02i",k));
  fSignalPlusBkg->  SetRange(0.0, 0.3);
  fGauss          = new TF1("fGauss", "[0]*exp(-0.5*((x-[1])/[2])^2)", 0.0, 0.3);
  fTail           = new TF1("fTail", "(x<[1])*[0]*(exp((x-[1])/[3])*(1-exp(-0.5*((x-[1])/[2])^2)))+(x>=[1])*0", 0.0, 0.3);
  fBkg            = new TF1("fBkg", "[4] + [5]*x", 0.0, 0.3);
  fGauss->SetParameter(0, fSignalPlusBkg->GetParameter(0));
  fGauss->SetParameter(1, fSignalPlusBkg->GetParameter(1));
  fGauss->SetParameter(2, fSignalPlusBkg->GetParameter(2));
  fTail-> SetParameter(0, fSignalPlusBkg->GetParameter(0));
  fTail-> SetParameter(1, fSignalPlusBkg->GetParameter(1));
  fTail-> SetParameter(2, fSignalPlusBkg->GetParameter(2));
  fTail-> SetParameter(3, fSignalPlusBkg->GetParameter(3));
  fBkg->  SetParameter(4, fSignalPlusBkg->GetParameter(4));
  fBkg->  SetParameter(5, fSignalPlusBkg->GetParameter(5));
  fSignalPlusBkg->SetNpx(1000);
  fGauss->        SetNpx(1000);
  fTail->         SetNpx(1000);
  fBkg->          SetNpx(1000);

  TLegend* legStandardParam = new TLegend(0.64, 0.85, 0.88, 0.9);
  TLegend* legStandardParam4 = new TLegend(0.59, 0.8, 0.88, 0.85);
  TLegend* legStandardParam2 = new TLegend(0.54, 0.75, 0.82, 0.8);
  TLegend* legStandardParam3 = new TLegend(0.64, 0.55, 0.88, 0.75);
  legStandardParam->AddEntry(hInvMass_Data, "signal", "lp");
  legStandardParam4->AddEntry((TObject*) 0x0, "+ corr. background", "");

  legStandardParam2->AddEntry((TObject*) 0x0, "parametrisation:", "");

  legStandardParam3->AddEntry(fSignalPlusBkg, "combined ", "l");
  legStandardParam3->AddEntry(fGauss, "gaussian ", "l");
  legStandardParam3->AddEntry(fTail, "tail ", "l");
  legStandardParam3->AddEntry(fBkg, "linear ", "l");


  OAhists->Add(hInvMass_Data);
  OAhists->Add(fSignalPlusBkg);
  OAhists->Add(fGauss);
  OAhists->Add(fTail);
  OAhists->Add(fBkg);
  OAhists->Add(legStandardParam);
  OAhists->Add(legStandardParam4);
  OAhists->Add(legStandardParam2);
  OAhists->Add(legStandardParam3);
  OAhists->Add(line_0);
  OAhists->Add(legSystem);
  OAhists->Add(legpT);

  cPatrick = makeCanvas(OAhists, 0, "notimeThickSquare", 0, 0);

  cPatrick->SaveAs(Form("../BachelorArbeit/StandardParam.eps"));
  cPatrick->Clear();
  OAhists->Clear();

  delete legpT;


  for(int k = 1; k <= 39; k++){
    delete lpTLines[k-1];
  }
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
  delete cPatrick;
  delete fGauss;
  delete fTail;
  delete fBkg;

}
