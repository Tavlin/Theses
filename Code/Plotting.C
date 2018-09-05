#include "CommonHeader.h"

// Start of the Main
void Plotting(std::string current_path){

  TString sPath = gDirectory->GetPath();            // retrieve neutral path

  //////////////////////////////////////////////////////////////////////////////
  // open ESD histo which is inside TLists inside a rootfile
  // There get the Histo with Number of Events (minimum Bias)

  TFile* ESDFile_data            = NULL;
  TList* lGammaCalo_data         = NULL;
  TList* lCutNumber_data         = NULL;
  TList* lESD_data               = NULL;
  TList* lGammaCalo_MC           = NULL;
  TList* lCutNumber_MC           = NULL;
  TList* lESD_MC                 = NULL;
  TList* lMC_MC                  = NULL;
  TList* lTrue_MC                = NULL;
  TH1D*  hNEvents                = NULL;
  TH1D*  hMC_Pi0InAcc_Pt         = NULL;
  TH2D*  hTrueDoubleCounting_Pi0 = NULL;


  TFile* ESDFile_MC       = SafelyOpenRootfile("./../Daten/" + current_path + ".root");
  if (ESDFile_MC->IsOpen() ) printf("ESDFile_MC opened successfully\n");

  ESDFile_data          = SafelyOpenRootfile("./../Daten/GammaCalo-data_503.root");
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
  hMC_Pi0InAcc_Pt         = (TH1D*)  lMC_MC->FindObject("MC_Pi0InAcc_Pt");
  hTrueDoubleCounting_Pi0 = (TH2D*)  lTrue_MC->FindObject("ESD_TrueDoubleCountPi0_InvMass_Pt");
  //////////////////////////////////////////////////////////////////////////////


  ESDFile_MC->Close();
  ESDFile_data->Close();
  //////////////////////////////////////////////////////////////////////////////


  gDirectory->Cd(sPath.Data());                     // for saftey resetting path

  //////////////////////////////////////////////////////////////////////////////
  // declaring global variables

  TString str;                                      // contains the pT range
  //////////////////////////////////////////////////////////////////////////////
  // open True Yield Path

  TFile* FData_corrected = SafelyOpenRootfile("./00010113_1111112067032220000_01631031000000d0/13TeV/Pi0_data_GammaConvV1Correction_00010113_1111112067032220000_01631031000000d0.root");
  if (FData_corrected->IsOpen() ) printf("FData_corrected opened successfully\n");


  TFile* MCFile = NULL;
  TFile* DataFile = NULL;

  ////////////////////////////////////////////////////////////////////////////
  // open MC histo path
  MCFile = SafelyOpenRootfile("./00010113_1111112067032220000_01631031000000d0/13TeV/Pi0_MC_GammaConvV1WithoutCorrection_00010113_1111112067032220000_01631031000000d0.root");
  if (MCFile->IsOpen() ) printf("MCFile opened successfully\n");


  ////////////////////////////////////////////////////////////////////////////
  // retrieve MC histograms
  data_MC = (TH1D*) MCFile->Get(Form("fHistoMappingSignalInvMass_in_Pt_Bin%02i",k)); //fHistoMappingSignalInvMass_in_Pt_Bin
  mc_full = (TH1D*) MCFile->Get(Form("Mapping_TrueMeson_InvMass_in_Pt_Bin%02i",k));


  DataFile = SafelyOpenRootfile("./00010113_1111112067032220000_01631031000000d0/13TeV/Pi0_data_GammaConvV1WithoutCorrection_00010113_1111112067032220000_01631031000000d0.root");
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

}
