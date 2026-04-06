#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TLine.h>
#include <TStyle.h>

#include <iostream>
#include <string>

#include "CommandLine.h"

using namespace std;

// ------------------------------------------------------------
int dataMCComp(string dataFile,
                       string mcFile,
                       string outPdf)
{
  gStyle->SetOptStat(0);

  // --- Open files ---
  TFile* fData = TFile::Open(dataFile.c_str(), "READ");
  TFile* fMC   = TFile::Open(mcFile.c_str(),   "READ");

  if (!fData || fData->IsZombie()) {
    cerr << "ERROR: cannot open data file " << dataFile << endl;
    return 1;
  }
  if (!fMC || fMC->IsZombie()) {
    cerr << "ERROR: cannot open MC file " << mcFile << endl;
    return 1;
  }

  // --- Get histograms ---
  TH1D* hData  = (TH1D*) fData->Get("hist_SR_sub_norm");
  TH1D* hMCSB  = (TH1D*) fMC  ->Get("hist_SR_sub_norm");
  TH1D* hMCGen = (TH1D*) fMC  ->Get("hist_GEN_norm");

  if (!hData || !hMCSB || !hMCGen) {
    cerr << "ERROR: failed to retrieve one or more histograms" << endl;
    return 1;
  }

  // Clone to detach from files
  hData  = (TH1D*) hData ->Clone("hData");
  hMCSB  = (TH1D*) hMCSB ->Clone("hMCSB");
  hMCGen = (TH1D*) hMCGen->Clone("hMCGen");
  hData ->SetDirectory(nullptr);
  hMCSB ->SetDirectory(nullptr);
  hMCGen->SetDirectory(nullptr);

  // --- Style ---
  // MC gen-matched: red
  hMCGen->SetLineColor(kRed);
  hMCGen->SetLineWidth(3);

  // MC sideband-sub: green
  hMCSB->SetLineColor(kGreen+2);
  hMCSB->SetMarkerColor(kGreen+2);
  hMCSB->SetMarkerStyle(20);
  hMCSB->SetLineWidth(3);

  // Data sideband-sub: green with markers
  hData->SetLineColor(kBlack);
  hData->SetMarkerColor(kBlack);
  hData->SetMarkerStyle(20);
  hData->SetLineWidth(3);

  // --- Ratios to MC gen ---
  TH1D* rData = (TH1D*) hData ->Clone("rData");
  TH1D* rMCSB = (TH1D*) hMCSB ->Clone("rMCSB");
  rData->Divide(hMCGen);
  rMCSB->Divide(hMCGen);

  rData->SetMarkerStyle(20);
  rData->SetLineColor(kBlack);
  rData->SetMarkerColor(kBlack);

  rMCSB->SetMarkerStyle(20);
  rMCSB->SetLineColor(kGreen+2);
  rMCSB->SetMarkerColor(kGreen+2);

  // --- Canvas ---
  TCanvas* c = new TCanvas("c", "comparison", 800, 800);

  TPad* pTop = new TPad("pTop", "", 0, 0.30, 1, 1);
  TPad* pBot = new TPad("pBot", "", 0, 0.00, 1, 0.30);

  pTop->SetBottomMargin(0.02);
  pTop->SetLeftMargin(0.13);
  pTop->SetRightMargin(0.05);

  pBot->SetTopMargin(0.02);
  pBot->SetBottomMargin(0.35);
  pBot->SetLeftMargin(0.13);
  pBot->SetRightMargin(0.05);

  pTop->Draw();
  pBot->Draw();

  // --- Top pad ---
  pTop->cd();
  double ymax = max({hData->GetMaximum(),
                     hMCSB->GetMaximum(),
                     hMCGen->GetMaximum()});
  hMCGen->SetMaximum(1.3 * ymax);
  hMCGen->GetXaxis()->SetLabelSize(0);

  hMCGen->Draw("hist");
  hMCSB->Draw("e same");
  hData ->Draw("e same");
  gPad->SetLogy(outPdf.find("DsvpvDistance")!=string::npos);

  TLegend* leg = new TLegend(0.55, 0.70, 0.92, 0.90);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hMCGen, "MC gen-matched", "l");
  leg->AddEntry(hMCSB,  "MC sideband sub.", "l");
  leg->AddEntry(hData,  "Data sideband sub.", "lep");
  leg->Draw();

  // --- Bottom pad ---
  pBot->cd();
  rData->SetTitle("");
  rData->SetMinimum(0.0);
  rData->SetMaximum(2.0);
  rData->GetYaxis()->SetTitle("Ratio to MC gen");
  rData->GetYaxis()->SetNdivisions(505);
  rData->GetYaxis()->SetTitleSize(0.10);
  rData->GetYaxis()->SetLabelSize(0.09);
  rData->GetYaxis()->SetTitleOffset(0.45);

  rData->GetXaxis()->SetTitle(hMCGen->GetXaxis()->GetTitle());
  rData->GetXaxis()->SetTitleSize(0.12);
  rData->GetXaxis()->SetLabelSize(0.10);

  rData->Draw("e");
  rMCSB->Draw("e same");
  gPad->SetLogy(0);

  TLine* one = new TLine(rData->GetXaxis()->GetXmin(), 1.0,
                         rData->GetXaxis()->GetXmax(), 1.0);
  one->SetLineStyle(2);
  one->Draw();

  c->SaveAs(outPdf.c_str());

  delete one;
  delete leg;
  delete c;

  fData->Close();
  fMC->Close();
  delete fData;
  delete fMC;

  return 0;
}

// ------------------------------------------------------------
int main(int argc, char* argv[])
{
  CommandLine CL(argc, argv);

  string dataFile = CL.Get("Data", "data.root");
  string mcFile   = CL.Get("MC",   "MC.root");
  string outPdf   = CL.Get("out",  "dataMCComp.pdf");

  return dataMCComp(dataFile, mcFile, outPdf);
}
