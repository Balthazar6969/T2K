import sys
import optparse
import os
import math 
import logging
import ROOT
from userAnalysisBase import userAnalysisBase 

# class name must be the same as the filename without the .py
class trackerReconAnalysis(userAnalysisBase): # inherits from userAnalysisBase
    """\
    trackerReconAnalysis.py
    Plot everything from trackerRecon to see that it is collected okay into
    oaAnalysis framework. 
    """

    ### __init__ gets called automatically upon instantiation
    def __init__(self, options):
        """\
        Specifies further trees to be added to oaAnalysisModuleTChains, which is
        set up in the base class userAnalysisBase.__init__
        Needs to be passed the command line options from optionParser.
        """
        ### Call __init__ in base class
        userAnalysisBase.__init__(self, options)
        # Truth information about all the vertices
        self.truthVertices = ROOT.TChain("TruthDir/Vertices")
        self.oaAnalysisModuleTChains.append(self.truthVertices)
        # Tracker Reconstruction
        self.reconTracker = ROOT.TChain("ReconDir/Tracker")
        self.oaAnalysisModuleTChains.append(self.reconTracker)
        # Event information
        self.eventHeader = ROOT.TChain("HeaderDir/BasicHeader")
        self.oaAnalysisModuleTChains.append(self.eventHeader)

    def BookHistogramsAndAnalysisSteps(self):
        # Define some histograms
        # Overall statistics:
        self.hNTracks   = ROOT.TH1F("hNTracks"   ,"Number of trackerRecon Tracks"    ,21,-0.5,20.5)
        self.hNFGDOther = ROOT.TH1F("hNFGDOther" ,"Number of trackerRecon FGD Other" ,21,-0.5,20.5)
        self.hNTPCOther = ROOT.TH1F("hTPCOther"  ,"Number of trackerRecon TPC Other" ,21,-0.5,20.5)
        self.hNTPCIso   = ROOT.TH1F("hTPCIso"    ,"Number of trackerRecon TPC Iso"   ,21,-0.5,20.5)
        self.hNFGDUnused= ROOT.TH1F("hNFGDUnused","Number of trackerRecon FGD Unused",101,-0.5,100.5)
        self.hNTPCUnused= ROOT.TH1F("hNTPCUnused","Number of trackerRecon TPC Unused",101,-0.5,100.5)
        self.statsHistos = [
            self.hNTracks   ,
            self.hNFGDOther ,
            self.hNTPCOther  ,
            self.hNTPCIso    ,
            self.hNFGDUnused,
            self.hNTPCUnused ]
        # TPC Other histograms
        self.hTPCOtherDet      = ROOT.TH1F("hTPCOtherDet"     ,"TPC With other track"   ,3,0.5,3.5)
        self.hTPC1OtherNhits   = ROOT.TH1F("hTPC1OtherNhits"  ,"TPC1 other track Nhits" ,73,-0.5,72.5)
        self.hTPC2OtherNhits   = ROOT.TH1F("hTPC2OtherNhits"  ,"TPC2 other track Nhits" ,73,-0.5,72.5)
        self.hTPC3OtherNhits   = ROOT.TH1F("hTPC3OtherNhits"  ,"TPC3 other track Nhits" ,73,-0.5,72.5)
        self.hTPC1OtherXYFront = ROOT.TH2F("hTPC1OtherXYFront","TPC1 other XY front"    ,50,-1100.,1100.,50,-1100.,1100.)
        self.hTPC1OtherXYBack  = ROOT.TH2F("hTPC1OtherXYBack" ,"TPC1 other XY back"     ,50,-1100.,1100.,50,-1100.,1100.)
        self.hTPC2OtherXYFront = ROOT.TH2F("hTPC2OtherXYFront","TPC2 other XY front"    ,50,-1100.,1100.,50,-1100.,1100.)
        self.hTPC2OtherXYBack  = ROOT.TH2F("hTPC2OtherXYBack" ,"TPC2 other XY back"     ,50,-1100.,1100.,50,-1100.,1100.)
        self.hTPC3OtherXYFront = ROOT.TH2F("hTPC3OtherXYFront","TPC3 other XY front"    ,50,-1100.,1100.,50,-1100.,1100.)
        self.hTPC3OtherXYBack  = ROOT.TH2F("hTPC3OtherXYBack" ,"TPC3 other XY back"     ,50,-1100.,1100.,50,-1100.,1100.)
        self.hTPC1OtherAngle   = ROOT.TH1F("hTPC1OtherAngle"  ,"TPC1 other cos(theta)"  ,50,-1.0,1.0)
        self.hTPC2OtherAngle   = ROOT.TH1F("hTPC2OtherAngle"  ,"TPC2 other cos(theta)"  ,50,-1.0,1.0)
        self.hTPC3OtherAngle   = ROOT.TH1F("hTPC3OtherAngle"  ,"TPC3 other cos(theta)"  ,50,-1.0,1.0)
        self.hTPCOtherYZ       = ROOT.TH2F("hTPCOtherYZ"      ,"TPC other YZ all hits"   ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.tpcOtherHistos = [
            self.hTPCOtherDet      ,
            self.hTPC1OtherNhits   ,
            self.hTPC2OtherNhits   ,
            self.hTPC3OtherNhits   ,
            self.hTPC1OtherXYFront ,
            self.hTPC1OtherXYBack  ,
            self.hTPC2OtherXYFront ,
            self.hTPC2OtherXYBack  ,
            self.hTPC3OtherXYFront ,
            self.hTPC3OtherXYBack  ,
            self.hTPC1OtherAngle   ,
            self.hTPC2OtherAngle   ,
            self.hTPC3OtherAngle   ,
            self.hTPCOtherYZ ]
        
        # TPC Unused histograms
        self.hTPCUnusedYZ      = ROOT.TH2F("hTPCUnusedYZ"    ,"TPC unused YZ"   ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hTPCUnusedXY      = ROOT.TH2F("hTPCUnusedXY"    ,"TPC unused XY"   ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hTPCUnusedTime    = ROOT.TH1F("hTPCUnusedTime"  ,"TPC unused Time" ,200,-15000.0,15000.0)
        self.hTPCUnusedQTot    = ROOT.TH1F("hTPCUnusedQtot"  ,"TPC unused Qtot" ,50,0.0,15000.0)
        # FGD Unused histograms
        self.hFGDUnusedYZ      = ROOT.TH2F("hFGDUnusedYZ"    ,"FGD unused YZ"   ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hFGDUnusedXY      = ROOT.TH2F("hFGDUnusedXY"    ,"FGD unused XY"   ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hFGDUnusedTime    = ROOT.TH1F("hFGDUnusedTime"  ,"FGD unused Time" ,200,-15000.0,15000.0)
        self.hFGDUnusedQTot    = ROOT.TH1F("hFGDUnusedQtot"  ,"FGD unused Qtot" ,50,0.0,150.0)
        self.unusedHistos = [
            self.hTPCUnusedYZ      ,
            self.hTPCUnusedXY      ,
            self.hTPCUnusedTime    ,
            self.hTPCUnusedQTot    ,
            self.hFGDUnusedYZ      ,
            self.hFGDUnusedXY      ,
            self.hFGDUnusedTime    ,
            self.hFGDUnusedQTot    ]

        # Track histograms
        self.hTrackPid1            = ROOT.TH1F("hTrackPid1"       , "Track Pid1"                ,  21, -0.5   ,   20.5)
        self.hTrackPid2            = ROOT.TH1F("hTrackPid2"       , "Track Pid2"                ,  21, -0.5   ,   20.5)
        self.hTrackWeight1         = ROOT.TH1F("hTrackWeight1"    , "Track Likelihood Pid1"     ,  20, 0.   ,   3.)  
        self.hTrackWeight2         = ROOT.TH1F("hTrackWeight2"    , "Track Likelihood Pid2"     ,  20, 0.   ,   3.)  
        self.hTrackYZ              = ROOT.TH2F("hTrackYZ"         , "Track YZ"                  ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hTrackXY              = ROOT.TH2F("hTrackXY"         , "Track XY"                  ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hTrackFrntYZ          = ROOT.TH2F("hTrackFrntYZ"     , "Track Front YZ"            ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hTrackFrntXY          = ROOT.TH2F("hTrackFrntXY"     , "Track Front XY"            ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hTrackBackYZ          = ROOT.TH2F("hTrackBackYZ"     , "Track Back YZ"             ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hTrackBackXY          = ROOT.TH2F("hTrackBackXY"     , "Track Back XY"             ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hTrackPQCosTh         = ROOT.TH2F("hTrackPQCosTh"    , "Track PQ vs CosTheta"      ,100,-1.,1.,100,-5000.0,5000.0)
        self.hTrackFrontPQCosTh    = ROOT.TH2F("hTrackFrntPQCosTh", "Track Front PQ vs CosTheta",100,-1.,1.,100,-5000.0,5000.0)
        self.hTrackBackPQCosTh     = ROOT.TH2F("hTrackBackPQCosTh", "Track Back PQ vs CosTheta" ,100,-1.,1.,100,-5000.0,5000.0)
        self.hTrackPhi             = ROOT.TH1F("hTrackPhi"        , "Track Phi"                 ,100,-3.15,3.15)
        self.hTrackFrntPhi         = ROOT.TH1F("hTrackFrntPhi"    , "Track FrontPhi"            ,100,-3.15,3.15)
        self.hTrackBackPhi         = ROOT.TH1F("hTrackBackPhi"    , "Track BackPhi"             ,100,-3.15,3.15)
        self.hTrackNTPC            = ROOT.TH1F("hTrackNTPC"       , "Track NTPC"                ,11 ,-0.5,10.5)
        self.hTrackNFGD            = ROOT.TH1F("hTrackNFGD"       , "Track NFGD"                ,11 ,-0.5,10.5)
        self.hTrackNHits           = ROOT.TH1F("hTrackNHits"      , "Track NHits"               ,100, 0.,200.)
        self.hTrackNNodes          = ROOT.TH1F("hTrackNNodes"     , "Track NNodes"              ,101,-0.5,100.5)
        self.hTrackNDOF            = ROOT.TH1F("hTrackNDOF"       , "Track NDOF"                ,101,-0.5,100.5)
        self.hTrackChi2            = ROOT.TH1F("hTrackChi2"       , "Track Chi2"                ,100, 0.0,500.0)
        self.hTrackQual            = ROOT.TH1F("hTrackQual"       , "Track fit Probability"     ,100, 0.0,1.0)
        self.hTrackNConstit        = ROOT.TH1F("hTrackNConstit"   , "Track NConstituents"       ,11,-0.5,10.5)
        self.hTrackNTotConstit     = ROOT.TH1F("hTrackNTotConstit", "Track NTotConstituents"    ,11,-0.5,10.5)
        self.hTrackIdxConstit      = ROOT.TH1F("hTrackIdxConstit" , "Track Index/NTotConstit"   ,11,-0.05,1.05)
        self.trackHistos = [
            self.hTrackPid1            , 
            self.hTrackPid2            , 
            self.hTrackWeight1         , 
            self.hTrackWeight2         , 
            self.hTrackYZ              , 
            self.hTrackXY              , 
            self.hTrackFrntYZ              , 
            self.hTrackFrntXY              , 
            self.hTrackBackYZ              , 
            self.hTrackBackXY              , 
            self.hTrackPQCosTh         , 
            self.hTrackFrontPQCosTh    , 
            self.hTrackBackPQCosTh     , 
            self.hTrackPhi             , 
            self.hTrackFrntPhi         , 
            self.hTrackBackPhi         , 
            self.hTrackNTPC            , 
            self.hTrackNFGD            , 
            self.hTrackNHits           , 
            self.hTrackNNodes          , 
            self.hTrackNDOF            , 
            self.hTrackChi2            ,            
            self.hTrackQual            ,             
            self.hTrackNConstit        ,
            self.hTrackNTotConstit     ,
            self.hTrackIdxConstit  ]
        # histograms for TPC objects used to build track
        self.hTPCDet             = ROOT.TH1F("hTPCDet"        , "TPC Detector"            ,3,0.5,3.5)
        self.hTPCYZ              = ROOT.TH2F("hTPCYZ"         , "TPC YZ"                  ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hTPCXY              = ROOT.TH2F("hTPCXY"         , "TPC XY"                  ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hTPCPQCosTh         = ROOT.TH2F("hTPCPQCosTh"    , "TPC PQ vs CosTheta"      ,100,-1.,1.,100,-5000.0,5000.0)
        self.hTPCPhi             = ROOT.TH1F("hTPCPhi"        , "TPC Phi"                 ,100,-3.15,3.15)
        self.hTPCNDOF            = ROOT.TH1F("hTPCNDOF"       , "TPC NDOF"                ,101,-0.5,100.5)
        self.hTPCNHits           = ROOT.TH1F("hTPCNHits"      , "TPC NHits"               ,101,-0.5,100.5)
        self.hTPCChi2            = ROOT.TH1F("hTPCChi2"       , "TPC Chi2"                ,100, 0.0,500.0)
        self.hTPCQual            = ROOT.TH1F("hTPCQual"       , "TPC fit probability"     ,100, 0.0,1.0)
        self.hTPCPullEle         = ROOT.TH1F("hTPCPullEle"    , "TPC electron Pull"       ,50, -10.0,10.0)
        self.hTPCPullMu          = ROOT.TH1F("hTPCPullMu"     , "TPC muon Pull"           ,50, -10.0,10.0)
        self.hTPCPullK           = ROOT.TH1F("hTPCPullK"      , "TPC kaon Pull"           ,50, -10.0,10.0)
        self.hTPCPullPi          = ROOT.TH1F("hTPCPullPi"     , "TPC pion Pull"           ,50, -10.0,10.0)
        self.hTPCPullP           = ROOT.TH1F("hTPCPullP"      , "TPC proton Pull"         ,50, -10.0,10.0)
        self.hTPCCcorPQ          = ROOT.TH2F("hTPCCcorPQ"     , "TPC Ccor vs Momentum*Q"  ,100,-2000.0, 2000.0,100,0.,1000.)
        self.hTPCCcorPQEle       = ROOT.TH2F("hTPCCcorPQEle"  , "TPC electron Ccor vs P*Q",100,-2000.0, 2000.0,100,0.,1000.)
        self.hTPCCcorPQMu        = ROOT.TH2F("hTPCCcorPQMu"   , "TPC muon Ccor vs P*Q"    ,100,-2000.0, 2000.0,100,0.,1000.)
        self.hTPC1YZ             = ROOT.TH2F("hTPC1YZ"        , "TPC1 YZ"                 ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hTPC2YZ             = ROOT.TH2F("hTPC2YZ"        , "TPC2 YZ"                 ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hTPC3YZ             = ROOT.TH2F("hTPC3YZ"        , "TPC3 YZ"                 ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hTPCDifvsDrift      = ROOT.TH2F("hTPCDifvsDrift" , "TPC Trans Diffus vs Mean Drift; Drift (mm); dc_{B} (um/sqrt(cm))",200,0.,1000.0,200,0, 2000.0 )
        self.hTPCNconstit        = ROOT.TH1F("hTPCNConstit"   , "TPC NConstituents"       ,11,-0.5,10.5)

        self.tpcHistos = [
            self.hTPCDet             ,
            self.hTPCYZ              ,
            self.hTPC1YZ             ,
            self.hTPC2YZ             ,
            self.hTPC3YZ             ,
            self.hTPCXY              ,
            self.hTPCPQCosTh         ,
            self.hTPCPhi             ,
            self.hTPCNDOF            ,
            self.hTPCNHits           ,
            self.hTPCChi2            ,
            self.hTPCQual            ,
            self.hTPCPullEle         ,
            self.hTPCPullMu          ,
            self.hTPCPullK           ,
            self.hTPCPullPi          ,
            self.hTPCPullP           ,
            self.hTPCCcorPQ          ,
            self.hTPCCcorPQEle       ,            
            self.hTPCCcorPQMu        ,
            self.hTPCDifvsDrift      ,
            self.hTPCNconstit ]        

        # histograms for FGD objects used to build track
        self.hFGDDet             = ROOT.TH1F("hFGDDet"        , "FGD Detector"            ,2,0.5,2.5)
        self.hFGDYZ              = ROOT.TH2F("hFGDYZ"         , "FGD YZ"                  ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hFGD1YZ             = ROOT.TH2F("hFGD1YZ"        , "FGD1 YZ"                 ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hFGD2YZ             = ROOT.TH2F("hFGD2YZ"        , "FGD2 YZ"                 ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hFGDXY              = ROOT.TH2F("hFGDXY"         , "FGD XY"                  ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hFGDEdeposit        = ROOT.TH1F("hFGDEdep"       , "FGD Edeposit"            ,100,0.,200.0)
        self.hFGDCosTh           = ROOT.TH1F("hFGDCosTh"      , "FGD CosTh"               ,100,-1.,1.)
        self.hFGDPhi             = ROOT.TH1F("hFGDPhi"        , "FGD Phi"                 ,100,-3.15,3.15)
        self.hFGDNDOF            = ROOT.TH1F("hFGDNDOF"       , "FGD NDOF"                ,101,-0.5,100.5)
        self.hFGDChi2            = ROOT.TH1F("hFGDChi2"       , "FGD Chi2"                ,100, 0.0,500.0)
        self.hFGDQual            = ROOT.TH1F("hFGDQual"       , "FGD fit probability"     ,100, 0.0,1.0)
        self.fgdHistos = [
            self.hFGDDet             ,
            self.hFGDYZ              ,
            self.hFGD1YZ             ,
            self.hFGD2YZ             ,
            self.hFGDXY              ,
            self.hFGDEdeposit        ,
            self.hFGDCosTh           ,
            self.hFGDPhi             ,
            self.hFGDNDOF            ,
            self.hFGDChi2            ,
            self.hFGDQual            ]
        

        self.hTrueMass          =  ROOT.TH1F("hTrueMass"           , "True Mass"                   ,100,0.,2000.)
        self.hTrueInitYZ        =  ROOT.TH2F("hTrueInitYZ"         , "True Init YZ"                ,100,-1000.0,3000.0,50,-1100.,1100.0) 
        self.hTrueInitXY        =  ROOT.TH2F("hTrueInitXY"         , "True Init XY"                ,50,-1100.,1100.0,50,-1100.,1100.0)   
        self.hTrueInitPQcosTh   =  ROOT.TH2F("hTrueInitPQCosTh"    , "True Init PQ vs CosTheta"    ,100,-1.05,1.05,100,-5000.0,5000.0)
        self.hTrueFinalYZ       =  ROOT.TH2F("hTrueFinalYZ"        , "True Final YZ"               ,100,-1000.0,3000.0,50,-1100.,1100.0) 
        self.hTrueFinalXY       =  ROOT.TH2F("hTrueFinalXY"        , "True Final XY"               ,50,-1100.,1100.0,50,-1100.,1100.0)   
        self.hTrueParYZ         =  ROOT.TH2F("hTrueParentYZ"       , "True Parent YZ"              ,100,-1000.0,3000.0,50,-1100.,1100.0) 
        self.hTrueParXY         =  ROOT.TH2F("hTrueParentXY"       , "True Parent XY"              ,50,-1100.,1100.0,50,-1100.,1100.0)   
        self.hTrueGrandYZ       =  ROOT.TH2F("hTrueGrandParYZ"     , "True GrandPar YZ"            ,100,-1000.0,3000.0,50,-1100.,1100.0)    
        self.hTrueGrandXY       =  ROOT.TH2F("hTrueGrandParXY"     , "True GrandPar XY"            ,50,-1100.,1100.0,50,-1100.,1100.0)      
        self.trueHistos = [
            self.hTrueMass          ,             
            self.hTrueInitXY        , 
            self.hTrueInitYZ        , 
            self.hTrueInitPQcosTh   , 
            self.hTrueFinalXY       , 
            self.hTrueFinalYZ       , 
            self.hTrueParXY         , 
            self.hTrueParYZ         , 
            self.hTrueGrandXY       , 
            self.hTrueGrandYZ       ]

        
        # apply vertex and tpc track length cuts
        self.hCutsXY              = ROOT.TH2F("hCutsXY"         , "Cuts XY"                  ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hCutsYZ              = ROOT.TH2F("hCutsYZ"         , "Cuts YZ"                  ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hCutsFrntXY          = ROOT.TH2F("hFrntCutsXY"     , "Cuts Front XY"            ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hCutsFrntYZ          = ROOT.TH2F("hFrntCutsYZ"     , "Cuts Front YZ"            ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hCutsBackXY          = ROOT.TH2F("hBackCutsXY"     , "Cuts Back XY"             ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hCutsBackYZ          = ROOT.TH2F("hBackCutsYZ"     , "Cuts Back YZ"             ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hCutsFGDXY           = ROOT.TH2F("hCutsFGDXY"      , "Cuts FGD XY"              ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hCutsFGDYZ           = ROOT.TH2F("hCutsFGDYZ"      , "Cuts FGD YZ"              ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hCutsPQCosTh         = ROOT.TH2F("hCutsPQCosTh"    , "Cuts PQ vs CosTheta"      ,100,-1.,1.,100,-5000.0,5000.0)
        self.hCutsPhi             = ROOT.TH1F("hCutsPhi"        , "Cuts Phi"                 ,100,-3.15,3.15)
        self.hCutsNTPC            = ROOT.TH1F("hCutsNTPC"       , "Cuts NTPC"                ,11 ,-0.5,10.5)
        self.hCutsNFGD            = ROOT.TH1F("hCutsNFGD"       , "Cuts NFGD"                ,11 ,-0.5,10.5)
        self.hCutsNHits           = ROOT.TH1F("hCutsNHits"      , "Cuts NHits"               ,100, 0.,200.)
        self.hCutsNNodes          = ROOT.TH1F("hCutsNNodes"     , "Cuts NNodes"              ,101,-0.5,100.5)
        self.hCutsNDOF            = ROOT.TH1F("hCutsNDOF"       , "Cuts NDOF"                ,101,-0.5,100.5)
        self.hCutsChi2            = ROOT.TH1F("hCutsChi2"       , "Cuts Chi2"                ,100, 0.0,500.0)
        self.hCutsQual            = ROOT.TH1F("hCutsQual"       , "Cuts fit probability"     ,100, 0.0,1.0)
        self.hCutsNConstit        = ROOT.TH1F("hCutsNConstit"   , "Cuts NConstituents"       ,11,-0.5,10.5)
        self.hCutsNTotConstit     = ROOT.TH1F("hCutsNTotConstit", "Cuts NTotConstituents"    ,11,-0.5,10.5)
        self.hCutsPullEle         = ROOT.TH1F("hCutsPullEle"    , "Cuts electron Pull"       ,50, -10.0,10.0)
        self.hCutsPullMu          = ROOT.TH1F("hCutsPullMu"     , "Cuts muon Pull"           ,50, -10.0,10.0)
        self.hCutsCcorPQ          = ROOT.TH2F("hCutsCcorPQ"     , "Cuts Ccor vs P*Q"    ,100,-2000.0, 2000.0,100,0.,1000.)
        self.cutHistos = [
            self.hCutsXY              , 
            self.hCutsYZ              , 
            self.hCutsFrntXY          , 
            self.hCutsFrntYZ          , 
            self.hCutsBackXY          , 
            self.hCutsBackYZ          , 
            self.hCutsFGDXY           , 
            self.hCutsFGDYZ           , 
            self.hCutsPQCosTh         , 
            self.hCutsPhi             , 
            self.hCutsNTPC            , 
            self.hCutsNFGD            , 
            self.hCutsNHits           , 
            self.hCutsNNodes          , 
            self.hCutsNDOF            , 
            self.hCutsChi2            , 
            self.hCutsQual            , 
            self.hCutsNConstit        , 
            self.hCutsNTotConstit     , 
            self.hCutsPullEle         , 
            self.hCutsPullMu          , 
            self.hCutsCcorPQ       ]

        # do numu analysis
        self.hNuMuXY              = ROOT.TH2F("hNuMuXY"         , "NuMu XY"                  ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hNuMuYZ              = ROOT.TH2F("hNuMuYZ"         , "NuMu YZ"                  ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hNuMuPQCosTh         = ROOT.TH2F("hNuMuPQCosTh"    , "NuMu PQ vs CosTheta"      ,100,-1.,1.,100,-5000.0,5000.0)
        self.hNuMuPhi             = ROOT.TH1F("hNuMuPhi"        , "NuMu Phi"                 ,100,-3.15,3.15)
        self.hNuMuNTPC            = ROOT.TH1F("hNuMuNTPC"       , "NuMu NTPC"                ,11 ,-0.5,10.5)
        self.hNuMuNFGD            = ROOT.TH1F("hNuMuNFGD"       , "NuMu NFGD"                ,11 ,-0.5,10.5)
        self.hNuMuNHits           = ROOT.TH1F("hNuMuNHits"      , "NuMu NHits"               ,100, 0.,200.)
        self.hNuMuNNodes          = ROOT.TH1F("hNuMuNNodes"     , "NuMu NNodes"              ,101,-0.5,100.5)
        self.hNuMuNDOF            = ROOT.TH1F("hNuMuNDOF"       , "NuMu NDOF"                ,101,-0.5,100.5)
        self.hNuMuChi2            = ROOT.TH1F("hNuMuChi2"       , "NuMu Chi2"                ,100, 0.0,500.0)
        self.hNuMuQual            = ROOT.TH1F("hNuMuQual"       , "NuMu fit probability"     ,100, 0.0,1.0)
        self.hNuMuNConstit        = ROOT.TH1F("hNuMuNConstit"   , "NuMu NConstituents"       ,11,-0.5,10.5)
        self.hNuMuNTotConstit     = ROOT.TH1F("hNuMuNTotConstit", "NuMu NTotConstituents"    ,11,-0.5,10.5)
        self.hNuMuPullMu          = ROOT.TH1F("hNuMuPullMu"     , "NuMu muon Pull"           ,50, -10.0,10.0)
        self.hNuMuCcorPQMu        = ROOT.TH2F("hNuMuCcorPQMu"   , "NuMu muon Ccor vs P*Q"    ,100,-2000.0, 2000.0,100,0.,1000.)
        self.numuHistos = [
            self.hNuMuXY              ,
            self.hNuMuYZ              ,
            self.hNuMuPQCosTh         ,
            self.hNuMuPhi             ,
            self.hNuMuNTPC            ,
            self.hNuMuNFGD            ,
            self.hNuMuNHits           ,
            self.hNuMuNNodes          ,
            self.hNuMuNDOF            ,
            self.hNuMuChi2            ,
            self.hNuMuQual            ,
            self.hNuMuNConstit        ,
            self.hNuMuNTotConstit     ,
            self.hNuMuPullMu          ,
            self.hNuMuCcorPQMu        ]
        
        # do nue analysis
        self.hNuEXY              = ROOT.TH2F("hNuEXY"         , "NuE XY"                  ,50,-1100.,1100.0,50,-1100.,1100.0)
        self.hNuEYZ              = ROOT.TH2F("hNuEYZ"         , "NuE YZ"                  ,100,-1000.0,3000.0,50,-1100.,1100.0)
        self.hNuEPQCosTh         = ROOT.TH2F("hNuEPQCosTh"    , "NuE PQ vs CosTheta"      ,100,-1.,1.,100,-5000.0,5000.0)
        self.hNuEPhi             = ROOT.TH1F("hNuEPhi"        , "NuE Phi"                 ,100,-3.15,3.15)
        self.hNuENTPC            = ROOT.TH1F("hNuENTPC"       , "NuE NTPC"                ,11 ,-0.5,10.5)
        self.hNuENFGD            = ROOT.TH1F("hNuENFGD"       , "NuE NFGD"                ,11 ,-0.5,10.5)
        self.hNuENHits           = ROOT.TH1F("hNuENHits"      , "NuE NHits"               ,100, 0.,200.)
        self.hNuENNodes          = ROOT.TH1F("hNuENNodes"     , "NuE NNodes"              ,101,-0.5,100.5)
        self.hNuENDOF            = ROOT.TH1F("hNuENDOF"       , "NuE NDOF"                ,101,-0.5,100.5)
        self.hNuEChi2            = ROOT.TH1F("hNuEChi2"       , "NuE Chi2"                ,100, 0.0,500.0)
        self.hNuEQual            = ROOT.TH1F("hNuEQual"       , "NuE fit probability"     ,100, 0.0,1.0)
        self.hNuENConstit        = ROOT.TH1F("hNuENConstit"   , "NuE NConstituents"       ,11,-0.5,10.5)
        self.hNuENTotConstit     = ROOT.TH1F("hNuENTotConstit", "NuE NTotConstituents"    ,11,-0.5,10.5)
        self.hNuEPullEle         = ROOT.TH1F("hNuEPullEle"    , "NuE electron Pull"       ,50, -10.0,10.0)
        self.hNuEPullMu          = ROOT.TH1F("hNuEPullMu"     , "NuE muon Pull"           ,50, -10.0,10.0)
        self.hNuECcorPQEle       = ROOT.TH2F("hNuECcorPQEle"  , "NuE electron Ccor vs P*Q",100,-2000.0, 2000.0,100,0.,1000.)
        self.nueHistos = [
            self.hNuEXY              ,
            self.hNuEYZ              ,
            self.hNuEPQCosTh         ,
            self.hNuEPhi             ,
            self.hNuENTPC            ,
            self.hNuENFGD            ,
            self.hNuENHits           ,
            self.hNuENNodes          ,
            self.hNuENDOF            ,
            self.hNuEChi2            ,
            self.hNuEQual            ,
            self.hNuENConstit        ,
            self.hNuENTotConstit     ,
            self.hNuEPullEle         ,
            self.hNuEPullMu          ,
            self.hNuECcorPQEle       ]
            
        # Define other quantities
        #self.trackerAnalysisUtil = ROOT.TTrackerAnalysisUtils()
        # defined fiducial within active FGD volume:
        self.fgdFiducialTrim = ROOT.TVector3(200.0,200.0,10.0) 

    def EventAnalysis(self):
        # abbreviations for use within this function
        truthVertices = self.truthVertices 
        reconTracker = self.reconTracker 
        fgdFiducialTrim = self.fgdFiducialTrim
        #trackerAnalysisUtil = self.trackerAnalysisUtil
        eventHeader = self.eventHeader

        # start filling plots with no cuts to check
        # that we generally get reasonable values

        # overall statistics
        self.hNTracks   .Fill( float( reconTracker.NTracks    ) )
        self.hNFGDOther .Fill( float( reconTracker.NFGDOther  ) )
        self.hNTPCOther .Fill( float( reconTracker.NTPCOther  ) )
        self.hNTPCIso   .Fill( float( reconTracker.NTPCIso    ) )
        self.hNFGDUnused.Fill( float( reconTracker.NFGDUnused ) )
        self.hNTPCUnused.Fill( float( reconTracker.NTPCUnused ) )

        # TPC other track histograms
        for ifit in xrange( reconTracker.NTPCOther ):
            tpcother = reconTracker.TPCOther[ ifit ] 
            self.hTPCOtherDet.Fill( float( tpcother.Detector ) )
            # check hit positions
            for ihit in xrange( tpcother.NHits ):
                hit = tpcother.Hits[ihit]
                self.hTPCOtherYZ.Fill( float( hit.Position.Z() ), float( hit.Position.Y() ) ) 

            if ( tpcother.Detector == 1 ):
                self.hTPC1OtherNhits.Fill( float( tpcother.NHits ) )
                self.hTPC1OtherXYFront.Fill( float( tpcother.FrontPosition.X() ),  float( tpcother.FrontPosition.Y() ) )
                self.hTPC1OtherXYBack.Fill( float( tpcother.BackPosition.X() ),  float( tpcother.BackPosition.Y() ) )
                # calculate a rough angle theta_yz estimate from start and end points
                # call along z axis zero degrees, -z as 180 degrees +y as 90 degrees
                dz = tpcother.BackPosition.Z() -  tpcother.FrontPosition.Z()
                dy = tpcother.BackPosition.Y() -  tpcother.FrontPosition.Y()
                if math.fabs(dz) > 0.:
                    self.hTPC1OtherAngle.Fill( float( dy / dz ) )
                else:
                    self.hTPC1OtherAngle.Fill( 0.0 )

            if ( tpcother.Detector == 2 ):
                self.hTPC2OtherNhits.Fill( float( tpcother.NHits ) )
                self.hTPC2OtherXYFront.Fill( float( tpcother.FrontPosition.X() ),  float( tpcother.FrontPosition.Y() ) )
                self.hTPC2OtherXYBack.Fill( float( tpcother.BackPosition.X() ),  float( tpcother.BackPosition.Y() ) )
                # calculate a rough angle theta_yz estimate from start and end points
                # call along z axis zero degrees, -z as 180 degrees +y as 90 degrees
                dz = tpcother.BackPosition.Z() -  tpcother.FrontPosition.Z()
                dy = tpcother.BackPosition.Y() -  tpcother.FrontPosition.Y()
                if math.fabs(dz) > 0.:
                    self.hTPC2OtherAngle.Fill( float( dy / dz ) )
                else:
                    self.hTPC2OtherAngle.Fill( 0.0 )

            if ( tpcother.Detector == 3 ):
                self.hTPC3OtherNhits.Fill( float( tpcother.NHits ) )
                self.hTPC3OtherXYFront.Fill( float( tpcother.FrontPosition.X() ),  float( tpcother.FrontPosition.Y() ) )
                self.hTPC3OtherXYBack.Fill( float( tpcother.BackPosition.X() ),  float( tpcother.BackPosition.Y() ) )
                # calculate a rough angle theta_yz estimate from start and end points
                # call along z axis zero degrees, -z as 180 degrees +y as 90 degrees
                dz = tpcother.BackPosition.Z() -  tpcother.FrontPosition.Z()
                dy = tpcother.BackPosition.Y() -  tpcother.FrontPosition.Y()
                if math.fabs(dz) > 0.:
                    self.hTPC3OtherAngle.Fill( float( dy / dz ) )
                else:
                    self.hTPC3OtherAngle.Fill( 0.0 )

        
        # TPC unused hit histograms
        for ihit in xrange( reconTracker.NTPCUnused ):
            unusedhit = reconTracker.TPCUnused[ihit]
            self.hTPCUnusedYZ.Fill( float( unusedhit.Position.Z() ), float( unusedhit.Position.Y() ) )
            self.hTPCUnusedXY.Fill( float( unusedhit.Position.X() ), float( unusedhit.Position.Y() ) )
            self.hTPCUnusedTime.Fill( float( unusedhit.Time ) )
            self.hTPCUnusedQTot.Fill( float( unusedhit.TotalCharge ) )

        # FGD unused hit histograms
        for ihit in xrange( reconTracker.NFGDUnused ):
            unusedhit = reconTracker.FGDUnused[ihit]
            self.hFGDUnusedYZ.Fill( float( unusedhit.Position.Z() ), float( unusedhit.Position.Y() ) )
            self.hFGDUnusedXY.Fill( float( unusedhit.Position.X() ), float( unusedhit.Position.Y() ) )
            self.hFGDUnusedTime.Fill( float( unusedhit.Time ) )
            self.hFGDUnusedQTot.Fill( float( unusedhit.TotalCharge ) )

        # Track histograms
        for itrack in xrange( reconTracker.NTracks ):
            track = reconTracker.Tracks[itrack]
            pids    = track.Pids
            likelis = track.Likelihoods
            # check pids
            for ipid in xrange ( len(pids) ):
                if ipid == 0:
                    self.hTrackPid1.Fill( pids[ipid] )
                    self.hTrackWeight1.Fill( likelis[ipid] )
                if ipid == 1:
                    self.hTrackPid2.Fill( pids[ipid] )
                    self.hTrackWeight2.Fill( likelis[ipid] )
            self.hTrackYZ.Fill( float( track.Position.Z() ),  float( track.Position.Y() ) )
            self.hTrackXY.Fill( float( track.Position.X() ),  float( track.Position.Y() ) )
            self.hTrackPQCosTh.Fill( float( track.Direction.CosTheta() ), float( track.Charge * track.Momentum ) )
            self.hTrackPhi.Fill( float( track.Direction.Phi() ) )
            self.hTrackNTPC.Fill( float( track.NTPCs ) )
            self.hTrackNFGD.Fill( float( track.NFGDs ) )
            self.hTrackNHits.Fill( float( track.NHits ) )
            self.hTrackNNodes.Fill( float( track.NNodes ) )
            self.hTrackNDOF.Fill( float( track.NDOF ) )
            self.hTrackChi2.Fill( float( track.Chi2 ) )
            self.hTrackQual.Fill( float( track.Quality ) )
            self.hTrackNConstit.Fill( float( track.NConstituents) )
            self.hTrackNTotConstit.Fill( float( track.NTotalConstituents) )

            # fill TPC constituent histograms
            for itpc in xrange( track.NTPCs ):
                tpctrack = track.TPC[ itpc ]
                self.hTPCDet.Fill    ( float( tpctrack.Detector        ) )
                if tpctrack.Detector == 1:
                    self.hTPC1YZ.Fill( float( tpctrack.Position.Z() )        , float( tpctrack.Position.Y()               ) )
                if tpctrack.Detector == 2:
                    self.hTPC2YZ.Fill( float( tpctrack.Position.Z() )        , float( tpctrack.Position.Y()               ) )
                if tpctrack.Detector == 3:
                    self.hTPC3YZ.Fill( float( tpctrack.Position.Z() )        , float( tpctrack.Position.Y()               ) )
                self.hTPCYZ.Fill     ( float( tpctrack.Position.Z() )        , float( tpctrack.Position.Y()               ) )
                self.hTPCXY.Fill     ( float( tpctrack.Position.X() )        , float( tpctrack.Position.Y()               ) )
                self.hTPCPQCosTh.Fill( float( tpctrack.Direction.CosTheta() ), float( tpctrack.Charge * tpctrack.Momentum ) )
                self.hTPCPhi.Fill    ( float( tpctrack.Direction.Phi() ) )
                self.hTPCNDOF.Fill   ( float( tpctrack.Ndof            ) )
                self.hTPCNHits.Fill  ( float( tpctrack.NHits           ) )
                self.hTPCChi2.Fill   ( float( tpctrack.Chi2            ) )
                if tpctrack.Ndof > 0:
                    self.hTPCQual.Fill( ROOT.TMath.Prob( float( tpctrack.Chi2 ), int( tpctrack.Ndof ) ) )
                else:
                    self.hTPCQual.Fill( 999999.0 )
                self.hTPCPullEle.Fill( float( tpctrack.PullEle ) )
                self.hTPCPullMu .Fill( float( tpctrack.PullMuon  ) )
                self.hTPCPullK  .Fill( float( tpctrack.PullKaon   ) )
                self.hTPCPullPi .Fill( float( tpctrack.PullPion  ) )
                self.hTPCPullP  .Fill( float( tpctrack.PullProton   ) )
                # use track charge instead of TPC charge...
                self.hTPCCcorPQ .Fill( float( track.Charge * tpctrack.Momentum ), float( tpctrack.Ccorr ) )
                if ( tpctrack.PullEle > -2.5 and tpctrack.PullEle < 3.0 ):
                    self.hTPCCcorPQEle .Fill( float( track.Charge * tpctrack.Momentum ), float( tpctrack.Ccorr ) )
                if ( tpctrack.Momentum > 200.0 ):
                    self.hTPCCcorPQMu  .Fill( float( track.Charge * tpctrack.Momentum ), float( tpctrack.Ccorr ) )
                self.hTPCDifvsDrift .Fill( math.fabs( float(tpctrack.MeanDrift) ), math.sqrt( math.fabs( float( tpctrack.Sigma1/10.0 ) ) )*10000.0 );
                self.hTPCNconstit.Fill( float( tpctrack.NConstituents ) )


            # fill FGD constituent histograms
            for ifgd in xrange( track.NFGDs ):
                fgdtrack = track.FGD[ ifgd ]
                self.hFGDDet.Fill     ( float( fgdtrack.Detector        ) )
                if fgdtrack.Detector == 1:
                    self.hFGD1YZ.Fill( float( fgdtrack.Position.Z() )        , float( fgdtrack.Position.Y()               ) )
                if fgdtrack.Detector == 2:
                    self.hFGD2YZ.Fill( float( fgdtrack.Position.Z() )        , float( fgdtrack.Position.Y()               ) )
                self.hFGDYZ.Fill      ( float( fgdtrack.Position.Z() )        , float( fgdtrack.Position.Y()               ) )
                self.hFGDXY.Fill      ( float( fgdtrack.Position.X() )        , float( fgdtrack.Position.Y()               ) )
                self.hFGDEdeposit.Fill( float( fgdtrack.EDeposit        ) )
                self.hFGDCosTh.Fill   ( float( fgdtrack.Direction.CosTheta() ) )
                self.hFGDPhi.Fill     ( float( fgdtrack.Direction.Phi() ) )
                self.hFGDNDOF.Fill    ( float( fgdtrack.Ndof            ) )
                self.hFGDChi2.Fill    ( float( fgdtrack.Chi2            ) )
                if fgdtrack.Ndof > 0:
                    self.hFGDQual.Fill( ROOT.TMath.Prob( float( fgdtrack.Chi2 ), int( fgdtrack.Ndof ) ) )
                else:
                    self.hFGDQual.Fill( 999999.0 )
            
            # fill True particle histograms
            trupar = track.TrueParticle
            self.hTrueMass.Fill( float( trupar.Mass ) )
            self.hTrueInitXY.Fill( float( trupar.InitPos.X() ),  float( trupar.InitPos.Y() ) ) 
            self.hTrueInitYZ.Fill( float( trupar.InitPos.Z() ),  float( trupar.InitPos.Y() ) ) 
            self.hTrueInitPQcosTh.Fill( float( trupar.InitMom.CosTheta() ), float( trupar.Charge * trupar.InitMom.Mag() ) )
            self.hTrueFinalXY.Fill( float( trupar.FinalPos.X() ),  float( trupar.FinalPos.Y() ) ) 
            self.hTrueFinalYZ.Fill( float( trupar.FinalPos.Z() ),  float( trupar.FinalPos.Y() ) )
            self.hTrueParXY.Fill( float( trupar.ParentInitPos.X() ),  float( trupar.ParentInitPos.Y() ) ) 
            self.hTrueParYZ.Fill( float( trupar.ParentInitPos.Z() ),  float( trupar.ParentInitPos.Y() ) )
            self.hTrueGrandXY.Fill( float( trupar.GrandParentInitPos.X() ),  float( trupar.GrandParentInitPos.Y() ) ) 
            self.hTrueGrandYZ.Fill( float( trupar.GrandParentInitPos.Z() ),  float( trupar.GrandParentInitPos.Y() ) )

            # Common analysis cuts.  Still inside loop over tracks            
            # Require > 1 TPC in track > 1 FGD in track
            if (  track.NFGDs > 0 and track.NTPCs > 0 ):
                # check if the track starts in one of FGDs?
                startsinfgd = -1
                #                fgdFiducialTrim = ROOT.TVector3(200.0,200.0,10.0)
                #if trackerAnalysisUtil.IsInFiducialFGD1( track.Position, fgdFiducialTrim ):
                #    startsinfgd = 1
                #if trackerAnalysisUtil.IsInFiducialFGD2( track.Position, fgdFiducialTrim ):
                #    startsinfgd = 2
                if (  track.Position[0] > -850.0 and track.Position[0] < 850.0 and
                      track.Position[1] > -850.0 and track.Position[1] < 850.0 ):
                    if ( track.Position[2] > 155.0 and track.Position[2] < 440.0  ):
                        startsinfgd = 1
                    if ( track.Position[2] > 1480.0 and track.Position[2] < 1800.0  ):
                        startsinfgd = 2
                
                # check TPC tracks
                ntpc1 = 0;
                ntpc2 = 0;
                ntpc3 = 0;
                for itpc in xrange( track.NTPCs ):
                    tpctrack = track.TPC[ itpc ]
                    #if ( tpctrack.Detector == 1 ):
                    #    ntpc1 = ntpc1+1
                    #if ( tpctrack.Detector == 2 ):
                    #    ntpc2 = ntpc2+1
                    #if ( tpctrack.Detector == 3 ):
                    #    ntpc3 = ntpc3+1
                    if ( tpctrack.Position[2] < 155.0 ):
                        ntpc1 = ntpc1+1
                    else:
                        if ( tpctrack.Position[2] < 1480.0 ):
                            ntpc2 = ntpc2+1
                        else:
                            ntpc3 = ntpc3+1

                # find tpc and fgd track indices if any
                idxtpc = -1
                idxfgd = -1
                # fgd1 vertex
                if ( ntpc1 == 0 and ntpc2>0 ):
                    # check fgd1 vertex
                    for ifgd in xrange( track.NFGDs ):
                        fgdtrack = track.FGD[ ifgd ]
 
                        if (  fgdtrack.Position[0] > -850.0 and fgdtrack.Position[0] < 850.0 and
                              fgdtrack.Position[1] > -850.0 and fgdtrack.Position[1] < 850.0 ):
                            if ( fgdtrack.Position[2] > 155.0 and fgdtrack.Position[2] < 440.0  ):
                                idxfgd = ifgd
                       
                        #if ( trackerAnalysisUtil.IsInFiducialFGD1( fgdtrack.Position, fgdFiducialTrim ) ):
                        #    idxfgd = ifgd
                             
                    # get highest momentum TPC2 track
                    tpcmaxmom = -99999.0;
                    for itpc in xrange( track.NTPCs ):
                        tpctrack = track.TPC[ itpc ]
                        if ( tpctrack.Momentum > tpcmaxmom and
                             tpctrack.NHits > 18 and
                             tpctrack.Detector == 2 ):
                            idxtpc = itpc
                            tpcmaxmom = tpctrack.Momentum
                
                # fgd2 vertex
                if ( ntpc1 == 0 and ntpc2 == 0 and ntpc3>0 ):
                    # check fgd2 vertex
                    for ifgd in xrange( track.NFGDs ):
                        fgdtrack = track.FGD[ ifgd ]
                        #if ( trackerAnalysisUtil.IsInFiducialFGD2( fgdtrack.Position, fgdFiducialTrim ) ):
                        #    idxfgd = ifgd
                        if (  fgdtrack.Position[0] > -850.0 and fgdtrack.Position[0] < 850.0 and
                              fgdtrack.Position[1] > -850.0 and fgdtrack.Position[1] < 850.0 ):
                            if ( fgdtrack.Position[2] > 1480.0 and fgdtrack.Position[2] < 1800.0  ):
                                idxfgd = ifgd
                                
                    # get highest momentum TPC3 track
                    tpcmaxmom = -99999.0;
                    for itpc in xrange( track.NTPCs ):
                        tpctrack = track.TPC[ itpc ]
                        if ( tpctrack.Momentum > tpcmaxmom and
                             tpctrack.NHits > 18 and
                             tpctrack.Detector == 3 ):
                            idxtpc = itpc
                            tpcmaxmom = tpctrack.Momentum


                #print 'track starts in fgd ',startsinfgd,' idxfgd=',idxfgd,' z,y=',track.Position.Z(),',',track.Position.Y()
                #if (idxfgd>=0):
                #    print '   fgd=',track.FGD[idxfgd].Detector,' z,y=',track.FGD[idxfgd].Position.Z(),',',track.FGD[idxfgd].Position.Y()
                # check that vertex was found:
                if (idxtpc >= 0 and idxfgd >= 0 and idxtpc >= 0 ): #and startsinfgd>0):
                    # make basic plots after vertex cut
                    # overall track plots
                    self.hCutsXY.Fill( float( track.Position.X() ),  float( track.Position.Y() ) )
                    self.hCutsYZ.Fill( float( track.Position.Z() ),  float( track.Position.Y() ) )
                    fgdtrack = track.FGD[ idxfgd ]
                    self.hCutsFGDXY.Fill( float( fgdtrack.Position.X() ),  float( fgdtrack.Position.Y() ) )
                    self.hCutsFGDYZ.Fill( float( fgdtrack.Position.Z() ),  float( fgdtrack.Position.Y() ) )
                    self.hCutsPQCosTh.Fill( float( track.Direction.CosTheta() ), float( track.Charge * track.Momentum ) )
                    self.hCutsPhi.Fill( float( track.Direction.Phi() ) )
                    self.hCutsNTPC.Fill( float( track.NTPCs ) )
                    self.hCutsNFGD.Fill( float( track.NFGDs ) )
                    self.hCutsNHits.Fill( float( track.NHits ) )
                    self.hCutsNNodes.Fill( float( track.NNodes ) )
                    self.hCutsNDOF.Fill( float( track.NDOF ) )
                    self.hCutsChi2.Fill( float( track.Chi2 ) )
                    self.hCutsQual.Fill( float( track.Quality ) )
                    self.hCutsNConstit.Fill( float( track.NConstituents) )
                    self.hCutsNTotConstit.Fill( float( track.NTotalConstituents) )
                    # TPC / PID specific plots
                    tpctrack = track.TPC[ idxtpc ]
                    self.hCutsPullEle.Fill( float( tpctrack.PullEle ) )
                    self.hCutsPullMu .Fill( float( tpctrack.PullMuon  ) )
                    self.hCutsCcorPQ .Fill( float( track.Charge * track.Momentum ), float( tpctrack.Ccorr ) )
                    
                    # NuMu Specific cuts   
                    if ( tpctrack.PullMuon >= -2.0 and tpctrack.PullMuon <= 2.0 ):
                        self.hNuMuCcorPQMu.Fill( float( track.Charge * track.Momentum ), float( tpctrack.Ccorr ) )
                        self.hNuMuXY.Fill( float( track.Position.X() ),  float( track.Position.Y() ) )
                        self.hNuMuYZ.Fill( float( track.Position.Z() ),  float( track.Position.Y() ) )
                        self.hNuMuPQCosTh.Fill( float( track.Direction.CosTheta() ), float( track.Charge * track.Momentum ) )
                        self.hNuMuPhi.Fill( float( track.Direction.Phi() ) )
                        self.hNuMuNTPC.Fill( float( track.NTPCs ) )
                        self.hNuMuNFGD.Fill( float( track.NFGDs ) )
                        self.hNuMuNHits.Fill( float( track.NHits ) )
                        self.hNuMuNNodes.Fill( float( track.NNodes ) )
                        self.hNuMuNDOF.Fill( float( track.NDOF ) )
                        self.hNuMuChi2.Fill( float( track.Chi2 ) )
                        self.hNuMuQual.Fill( float( track.Quality ) )
                        self.hNuMuNConstit.Fill( float( track.NConstituents) )
                        self.hNuMuNTotConstit.Fill( float( track.NTotalConstituents) )
                        # TPC / PID specific plots
                        self.hNuMuPullMu .Fill( float( tpctrack.PullMuon  ) )
                        self.hNuMuCcorPQMu.Fill( float( track.Charge * track.Momentum ), float( tpctrack.Ccorr ) )
                        # check vertex info...
                        if ( track.Position.X() == 0.0 and track.Position.Y() == 0.0 ):
                            print "Run ",eventHeader.RunID," Event ",eventHeader.EventNumber," Muon like"
                            print "   x,y,z=",track.Position.X(),",",track.Position.Y(),",",track.Position.Z()
                            print "   PQ=",track.Charge * track.Momentum,", costh=", track.Direction.CosTheta()
                            print "   CCor=",tpctrack.Ccorr 

                    # NuE Specific cuts
                    if ( tpctrack.PullMuon >= 2.5 and tpctrack.PullEle >= -1.0 and tpctrack.PullEle <= 2.0  ):
                        self.hNuECcorPQEle .Fill( float( track.Charge * track.Momentum ), float( tpctrack.Ccorr ) )
                        self.hNuEXY.Fill( float( track.Position.X() ),  float( track.Position.Y() ) )
                        self.hNuEYZ.Fill( float( track.Position.Z() ),  float( track.Position.Y() ) )
                        self.hNuEPQCosTh.Fill( float( track.Direction.CosTheta() ), float( track.Charge * track.Momentum ) )
                        self.hNuEPhi.Fill( float( track.Direction.Phi() ) )
                        self.hNuENTPC.Fill( float( track.NTPCs ) )
                        self.hNuENFGD.Fill( float( track.NFGDs ) )
                        self.hNuENHits.Fill( float( track.NHits ) )
                        self.hNuENNodes.Fill( float( track.NNodes ) )
                        self.hNuENDOF.Fill( float( track.NDOF ) )
                        self.hNuEChi2.Fill( float( track.Chi2 ) )
                        self.hNuEQual.Fill( float( track.Quality ) )
                        self.hNuENConstit.Fill( float( track.NConstituents) )
                        self.hNuENTotConstit.Fill( float( track.NTotalConstituents) )
                        # TPC / PID specific plots
                        self.hNuEPullEle.Fill( float( tpctrack.PullEle ) )
                        self.hNuEPullMu .Fill( float( tpctrack.PullMuon  ) )
                        self.hNuECcorPQEle.Fill( float( track.Charge * track.Momentum ), float( tpctrack.Ccorr ) )
                        # check vertex info...
                        if ( track.Position.X() == 0.0 and track.Position.Y() == 0.0 ):
                            print "Run ",eventHeader.RunID," Event ",eventHeader.EventNumber," Electron like"
                            print "   x,y,z=",track.Position.X(),",",track.Position.Y(),",",track.Position.Z()
                            print "   PQ=",track.Charge * track.Momentum,", costh=", track.Direction.CosTheta()
                            print "   CCor=",tpctrack.Ccorr 


    def WriteResultsToOutputFile(self):
        ####################################################################
        # Write out analysis results
        if self.rootOutput:
            self.rootOutput.cd()
            for histo in self.statsHistos:
                histo.Write()
            adir = self.rootOutput.mkdir("tpcother")
            adir.cd()
            for histo in self.tpcOtherHistos:
                histo.Write()
            self.rootOutput.cd()
            adir = self.rootOutput.mkdir("unusedhits")
            adir.cd()
            for histo in self.unusedHistos:
                histo.Write()
            self.rootOutput.cd()
            adir = self.rootOutput.mkdir("analysis")
            adir.cd()
            for histo in self.cutHistos:
                histo.Write()
            bdir = adir.mkdir("numu")
            bdir.cd()
            for histo in self.numuHistos:
                histo.Write()
            adir.cd()
            bdir = adir.mkdir("nue")
            bdir.cd()
            for histo in self.nueHistos:
                histo.Write()
            self.rootOutput.cd()
            adir = self.rootOutput.mkdir("tracks")
            adir.cd()
            for histo in self.trackHistos:
                histo.Write()
            bdir = adir.mkdir( "tpc" )
            bdir.cd()
            for histo in self.tpcHistos:
                histo.Write()
            adir.cd()
            bdir = adir.mkdir( "fgd" )
            bdir.cd()
            for histo in self.fgdHistos:
                histo.Write()
            adir.cd()
            bdir = adir.mkdir( "constituents" )
            adir.cd()
            bdir = adir.mkdir( "true" )
            bdir.cd()
            for histo in self.trueHistos:
                histo.Write()
            

            

            self.rootOutput.Close()
            print "Output saved in " + self.options.output

