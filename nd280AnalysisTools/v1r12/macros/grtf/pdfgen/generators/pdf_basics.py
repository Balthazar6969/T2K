import ROOT
from grtf.tools import timing_cuts
from grtf.pdfgen.generators import pdf_base

class pdf_offsets(pdf_base.pdf_base):
    def DrawOffsetPlots(self, dir):
        """
        Draw plots related to the offset between two objects. 
        
        Pages are printed containing positional and angular offsets, 
        as well as a momentum offset if appropriate. 

        Keyword arguments:
        dir - The directory containing the plots.
        """

        self.SplitCanvas(2,2,True)
        self.WriteTitle(dir.GetPath())

        pad   = self.pads[3].cd()
        histo = self.GetHistoInDir(dir, "X")
        self.DrawTH1(histo, dir.GetPath() + "/X", pad)

        pad   = self.pads[4].cd()
        histo = self.GetHistoInDir(dir, "Y")
        self.DrawTH1(histo, dir.GetPath() + "/Y", pad)

        pad   = self.pads[5].cd()
        histo = self.GetHistoInDir(dir, "Z")
        self.DrawTH1(histo, dir.GetPath() + "/Z", pad)

        pad   = self.pads[6].cd()
        histo = self.GetHistoInDir(dir, "Mag")
        self.DrawTH1(histo, dir.GetPath() + "/Mag", pad)

        self.PrintCanvas()

        histo = self.GetHistoInDir(dir, "CosAngle")
        if histo:
            self.SplitCanvas(1,1,True)
            self.WriteTitle(dir.GetPath())
            pad   = self.pads[3].cd()
            self.DrawTH1(histo, dir.GetPath() + "/CosAngle", pad)
            self.PrintCanvas()

        histo = self.GetHistoInDir(dir, "Mom")
        if histo:
            self.SplitCanvas(1,1,True)
            self.WriteTitle(dir.GetPath())
            pad   = self.pads[3].cd()
            self.DrawTH1(histo, dir.GetPath() + "/Mom", pad)
            self.PrintCanvas()

        return

    def DrawObjectOffsetPlots(self, dir):
        """
        Draw plots related to the offset between two objects. 
        
        Pages are printed containing positional offsets. These offsets
        are for XZ/YZ/XY/XYZ measurements, so not all X/Y/Z offsets
        may be available, and the Mag of the offset is not appropriate.

        Keyword arguments:
        dir - The directory containing the plots.
        """

        self.SplitCanvas(2,2,True)
        self.WriteTitle(dir.GetPath())

        histo = self.GetHistoInDir(dir, "X")
        if histo:
            pad   = self.pads[3].cd()
            self.DrawTH1(histo, dir.GetPath() + "/X", pad)

        histo = self.GetHistoInDir(dir, "Y")
        if histo:
            pad   = self.pads[4].cd()
            self.DrawTH1(histo, dir.GetPath() + "/Y", pad)

        histo = self.GetHistoInDir(dir, "Z")
        if histo:
            pad   = self.pads[5].cd()
            self.DrawTH1(histo, dir.GetPath() + "/Z", pad)

        self.PrintCanvas()

        return


class pdf_directions(pdf_base.pdf_base):
    def DrawDirectionPlots(self, dir):
        """
        Draw plots related to the angle of an object. 
        
        Page contains CosTheta and Phi plots.

        Keyword arguments:
        dir - The directory containing the plots.
        """

        self.SplitCanvas(2,2,True)
        self.WriteTitle(dir.GetPath())

        pad   = self.pads[3].cd()
        histo = self.GetHistoInDir(dir, "CosTheta")
        self.DrawTH1(histo, dir.GetPath() + "/CosTheta", pad)

        pad   = self.pads[4].cd()
        histo = self.GetHistoInDir(dir, "Phi")
        self.DrawTH1(histo, dir.GetPath() + "/Phi", pad)

        self.PrintCanvas()

        return

class pdf_times(pdf_base.pdf_base):
    def DrawTimingPlot(self, histo, path):
        """
        Draw a plot with timing cuts overlaid.

        Timing cuts are extracted from timing_cuts.
        X axis of timing plot must be in ns.

        CURRENTLY ONLY THE TIMING RANGE 0-9000ns IS SUPPORTED!

        Keyword arguments:
        histo - The TH1 to plot
        path  - The location of the plot in the file
        """

        self.SplitCanvas(1,1,True)
        self.WriteTitle(path)

        pad = self.pads[3].cd()

        # The two histograms are automatically overlaid if needed
        self.DrawTH1(histo, path, pad)

        pad.cd()

        mcpWidth = timing_cuts.GetWidth(True, 0)
        mcpTimes = timing_cuts.GetTimes(True, 0)
        rdpWidth = timing_cuts.GetWidth(False, 7500)
        rdpTimes = timing_cuts.GetTimes(False, 7500)
        tboxes = []

        min = float(histo.GetXaxis().GetXmin())
        max = float(histo.GetXaxis().GetXmax())
        ran = max - min

        if "MCP" in self.files or "OLDMCP" in self.files:
            for ii, width in enumerate(mcpWidth):
                time = mcpTimes[ii]
                tbox = ROOT.TPave(0.1 + (0.8 / ran * (time - 4 * width - min)), 0.1, 0.1 + (0.8 / ran * (time + 4 * width - min)), 0.9, 0, "blNDC")
                tbox.SetFillStyle(3002)
                tbox.SetLineWidth(1)
                tbox.SetLineStyle(3)
                tbox.SetLineColor(2)
                tbox.SetFillColor(2)
                tbox.Draw()
                pad.Modified()
                tboxes.append(tbox)

        if "RDP" in self.files or "OLDRDP" in self.files:
            for ii, width in enumerate(rdpWidth):
                time = rdpTimes[ii]
                tbox = ROOT.TPave(0.1 + (0.8 / ran * (time - 4 * width - min)), 0.1, 0.1 + (0.8 / ran * (time + 4 * width - min)), 0.9, 0, "blNDC")
                tbox.SetFillStyle(3002)
                tbox.SetLineWidth(1)
                tbox.SetLineStyle(3)
                tbox.SetLineColor(1)
                tbox.SetFillColor(1)
                tbox.Draw()
                pad.Modified()
                tboxes.append(tbox)

        # Re-draw the stats boxes so they are on top of the cuts TPaves.
        for type in self.files.iterkeys():
            histo = self.canvas.FindObject(type)
            if histo:
                stats = histo.FindObject("stats")
                if stats:
                    stats.Draw()
                    pad.Modified()

        self.PrintCanvas()

        return
