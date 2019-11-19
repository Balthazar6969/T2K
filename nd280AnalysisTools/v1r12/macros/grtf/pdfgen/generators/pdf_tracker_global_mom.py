from grtf.pdfgen.generators import pdf_base

class pdf_tracker_global_mom(pdf_base.pdf_base):
    def CustomPlots(self, obj, path):
        todo = True

        if obj.IsA().InheritsFrom("TDirectory"):
            if obj.GetPath().count("(") == 2 or obj.GetName() == "forwards" or obj.GetName() == "backwards":
                self.Drawtracker_global_momPlots(obj)
                todo = False

        return todo

    def Drawtracker_global_momPlots(self, dir):
        """
        Draw plots realted to the change in momentum when adding to tracker objects.

        1st page contains 1D Fractional plots.
        2nd page contains 2D plots
        3rd page contains 1D plots split into momentum ranges.

        Keyword arguments:
        dir - The directory containing the plots.
        """
       
        # 1st page
        self.SplitCanvas(2,2,True)
        self.WriteTitle(dir.GetPath())

        pad   = self.pads[3].cd()
        histo = self.GetHistoInDir(dir, "1D")
        self.DrawTH1(histo, dir.GetPath() + "/1D", pad)

        pad   = self.pads[5].cd()
        histo = self.GetHistoInDir(dir, "Fractional")
        self.DrawTH1(histo, dir.GetPath() + "/Fractional", pad)

        self.PrintCanvas()

        # 3rd page
        self.SplitCanvas(2,2,True)
        self.WriteTitle(dir.GetPath())

        histo = self.GetHistoInDir(dir, "2D")
        path = dir.GetPath() + "/2D"
        histos = self.GetDataAndMCHistos(dir.GetPath() + "/2D", histo.GetName())
        self.ScaleTH2(histos, path)

        for index, type in enumerate(self.fileorder):
            if type in histos:
                path = self.GetPath(path, type)
                self.DrawTH2(histos[type], path, self.pads[3 + index])

        self.PrintCanvas()

        # 3rd page
        self.SplitCanvas(2,2,True)
        self.WriteTitle(dir.GetPath())

        pad   = self.pads[3].cd()
        histo = self.GetHistoInDir(dir, "0-200")
        self.DrawTH1(histo, dir.GetPath() + "/0-200", pad)

        pad   = self.pads[4].cd()
        histo = self.GetHistoInDir(dir, "200-500")
        self.DrawTH1(histo, dir.GetPath() + "/200-500", pad)

        pad   = self.pads[5].cd()
        histo = self.GetHistoInDir(dir, "500-1000")
        self.DrawTH1(histo, dir.GetPath() + "/500-1000", pad)

        pad   = self.pads[6].cd()
        histo = self.GetHistoInDir(dir, "1000-1500")
        self.DrawTH1(histo, dir.GetPath() + "/1000-1500", pad)

        self.PrintCanvas()

        return
