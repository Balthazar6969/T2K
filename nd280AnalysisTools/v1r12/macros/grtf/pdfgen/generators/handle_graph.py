#!/usr/bin/python
import ROOT
import re
import datetime
import math
import sys
import os


class OneInput:
    """
    """

    def __init__(self, X, filename):
        """
        Stores the information of one input ROOT file

        Arguments:
        """
        self.X = float(X)
        self.Filename = filename
        self.File = ROOT.TFile(filename, 'READ')
        if not self.File.IsOpen():
            print "ERROR: Cannot open ROOT input file",filename,"\nExit"
            sys.exit(1)

    def GetPath(self, basepath):
        newbase = re.sub(":/.*", "", self.File.GetPath())
        oldbase = re.sub(":/.*", "", basepath)

        return basepath.replace(oldbase, newbase)

    def GetHistogram(self, path, histoname, projdir):
        subpath = self.GetPath(path)
        Dir  = self.File.GetDirectory(subpath)
        if Dir:
            self.File.Cd(subpath)
            if projdir == 'X':
                histo2D = ROOT.gDirectory.Get(histoname)
                return histo2D.ProjectionX()
            elif projdir == 'Y':
                histo2D = ROOT.gDirectory.Get(histoname)
                return histo2D.ProjectionY()
            else:
                H = ROOT.gDirectory.Get(histoname)
                return ROOT.gDirectory.Get(histoname)
        return False


class Handle_Graph:
    """
    """

    def __init__(self, txtfile, noerrors):
        """
        Automatically called when a new object is created.

        Handles the command-line arguments that the user passed in.

        Arguments:
        txtfile    - full path and filename of the textfile containing the list of ROOT input files
        noerrors   - Boolean preventing drawing the errors
        """

        # Parse the text file
        # For each line, get the X value and open the TFile
        # If TFile doesn't exist, exit the whole script

        self.Input = []
        self.Title = 'NOT SET'
        self.NoErrors = noerrors

        tf = open(txtfile)
        for line in tf:
            res = re.match(r'^#', line)
            if (res != None):
                continue

            res = re.match(r'^Title\s+(.*)', line)
            if (res != None):
                self.Title = res.group(1)
                continue

            res = re.match(r'(.*\.root)\s+(.*)', line)
            if ( res != None):
                self.Input.append(OneInput(res.group(2), res.group(1)))

    def GetAllX(self):
        allX = []
        for I in self.Input:
            allX.append(I.X)
        return allX

    def GenerateGraph(self, histo, dirpath, proj, customize):
        if self.NoErrors:
            graph = ROOT.TGraph()
        else:
            graph = ROOT.TGraphErrors()

        for i,I in enumerate(self.Input):
            hist = I.GetHistogram(dirpath, histo.GetName(), proj)
            Title = hist.GetTitle()
            
            if type(customize) is str:
                if customize == 'entries':
                    Y = hist.GetEntries()
                    eY = 0.0
                elif customize == 'mean':
                    Y = hist.GetMean()
                    eY = hist.GetMeanError()
                elif customize == 'rms':
                    Y = hist.GetRMS()
                    eY = hist.GetRMSError()
                elif customize == 'fit_mean' or customize == 'fit_sigma':
                    Func = ROOT.TF1("g","gaus",hist.GetXaxis().GetXmin(),hist.GetXaxis().GetXmax())
                    Func.SetParameter(0, hist.Integral())
                    Func.SetParameter(1, hist.GetMean())
                    Func.SetParameter(2, hist.GetRMS())
                    hist.Fit("g","Q")
                    if Func == None:
                        print "WARNING: problem trying to get",customize,"from the histogram",I.GetPath(dirpath)+histo.GetName()
                        continue
                    if customize == 'fit_mean':
                        Y = Func.GetParameter(1)
                        eY = Func.GetParError(1)
                    elif customize == 'fit_sigma':
                        Y = Func.GetParameter(2)
                        eY = Func.GetParError(2)
                else:
                    Y = 0.0
                    eY = 0.0
            else:
                [Y,eY] = customize(hist)

            graph.SetPoint(i, I.X, Y)
            if not self.NoErrors:
                graph.SetPointError(i, 0.0, eY)

        graph.SetTitle(Title)
        return graph

