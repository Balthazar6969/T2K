#!/usr/bin/python
import ROOT
import re
import datetime
import math
import sys
import os
from grtf.pdfgen.generators import pdf_base
from handle_graph import Handle_Graph

def DoNothing(Hist):
    pass


class Fifo:
    """
    Fifo class to store temporarily the graph pointers
    between the creation of the graph and when it is drawn.
    Store only up to 50 pointers, then remove the oldest
    to avoid using too much memory for nothing
    """
    def __init__(self):
        self.nextin = 0
        self.nextout = 0
        self.data = {}
    def append(self, value):
        self.data[self.nextin] = value
        self.nextin += 1
        if len(self.data) > 50:
            self.pop()
    def pop(self, n=-1):
        value = self.data[self.nextout]
        del self.data[self.nextout]
        self.nextout += 1
        return value


class pdf_graphs_base(pdf_base.pdf_base):
    """
    """

    def __init__(self, options):
        """
        Automatically called when a new object is created.

        Handles the command-line arguments that the user passed in.

        Arguments:
        options  - Dict with the following keys
                    out      - Prefix for PDF document names. The prefix is followed by [detector].pdf.
                    title    - The title string to appear on the first page of generated PDFs
                    comment  - The comment string to appear on the first page of generated PDFs.
                    macros   - Whether to generate .root macros as well as PDFs.
                    graphs   - List of input text files
        """

        self.graphs = []
        self.allX = []
        tmpAllX = []

        inputtxtFiles = options.graphs

        self.noerrors = options.noerrors
        for T in inputtxtFiles:
            self.graphs.append(Handle_Graph(T, options.noerrors))
            tmpAllX.extend(self.graphs[-1].GetAllX())

        tmpAllX.sort()
        diffX = tmpAllX[-1] - tmpAllX[0]
        self.minX = tmpAllX[0] - (0.1*diffX)
        self.maxX = tmpAllX[-1] + (0.1*diffX)

        # remove duplicates from allX
        for X in tmpAllX:
          if not X in self.allX:
            self.allX.append(X)

        # Prioritize the good colours
        self.colours = [1,2,4,6,7,9,3,5,8]+range(30,50)

        # Text file for the text output
        self.text = options.text
        self.txtFile = None

        # Added to be compatible with pdf_base
        self.scale = 0
        self.files = {}

        self.outFilePrefix = options.out
        self.title = options.title
        self.comment = options.comment
        if not options.xaxis == None:
            self.xaxis = options.xaxis
        else:
            self.xaxis = ''
        #self.macros = options.macros
        self.macros = 0
        self.web = options.web

        # The canvas to write to and the pads it contains
        self.canvas = ROOT.TCanvas("c", "c", 800, 600)
        self.pads = []

        self.graphptr = Fifo()
        self.legendptr = Fifo()

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

        ROOT.gStyle.SetFillColor(10)

    def GetFirstFile(self):
        return self.graphs[0].Input[0].File

    def CustomPlots(self, obj, path):
        """
        Generate any custom plots, and return whether the object still needs processing.

        This is the main function that is overridden by most of the customisation scripts.
        """

        return True

    def DrawGraphForDir(self, histo, dirpath, pad, projdir = '', customize='mean'):
        """
        Create the graph for the histogram in this directory

        The first histogram in subdir is returned. If the input files were
        produced by a GRTF analysis module, then there will only be one
        histogram in the directory.

        Arguments:
        histo      - The histogram to print.
        dirpath    - The path in which the histogram is found.
        title      - Title of the newly created TGraph
        customize  - This is either a string for standard variables, or a function
                     that will return one value from the histogram
        """

        pad.Clear()
        pad.cd()

        if not histo:
            return []
    
        minY = 0xABCDEF
        maxY = -0xABCDEF

        graphsmade = []

        # Loop over the graph containers
        for gr,Gr in enumerate(self.graphs):
            tmpgraph = Gr.GenerateGraph(histo, dirpath, projdir, customize)

            tmpgraph.SetLineColor(self.colours[gr])
            #tmpgraph.SetLineWidth(2)
            tmpgraph.SetMarkerColor(self.colours[gr])
            tmpgraph.SetMarkerStyle(2+gr)
            
            minY = min(minY, tmpgraph.GetHistogram().GetMinimum())
            maxY = max(maxY, tmpgraph.GetHistogram().GetMaximum())
            # Apparently must save the graph in memory until we plot
            self.graphptr.append(tmpgraph)
            graphsmade.append(tmpgraph)

        diffY = maxY - minY
        minY = minY - (0.1*diffY)
        maxY = maxY + (0.1*diffY)

        Leg = ROOT.TLegend(0.75,(1.0-(0.05*len(self.graphs))),0.99,1.00)
        self.legendptr.append(Leg)

        for g,G in enumerate(graphsmade):
            if g == 0:
                G.Draw('AP')
                if type(customize) is str:
                    if projdir == 'Y':
                      G.SetTitle(histo.GetTitle()+";"+self.xaxis+";"+customize+", "+histo.GetYaxis().GetTitle())
                    else:
                      G.SetTitle(histo.GetTitle()+";"+self.xaxis+";"+customize+", "+histo.GetXaxis().GetTitle())
                else:
                    G.SetTitle(histo.GetTitle()+";"+self.xaxis)
                baseHist = G.GetHistogram()
                baseHist.GetXaxis().SetLimits(self.minX,self.maxX)
                G.SetMinimum(minY)
                G.SetMaximum(maxY)
                # baseHist.GetYaxis().SetLimits(minY,maxY)
                baseHist.GetXaxis().CenterTitle()
                baseHist.GetYaxis().CenterTitle()
            else:
                G.Draw('Psame')
            Leg.AddEntry(G, self.graphs[g].Title, 'lp')

        Leg.Draw()

        ROOT.gPad.Update()

        outText = []
        ###### Loop over the points info to create the table and print it out
        if self.text:
          for X in self.allX:
            outText.append(str(X))

          ListLeg = ""
          x = ROOT.Double(0)
          y = ROOT.Double(0)
          for g,G in enumerate(graphsmade):
            ListLeg += "\t"+self.graphs[g].Title
            tmpCol = []
            for Xi in range(len(self.allX)):
              tmpCol.append("\t____N/A____")
            for p in range(G.GetN()):
              G.GetPoint(p, x, y)
              for Xi,X in enumerate(self.allX):
                if abs(X-x) < 1e-8:
                  tmpCol[Xi] = "\t"+str(y)
                  if not self.noerrors:
                    tmpCol[Xi] += "\t"+str(G.GetErrorYhigh(p))
                  break
            for Xi in range(len(self.allX)):
              outText[Xi] += tmpCol[Xi]

          [Title,XTitle,YTitle] = (graphsmade[0].GetTitle()).split(';')
          self.txtFile.write("#\n###### "+Title+"\n")
          self.txtFile.write("#               "+YTitle+"\n")
          self.txtFile.write("# "+XTitle+"    "+ListLeg+"\n")
          for oT in outText:
            self.txtFile.write(oT+"\n")


