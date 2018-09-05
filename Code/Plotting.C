#include "CommonHeader.h"

// Start of the Main
void Plotting(std::string current_path){

  //////////////////////////////////////////////////////////////////////////////
  // setting up the canvas to draw on. Will later be changed for the chi2 pic
  TCanvas* c1 = new TCanvas("c1","",1200,1200);
  c1->cd();
  c1->SetTopMargin(0.05);
  c1->SetBottomMargin(0.09);
  c1->SetRightMargin(0.12);
  c1->SetLeftMargin(0.09);
  c1->SetTicky();
  c1->SetTickx();
  c1->SetLogz(1);
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
  TH1D*  data_MC                  = NULL;
  TH1D*  mc_full                  = NULL;
  TH1D*  data                     = NULL;
  TH1D*  korrBG                   = NULL;

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

  SetHistoStandardSettings2(hInvMass_pT_Signal);
  SetHistoStandardSettings2(hInvMass_pT_Bkg);

  hInvMass_pT_Signal->GetXaxis()->SetRangeUser(0.0, 0.3);
  hInvMass_pT_Signal->GetYaxis()->SetRangeUser(0.0, 16.0);
  hInvMass_pT_Signal->SetXTitle(minv_str);
  hInvMass_pT_Signal->SetYTitle(pt_str);

  hInvMass_pT_Signal->Draw("AXIS");
  hInvMass_pT_Signal->Draw("SAME colz");
  c1->Update();
  c1->SaveAs(Form("../BachelorArbeit/hInvMass_pT_Signal.png"));
  c1->Clear();

  hInvMass_pT_Bkg->GetXaxis()->SetRangeUser(0.0, 0.3);
  hInvMass_pT_Bkg->GetYaxis()->SetRangeUser(0.0, 16.0);
  hInvMass_pT_Bkg->SetXTitle(minv_str);
  hInvMass_pT_Bkg->SetYTitle(pt_str);

  hInvMass_pT_Bkg->Draw("AXIS");
  hInvMass_pT_Bkg->Draw("SAME colz");
  c1->Update();
  c1->SaveAs(Form("../BachelorArbeit/hInvMass_pT_Bkg.png"));
  c1->Clear();




  ESDFile_MC->Close();
  ESDFile_data->Close();
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
  data_MC = (TH1D*) MCFile->Get(Form("fHistoMappingSignalInvMass_in_Pt_Bin%02i",k)); //fHistoMappingSignalInvMass_in_Pt_Bin
  mc_full = (TH1D*) MCFile->Get(Form("Mapping_TrueMeson_InvMass_in_Pt_Bin%02i",k));


  DataFile = SafelyOpenRootfile("/data4/mhemmer/Documents/BachelorArbeit/GammaCalo-All_503_normal_and_extra/00010113_1111112067032220000_01631031000000d0/13TeV/Pi0_data_GammaConvV1WithoutCorrection_00010113_1111112067032220000_01631031000000d0.root");
  if (DataFile->IsOpen() ) printf("DataFile opened successfully\n");
  data = (TH1D*) DataFile->Get(Form("fHistoMappingSignalInvMass_in_Pt_Bin%02i",k));

  ////////////////////////////////////////////////////////////////////////////
  // Getting the purposed corr Background
  korrBG = (TH1D*) data_MC->Clone("korrBG");
  korrBG->Add(mc_full,-1);


  //////////////////////////////////////////////////////////////////////////
  // Fix! Changes < in TLatex to #leq
  str = data_MC->GetTitle();
  TString str_copy = str.Copy();
  str_copy.ReplaceAll("<","#leq");
  str.Replace(0,20,str_copy,23);

  data->SetTitle(str);
  MCFile->Close();
  DataFile->Close();

  delete c1;

}
