/* + Descriptions:
 *      Macro to dump the new skim format into .txt and histograms in .root, for the validations btw two skims
 * + Output:
 *      + .txt:  printing of event-, reco-D- and gen-D-level quantities (not implemented here)
 *      + .root: contains histograms defined below
 * + Todo:
 *      + The current MC skim doesn't have the gammaN, Ngamma info, so I assume 0 for the current being
 */

#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TSystem.h"

#include "CommandLine.h"
#include "CommonFunctions.h"
#include "Messenger.h"
#include "ProgressBar.h"

void DrawOverlay(TH1D *h0nXn, TH1D *hXn0n,
   int isel, const std::string &SelLabel,
   const std::string &baseName)
{
   TCanvas *c = new TCanvas(
      Form("c_%s_sel%d", baseName.c_str(), isel),
      Form("%s overlay, sel %d (%s)", baseName.c_str(), isel, SelLabel.c_str()),
      800, 600);

   c->SetLogy();
   gStyle->SetOptStat(0);   // no stat boxes on all histograms

   h0nXn->SetLineColor(kRed);
   hXn0n->SetLineColor(kBlue);
   h0nXn->SetLineWidth(2);
   hXn0n->SetLineWidth(2);

   // Scale
   double max1 = h0nXn->GetMaximum();
   double max2 = hXn0n->GetMaximum();
   double ymax = std::max(max1, max2) * 1.20;
   h0nXn->SetMaximum(ymax);
   h0nXn->SetMinimum(0.5);

   h0nXn->Draw("HIST");
   hXn0n->Draw("HIST SAME");

   TLegend *leg = new TLegend(0.50, 0.70, 0.88, 0.88);
   leg->SetBorderSize(0);
   leg->SetFillStyle(0);
   leg->AddEntry(h0nXn, "0nXn (ZDC- < 1000, ZDC+ > 1100)", "l");
   leg->AddEntry(hXn0n, "Xn0n (ZDC- > 1000, ZDC+ < 1100)", "l");
   leg->Draw();

   c->SaveAs(Form("out/%s_overlay_sel%d.png", baseName.c_str(), isel));
   c->Write();
}

