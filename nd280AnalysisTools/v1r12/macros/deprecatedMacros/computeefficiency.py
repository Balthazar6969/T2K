#!/usr/bin/python

"""\
Python script to process trees from ComputeEfficiency.exe.
Blair Jamieson (August 2010)
"""

import sys
import math
import os
import logging
import optparse

import ROOT

def CalcEff( heff, hnum, hden ):
    """\
    Function to take ratio of hnum/hden and put it in heff, with
    proper error-bar calculation for efficiencies. Assumes all
    the histograms have the same binning
    """
    if ( not heff or
         not hnum or
         not hden ): return
    for ibin in range( 0 , hnum.GetSize() ):
        A   = hnum.GetBinContent( ibin )
        ApB = hden.GetBinContent( ibin )
        B   = ApB - A
        if ApB > 0:
            heff.SetBinContent( ibin, A/ApB )
            if B>=0:
                heff.SetBinError( ibin, math.sqrt( A*B/ApB/ApB/ApB ) )
    # some root niceties to make the default plotting nicer
    hnum.SetLineColor(ROOT.kRed)
    hnum.SetLineWidth(2)
    hnum.SetLineStyle(2)
    hnum.GetXaxis().SetLabelSize(0.06)
    hnum.GetYaxis().SetLabelSize(0.06)
    hnum.GetXaxis().SetTitleSize(0.06)
    hnum.GetYaxis().SetTitleSize(0.06)
    hnum.GetXaxis().SetLabelFont(132)
    hnum.GetYaxis().SetLabelFont(132)
    hnum.GetXaxis().SetTitleFont(132)
    hnum.GetYaxis().SetTitleFont(132)
    hnum.GetYaxis().SetTitleOffset(0.65)
    if (hnum.GetNbinsY()>1): hnum.SetDrawOption("colz")
    else: hnum.SetDrawOption("hist")
    #
    hden.SetLineColor(ROOT.kGreen+3)
    hden.SetLineWidth(2)
    hden.GetXaxis().SetLabelSize(0.06)
    hden.GetYaxis().SetLabelSize(0.06)
    hden.GetXaxis().SetTitleSize(0.06)
    hden.GetYaxis().SetTitleSize(0.06)
    hden.GetXaxis().SetLabelFont(132)
    hden.GetYaxis().SetLabelFont(132)
    hden.GetXaxis().SetTitleFont(132)
    hden.GetYaxis().SetTitleFont(132)
    hden.GetYaxis().SetTitleOffset(0.68)
    if (hden.GetNbinsY()>1): hden.SetDrawOption("colz")
    else: hden.SetDrawOption("hist")
    #
    heff.SetMarkerColor(ROOT.kBlue)
    heff.SetMarkerStyle(8)
    heff.SetLineColor(ROOT.kBlue)
    heff.SetLineWidth(2)
    heff.GetXaxis().SetLabelSize(0.06)
    heff.GetYaxis().SetLabelSize(0.06)
    heff.GetXaxis().SetTitleSize(0.06)
    heff.GetYaxis().SetTitleSize(0.06)
    heff.GetXaxis().SetLabelFont(132)
    heff.GetYaxis().SetLabelFont(132)
    heff.GetXaxis().SetTitleFont(132)
    heff.GetYaxis().SetTitleFont(132)
    heff.GetYaxis().SetTitleOffset(0.68)
    if (heff.GetNbinsY()>1): heff.SetDrawOption("colz")
    else: heff.SetDrawOption("e1")
    return 

def OverlayEff( eff, num, den ):
    """\
    Function to overlay numerator and denominator histograms,
    and plot efficiency underneith with same x-axis.  Returns
    the canvas with the overlay.
    """
    tcname = 'tc'+eff.GetName()
    tc = ROOT.TCanvas(tcname,eff.GetTitle(),600,600)
    tc.Divide(1,2,0.001,0.0001,0)
    pad1 = tc.GetPad(1)
    pad2 = tc.GetPad(2)
    pad1.SetBottomMargin(0)
    pad1.SetLeftMargin(0.14)
    pad1.SetRightMargin(0.03)
    pad2.SetTopMargin(0)
    pad2.SetBottomMargin(0.2)
    pad2.SetLeftMargin(0.14)
    pad2.SetRightMargin(0.03)
    pad1.SetLogy(1)
    pad1.cd()
    pad1.SetGridx()
    den.Draw()
    num.Draw("same")
    tl = pad1.BuildLegend()
    tl.SetFillColor(10)
    tl.SetBorderSize(0)
    tl.SetLineWidth(0)
    tl.SetTextFont(132)
    tl.SetTextSize(0.06)
    pad2.cd()
    pad2.SetGridx()
    eff.Draw()
    return tc


