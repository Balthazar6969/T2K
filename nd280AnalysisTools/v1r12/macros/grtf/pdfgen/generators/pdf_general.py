from grtf.tools import timing_cuts
from grtf.pdfgen.generators import pdf_basics
import re

class pdf_general(pdf_basics.pdf_offsets, pdf_basics.pdf_directions, pdf_basics.pdf_times):
    def CustomPlots(self, obj, path):
        todo = False

        if obj.IsA().InheritsFrom("TH1") and path.find("Time") != -1:
                self.DrawTimingPlot(obj, path)
        elif obj.IsA().InheritsFrom("TDirectory"):
            if obj.GetPath().find("Offsets") != -1 and obj.GetPath().find("atched") != -1 and obj.GetPath().find("node object") == -1:
                self.DrawOffsetPlots(obj)
            elif obj.GetPath().find("Offsets") != -1 and obj.GetPath().find("node object") != -1 and obj.GetPath().find("Measure") != -1:
                self.DrawObjectOffsetPlots(obj)
            elif obj.GetPath().find("Directions") != -1 and obj.GetPath().find("Directions") != (len(obj.GetPath()) - 10):
                self.DrawDirectionPlots(obj)
            elif obj.GetPath().find("Eff") != -1 and obj.GetPath().find("Efficiency of refitting") != (len(obj.GetPath()) - 23):
                self.DrawRefitEffPlots(obj)
            elif obj.GetPath().find("Truth-matching") != -1:
                self.DrawTruthPlots(obj)
            elif obj.GetPath().find("Constituents") != -1 and obj.GetPath().find("Constituents") != (len(obj.GetPath()) - 12):
                self.DrawConstituentPlots(obj)
            elif obj.GetPath().find("NGlobalReconObjects") != -1:
                self.DrawGlobalCountPlots(obj)
            elif obj.GetPath().find("Quality") != -1 and obj.GetPath().find("Global object") != -1:
                self.DrawQualityPlots(obj)
            elif obj.GetPath().find("Quality") != -1 and obj.GetPath().find("Quality") != (len(obj.GetPath()) - 7):
                self.DrawQualityPlots(obj)
            else:
                todo = True
        else:
            todo = True

        return todo

    def DrawRefitEffPlots(self, dir):
        """
        Draw plots related to the efficency of refitting objects. 
        
        Page includes Total, Refitted and Efficiency plots.

        Keyword arguments:
        dir - The directory containing the plots.
        """
        self.SplitCanvas(2,2,True)
        self.WriteTitle(dir.GetPath())

        pad   = self.pads[3].cd()
        histo = self.GetHistoInDir(dir, "Eff")
        self.DrawTH1(histo, dir.GetPath() + "/Eff", pad)

        pad   = self.pads[6].cd()
        histo = self.GetHistoInDir(dir, "Total")
        self.DrawTH1(histo, dir.GetPath() + "/Total", pad)

        pad   = self.pads[4].cd()
        histo = self.GetHistoInDir(dir, "Refitted")
        self.DrawTH1(histo, dir.GetPath() + "/Refitted", pad)

        self.PrintCanvas()

        return

    def DrawQualityPlots(self, dir):
        """
        Draw plots related to the quality of objects. 
        
        1st page includes Quality, NDOF and Quality/NDOF plots.
        2nd and 3rd pages include these quantites plotted as a 
        function of various other parameters.

        Keyword arguments:
        dir - The directory containing the plots.
        """

        # Page 1
        self.SplitCanvas(2,2,True)
        self.WriteTitle(dir.GetPath())

        pad   = self.pads[3].cd()
        histo = self.GetHistoInDir(dir, "QualityPerNDOF")
        self.DrawTH1(histo, dir.GetPath() + "/QualityPerNDOF", pad)

        pad   = self.pads[4].cd()
        histo = self.GetHistoInDir(dir, "Quality")
        self.DrawTH1(histo, dir.GetPath() + "/Quality", pad)

        pad   = self.pads[6].cd()
        histo = self.GetHistoInDir(dir, "NDOF")
        self.DrawTH1(histo, dir.GetPath() + "/NDOF", pad)

        self.PrintCanvas()

        # Subsequent pages
        for plot in ("QualityvsAngle", "NDOFvsAngle", "QualityPerNDOFvsAngle", "NDOFIsShower"):
            histo = self.GetHistoInDir(dir, plot)
            path = dir.GetPath() + "/" + plot

            if histo:
                self.SplitCanvas(2,2,True)
                self.WriteTitle(dir.GetPath())

                histos = self.GetDataAndMCHistos(path, histo.GetName())
                self.ScaleTH2(histos, path)

                for index, type in enumerate(self.fileorder):
                    path = self.GetPath(path, type)
                    self.DrawTH2(histos[type], path, self.pads[3 + index])

                self.PrintCanvas()

        return

    def DrawTruthPlots(self, dir):
        """
        Draw plots related to the truth-matching of objects. 
        
        Page includes Efficiency and Purity of truth-matching.

        Keyword arguments:
        dir - The directory containing the plots.
        """

        self.SplitCanvas(2,2,True)
        self.WriteTitle(dir.GetPath())

        pad   = self.pads[3].cd()
        histo = self.GetHistoInDir(dir, "Efficiency")
        self.DrawTH1(histo, dir.GetPath() + "/Efficiency", pad)

        pad   = self.pads[4].cd()
        histo = self.GetHistoInDir(dir, "Purity")
        self.DrawTH1(histo, dir.GetPath() + "/Purity", pad)

        self.PrintCanvas()

        return

    def DrawConstituentPlots(self, dir):
        """
        Draw the plots for the number of constituents in a subdetector. 
        
        Page includes "Nxxx" and "Nxxx vs NConstituents" plots.

        Keyword arguments:
        dir - The directory containing the plots.
        """

        # The lowest-level directory contains the name of the counter
        # we're looking at (NP0D, for example).
        counter = re.sub(".*/", "", dir.GetPath())

        if counter == "NConstituents":
            # The total number of constituents.
            self.SplitCanvas(1,1,True)
            self.WriteTitle(dir.GetPath())
            pad   = self.pads[3].cd()
            histo = self.GetHistoInDir(dir, counter)
            self.DrawTH1(histo, dir.GetPath() + "/" + counter, pad)
        else:
            # The number of constituents in a certain module.
            self.SplitCanvas(2,2,True)
            self.WriteTitle(dir.GetPath())

            pad   = self.pads[3].cd()
            histo = self.GetHistoInDir(dir, counter)
            self.DrawTH1(histo, dir.GetPath() + "/" + counter, pad)

            histo = self.GetHistoInDir(dir, counter + " vs NConstituents")
            path = dir.GetPath() + "/" + counter + " vs NConstituents"
            histos = self.GetDataAndMCHistos(dir.GetPath() + "/" + counter + " vs NConstituents", histo.GetName())
            self.ScaleTH2(histos, path)

            firstpad = 4
            incrementer = 2

            if len(histos) > 2:
                self.PrintCanvas()

                self.SplitCanvas(2,2,True)
                self.WriteTitle(dir.GetPath())

                firstpad = 3
                incrementer = 1

            for index, type in enumerate(self.fileorder):
                path = self.GetPath(path, type)
                if type in histos:
                    self.DrawTH2(histos[type], path, self.pads[firstpad + index * incrementer])

        self.PrintCanvas()

        return

    def DrawGlobalCountPlots(self, dir):
        """
        Draw plots related to the number of global objects in an event.

        1st page includes total number of global objects in event and number 
        of successfully-fitted objects in event.

        Subsequent pages include number of objects in each module, grouped by 
        module type (TPC1, TPC2, TPC3 on the same page, for example).

        Keyword arguments:
        dir - The directory containing the plots.
        """

        # 1st page
        self.SplitCanvas(2,2,True)
        self.WriteTitle(dir.GetPath())

        pad   = self.pads[3].cd()
        histo = self.GetHistoInDir(dir, "All")
        self.DrawTH1(histo, dir.GetPath() + "/All", pad)

        pad   = self.pads[4].cd()
        histo = self.GetHistoInDir(dir, "Success")
        self.DrawTH1(histo, dir.GetPath() + "/Success", pad)

        self.PrintCanvas()

        # Subsequent pages
        subdir = dir.Get("Containing Module")

        if subdir:
            pages = [ {"TPC"    : ["TPC1", "TPC2", "TPC3"]},
                      {"FGD"    : ["FGD1", "FGD2"]},
                      {"P0D"    : ["P0D"]},
                      {"DSECal" : ["DSECal"]},
                      {"TECal"  : ["LTECal", "RTECal", "TTECal", "BTECal"]},
                      {"PECal"  : ["LPECal", "RPECal", "TPECal", "BPECal"]},
                      {"SMRD"   : ["LSMRD", "RSMRD", "TSMRD", "BSMRD"]} ]

            for page in pages:
                for group, modules in page.iteritems():
                    if len(modules) > 1:
                        self.SplitCanvas(2,2,True)
                    else:
                        self.SplitCanvas(1,1,True)

                    self.WriteTitle(subdir.GetPath() + "/" + group)

                    for index, module in enumerate(modules):
                        pad   = self.pads[3 + index].cd()
                        histo = self.GetHistoInDir(subdir, module)
                        self.DrawTH1(histo, subdir.GetPath() + "/" + module, pad)

                    self.PrintCanvas()

        return
