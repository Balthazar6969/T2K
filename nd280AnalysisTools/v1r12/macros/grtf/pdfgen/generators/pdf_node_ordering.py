from grtf.pdfgen.generators import pdf_base

class pdf_node_ordering(pdf_base.pdf_base):
    def CustomPlots(self, obj, path):
        todo = True

        if obj.IsA().InheritsFrom("TDirectory"):
            if obj.GetPath().find(" - ") != -1:
                self.Drawnode_orderingPlots(obj)
                todo = False

        return todo

    def Drawnode_orderingPlots(self, dir):
        """
        Draw plots related to the ordering of nodes.

        Keyword arguments:
        dir - The directory containing the plots.
        """
        self.sup.SplitCanvas(1,1,True)
        self.sup.WriteTitle(dir.GetPath())

        pad   = self.pads[3].cd()
        histo = self.GetHistoInDir(dir, "ByPos")
        self.DrawTH1(histo, dir.GetPath() + "/ByPos", pad)

        self.PrintCanvas()

        return
