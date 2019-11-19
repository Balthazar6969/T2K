#!/usr/bin/python
import ROOT
import re
import datetime
import math
import sys
import os
from optparse import OptionParser

class make_pdf:
    """
    Class to generate PDF documents for GRTF validations. 
    
    The input files should be generated using one of the GRTF nd280AnalysisTools macros.
    The format of the input files should be a logically-organised multi-level TDirectory 
    structure. A separate PDF document is created for each top-level directory. 
    """
    def __init__(self, mcpFile, rdpFile, oldmcpFile, oldrdpFile, options):
        """
        Instantiate the requested custom generator class.
        """

        if not options.custom:
            options.custom = "pdf_base"

        try:
            _package = __import__('grtf.pdfgen.generators', globals(), locals(), [options.custom])
            _module = getattr(_package, options.custom)
        except AttributeError:
            print 'NOTE: Custom module "'+options.custom+'" not found in grtf.pdfgen.generators. Importing grtf_VFT.pdfgen.generators.'
            try:
                _package = __import__('grtf_VFT.pdfgen.generators', globals(), locals(), [options.custom])
                _module = getattr(_package, options.custom)
            except AttributeError:
                print 'WARNING: Custom module "'+options.custom+'" not found in grtf_VFT.pdfgen.generators. ====> EXIT !'
                sys.exit(1)

        _class = getattr(_module, options.custom)

        self.generator = _class(mcpFile, rdpFile, oldmcpFile, oldrdpFile, options)

    def MakePlots(self, logscale, noerrors = False):
        """
        The main function called to generate the PDFs. Returns the total number of
        pages of plots produced.

        Keyword arguments:
        logscale - Whether to use a logarithmic Y/Z axis for TH1/TH2 plots.
        noerrors - Whether to draw the error bars on the TH1 plots
        """

        self.generator.logscale = logscale
        self.generator.noerrors = noerrors
        self.generator.paths = []

        if len(self.generator.files) == 0:
            print "No files specified - nothing to do. Use -h option for help."
            return

        top = self.generator.files.values()[0]
        links = []
        prevNumCanvases = 0

        comment = ""
        title = ""
        if isinstance(self.generator.comment, str):
          comment = " (" + self.generator.comment + ")"
        if isinstance(self.generator.title, str):
          title = self.generator.title

        # Get the list of top-level objects and generate a PDF document for each
        for key in top.GetListOfKeys():
            name = key.GetName()
            obj = top.Get(name)

            # We may have stored the total number of events in a top-level TTree - just skip over it
            if obj.IsA().InheritsFrom("TTree"):
                continue

            self.generator.slide["Number"] = 0
            self.generator.topLevelDir = name
            self.generator.canvasDetails = []
            self.generator.numCanvasesWithTitle = {}
            self.generator.outFile = self.generator.outFilePrefix + name
            self.generator.outFile += ".pdf"

            # This syntax opens the PDF document, doesn't print anything, and keeps it open
            self.generator.canvas.Print(self.generator.outFile + "[")

            if self.generator.web and not os.path.exists(self.generator.web + "/" + name):
                os.makedirs(self.generator.web + "/" + name)

            # Do the main work whilst the PDF document is open
            for type in self.generator.files.iterkeys():
                try:
                    self.generator.GenerateNumEvents(type)
                except UserWarning:
                    pass
            self.generator.PrintTitlePage(name)
            self.Recurse(obj, top.GetPath())

            # This syntax closes the PDF document without printing anything
            self.generator.canvas.Print(self.generator.outFile + "]")

            if self.generator.web:
                toc = ""
                body = ""
                for enum, slide in enumerate(self.generator.canvasDetails):
                    num = str(enum + 1)
                    toc += "<li><a href=\"#" + num + "\">" + slide["title"] + "</a></li>\n"
                    body += "<p><a name=\"" + num + "\"></a>" + slide["title"] + "</p><p><img src=\"" + slide["location"].replace(self.generator.web + "/" + self.generator.topLevelDir, "").lstrip("/") + "\" height=\"95%\" /></p>\n"
                html = "<html><head><title>" + title + comment + " :: " + name + "</title></head><body><h2>" + name + "</h2><ol>" + toc + "</ol><p></p>" + body + "</body></html>"

                index = open(self.generator.web + "/" + name + "/index.html", "w")
                index.write(html)
                index.close()

                thisNumCanvases = self.generator.numCanvases - prevNumCanvases
                prevNumCanvases = self.generator.numCanvases
                links.append("<li><a href=\"" + name + "\">" + name + " (" + str(thisNumCanvases) + " pages of plots)</a></li>\n")

        if self.generator.web:
            html = "<html><head><title>" + title + comment + "</title></head><body><ul>" + "".join(sorted(links)) + "</ul></body></html>"
            index = open(self.generator.web + "/index.html", "w")
            index.write(html)
            index.close()
            if self.generator.webtar:
              os.system("tar zcf "+self.generator.web+".tar.gz "+self.generator.web)

        return self.generator.numCanvases

    def Recurse(self, obj, path):
        """
        Main function for recursing down a directory structure in a TFile. 
        
        If the object passed in is a TDirectory, this function is called for all the objects within it. 
        If a TH1/TH2/TCanvas then the object is printed to the current PDF document (after merging 
        Data/MC histograms if required). Special functions are called for some directories, in order to 
        prettify the printing. The set of special functions available is controlled by the self.custom 
        parameter (--custom on the command line).

        Keyword arguments:
        obj  - The current object. Can be any TObject, and will be handled appropriately.
        path - The current path within the file.
        """

        # First see if a custom plot has been defined
        todo = self.generator.CustomPlots(obj, path)

        if not todo:
            # Already been handled by the custom module
            return

        # If not, perform the default actions
        if obj.IsA().InheritsFrom("TH2"):
            # 2D histograms - side-by-side on the same page 
            histos = self.generator.GetDataAndMCHistos(path, obj.GetName())
            if len(self.generator.fileorder) > 1:
                self.generator.SplitCanvas(2,2,True)
            else:
                self.generator.SplitCanvas(1,1,True)
            self.generator.WriteTitle(path)
            self.generator.ScaleTH2(histos, path)

            for index, type in enumerate(self.generator.fileorder):
                if histos[type]:
                    newpath = self.generator.GetPath(path, type)
                    self.generator.DrawTH2(histos[type], newpath, self.generator.pads[3 + index])

            self.generator.PrintCanvas()
        elif obj.IsA().InheritsFrom("TH1"):
            # 1D histograms - overlaid on the same plot
            self.generator.SplitCanvas(1,1,True)
            self.generator.WriteTitle(path)
            self.generator.DrawTH1(obj, path, self.generator.pads[3])
            self.generator.PrintCanvas()
        elif obj.IsA().InheritsFrom("TGraph"):
            # Simply print a TCanvas verbatim
            self.generator.SplitCanvas(1,1,True)
            self.generator.WriteTitle(path)
            self.generator.DrawTH1(obj, path, self.generator.pads[3])
            self.generator.PrintCanvas()
        elif obj.IsA().InheritsFrom("TCanvas"):
            # Simply print a TCanvas verbatim
            obj.generator.Print(self.generator.outFile)
        elif obj.IsA().InheritsFrom("TTree"):
            # Can't do anything useful with a TTree so just skip over it
            pass
        elif obj.IsA().InheritsFrom("TDirectory"):
            # Directory - recurse down into it
            keys = obj.GetListOfKeys()

            for key in sorted(keys):
                obj.cd()
                subobj = obj.Get(key.GetName())
                self.Recurse(subobj, obj.GetPath())
        else:
            print "Unhandled object type: " + obj.IsA().GetName()

        return