class EfficiencyComputation:
    """\
    Class to chain together ROOT.TTrees created by
    ComputeEfficiency.exe, and make histograms of the track matching
    efficiencies for the different detectors.
    """

    # keep track of number of instances, only load root library on first
    # instance
    instances = 0

    def __init__(self,files,outputfile,chi2cut=50.0):
        """\
        Initialization of the class takes list of root filenames as
        argument, and name of an output root file.  Calls methods to
        read the files, book the histograms, Process the chain,
        compute the efficiencies, and save the results.
        """

        # setup some root defaults
        ROOT.gROOT.SetStyle("Plain")
        ROOT.gStyle.SetHistTopMargin(0)
        # set the matching chi2 cut value
        self.Chi2CutValue = chi2cut
        #
        self.ChainFiles(files)
        self.BookHistograms(outputfile)
        self.ProcessChain()
        self.ComputeEfficiencies()
        self.OutputFile.Write()
        self.OutputFile.Close()

        return

    def MakeProject(self):
        """\        
        Method to use root TTree MakeClass to build library used to
        access the tree entries.
        """
        EfficiencyComputation.instances = EfficiencyComputation.instances + 1
        if (EfficiencyComputation.instances==1):
            self.Chain.MakeClass("eff")
            ROOT.gROOT.LoadMacro("eff.C++")
        self.EffTree = ROOT.eff( self.Chain )
        return


    def ChainFiles(self,files):
        """\
        Method to make TChain of trees called eff that are in the list
        of root files called files.
        """
        # Create the TChain
        self.Chain = ROOT.TChain("eff")
        
        # First check files exist
        for file in files:
            if os.access(file,os.R_OK)==True:
                # File exists so add it to the chain
                self.Chain.AddFile( file )
            else:
                logging.error("ChainFiles "+file+" does not exist")
                sys.exit(1)
        # Make the tree code
        self.MakeProject()
        return

    def BookHistograms(self,outputfile):
        """\
        Method to make output file, and book all of the histograms
        that will be filled and put into the output file.
        """
        self.OutputFile = ROOT.TFile(outputfile,"recreate")
        # Some basic distributions to determine appropriate cut levels
        # and total number of different kinds of matches that were made
        self.OutputFile.cd()
        matchdir = self.OutputFile.mkdir("MatchingChi2","Matching Chi2 Before Cuts")
        matchdir.cd()
        self.hMatchChi2_123_13 = ROOT.TH1F("hMatchChi2_123_13","Matching Chi2 TPC1/TPC2/TPC3 TPC1-TPC3",50,0.,100.0)
        self.hMatchChi2_123_12 = ROOT.TH1F("hMatchChi2_123_12","Matching Chi2 TPC1/TPC2/TPC3 TPC1-TPC2",50,0.,100.0)
        self.hMatchChi2_14p2_12 = ROOT.TH1F("hMatchChi2_14p2_12","Matching Chi2 TPC1/FGD1Hits/TPC2 TPC1-TPC2 +sense",50,0.,100.0)
        self.hMatchChi2_14p2_14 = ROOT.TH1F("hMatchChi2_14p2_14","Matching Chi2 TPC1/FGD1Hits/TPC2 TPC1-FGD1 +sense",50,0.,100.0)
        self.hMatchChi2_14m2_12 = ROOT.TH1F("hMatchChi2_14m2_12","Matching Chi2 TPC1/FGD1Hits/TPC2 TPC1-TPC2 -sense",50,0.,100.0)
        self.hMatchChi2_14m2_14 = ROOT.TH1F("hMatchChi2_14m2_14","Matching Chi2 TPC1/FGD1Hits/TPC2 TPC1-FGD1 -sense",50,0.,100.0)
        self.hMatchChi2_25p3_23 = ROOT.TH1F("hMatchChi2_25p3_23","Matching Chi2 TPC2/FGD2Hits/TPC3 TPC2-TPC3 +sense",50,0.,100.0)
        self.hMatchChi2_25p3_25 = ROOT.TH1F("hMatchChi2_25p3_25","Matching Chi2 TPC2/FGD2Hits/TPC3 TPC2-FGD2 +sense",50,0.,100.0)
        self.hMatchChi2_25m3_23 = ROOT.TH1F("hMatchChi2_25m3_23","Matching Chi2 TPC2/FGD2Hits/TPC3 TPC2-TPC3 -sense",50,0.,100.0)
        self.hMatchChi2_25m3_25 = ROOT.TH1F("hMatchChi2_25m3_25","Matching Chi2 TPC2/FGD2Hits/TPC3 TPC2-FGD2 -sense",50,0.,100.0)
        
        # Make dictionaries of histograms for each detector
        
        self.dictTPC1hists = self.MakeDetectorHistograms('TPC1')
        self.dictTPC2hists = self.MakeDetectorHistograms('TPC2')
        self.dictTPC3hists = self.MakeDetectorHistograms('TPC3')
        
        self.dictFGD1phists = self.MakeDetectorHistograms('FGD1p')
        self.dictFGD2phists = self.MakeDetectorHistograms('FGD2p')

        self.dictFGD1mhists = self.MakeDetectorHistograms('FGD1m')
        self.dictFGD2mhists = self.MakeDetectorHistograms('FGD2m')

        return

    def MakeDetectorHistograms(self,det='TPC2'):
        """\
        Method to make efficiency histograms for a given detector.
        That way the method can be called for each detector to save
        some lines of code. Returns a dictionary of histogram name
        and histogram pointer.
        """
        dict = {}
        self.OutputFile.cd()
        detdir = self.OutputFile.mkdir(det,det+" Matching Efficiency Plots")
        detdir.cd()
        detnumdir = detdir.mkdir("numerator",det+" Numerator Plots")
        detnumdir.cd()
        dict['pq_num']    = ROOT.TH1F("h"+det+"pq_num"   ,det+" P/q numerator"          ,50,-5000.,5000.)
        dict['costh_num'] = ROOT.TH1F("h"+det+"costh_num",det+" cos#theta numerator"    ,50,-1.,1.)
        dict['phi_num']   = ROOT.TH1F("h"+det+"phi_num"  ,det+" #phi numerator"         ,50,-math.pi,math.pi)
        dict['nhits_num'] = ROOT.TH1F("h"+det+"nhits_num",det+" Nhits numerator"        ,50,0.,100.)
        dict['pid_num']   = ROOT.TH1F("h"+det+"pid_num"  ,det+" e=1,#mu=2,p=3 numerator",7,-3.5,3.5)
        dict['xy_num']    = ROOT.TH2F("h"+det+"xy_num"   ,det+" Y vs X numerator"       ,12,-1000.,1000.,12,-1000.,1000.)
        #
        detnumeledir = detnumdir.mkdir("electrons",det+" Electron-Like Numerator Plots")
        detnumeledir.cd()
        dict['pqe_num']    = ROOT.TH1F("h"+det+"pqe_num"   ,det+" P/q e-like numerator"      ,50,-5000.,5000.)
        dict['costhe_num'] = ROOT.TH1F("h"+det+"costhe_num",det+" cos#theta e-like numerator",50,-1.,1.)
        dict['phie_num']   = ROOT.TH1F("h"+det+"phie_num"  ,det+" #phi e-like numerator"     ,50,-math.pi,math.pi)
        dict['nhitse_num'] = ROOT.TH1F("h"+det+"nhitse_num",det+" Nhits e-like numerator"    ,50,0.,100.)
        #
        detnummudir = detnumdir.mkdir("muons",det+" Muon-Like Numerator Plots")
        detnummudir.cd()        
        dict['pqmu_num']    = ROOT.TH1F("h"+det+"pqmu_num"   ,det+" P/q #mu-like numerator"       ,50,-5000.,5000.)
        dict['costhmu_num'] = ROOT.TH1F("h"+det+"costhmu_num",det+" cos#theta #mu-like numerator" ,50,-1.,1.)
        dict['phimu_num']   = ROOT.TH1F("h"+det+"phimu_num"  ,det+" #phi #mu-like numerator"      ,50,-math.pi,math.pi)
        dict['nhitsmu_num'] = ROOT.TH1F("h"+det+"nhitsmu_num",det+" Nhits #mu-like numerator"     ,50,0.,100.)
        #
        detnumpdir = detnumdir.mkdir("protons",det+" Proton-Like Numerator Plots")
        detnumpdir.cd()
        dict['pqp_num']    = ROOT.TH1F("h"+det+"pqp_num"   ,det+" P/q proton-like numerator"      ,50,0.,5000.)
        dict['costhp_num'] = ROOT.TH1F("h"+det+"costhp_num",det+" cos#theta proton-like numerator",50,-1.,1.)
        dict['phip_num']   = ROOT.TH1F("h"+det+"phip_num"  ,det+" #phi proton-like numerator"     ,50,-math.pi,math.pi)
        dict['nhitsp_num'] = ROOT.TH1F("h"+det+"nhitsp_num",det+" Nhits proton-like numerator"    ,50,0.,100.)
        #
        # denominator plots
        detdir.cd()
        detdendir = detdir.mkdir("denominator",det+" Denominator Plots")
        detdendir.cd()
        dict['pq_den']    = ROOT.TH1F("h"+det+"pq_den"   ,det+" P/q denominator;;Events"          ,50,-5000.,5000.)
        dict['costh_den'] = ROOT.TH1F("h"+det+"costh_den",det+" cos#theta denominator;;Events"    ,50,-1.,1.)
        dict['phi_den']   = ROOT.TH1F("h"+det+"phi_den"  ,det+" #phi denominator;;Events"         ,50,-math.pi,math.pi)
        dict['nhits_den'] = ROOT.TH1F("h"+det+"nhits_den",det+" Nhits denominator;;Events"        ,50,0.,100.)
        dict['pid_den']   = ROOT.TH1F("h"+det+"pid_den"  ,det+" e=1,#mu=2,p=3 denominator;;Events",7,-3.5,3.5)
        dict['xy_den']    = ROOT.TH2F("h"+det+"xy_den"   ,det+" Y vs X denominator;;Events"       ,12,-1000.,1000.,12,-1000.,1000.)
        # 
        detdeneledir = detdendir.mkdir("electrons",det+" Electron-Like Denominator Plots")
        detdeneledir.cd()
        dict['pqe_den']    = ROOT.TH1F("h"+det+"pqe_den"   ,det+" P/q e-like denominator;;Events"      ,50,-5000.,5000.)
        dict['costhe_den'] = ROOT.TH1F("h"+det+"costhe_den",det+" cos#theta e-like denominator;;Events",50,-1.,1.)
        dict['phie_den']   = ROOT.TH1F("h"+det+"phie_den"  ,det+" #phi e-like denominator;;Events"     ,50,-math.pi,math.pi)
        dict['nhitse_den'] = ROOT.TH1F("h"+det+"nhitse_den",det+" Nhits e-like denominator;;Events"    ,50,0.,100.)
        #
        detdenmudir = detdendir.mkdir("muons",det+" Muon-Like Denominator Plots")
        detdenmudir.cd()
        dict['pqmu_den']    = ROOT.TH1F("h"+det+"pqmu_den"   ,det+" P/q #mu-like denominator;;Events"       ,50,-5000.,5000.)
        dict['costhmu_den'] = ROOT.TH1F("h"+det+"costhmu_den",det+" cos#theta #mu-like denominator;;Events" ,50,-1.,1.)
        dict['phimu_den']   = ROOT.TH1F("h"+det+"phimu_den"  ,det+" #phi #mu-like denominator;;Events"      ,50,-math.pi,math.pi)
        dict['nhitsmu_den'] = ROOT.TH1F("h"+det+"nhitsmu_den",det+" Nhits #mu-like denominator;;Events"     ,50,0.,100.)
        #
        detdenpdir = detdendir.mkdir("protons",det+" Proton-Like Denominator Plots")
        detdenpdir.cd()
        dict['pqp_den']    = ROOT.TH1F("h"+det+"pqp_den"   ,det+" P/q proton-like denominator;;Events"      ,50,0.,5000.)
        dict['costhp_den'] = ROOT.TH1F("h"+det+"costhp_den",det+" cos#theta proton-like denominator;;Events",50,-1.,1.)
        dict['phip_den']   = ROOT.TH1F("h"+det+"phip_den"  ,det+" #phi proton-like denominator;;Events"     ,50,-math.pi,math.pi)
        dict['nhitsp_den'] = ROOT.TH1F("h"+det+"nhitsp_den",det+" Nhits proton-like denominator;;Events"    ,50,0.,100.)
        #
        # efficiency plots
        detdir.cd()
        deteffdir = detdir.mkdir("efficiency",det+" Efficiency Plots")
        deteffdir.cd()
        dict['pq_eff']    = ROOT.TH1F("h"+det+"pq_eff"   ,det+" P/q efficiency          ; P/q (MeV/c); "+det+" efficiency   ",50,-5000.,5000.)
        dict['costh_eff'] = ROOT.TH1F("h"+det+"costh_eff",det+" cos#theta efficiency    ; cos#theta; "+det+" efficiency    ",50,-1.,1.)
        dict['phi_eff']   = ROOT.TH1F("h"+det+"phi_eff"  ,det+" #phi efficiency         ; #phi (rad); "+det+" efficiency         ",50,-math.pi,math.pi)
        dict['nhits_eff'] = ROOT.TH1F("h"+det+"nhits_eff",det+" Nhits efficiency        ; Nhits; "+det+" efficiency        ",50,0.,100.)
        dict['pid_eff']   = ROOT.TH1F("h"+det+"pid_eff"  ,det+" e=1,#mu=2,p=3 efficiency; pid (e=1,#mu=2,p=3);"+det+" efficiency",7,-3.5,3.5)
        dict['xy_eff']    = ROOT.TH2F("h"+det+"xy_eff"   ,det+" Y vs X efficiency       ; X (mm) ; Y (mm); "+det+" efficiency       ",12,-1000.,1000.,12,-1000.,1000.)
        deteffdir.Add( OverlayEff( dict['pq_eff']    , dict['pq_num']    ,dict['pq_den']    ) )
        deteffdir.Add( OverlayEff( dict['costh_eff'] , dict['costh_num'] ,dict['costh_den'] ) ) 
        deteffdir.Add( OverlayEff( dict['phi_eff']   , dict['phi_num']   ,dict['phi_den']   ) )
        deteffdir.Add( OverlayEff( dict['nhits_eff'] , dict['nhits_num'] ,dict['nhits_den'] ) )
        deteffdir.Add( OverlayEff( dict['pid_eff']   , dict['pid_num']   ,dict['pid_den']   ) )
        # 
        deteffeledir = deteffdir.mkdir("electrons",det+" Electron-Like Efficiency Plots")
        deteffeledir.cd()
        dict['pqe_eff']    = ROOT.TH1F("h"+det+"pqe_eff"   ,det+" P/q e-like efficiency      ;P/q (MeV/c) e-like ;"+det+" efficiency      ",50,-5000.,5000.)
        dict['costhe_eff'] = ROOT.TH1F("h"+det+"costhe_eff",det+" cos#theta e-like efficiency;cos#theta e-like ;"+det+" efficiency",50,-1.,1.)
        dict['phie_eff']   = ROOT.TH1F("h"+det+"phie_eff"  ,det+" #phi e-like efficiency     ;#phi (rad) e-like ;"+det+" efficiency     ",50,-math.pi,math.pi)
        dict['nhitse_eff'] = ROOT.TH1F("h"+det+"nhitse_eff",det+" Nhits e-like efficiency    ;Nhits e-like ;"+det+" efficiency    ",50,0.,100.)
        deteffeledir.Add( OverlayEff( dict['pqe_eff']    , dict['pqe_num']    ,dict['pqe_den']    ) )
        deteffeledir.Add( OverlayEff( dict['costhe_eff'] , dict['costhe_num'] ,dict['costhe_den'] ) )
        deteffeledir.Add( OverlayEff( dict['phie_eff']   , dict['phie_num']   ,dict['phie_den']   ) )
        deteffeledir.Add( OverlayEff( dict['nhitse_eff'] , dict['nhitse_num'] ,dict['nhitse_den'] ) )
        #
        deteffmudir = deteffdir.mkdir("muons",det+" Muon-Like Efficiency Plots")
        deteffmudir.cd()
        dict['pqmu_eff']    = ROOT.TH1F("h"+det+"pqmu_eff"   ,det+" P/q #mu-like efficiency      ;P/q (MeV/c) #mu-like;"+det+" efficiency      "  ,50,-5000.,5000.)
        dict['costhmu_eff'] = ROOT.TH1F("h"+det+"costhmu_eff",det+" cos#theta #mu-like efficiency;cos#theta #mu-like;"+det+" efficiency" ,50,-1.,1.)
        dict['phimu_eff']   = ROOT.TH1F("h"+det+"phimu_eff"  ,det+" #phi #mu-like efficiency     ;#phi (rad) #mu-like;"+det+" efficiency     " ,50,-math.pi,math.pi)
        dict['nhitsmu_eff'] = ROOT.TH1F("h"+det+"nhitsmu_eff",det+" Nhits #mu-like efficiency    ;Nhits #mu-like;"+det+" efficiency    " ,50,0.,100.)
        deteffmudir.Add( OverlayEff( dict['pqmu_eff']    , dict['pqmu_num']    ,dict['pqmu_den']    ) )
        deteffmudir.Add( OverlayEff( dict['costhmu_eff'] , dict['costhmu_num'] ,dict['costhmu_den'] ) )
        deteffmudir.Add( OverlayEff( dict['phimu_eff']   , dict['phimu_num']   ,dict['phimu_den']   ) )
        deteffmudir.Add( OverlayEff( dict['nhitsmu_eff'] , dict['nhitsmu_num'] ,dict['nhitsmu_den'] ) )
        #
        deteffpdir = deteffdir.mkdir("protons",det+" Proton-Like Efficiency Plots")
        deteffpdir.cd()
        dict['pqp_eff']    = ROOT.TH1F("h"+det+"pqp_eff"   ,det+" P/q proton-like efficiency      ;P/q (MeV/c) proton-like;"+det+" efficiency      ",50,0.,5000.)
        dict['costhp_eff'] = ROOT.TH1F("h"+det+"costhp_eff",det+" cos#theta proton-like efficiency;cos#theta proton-like;"+det+" efficiency",50,-1.,1.)
        dict['phip_eff']   = ROOT.TH1F("h"+det+"phip_eff"  ,det+" #phi proton-like efficiency     ;#phi (rad) proton-like;"+det+" efficiency     ",50,-math.pi,math.pi)
        dict['nhitsp_eff'] = ROOT.TH1F("h"+det+"nhitsp_eff",det+" Nhits proton-like efficiency    ;Nhits proton-like;"+det+" efficiency    ",50,0.,100.)
        deteffpdir.Add( OverlayEff( dict['pqp_eff']    , dict['pqp_num']    ,dict['pqp_den']    ) )
        deteffpdir.Add( OverlayEff( dict['costhp_eff'] , dict['costhp_num'] ,dict['costhp_den'] ) )
        deteffpdir.Add( OverlayEff( dict['phip_eff']   , dict['phip_num']   ,dict['phip_den']   ) )
        deteffpdir.Add( OverlayEff( dict['nhitsp_eff'] , dict['nhitsp_num'] ,dict['nhitsp_den'] ) )

        # residual plots for TPC1-TPC3 matching

        # residual plots for TPC1-TPC2 matching

        self.OutputFile.cd()
        return dict
        
    def ProcessChain(self):
        """\
        Method to loop over entries in TChain, and fill all of the
        booked histograms.
        """
        for entry in xrange( self.Chain.GetEntries() ):
            if self.EffTree.GetEntry( entry ) < 0: break
            # Say something every 100 events                                        
            if entry % 100 == 0: print "Events Processed " + str(entry)
            # Set shortcut name for the tree entry
            eff = self.EffTree
            
            # fill the chi2 for different kinds of matches done
            if ( eff.det[0] == 1 and eff.det[1] == 2 and eff.det[2] == 3):
                self.hMatchChi2_123_13.Fill( eff.matchchi2[0] )
                self.hMatchChi2_123_12.Fill( eff.matchchi2[1] )
            if ( eff.det[0] == 1 and eff.det[1] == 4 and eff.det[2] == 2):
                # check if match to FGD1 hits or track?
                if ( eff.addfgdhits == 1 ): # hits in plus sense
                    self.hMatchChi2_14p2_12.Fill( eff.matchchi2[0] )
                    self.hMatchChi2_14p2_14.Fill( eff.matchchi2[1] )
            if ( eff.det[0] == 2 and eff.det[1] == 4 and eff.det[2] == 1): # hits in minus sense
                self.hMatchChi2_14m2_12.Fill( eff.matchchi2[0] )
                self.hMatchChi2_14m2_14.Fill( eff.matchchi2[1] )
            if ( eff.det[0] == 2 and eff.det[1] == 5 and eff.det[2] == 3):
                # check if match to FGD1 hits or track?
                if ( eff.addfgdhits == 1 ): # hits in plus sense
                    self.hMatchChi2_25p3_23.Fill( eff.matchchi2[0] )
                    self.hMatchChi2_25p3_25.Fill( eff.matchchi2[1] )
            if ( eff.det[0] == 3 and eff.det[1] == 5 and eff.det[2] == 2): # hits in minus sense
                self.hMatchChi2_25m3_23.Fill( eff.matchchi2[0] )
                self.hMatchChi2_25m3_25.Fill( eff.matchchi2[1] )

            # Fill plots for efficiency calculation for TPC2:
            # uses tpc1-tpc2-tpc3 matches between tpc1-tpc3, then tpc1-tpc2
            self.FillHistograms( self.dictTPC2hists,
                                 det1 = 1,
                                 det2 = 2,
                                 det3 = 3,
                                 addfgdhits = 0,
                                 refidx = 0,
                                 reftrackidx = 0,
                                 effidx = 1 )

            # Fill plots for efficiency calculation for TPC1:
            # uses tpc2,fgd1,tpc1 matches between TPC2-TPC1, then TPC2-FGD1
            self.FillHistograms( self.dictTPC1hists,
                                 det1 = 2,
                                 det2 = 4,
                                 det3 = 1,
                                 addfgdhits = 1,
                                 refidx = 1,
                                 reftrackidx = 1,
                                 effidx = 2 )

            # Fill plots for efficiency calculation for TPC3:
            # uses tpc1-tpc2-tpc3 matches beteween tpc1-tpc3, then tpc1-tpc2 ***
            self.FillHistograms( self.dictTPC3hists,
                                 det1 = 1,
                                 det2 = 2,
                                 det3 = 3,
                                 addfgdhits = 0,
                                 refidx = 1,
                                 reftrackidx = 1,
                                 effidx = 2 )

            # Fill plots for efficiency calculation for FGD1 plus sense
            # use tpc1-fgd1-tpc2 matches between TPC1-TPC2, then TPC1-FGD1
            self.FillHistograms( self.dictFGD1phists,
                                 det1 = 1,
                                 det2 = 4,
                                 det3 = 2,
                                 addfgdhits = 1,
                                 refidx = 0,
                                 reftrackidx = 0,
                                 effidx = 1 )

            # Fill plots for efficiency calculation for FGD2 plus sense
            # use tpc2-fgd2-tpc3 matches between TPC2-TPC3, then TPC2-FGD2
            self.FillHistograms( self.dictFGD2phists,
                                 det1 = 2,
                                 det2 = 5,
                                 det3 = 3,
                                 addfgdhits = 1,
                                 refidx = 0,
                                 reftrackidx = 0,
                                 effidx = 1 )


            # Fill plots for efficiency calculation for FGD1 minus sense
            # use tpc2-fgd1-tpc1 matches between TPC2-TPC1, then TPC2-FGD1
            self.FillHistograms( self.dictFGD1mhists,
                                 det1 = 2,
                                 det2 = 4,
                                 det3 = 1,
                                 addfgdhits = 1,
                                 refidx = 0,
                                 reftrackidx = 0,
                                 effidx = 1 )

            # Fill plots for efficiency calculation for FGD2 minus sense
            # use tpc3-fgd2-tpc2 matches between TPC3-TPC2, then TPC3-FGD2
            self.FillHistograms( self.dictFGD2mhists,
                                 det1 = 3,
                                 det2 = 5,
                                 det3 = 2,
                                 addfgdhits = 1,
                                 refidx = 0,
                                 reftrackidx = 0,
                                 effidx = 1 )

        return

    def FillHistograms( self, dict, det1, det2, det3, addfgdhits, refidx, reftrackidx, effidx ):
        """\
        Method to fill histograms in dictionary dict, with matches
        with detector in index 0 of det1, detector in index 1 of
        det2, and detector in index 2 of det3.  The tree entries
        with value addfgdhits only will be checked.  The
        chi2matching for the reference matching is in index
        refidx, and the index to the reference track is effidx.
        The index to the track whose matching efficiency is being
        estimated is effidx.
        """
        # Set shortcut name for the tree entry
        eff = self.EffTree
        # get attempts to match det1,det2,det3 tracks
        # efficiency of matching between det1 and det2 will be tested,
        # so require a good track in det1!
        if ( eff.det[0] == det1 and
             eff.det[1] == det2 and
             eff.det[2] == det3 and
             eff.nhits[ reftrackidx ] > 0 ):
            # check match between det1 and det3
            if ( eff.matchchi2[ refidx ] < self.Chi2CutValue ):
                # fill denominators
                dict['pq_den'].Fill( eff.p[reftrackidx] * eff.q[reftrackidx] )
                dict['costh_den'].Fill( eff.tz[reftrackidx] )
                dict['phi_den'].Fill( math.atan2( eff.ty[reftrackidx], eff.tx[reftrackidx] ) )
                dict['nhits_den'].Fill( eff.nhits[effidx] )
                dict['xy_den'].Fill( eff.x[reftrackidx], eff.y[reftrackidx] )
                # check if this is electron like?
                if ( eff.pullmu[reftrackidx] > 2.5 and eff.pulle[reftrackidx] > -1.0 and eff.pulle[reftrackidx] < 2.0 ):
                    dict['pid_den'].Fill( 1.0*eff.q[reftrackidx] )
                    dict['pqe_den'].Fill( eff.p[reftrackidx] * eff.q[reftrackidx] )
                    dict['costhe_den'].Fill( eff.tz[reftrackidx] )
                    dict['phie_den'].Fill( math.atan2( eff.ty[reftrackidx], eff.tx[reftrackidx] ) )
                    dict['nhitse_den'].Fill( eff.nhits[effidx] )

                # check if this is muon like?
                if ( eff.p[reftrackidx] > 200.0 and eff.pullmu[reftrackidx] > -2.0 and eff.pullmu[reftrackidx] < 2.0 ):
                    dict['pid_den'].Fill( 2.0*eff.q[reftrackidx] )
                    dict['pqmu_den'].Fill( eff.p[reftrackidx] * eff.q[reftrackidx] )
                    dict['costhmu_den'].Fill( eff.tz[reftrackidx] )
                    dict['phimu_den'].Fill( math.atan2( eff.ty[reftrackidx], eff.tx[reftrackidx] ) )
                    dict['nhitsmu_den'].Fill( eff.nhits[effidx] )
                        
                # check if this is proton like?
                if ( eff.p[reftrackidx] > 200.0 and eff.q[reftrackidx] > 0 and eff.pullp[reftrackidx] > -2.0 and eff.pullp[reftrackidx] < 2.0 ):
                    dict['pid_den'].Fill( 3.0 )
                    dict['pqp_den'].Fill( eff.p[reftrackidx] * eff.q[reftrackidx] )
                    dict['costhp_den'].Fill( eff.tz[reftrackidx] )
                    dict['phip_den'].Fill( math.atan2( eff.ty[reftrackidx], eff.tx[reftrackidx] ) )
                    dict['nhitsp_den'].Fill( eff.nhits[effidx] )
                    
                # Now check det1 det2 matching to get det2 efficiency (numerator)
                if ( eff.matchchi2[ (refidx+1)%2 ] < self.Chi2CutValue and eff.nhits[effidx] > 0 ):
                    # fill numerators
                    dict['pq_num'].Fill( eff.p[reftrackidx] * eff.q[reftrackidx] )
                    dict['costh_num'].Fill( eff.tz[reftrackidx] )
                    dict['phi_num'].Fill( math.atan2( eff.ty[reftrackidx], eff.tx[reftrackidx] ) )
                    dict['nhits_num'].Fill( eff.nhits[effidx] )
                    dict['xy_num'].Fill( eff.x[reftrackidx], eff.y[reftrackidx] )
                    # check if this is electron like?
                    if ( eff.pullmu[reftrackidx] > 2.5 and eff.pulle[reftrackidx] > -1.0 and eff.pulle[reftrackidx] < 2.0 ):
                        dict['pid_num'].Fill( 1.0*eff.q[reftrackidx] )
                        dict['pqe_num'].Fill( eff.p[reftrackidx] * eff.q[reftrackidx] )
                        dict['costhe_num'].Fill( eff.tz[reftrackidx] )
                        dict['phie_num'].Fill( math.atan2( eff.ty[reftrackidx], eff.tx[reftrackidx] ) )
                        dict['nhitse_num'].Fill( eff.nhits[effidx] )
                    # check if this is muon like?
                    if ( eff.p[reftrackidx] > 200.0 and eff.pullmu[reftrackidx] > -2.0 and eff.pullmu[reftrackidx] < 2.0 ):
                        dict['pid_num'].Fill( 2.0*eff.q[reftrackidx] )
                        dict['pqmu_num'].Fill( eff.p[reftrackidx] * eff.q[reftrackidx] )
                        dict['costhmu_num'].Fill( eff.tz[reftrackidx] )
                        dict['phimu_num'].Fill( math.atan2( eff.ty[reftrackidx], eff.tx[reftrackidx] ) )
                        dict['nhitsmu_num'].Fill( eff.nhits[effidx] )
                    # check if this is proton like?
                    if ( eff.p[reftrackidx] > 200.0 and eff.q[reftrackidx] > 0 and eff.pullp[reftrackidx] > -2.0 and eff.pullp[reftrackidx] < 2.0 ):
                        dict['pid_num'].Fill( 3.0 )
                        dict['pqp_num'].Fill( eff.p[reftrackidx] * eff.q[reftrackidx] )
                        dict['costhp_num'].Fill( eff.tz[reftrackidx] )
                        dict['phip_num'].Fill( math.atan2( eff.ty[reftrackidx], eff.tx[reftrackidx] ) )
                        dict['nhitsp_num'].Fill( eff.nhits[effidx] )
        return


    def ComputeEfficiencies(self):
        """\
        Method to take ratios of all of the numerator and denominator
        histograms to make efficiency histograms.
        """
        self.ComputeEff( self.dictTPC1hists )
        self.ComputeEff( self.dictTPC2hists )
        self.ComputeEff( self.dictTPC3hists )
        self.ComputeEff( self.dictFGD1phists )
        self.ComputeEff( self.dictFGD2phists )
        self.ComputeEff( self.dictFGD1mhists )
        self.ComputeEff( self.dictFGD2mhists )

        return

    def ComputeEff(self, dict):
        """\
        Method to take ratios of all the numerator and denominator
        histograms for a give detector.  Argument is dictionary of
        histograms for the detetector.
        """
        CalcEff( dict['pq_eff']    , dict['pq_num']    ,dict['pq_den']    )
        CalcEff( dict['costh_eff'] , dict['costh_num'] ,dict['costh_den'] )
        CalcEff( dict['phi_eff']   , dict['phi_num']   ,dict['phi_den']   )
        CalcEff( dict['nhits_eff'] , dict['nhits_num'] ,dict['nhits_den'] )
        CalcEff( dict['pid_eff']   , dict['pid_num']   ,dict['pid_den']   )
        CalcEff( dict['xy_eff']    , dict['xy_num']    ,dict['xy_den']    )

        CalcEff( dict['pqe_eff']    , dict['pqe_num']    ,dict['pqe_den']    )
        CalcEff( dict['costhe_eff'] , dict['costhe_num'] ,dict['costhe_den'] )
        CalcEff( dict['phie_eff']   , dict['phie_num']   ,dict['phie_den']   )
        CalcEff( dict['nhitse_eff'] , dict['nhitse_num'] ,dict['nhitse_den'] )

        CalcEff( dict['pqmu_eff']    , dict['pqmu_num']    ,dict['pqmu_den']    )
        CalcEff( dict['costhmu_eff'] , dict['costhmu_num'] ,dict['costhmu_den'] )
        CalcEff( dict['phimu_eff']   , dict['phimu_num']   ,dict['phimu_den']   )
        CalcEff( dict['nhitsmu_eff'] , dict['nhitsmu_num'] ,dict['nhitsmu_den'] )

        CalcEff( dict['pqp_eff']    , dict['pqp_num']    ,dict['pqp_den']    )
        CalcEff( dict['costhp_eff'] , dict['costhp_num'] ,dict['costhp_den'] )
        CalcEff( dict['phip_eff']   , dict['phip_num']   ,dict['phip_den']   )
        CalcEff( dict['nhitsp_eff'] , dict['nhitsp_num'] ,dict['nhitsp_den'] )
        return

def DoAnalysis(files,outname,chi2cut):
    """\
    Very simple function to make sure instance goes out of
    scope, before next analysis is started.  Not sure if this
    is really necessary?
    """
    comp = EfficiencyComputation( files, outname, chi2cut )
    return
        
# Do the analysis

# First parse the input file names from the comand line
optionParser = optparse.OptionParser()
(options,args) = optionParser.parse_args()
if len(args) < 1:
    logging.critical("Need at least one input file")
    sys.exit(1)

# Now do the efficiency computation for several different
# values of the chi2 cut
DoAnalysis( args, "computeefficiency20.root", 20 )
DoAnalysis( args, "computeefficiency40.root", 40 )
DoAnalysis( args, "computeefficiency60.root", 60 )
DoAnalysis( args, "computeefficiency80.root", 80 )