int main(int argc, char *argv[])
{
   // Input / output
//    TFile InputFile("/data00/yuchenc/UPCDzeroAnalysis/SkimsData/Dzero_2025Data_PromptRecoV1_HIForward2_399543_20251119.root", "READ");
   TFile InputFile(argv[1], "READ");
   DzeroUPCTreeMessenger MDzeroUPC(InputFile, "Tree", true);

   int triggerChoice = atoi(argv[2]); // 0:     ZeroBias, 1: ZDCOR
                                      // 400:   ZeroBias_Max400, 1400: ZDCOR_Max400
                                      // 10000: ZeroBias_Min400, 110000: ZDCOR_Min400
   string trigName = (triggerChoice ==      0) ? "ZBTrigger" : 
                     (triggerChoice ==      1) ? "ZDCORTrigger":
                     (triggerChoice ==    400) ? "ZBTrigger_Max400":
                     (triggerChoice ==   1400) ? "ZDCORTrigger_Max400":
                     (triggerChoice ==  10000) ? "ZBTrigger_Min400":
                     (triggerChoice == 110000) ? "ZDCORTrigger_Min400":
                     (triggerChoice ==  14000) ? "ZBTrigger_Min400_or_Max400":
                     (triggerChoice == 114000) ? "ZDCORTrigger_Min400_or_Max400":
                                                 "All";
    string baseName = gSystem->BaseName(argv[1]);
    string outFileName = "out/" + trigName + "_" + baseName;
    TFile OutputFile(outFileName.c_str(), "RECREATE");

   int EntryCount = MDzeroUPC.GetEntries();
   ProgressBar Bar(cout, EntryCount);
   Bar.SetStyle(-1);

   // Histograms
   TH1D *h_VX_ZDC0nXn[4];
   TH1D *h_VY_ZDC0nXn[4];
   TH1D *h_VZ_ZDC0nXn[4];
   TH1D *h_VXError_ZDC0nXn[4];
   TH1D *h_VYError_ZDC0nXn[4];
   TH1D *h_VZError_ZDC0nXn[4];
   TH1D *h_ZDCsumPlus_ZDC0nXn[4];
   TH1D *h_ZDCsumMinus_ZDC0nXn[4];
   TH1D *h_HFEMaxPlus_ZDC0nXn[4];
   TH1D *h_HFEMaxMinus_ZDC0nXn[4];
   TH1D *h_Dmass_ZDC0nXn[4];

   TH1D *h_VX_ZDCXn0n[4];
   TH1D *h_VY_ZDCXn0n[4];
   TH1D *h_VZ_ZDCXn0n[4];
   TH1D *h_VXError_ZDCXn0n[4];
   TH1D *h_VYError_ZDCXn0n[4];
   TH1D *h_VZError_ZDCXn0n[4];
   TH1D *h_ZDCsumPlus_ZDCXn0n[4];
   TH1D *h_ZDCsumMinus_ZDCXn0n[4];
   TH1D *h_HFEMaxPlus_ZDCXn0n[4];
   TH1D *h_HFEMaxMinus_ZDCXn0n[4];
   TH1D *h_Dmass_ZDCXn0n[4];

   // Optional: labels for selections
   std::string SelLabel[4] = {
      trigName,
      trigName + "+Cls. Comp.",
      trigName + "+Cls. Comp.+PV",
      trigName + "+Cls. Comp.+PV+HF"
   };

   long long count0nXn[4] = {0};
   long long countXn0n[4] = {0};

   // Book histograms
   for (int i = 0; i < 4; i++)
   {
      // Ngamma (0nXn)
      h_VX_ZDC0nXn[i]       = new TH1D(Form("h_VX_ZDC0nXn_%d",       i), Form("%s;V_{x} [cm];Counts", SelLabel[i].c_str()), 40, -0.5, 0.5);
      h_VY_ZDC0nXn[i]       = new TH1D(Form("h_VY_ZDC0nXn_%d",       i), Form("%s;V_{y} [cm];Counts", SelLabel[i].c_str()), 40, -0.5, 0.5);
      h_VZ_ZDC0nXn[i]       = new TH1D(Form("h_VZ_ZDC0nXn_%d",       i), Form("%s;V_{z} [cm];Counts", SelLabel[i].c_str()), 60, -30, 30);
      h_VXError_ZDC0nXn[i]  = new TH1D(Form("h_VXError_ZDC0nXn_%d",  i), Form("%s;#sigma(V_{x}) [cm];Counts", SelLabel[i].c_str()), 40, 0, 0.1);
      h_VYError_ZDC0nXn[i]  = new TH1D(Form("h_VYError_ZDC0nXn_%d",  i), Form("%s;#sigma(V_{y}) [cm];Counts", SelLabel[i].c_str()), 40, 0, 0.1);
      h_VZError_ZDC0nXn[i]  = new TH1D(Form("h_VZError_ZDC0nXn_%d",  i), Form("%s;#sigma(V_{z}) [cm];Counts", SelLabel[i].c_str()), 40, 0, 0.5);
      h_ZDCsumPlus_ZDC0nXn[i]  = new TH1D(Form("h_ZDCsumPlus_ZDC0nXn_%d",  i), Form("%s;ZDCsumPlus;Counts", SelLabel[i].c_str()), 100, 0, 10000);
      h_ZDCsumMinus_ZDC0nXn[i] = new TH1D(Form("h_ZDCsumMinus_ZDC0nXn_%d", i), Form("%s;ZDCsumMinus;Counts", SelLabel[i].c_str()), 100, 0, 10000);
      h_HFEMaxPlus_ZDC0nXn[i]  = new TH1D(Form("h_HFEMaxPlus_ZDC0nXn_%d",  i), Form("%s;HFEMaxPlus;Counts", SelLabel[i].c_str()), 100, 0, 1000);
      h_HFEMaxMinus_ZDC0nXn[i] = new TH1D(Form("h_HFEMaxMinus_ZDC0nXn_%d", i), Form("%s;HFEMaxMinus;Counts", SelLabel[i].c_str()), 100, 0, 1000);
      h_Dmass_ZDC0nXn[i] = new TH1D(Form("h_Dmass_ZDC0nXn_%d", i), Form("%s;Dmass;Counts", SelLabel[i].c_str()), 40, 1.5, 2.2);

      // gammaN (Xn0n)
      h_VX_ZDCXn0n[i]       = new TH1D(Form("h_VX_ZDCXn0n_%d",       i), Form("%s;V_{x} [cm];Counts", SelLabel[i].c_str()), 40, -0.5, 0.5);
      h_VY_ZDCXn0n[i]       = new TH1D(Form("h_VY_ZDCXn0n_%d",       i), Form("%s;V_{y} [cm];Counts", SelLabel[i].c_str()), 40, -0.5, 0.5);
      h_VZ_ZDCXn0n[i]       = new TH1D(Form("h_VZ_ZDCXn0n_%d",       i), Form("%s;V_{z} [cm];Counts", SelLabel[i].c_str()), 60, -30, 30);
      h_VXError_ZDCXn0n[i]  = new TH1D(Form("h_VXError_ZDCXn0n_%d",  i), Form("%s;#sigma(V_{x}) [cm];Counts", SelLabel[i].c_str()), 40, 0, 0.1);
      h_VYError_ZDCXn0n[i]  = new TH1D(Form("h_VYError_ZDCXn0n_%d",  i), Form("%s;#sigma(V_{y}) [cm];Counts", SelLabel[i].c_str()), 40, 0, 0.1);
      h_VZError_ZDCXn0n[i]  = new TH1D(Form("h_VZError_ZDCXn0n_%d",  i), Form("%s;#sigma(V_{z}) [cm];Counts", SelLabel[i].c_str()), 40, 0, 0.5);
      h_ZDCsumPlus_ZDCXn0n[i]  = new TH1D(Form("h_ZDCsumPlus_ZDCXn0n_%d",  i), Form("%s;ZDCsumPlus;Counts", SelLabel[i].c_str()), 100, 0, 10000);
      h_ZDCsumMinus_ZDCXn0n[i] = new TH1D(Form("h_ZDCsumMinus_ZDCXn0n_%d", i), Form("%s;ZDCsumMinus;Counts", SelLabel[i].c_str()), 100, 0, 10000);
      h_HFEMaxPlus_ZDCXn0n[i]  = new TH1D(Form("h_HFEMaxPlus_ZDCXn0n_%d",  i), Form("%s;HFEMaxPlus;Counts", SelLabel[i].c_str()), 100, 0, 1000);
      h_HFEMaxMinus_ZDCXn0n[i] = new TH1D(Form("h_HFEMaxMinus_ZDCXn0n_%d", i), Form("%s;HFEMaxMinus;Counts", SelLabel[i].c_str()), 100, 0, 1000);
      h_Dmass_ZDCXn0n[i] = new TH1D(Form("h_Dmass_ZDCXn0n_%d", i), Form("%s;Dmass;Counts", SelLabel[i].c_str()), 40, 1.5, 2.2);
   }

   // Event loop
   for (int iE = 0; iE < EntryCount; iE++)
   {
      if (EntryCount < 300 || (iE % (EntryCount / 250)) == 0)
      {
         Bar.Update(iE);
         Bar.Print();
      }

      MDzeroUPC.GetEntry(iE);

      bool evtsel[4] = {false, false, false, false};

      // base trigger
      evtsel[0] = (triggerChoice ==      0) ? MDzeroUPC.isZeroBias : 
                  (triggerChoice ==      1) ? MDzeroUPC.isL1ZDCOr :
                  (triggerChoice ==    400) ? MDzeroUPC.isZeroBias_Max400 :
                  (triggerChoice ==   1400) ? MDzeroUPC.isL1ZDCOr_Max400 :
                  (triggerChoice ==  10000) ? MDzeroUPC.isZeroBias_Min400 :
                  (triggerChoice == 110000) ? MDzeroUPC.isL1ZDCOr_Min400 :
                  (triggerChoice ==  14000) ? MDzeroUPC.isZeroBias_Min400 || MDzeroUPC.isZeroBias_Max400:
                  (triggerChoice == 114000) ? MDzeroUPC.isL1ZDCOr_Min400  || MDzeroUPC.isL1ZDCOr_Max400:
                                              true;
      evtsel[1] = evtsel[0] && MDzeroUPC.selectedBkgFilter;
      evtsel[2] = evtsel[1] && MDzeroUPC.selectedVtxFilter;

      // Ngamma: ZDC_- < 1000, ZDC_+ > 1100
      if (MDzeroUPC.ZDCsumMinus < 1000 && MDzeroUPC.ZDCsumPlus > 1100)
      {
         evtsel[3] = evtsel[2] && (MDzeroUPC.HFEMaxMinus < 8.6);

         for (int isel = 0; isel < 4; isel++)
         {
            if (!evtsel[isel]) continue;
            count0nXn[isel]++;

            h_VX_ZDC0nXn[isel]      ->Fill(MDzeroUPC.VX);
            h_VY_ZDC0nXn[isel]      ->Fill(MDzeroUPC.VY);
            h_VZ_ZDC0nXn[isel]      ->Fill(MDzeroUPC.VZ);
            h_VXError_ZDC0nXn[isel] ->Fill(MDzeroUPC.VXError);
            h_VYError_ZDC0nXn[isel] ->Fill(MDzeroUPC.VYError);
            h_VZError_ZDC0nXn[isel] ->Fill(MDzeroUPC.VZError);
            h_ZDCsumPlus_ZDC0nXn[isel]  ->Fill(MDzeroUPC.ZDCsumPlus);
            h_ZDCsumMinus_ZDC0nXn[isel] ->Fill(MDzeroUPC.ZDCsumMinus);
            h_HFEMaxPlus_ZDC0nXn[isel]  ->Fill(MDzeroUPC.HFEMaxPlus);
            h_HFEMaxMinus_ZDC0nXn[isel] ->Fill(MDzeroUPC.HFEMaxMinus);

            for (int iD = 0; iD < MDzeroUPC.Dpt->size(); ++iD) {
                bool passDSelection = MDzeroUPC.DpassCutNominal->at(iD) &&
                                      MDzeroUPC.Dpt->at(iD) > 1 && 
                                      ((MDzeroUPC.Dtrk1PixelHit->at(iD)+MDzeroUPC.Dtrk1StripHit->at(iD)) >= 11 &&
                                       (MDzeroUPC.Dtrk2PixelHit->at(iD)+MDzeroUPC.Dtrk2StripHit->at(iD)) >= 11) &&
                                      ( MDzeroUPC.Dtrk1PtErr->at(iD)/MDzeroUPC.Dtrk1Pt->at(iD) < 0.1 && 
                                        MDzeroUPC.Dtrk2PtErr->at(iD)/MDzeroUPC.Dtrk2Pt->at(iD) < 0.1) &&
                                      ((MDzeroUPC.Dtrk1MassHypo->at(iD) > 0.3)? 
                                       ((MDzeroUPC.Dtrk1KaonScore->at(iD) > -3 && MDzeroUPC.Dtrk1KaonScore->at(iD) < 3 && MDzeroUPC.Dtrk1P->at(iD) < 0.85) || 
                                         MDzeroUPC.Dtrk1P->at(iD) > 0.85) :
                                       ((MDzeroUPC.Dtrk2KaonScore->at(iD) > -3 && MDzeroUPC.Dtrk2KaonScore->at(iD) < 3 && MDzeroUPC.Dtrk2P->at(iD) < 0.85) || 
                                       MDzeroUPC.Dtrk2P->at(iD) > 0.85));
                if (!passDSelection) continue;
                h_Dmass_ZDC0nXn[isel]->Fill(MDzeroUPC.Dmass->at(iD));
            }
         }
      }

      // gammaN: ZDC_- > 1000, ZDC_+ < 1100
      if (MDzeroUPC.ZDCsumMinus > 1000 && MDzeroUPC.ZDCsumPlus < 1100)
      {
         evtsel[3] = evtsel[2] && (MDzeroUPC.HFEMaxPlus < 9.2);

         for (int isel = 0; isel < 4; isel++)
         {
            if (!evtsel[isel]) continue;
            countXn0n[isel]++;

            h_VX_ZDCXn0n[isel]      ->Fill(MDzeroUPC.VX);
            h_VY_ZDCXn0n[isel]      ->Fill(MDzeroUPC.VY);
            h_VZ_ZDCXn0n[isel]      ->Fill(MDzeroUPC.VZ);
            h_VXError_ZDCXn0n[isel] ->Fill(MDzeroUPC.VXError);
            h_VYError_ZDCXn0n[isel] ->Fill(MDzeroUPC.VYError);
            h_VZError_ZDCXn0n[isel] ->Fill(MDzeroUPC.VZError);
            h_ZDCsumPlus_ZDCXn0n[isel]  ->Fill(MDzeroUPC.ZDCsumPlus);
            h_ZDCsumMinus_ZDCXn0n[isel] ->Fill(MDzeroUPC.ZDCsumMinus);
            h_HFEMaxPlus_ZDCXn0n[isel]  ->Fill(MDzeroUPC.HFEMaxPlus);
            h_HFEMaxMinus_ZDCXn0n[isel] ->Fill(MDzeroUPC.HFEMaxMinus);

            for (int iD = 0; iD < MDzeroUPC.Dpt->size(); ++iD) {
                bool passDSelection = MDzeroUPC.DpassCutNominal->at(iD) &&
                                      MDzeroUPC.Dpt->at(iD) > 1 && 
                                      ((MDzeroUPC.Dtrk1PixelHit->at(iD)+MDzeroUPC.Dtrk1StripHit->at(iD)) >= 11 &&
                                       (MDzeroUPC.Dtrk2PixelHit->at(iD)+MDzeroUPC.Dtrk2StripHit->at(iD)) >= 11) &&
                                      ( MDzeroUPC.Dtrk1PtErr->at(iD)/MDzeroUPC.Dtrk1Pt->at(iD) < 0.1 && 
                                        MDzeroUPC.Dtrk2PtErr->at(iD)/MDzeroUPC.Dtrk2Pt->at(iD) < 0.1) &&
                                      ((MDzeroUPC.Dtrk1MassHypo->at(iD) > 0.3)? 
                                       ((MDzeroUPC.Dtrk1KaonScore->at(iD) > -3 && MDzeroUPC.Dtrk1KaonScore->at(iD) < 3 && MDzeroUPC.Dtrk1P->at(iD) < 0.85) || 
                                         MDzeroUPC.Dtrk1P->at(iD) > 0.85) :
                                       ((MDzeroUPC.Dtrk2KaonScore->at(iD) > -3 && MDzeroUPC.Dtrk2KaonScore->at(iD) < 3 && MDzeroUPC.Dtrk2P->at(iD) < 0.85) || 
                                       MDzeroUPC.Dtrk2P->at(iD) > 0.85));
                if (!passDSelection) continue;
                h_Dmass_ZDCXn0n[isel]->Fill(MDzeroUPC.Dmass->at(iD));
            }
         }
      }
   }

   Bar.Update(EntryCount);
   Bar.Print();
   cout << endl;

   cout << "========================================" << endl;
   cout << " Event-category summary by selection level:" << endl;
   for (int i = 0; i < 4; i++)
   {
      cout << "  Selection " << i
           << " (" << SelLabel[i] << "): " << endl;
      cout << "     0nXn (Ngamma) : " << count0nXn[i] << endl;
      cout << "     Xn0n (gammaN) : " << countXn0n[i] << endl;
      cout << "     Ratio : " << count0nXn[i] / ((double)countXn0n[i]) << endl;
   }
   cout << "========================================" << endl;

   // Write histograms to file
   OutputFile.cd();
   for (int i = 0; i < 4; i++)
   {
      h_VX_ZDC0nXn[i]->Write();
      h_VY_ZDC0nXn[i]->Write();
      h_VZ_ZDC0nXn[i]->Write();
      h_VXError_ZDC0nXn[i]->Write();
      h_VYError_ZDC0nXn[i]->Write();
      h_VZError_ZDC0nXn[i]->Write();
      h_ZDCsumPlus_ZDC0nXn[i]->Write();
      h_ZDCsumMinus_ZDC0nXn[i]->Write();
      h_HFEMaxPlus_ZDC0nXn[i]->Write();
      h_HFEMaxMinus_ZDC0nXn[i]->Write();
      h_Dmass_ZDC0nXn[i]->Write();

      h_VX_ZDCXn0n[i]->Write();
      h_VY_ZDCXn0n[i]->Write();
      h_VZ_ZDCXn0n[i]->Write();
      h_VXError_ZDCXn0n[i]->Write();
      h_VYError_ZDCXn0n[i]->Write();
      h_VZError_ZDCXn0n[i]->Write();
      h_ZDCsumPlus_ZDCXn0n[i]->Write();
      h_ZDCsumMinus_ZDCXn0n[i]->Write();
      h_HFEMaxPlus_ZDCXn0n[i]->Write();
      h_HFEMaxMinus_ZDCXn0n[i]->Write();
      h_Dmass_ZDCXn0n[i]->Write();
   }

   for (int isel = 0; isel < 4; isel++)
   {
      DrawOverlay(h_VX_ZDC0nXn[isel],      h_VX_ZDCXn0n[isel],
         isel, SelLabel[isel], "VX");

      DrawOverlay(h_VY_ZDC0nXn[isel],      h_VY_ZDCXn0n[isel],
         isel, SelLabel[isel], "VY");

      DrawOverlay(h_VZ_ZDC0nXn[isel],      h_VZ_ZDCXn0n[isel],
         isel, SelLabel[isel], "VZ");

      // DrawOverlay(h_VXError_ZDC0nXn[isel], h_VXError_ZDCXn0n[isel],
      //    isel, SelLabel[isel], "VXError");

      // DrawOverlay(h_VYError_ZDC0nXn[isel], h_VYError_ZDCXn0n[isel],
      //    isel, SelLabel[isel], "VYError");

      // DrawOverlay(h_VZError_ZDC0nXn[isel], h_VZError_ZDCXn0n[isel],
      //    isel, SelLabel[isel], "VZError");

      DrawOverlay(h_ZDCsumPlus_ZDC0nXn[isel], h_ZDCsumPlus_ZDCXn0n[isel],
         isel, SelLabel[isel], "ZDCsumPlus");

      DrawOverlay(h_ZDCsumMinus_ZDC0nXn[isel], h_ZDCsumMinus_ZDCXn0n[isel],
         isel, SelLabel[isel], "ZDCsumMinus");

      DrawOverlay(h_HFEMaxPlus_ZDC0nXn[isel], h_HFEMaxPlus_ZDCXn0n[isel],
         isel, SelLabel[isel], "HFEMaxPlus");

      DrawOverlay(h_HFEMaxMinus_ZDC0nXn[isel], h_HFEMaxMinus_ZDCXn0n[isel],
         isel, SelLabel[isel], "HFEMaxMinus");

      DrawOverlay(h_Dmass_ZDC0nXn[isel], h_Dmass_ZDCXn0n[isel],
         isel, SelLabel[isel], "Dmass");
   }

   OutputFile.Close();
   InputFile.Close();

   return 0;
}