"""
This is the main script which is run.
"""
parser = OptionParser() 
parser.add_option('--rdp', dest='rdp', help='RDP file')
parser.add_option('--mcp', dest='mcp', help='MCP file')
parser.add_option('--oldrdp', dest='oldrdp', help='Old RDP file')
parser.add_option('--oldmcp', dest='oldmcp', help='Old MCP file')
parser.add_option('--out', dest='out', help='Output file prefix', default="GRTF-")
parser.add_option('--web', dest='web', help='Path to generate webpages beneath')
parser.add_option('--webtar', dest='webtar', action="store_true", help='Create a .tar.gz archive of the webpages. Does not erase the webpage directory.')
parser.add_option('--scale', dest='scale', type='int', help='Method of scaling histograms, 0:No scaling, 1:Scale to 1, 2:Scale to number of events', default=0)
parser.add_option('--comment', dest='comment', help='Comment for title page of PDF documents')
parser.add_option('--custom', dest='custom', help='Module to use for generators, (general, tracker_global_mom etc.)')
parser.add_option('--macros', dest='macros', action="store_true", help='Generate .root macros for slides')
parser.add_option('--noerrors', dest='noerrors', action="store_true", help='Prevent the drawing of error bars')
parser.add_option('--logscale', dest='logscale', action="store_true", help='Scale plots logarithmically')
parser.add_option('--title', dest='title', default="GRTF Verification Plots:", help='Override the main title of PDF documents')
parser.add_option('--rdptitle', dest='rdptitle', default="RDP", help='Override the name for RDP plots')
parser.add_option('--mcptitle', dest='mcptitle', default="MCP", help='Override the name for MCP plots')
parser.add_option('--oldrdptitle', dest='oldrdptitle', default="Old RDP", help='Override the name for old RDP plots')
parser.add_option('--oldmcptitle', dest='oldmcptitle', default="Old MCP", help='Override the name for old MCP plots')
(options, args) = parser.parse_args()

# Use batch mode, and supress all Info level messages
ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch()
ROOT.gErrorIgnoreLevel = 1001
ROOT.gStyle.SetOptStat(112211)

mcpFile = False
rdpFile = False
oldmcpFile = False
oldrdpFile = False

if options.mcp:
    mcpFile = ROOT.TFile.Open(options.mcp)
if options.rdp:
    rdpFile = ROOT.TFile.Open(options.rdp)
if options.oldmcp:
    oldmcpFile = ROOT.TFile.Open(options.oldmcp)
if options.oldrdp:
    oldrdpFile = ROOT.TFile.Open(options.oldrdp)

if options.scale > 2 or options.scale < 0:
    print "--scale must be 0, 1, 2"
    sys.exit(1)

pdf = make_pdf(mcpFile, rdpFile, oldmcpFile, oldrdpFile, options)
numCanvases = pdf.MakePlots(options.logscale, options.noerrors)
print "Generated", numCanvases, "pages of plots"
