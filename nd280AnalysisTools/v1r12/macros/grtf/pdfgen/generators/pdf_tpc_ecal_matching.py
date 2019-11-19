from grtf.pdfgen.generators import pdf_basics

class pdf_tpc_ecal_matching(pdf_basics.pdf_offsets):
    def CustomPlots(self, obj, path):
        todo = True

        if obj.IsA().InheritsFrom("TDirectory"):
            if obj.GetPath().find("Offsets") != -1 and obj.GetPath().find("atched") != -1:
                self.DrawOffsetPlots(obj)
                todo = False

        return todo

    def GenerateNumEvents(self, filetype):
        """
        Get the number of events that were used to create the given file. 

        Two methods are currently supported:
        1) Get the number of entries in the histogram
           "/Event/NGlobalReconObjects/All/hNGlobalReconObjectsAll".
        2) Get the value of a branch in the TTree "/NumEvents". The default 
           branch is "All", but for TPC-Other matching studies a branch 
           relevant to the exact matching is studied (e.g. the "TPC1P0D" branch
           for TPC1-P0D matching).

        If the input file contains neither of these objects a UserWarning
        is raised.

        If a value is successfully found, it is cached for further calls to
        this function.

        Keyword arguments:
        filetype - Either "MCP" or "RDP".
        """

        file = self.files[filetype]
        ttree = file.Get(file.GetPath() + "/NumEvents")

        if not ttree:
            raise UserWarning(filetype + " File is missing info needed to determine number of events.")

        ttree.GetEntry(0)

        if self.topLevelDir == "DEBUG" or self.topLevelDir == "Time":
            self.numEvents[filetype] = float(ttree.All)
        else:
            self.numEvents[filetype] = float(getattr(ttree, self.topLevelDir.replace("-", "")))

        return self.numEvents[filetype]
