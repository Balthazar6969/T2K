#!/usr/bin/python
import ROOT
import re
import datetime
import math
import sys
import os
from optparse import OptionParser

def DoNothing(Hist):
    pass

class pdf_base(object):
    """
    """

    def __init__(self, mcpFile, rdpFile, oldmcpFile, oldrdpFile, options):
        """
        Automatically called when a new object is created.

        Handles the command-line arguments that the user passed in.

        Arguments:
        mcpFile  - The TFile containing MCP histograms, or False if MCP plots are not required.
        rdpFile  - The TFile containing RDP histograms, or False if RDP plots are not required.
        options  - Dict with the following keys
                    out      - Prefix for PDF document names. The prefix is followed by [detector].pdf.
                    scale    - The method by which to scale RDP/MCP histograms.
                    title    - The title string to appear on the first page of generated PDFs
                    comment  - The comment string to appear on the first page of generated PDFs.
                    macros   - Whether to generate .root macros as well as PDFs.
                    rdptitle - Override the name of RDP histograms (if doing comparisons that aren't Data/MC)
                    mcptitle - Override the name of MCP histograms (if doing comparisons that aren't Data/MC)
        """

        self.files = {}
        self.titles = {}
        colourSet = [1,2,4,6]
        self.colours = {}
        self.fileorder = []

        if mcpFile:
            self.files["MCP"] = mcpFile
            self.titles["MCP"] = options.mcptitle
            self.fileorder.append("MCP")
        if rdpFile:
            self.files["RDP"] = rdpFile
            self.titles["RDP"] = options.rdptitle
            self.fileorder.append("RDP")
        if oldmcpFile:
            self.files["OLDMCP"] = oldmcpFile
            self.titles["OLDMCP"] = options.oldmcptitle
            self.fileorder.append("OLDMCP")
        if oldrdpFile:
            self.files["OLDRDP"] = oldrdpFile
            self.titles["OLDRDP"] = options.oldrdptitle
            self.fileorder.append("OLDRDP")

        for f,F in enumerate(self.fileorder):
            self.colours[F] = colourSet[f]


        self.outFilePrefix = options.out
        self.scale = options.scale
        self.title = options.title
        self.comment = options.comment
        self.macros = options.macros
        self.web = options.web
        self.webtar = options.webtar

        # The canvas to write to and the pads it contains
        self.canvas = ROOT.TCanvas("c", "c", 800, 600)
        self.pads = []

        # Number of RDP/MCP events, for scaling histograms
        self.numEvents = {}

        # Simple counter of the number of pages of plots we've created
        self.numCanvases = 0

        # Each canvases has a title, and we keep track of them here. If there are any 
        # duplicates, we need to know about them when deciding where to save PNG images
        # for the web-based output.
        self.numCanvasesWithTitle = {}

        # Details of each canvas, stored so we generate a table of contents for webpages
        self.canvasDetails = []

        # Information on the setup of the current slide we're writing to.
        # Default values here; the first printed page will be number 1.
        self.slide = { "Number" : 0, "Header" : False, "Title" : "" }

        # If an object is defined and drawn in a function, but the canvas is printed after 
        # the function has returned, ROOT will "forget" about it, and not draw it. Adding
        # the object to this array will keep it active until the canvas is printed.
        self.tmpstorage = []


    def CustomPlots(self, obj, path):
        """
        Generate any custom plots, and return whether the object still needs processing.

        This is the main function that is overridden by most of the customisation scripts.
        """

        return True

    def PrintTitlePage(self, name):
        """
        Print the cover page for the current PDF document.
        
        Arguments:
        name - The category for this PDF document (DsECal, Global etc.).
        """

        self.SplitCanvas(1,1,False)
        pad = self.pads[1].cd()

        # Main titles
        tl = ROOT.TLatex()
        tl.SetTextColor(1)
        tl.SetTextFont(62)
        tl.SetTextAlign(11)
        tl.SetTextSize(0.1)
        tl.DrawLatex(0.10,0.75,self.title)
        tl.DrawLatex(0.10,0.6,name)

        # General information
        tl.SetTextSize(0.05)
        if self.comment:
            tl.DrawLatex(0.10,0.4,self.comment)
        plotinfo = ""
        if self.scale == 0:
            plotinfo += "Plots not normalised"
        elif self.scale == 1:
            plotinfo += "Plots normalised to 1"
        elif self.scale == 2:
            plotinfo += "Plots scaled by number of events"
        tl.DrawLatex(0.10,0.3,plotinfo)

        # Detailed information
        tl.SetTextSize(0.025)
        detail = "Generated: " + datetime.datetime.now().ctime()

        # By default we print the number of events on the cover. However the input files
        # may not contain this information. If we actually need this information (for 
        # example to scale plots by the number of events in the sample) then the 
        # exception will be raised and not caught at that point.
        try:
            detail += ". NumEvents: "
            for index, (type, file) in enumerate(self.files.iteritems()):
                num = self.GetNumEvents(type)
                if index > 0:
                    detail += ", "
                detail += self.titles[type] + " " + str(int(num)) 
        except UserWarning:
            pass

        tl.DrawLatex(0.10,0.10, detail)

        # Actually print the cover page.
        self.pads[1].Modified()
        self.pads[1].Draw()
        self.canvas.Modified()
        self.canvas.Draw()
        self.canvas.cd()
        self.canvas.Print(self.outFile)

        if self.web:
            saveas = self.web + "/" + self.topLevelDir + "/Title.png"
            self.canvas.SaveAs(saveas)
            self.canvasDetails.append({"title" : "Title page", "location" : saveas})

        return

    def PrintCanvas(self):
        """
        Tidy up a canvas and print it to the current PDF document. 
        
        Backgrounds of all pads are set to white, and a slide number 
        is added in the top right corner of the slide.
        """

        # Make all the backgrounds white.
        for pad in reversed(self.pads):
            pad.cd()
            pad.SetFillColor(0)
            pad.Modified()
            pad.Draw()

        # Add the slide number in the top right.
        if self.slide["Header"]:
            self.pads[1].cd()
        else:
            self.canvas.cd()

        tl = ROOT.TLatex()
        tl.SetTextSize(0.33)
        tl.SetTextFont(42)
        tl.SetTextAlign(33)
        tl.DrawLatex(0.99,0.90,str(self.slide["Number"]))
        ROOT.gPad.Modified()
        ROOT.gPad.Draw()

        # Print the slide
        self.canvas.cd()
        self.canvas.Modified()
        self.canvas.Print(self.outFile)

        # Generate a .root macro if the user asked for it.
        if self.macros:
            self.canvas.Print(self.outFilePrefix + self.topLevelDir + "_slide_" + str(self.slide["Number"]) + ".root")

        if self.web:
            saveas = self.GetWebImageLocation()
            self.canvas.SaveAs(saveas)
            self.canvasDetails.append({"title" : self.slide["Title"], "location" : saveas})

        self.numCanvases += 1
        try:
          x
        except NameError:
          pass
        else:
          del self.tmpstorage[:]
          self.tmpstorage = []

        return

    def GetWebImageLocation(self, makePath = True):
        """
        Get the location on disk to store the current slide as a PNG.

        The location is based on the title of the slide, and is 
        guaranteed to be unique.

        Arguments:
        makePath - Whether to create the directory in which the image
                   will be saved.
        """

        if self.slide["Title"] not in self.numCanvasesWithTitle:
            self.numCanvasesWithTitle[self.slide["Title"]] = 0
        else:
            self.numCanvasesWithTitle[self.slide["Title"]] += 1

        suffix = ""
        if self.numCanvasesWithTitle[self.slide["Title"]] > 0:
            suffix = "-" + str(self.numCanvasesWithTitle[self.slide["Title"]])

        saveasdir = "/".join([self.web, self.topLevelDir]) + "/" + "/".join(self.slide["Title"].split(" :: "))
        saveas = "/".join([saveasdir, saveasdir.split("/")[-1] + suffix + ".png"])

        if makePath and not os.path.exists(saveasdir):
            os.makedirs(saveasdir)

        return saveas
        
    def GetHistoInDir(self, dir, subdirname):
        """
        Retrieve a histogram from the given subdirectory.

        The first histogram in subdir is returned. If the input files were
        produced by a GRTF analysis module, then there will only be one
        histogram in the directory.

        Arguments:
        dir        - The TDirectory to search in.
        subdirname - The name of the TDirectory within dir from which to retrieve
                     a histogram.
        """

        subdir = dir.Get(subdirname)

        if subdir and subdir.IsA().InheritsFrom("TDirectory"):
            keys = subdir.GetListOfKeys()
            for key in keys:
                obj = subdir.Get(key.GetName())
                if obj.IsA().InheritsFrom("TH1"):
                    return obj
                elif obj.IsA().InheritsFrom("TGraph"):
                    return obj
                else:
                    print obj.IsA().GetName()

        return False


    def GetDataAndMCHistos(self, path, histoname, projdir = ''):
        """
        Retrive a histogram from the RDP and MCP files, returning them in a dict.
        
        The dict's keys are "MCP" and "RDP". If the histogram can not be found, 
        the value in the dict is set to False.

        Arguments:
        path      - The path within which to histogram is found.
        histoname - The name of the histogram to retrieve.
        """

        results = {}

        for type, file in self.files.iteritems():
            path = self.GetPath(path, type)
            dir  = file.GetDirectory(path)

            if dir:
                file.Cd(path)
                if projdir == 'X':
                    histo2D = ROOT.gDirectory.Get(histoname)
                    results[type] = histo2D.ProjectionX()
                elif projdir == 'Y':
                    histo2D = ROOT.gDirectory.Get(histoname)
                    results[type] = histo2D.ProjectionY()
                elif projdir == 'Xprofile':
                    histo2D = ROOT.gDirectory.Get(histoname)
                    results[type] = histo2D.ProfileX()
                elif projdir == 'Yprofile':
                    histo2D = ROOT.gDirectory.Get(histoname)
                    results[type] = histo2D.ProfileY()
                else:
                    results[type] = ROOT.gDirectory.Get(histoname)

        return results

    def IsRDP(self, path):
        """
        Whether the given path is from the RDP or MCP file. 
        
        Returns True if from the RDP file, or False if not.

        Arguments:
        path - The path to query.
        """

        rdpbase = re.sub(":/.*", "", files["RDP"].GetPath())
        oldrdpbase = re.sub(":/.*", "", files["OLDRDP"].GetPath())
        thisbase = re.sub(":/.*", "", path)

        return (thisbase == rdpbase or thisbase == oldrdpbase)

    def GetFileType(self, path):
        thisbase = re.sub(":/.*", "", path)

        for type, file in self.files.iteritems():
            typebase = re.sub(":/.*", "", file.GetPath())

            if thisbase == typebase:
                return type

        return False


    def WriteTitle(self, title, extra=False):
        """
        Write the title of a slide. 

        The title is expected to be based on the path of the TDirectory being
        written to this slide. For that reason, all '/' are replaced by 
        ' :: ' for nicer formatting.

        Arguments:
        title - The path to convert to a title.
        extra - If set to "MCP" or "RDP", the extra string is added to the title.
        """

        if extra == "MCP":
            title += "/#color[2]{ MCP}"
        elif extra == "RDP":
            title += "/RDP"
        elif extra == "OLDMCP":
            title += "/#color[46]{ OLDMCP}"
        elif extra == "OLDRDP":
            title += "/#color[15]{ OLDRDP}"

        title = re.sub(".*:/", "", title)
        title = re.sub("/", " :: ", title)
        self.slide["Title"] = title

        self.pads[1].cd()
        tl = ROOT.TLatex()
        tl.SetTextColor(1)
        tl.SetTextAlign(23)
        tl.SetTextSize(0.2)
        tl.DrawLatex(0.5,0.5,title)
        self.pads[1].Modified()
        self.pads[1].Draw()
        self.canvas.Modified()
        self.canvas.Draw()
        self.canvas.cd()

        return

    def GetNumEvents(self, filetype):
        if filetype in self.numEvents:
            return self.numEvents[filetype]

        raise UserWarning(filetype + " File is missing info needed to determine number of events.")

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

        Arguments:
        filetype - Either "MCP", "RDP", "OLDMCP", "OLDRDP".
        """

        # Check for a cached result
        if filetype in self.numEvents:
            return self.numEvents[filetype]

        file = self.files[filetype]

        # Method 1 - number of entries in histogram
        histo = file.Get(file.GetPath() + "/Event/NGlobalReconObjects/All/hNGlobalReconObjectsAll")

        if histo:
            self.numEvents[filetype] = float(histo.GetEntries())
        else:
            ttree = file.Get(file.GetPath() + "/NumEvents")

            if not ttree:
                raise UserWarning(filetype + " File is missing info needed to determine number of events.")

            ttree.GetEntry(0)
            self.numEvents[filetype] = float(ttree.All)

        return self.numEvents[filetype]

    def GetMaxMin(self, histos):
        """
        Get the maximum and minimum value of the RDP and MCP histograms.

        Returns an array of [max, min].

        Arguments:
        histos - dict containing histograms
        """

        maxi = -1e20
        mini = 1e20

        for type, histo in histos.iteritems():
            maxi = max(histo.GetMaximum(), maxi)
            mini = min(histo.GetMinimum(0.000001), mini)

        return [maxi, mini]

    def DrawTH1(self, histo, dirpath, pad, proj = '', customize=DoNothing):
        """
        Draw a 1D histogram on the given pad, overlaying Data/MC if appropriate.

        Histograms are scaled if requested, and the MCP histogram and stats box
        is always coloured red. 

        Arguments:
        histo      - The histogram to print.
        dirpath    - The path in which the histogram is found.
        pad        - The TPad/TCanvas on which to draw the histogram.
        proj       - Direction on which to project a 2D histo
        customize  - A function to modify the histogram before drawing. Exp: changing the range.
        """

        pad.Clear()

        if self.logscale:
            pad.SetLogy(1)
            pad.SetLogz(0)
        else:
            pad.SetLogy(0)
            pad.SetLogz(0)

        if histo:
            histos = self.GetDataAndMCHistos(dirpath, histo.GetName(), proj)
            pad.cd()

            # Scale the plots in needed
            if self.scale and histo.IsA().InheritsFrom("TH1"):
                for index, (type, file) in enumerate(self.files.iteritems()):
                    if dirpath.rfind("Eff") != (len(dirpath) - 3):
                        if not type in histos:
                            continue

                        histos[type].Sumw2()
                        integral = histos[type].Integral(0, histos[type].GetNbinsX() + 1)

                        if integral > 0:
                            if self.scale == 1:
                                # Scale to 1
                                histos[type].Scale(1 / integral)
                            elif self.scale == 2: 
                                # Scale to number of events
                                numer = False

                                for testtype in ("RDP", "MCP", "OLDRDP", "OLDMCP"):
                                    try:
                                        numer = self.GetNumEvents(testtype)
                                        break
                                    except UserWarning:
                                        # No files contain scaling info - must raise an error
                                        if testtype == "OLDMCP":
                                            raise
                                
                                histos[type].Scale(numer / self.GetNumEvents(type))

            # Set the maximum and minimum ranges. These are chosen so the stats boxes
            # do not cover up any histogram points.
            if histo.IsA().InheritsFrom("TH1"):
                [maxi, mini] = self.GetMaxMin(histos)

                if self.logscale:
                    if maxi > 0:
                        maxi = math.pow(10, int(math.log10(maxi)) + 1.5)
                    else:
                        maxi = 10

                    if mini > 0:
                        mini = math.pow(10, int(math.log10(mini)) - 1.5)
                    else:
                        mini = 0.001
                else:
                    if 0 < maxi < 1.5:
                        maxi = maxi * 1.3
                    elif maxi > 0:
                        maxi = (maxi + math.sqrt(maxi)) * 1.3
                    else:
                        maxi = 2

                    mini = 0

            # Do the actual drawing
            numdrawn = 0

            for index, type in enumerate(("OLDMCP", "OLDRDP", "MCP", "RDP")):
                if not type in histos:
                    continue

                if histo.IsA().InheritsFrom("TH1"):
                    histos[type].SetMaximum(maxi)
                    histos[type].SetMinimum(mini)

                # Draw the histogram
                histos[type].SetLineColor(self.colours[type])
                histos[type].SetMarkerColor(self.colours[type])
                histos[type].SetName(self.titles[type])
                if self.noerrors:
                    drawopt = ""
                else:
                    drawopt = "E "

                if histo.IsA().InheritsFrom("TGraph"):
                    if numdrawn == 0:
                        drawopt += 'A'
                    drawopt += 'P'

                if numdrawn > 0:
                    drawopt += "SAMES"

                histos[type].Draw(drawopt)
                ROOT.gPad.Update()
                customize(histos[type])
                histos[type].Draw(drawopt)
                ROOT.gPad.Update()

                # Fiddle with the stats box
                stats = histos[type].FindObject("stats")
                if stats:
                    x1 = stats.GetX1NDC()
                    y1 = stats.GetY1NDC()
                    x2 = stats.GetX2NDC()
                    y2 = stats.GetY2NDC()
                    stats.SetX2NDC(x2 - (x2-x1)/2)
                    stats.SetX1NDC(x1 - (x2-x1)/4)
                    stats.SetY1NDC(y1 + (y2-y1)/4)
                    currwidth = stats.GetX2NDC() - stats.GetX1NDC()
                    stats.SetX1NDC(stats.GetX1NDC() - (1 - numdrawn) * currwidth)
                    stats.SetX2NDC(stats.GetX2NDC() - (1 - numdrawn) * currwidth)
                    stats.SetLineColor(self.colours[type])
                    stats.SetTextColor(self.colours[type])
                elif histo.IsA().InheritsFrom("TGraph"):
                    # TGraphs don't have stats boxes - add our own labels
                    label = ROOT.TPaveText(0.8 - numdrawn * 0.1, 0.92, 0.9 - numdrawn * 0.1, 0.98, "NDC T")
                    label.AddText(self.titles[type])
                    label.SetTextColor(self.colours[type])
                    label.SetFillColor(0)
                    label.SetLineColor(0)
                    label.SetLineWidth(1)
                    label.SetShadowColor(0)
                    self.tmpstorage.append(label)
                    label.Draw()
                    ROOT.gPad.Update()

                numdrawn += 1

        # Re-draw the stats boxes so they are on top
        for type in self.files.iterkeys():
            histo = pad.FindObject(type)
            if histo:
                stats = histo.FindObject("stats")
                if stats:
                    stats.Draw()
                    pad.Modified()

        return

    def DrawTH2(self, histo, dirpath, pad, customize=DoNothing):
        """
        Print a TH2 histogram to a pad. 
        
        The histogram should already have been scaled if appropriate.
        The stats box of an MCP histogram is coloured red.

        Arguments:
        histo      - The histogram to print.
        dirpath    - The path in which the histogram is found.
        pad        - The TPad/TCanvas on which to draw the histogram.
        customize  - A function to modify the histogram before drawing. Exp: changing the range.
        """

        pad.Clear()
        pad.cd()

        if self.logscale:
            pad.SetLogy(0)
            pad.SetLogz(1)
        else:
            pad.SetLogy(0)
            pad.SetLogz(0)

        if histo:
            type = self.GetFileType(dirpath)
            histo.SetName(self.titles[type])

            histo.Draw("COLZ")
            ROOT.gPad.Update()
            customize(histo)
            histo.Draw("COLZ")
            ROOT.gPad.Update()

            # Fiddle with the stats box
            stats = histo.FindObject("stats")
            if stats:
                x1 = stats.GetX1NDC()
                y1 = stats.GetY1NDC()
                x2 = stats.GetX2NDC()
                y2 = stats.GetY2NDC()
                stats.SetX2NDC(x2 - (x2-x1)/2)
                stats.SetX1NDC(x1 - (x2-x1)/4)
                stats.SetY1NDC(y1 + (y2-y1)/2)

                stats.SetLineColor(self.colours[type])
                stats.SetTextColor(self.colours[type])

                stats.Draw()
            ROOT.gPad.Update()

        return

    def DrawAutoTH2(self, histo, dirpath, pad, customize=DoNothing):
        """
        Print a TH2 histogram to a pad. 
        
        The histogram should already have been scaled if appropriate.
        The stats box of an MCP histogram is coloured red.

        Arguments:
        histo      - The histogram to print.
        dirpath    - The path in which the histogram is found.
        pad        - The TPad/TCanvas on which to draw the histogram.
        customize  - A function to modify the histogram before drawing. Exp: changing the range.
        """

        pad.Clear()
        pad.cd()

        histos = self.GetDataAndMCHistos(dirpath, histo.GetName())

        subPad = []
        if len(histos) > 1:
            pad.Divide(2,2)
            for p in range(1,5):
                subPad.append(pad.GetPrimitive(pad.GetName()+"_"+str(p)))
        else:
            subPad.append(pad)

        for t,T in enumerate(histos.keys()):
            if self.logscale:
                subPad[t].SetLogy(0)
                subPad[t].SetLogz(1)
            else:
                subPad[t].SetLogy(0)
                subPad[t].SetLogz(0)

            subPad[t].cd()
            histos[T].SetName(self.titles[T])


            histos[T].Draw("COLZ")
            subPad[t].Update()
            customize(histos[T])
            histos[T].Draw("COLZ")
            subPad[t].Update()

            # Fiddle with the stats box
            stats = histos[T].FindObject("stats")
            x1 = stats.GetX1NDC()
            y1 = stats.GetY1NDC()
            x2 = stats.GetX2NDC()
            y2 = stats.GetY2NDC()
            stats.SetX2NDC(x2 - (x2-x1)/2)
            stats.SetX1NDC(x1 - (x2-x1)/4)
            stats.SetY1NDC(y1 + (y2-y1)/2)

            stats.SetLineColor(self.colours[T])
            stats.SetTextColor(self.colours[T])

            stats.Draw()
            subPad[t].Update()

        return

    def ScaleTH2(self, histos, path):
        """
        Scale 2D histograms, if appropriate.

        Arguments:
        histos - The histograms to scale
        path   - The path the 
        """

        for type, histo in histos.iteritems():
            path = self.GetPath(path, type)

            if path.rfind("Eff") != (len(path) - 3):
                # Don't scale efficiency plots
                histo.Sumw2()
                integral = histo.Integral(0, histo.GetNbinsX() + 1, 0, histo.GetNbinsY() + 1)

                if integral > 0:
                    if self.scale == 1:
                        histo.Scale(1 / integral)
                    elif self.scale == 2:
                        numer = False

                        for testtype in ("RDP", "MCP", "OLDRDP", "OLDMCP"):
                            try:
                                numer = self.GetNumEvents(testtype)
                                break
                            except UserWarning:
                                if testtype == "OLDMCP":
                                    raise

                        histo.Scale(numer / self.GetNumEvents(type))

        [maxi, mini] = self.GetMaxMin(histos)

        for type, histo in histos.iteritems():
            histo.SetMaximum(maxi * 1.1)

        return

    def SplitCanvas(self, x, y, header=False):
       """
       Split the self.canvas object into smaller subpads.

       Arguments:
       x      - The number of pads to split into in the X direction.
       y      - The number of pads to split into in the Y direction.
       header - Whether to include a pad for the title of the page (top 10%).
       """

       # Remove all the current pads
       for pad in reversed(self.pads):
           if pad:
               pad.Clear()
           else:
               print "No pad to clear..." + ROOT.gDirectory.GetPath()

       self.canvas.Clear()
       self.canvas.cd()
       self.pads = []
       self.pads.append(self.canvas)

       base = self.canvas

       self.slide["Header"] = header
       self.slide["Number"] += 1

       if header:
           self.pads.append(ROOT.TPad("p" + str(len(self.pads)), "p" + str(len(self.pads)), 0, 0.9, 1, 1))
           self.pads[-1].Draw()
           self.pads[-1].cd()
           self.pads[-1].Range(0,0,1,1)
           self.pads[-1].Modified()
           self.canvas.Modified()
           self.canvas.cd()

           self.pads.append(ROOT.TPad("p" + str(len(self.pads)), "p" + str(len(self.pads)), 0, 0, 1, 0.9))
           self.pads[-1].Draw()
           self.pads[-1].cd()
           self.pads[-1].Range(0,0,1,1)
           self.pads[-1].Modified()
           self.canvas.Modified()
           base = self.pads[-1]
           base.cd()

       # At this point, the "base" is the bit of the canvas that will have 
       # plots drawn on it. Either the bottom 90% of the canvas (if there is
       # a header) or th ecanvas itself (if there isn't). Now create as many
       # new pads as we need within the "base". Whatever the "base" is, the
       # range is (0,0) to (1,1). Pads are numbered l-r, t-b.
       for this_y in xrange(y):
           for this_x in xrange(x):
               base.cd()
               x_min = this_x / float(x)
               x_max = (this_x + 1) / float(x) 
               y_max = 1 - this_y / float(y)
               y_min = 1 - (this_y + 1) / float(y) 

               self.pads.append(ROOT.TPad("p" + str(len(self.pads)), "p" + str(len(self.pads)), x_min, y_min, x_max, y_max))
               self.pads[-1].Draw()
               self.pads[-1].cd()
               self.pads[-1].Range(0,0,1,1)
               base.cd()
               base.Modified()

       self.canvas.cd()
       self.canvas.Modified()

       return

    def GetPath(self, basepath, type):
        newbase = re.sub(":/.*", "", self.files[type].GetPath())
        oldbase = re.sub(":/.*", "", basepath)

        return basepath.replace(oldbase, newbase)
