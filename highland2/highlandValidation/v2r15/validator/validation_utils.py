"""
This module is for utility functions needed by the other modules.
"""
import config as cfg
import re
import ROOT as rt
import subprocess
import os
from tianlu import utils, rootutils
from functools import wraps

# Format for storing the outputs from highland analyses
# 'progname_analysisver_highlandverValidation.root'
OUTPUT_TEMPLATE = '{0}_{1}_{2}_{3}_{4}_{5}Validation.root'

# Some regex patterns for matching
RE_PROG_STR = 'Run[a-zA-Z0-9]*'
RE_SYST_TYPE_STR = 'nom|syst'
RE_INPUT_TYPE_STR = 'oaanalinput|flattreeinput|minitreeinput'
RE_INPUT_FILE_STR = 'input-file-.*-p[0-9]+'
RE_HIGHLAND_VERSION_STR = 'highland[^-]2?|psyche'
RE_ANALYSIS_VERSION_STR = 'v[0-9]+r[0-9]+(p[0-9]+)?|HEAD'
RE_OUTPUT_STR = OUTPUT_TEMPLATE.format(RE_PROG_STR,
                                       RE_SYST_TYPE_STR,
                                       RE_INPUT_TYPE_STR,
                                       RE_INPUT_FILE_STR,
                                       RE_ANALYSIS_VERSION_STR,
                                       RE_HIGHLAND_VERSION_STR)


def info(test_func):
    """ prints out the current highland analysis being tested, the nominal
    version and the test version.  wraps the test function.
    """

    @wraps(test_func)
    def test_func_wrapper(nom, test):
        print '\n============================================================='
        print 'Starting test:', test_func.__name__
        print 'Testing highland analysis:', nom.name
        print 'originally processed with:', nom.input_file
        print 'nominal version:', nom.input_type, nom.version
        print 'test version:', test.input_type, test.version, '\n'
        test_func(nom, test)

    return test_func_wrapper


def re_search_input_type(search_string):
    """ Returns the input type associated with the highland analysis

    This will be useful when we compare outputs from different highland
    versions
    """
    match = re.search(RE_INPUT_TYPE_STR, search_string)
    return match.group() if match else None


def re_search_input_file(search_string):
    """ Returns the input file  associated with the highland analysis

    This will be useful when we compare outputs from different highland
    versions
    """
    match = re.search(RE_INPUT_FILE_STR, search_string)
    return match.group() if match else None


def re_search_analysis_version(search_string):
    """ Returns the version string associated with the highland analysis

    This will be useful when we compare outputs from different highland
    versions
    """
    match = re.search(RE_ANALYSIS_VERSION_STR, search_string)
    return match.group() if match else 'HEAD'


def re_search_highland_version(search_string):
    """ Returns the version string associated with highland itself

    This allows us to distinguish between highland2, highland and psyche analyses
    """
    match = re.search(RE_HIGHLAND_VERSION_STR, search_string)
    return match.group() if match else None


def re_search_prog(search_string):
    """ Returns the prog name associated with the highland program
    """
    print search_string
    match = re.search(RE_PROG_STR, search_string)
    return match.group() if match else None


def is_highland_analysis(filename):
    """ returns true if the file is of the form
    Run*_v*r*_highlandValidation.root
    """
    return re.search(RE_OUTPUT_STR, filename)


def entries_compare(nom_tree, test_tree, cut):
    """Used in tests.py. This function is wrapped and returned as a
    wrapped-function which is passed to the tester
    """
    print 'nom entries: {0}'.format(nom_tree.GetEntries(cut))
    print 'test entries: {0}'.format(test_tree.GetEntries(cut))

    assert (test_tree.GetEntries(cut) == nom_tree.GetEntries(cut))


@rootutils.title
def histograms_compare(nom,
                       test,
                       tree,
                       title,
                       var,
                       cut,
                       binning,
                       nom_leg,
                       test_leg,
                       plots_file,
                       use_draw_tool):
    """ Draws histograms from nom and test for comparison.

    Used in tests.py
    """
    nom_tree  = nom.rootfile.Get(tree)
    test_tree = test.rootfile.Get(tree)

    if use_draw_tool:
        nom_h = nom.tool_base.GetHisto(nom_tree, 'nom_h', var,
                                       len(binning)-1, binning,
                                       cut, 'goff', '', 1.)
        test_h = test.tool_base.GetHisto(test_tree, 'test_h', var,
                                       len(binning)-1, binning,
                                       cut, 'goff', '', 1.)
        good_draw = True
    else:
        nom_h = rt.TH1F('nom_h', '', len(binning)-1, binning)
        test_h = rt.TH1F('test_h', '', len(binning)-1, binning)

        nom_draw_check = nom_tree.Draw(var+'>>nom_h', cut, 'goff')
        test_draw_check = test_tree.Draw(var+'>>test_h', cut, 'goff')

        # ensure that the variables are drawn properly
        good_draw = nom_draw_check != -1 and test_draw_check != -1
        if not good_draw:
            raise RuntimeError('Could not draw var "{0}". Does it exist?'.
                               format(var))

    nom_h.SetTitle(title)
    nom_h.GetXaxis().SetTitle(var)
    test_h.GetXaxis().SetTitle(var)

    nom_h.SetLineColor(rt.kBlack)
    test_h.SetLineColor(rt.kRed)
    test_h.SetLineStyle(2)


    leg = rootutils.get_legend(0.65, 0.9, 0.65, 0.75)
    leg.AddEntry(nom_h, nom_leg, 'l')
    leg.AddEntry(test_h, test_leg, 'l')

    rt.gStyle.SetStatW(0.2)
    rt.gStyle.SetStatH(0.06)

    rt.gStyle.SetStatX(0.8 -rt.gStyle.GetPadRightMargin())
    rt.gStyle.SetStatY(1.0 -rt.gStyle.GetPadTopMargin())
    nom_h.Draw('hist')

    rt.gStyle.SetOptStat(1110)

    rt.gStyle.SetStatX(1.0 -rt.gStyle.GetPadRightMargin())
    rt.gStyle.SetStatY(1.0 -rt.gStyle.GetPadTopMargin())
    test_h.Draw('histsames')

    leg.Draw('same')
    rt.gPad.Update()

    if good_draw:
        rt.gPad.SaveAs(plots_file)

    assert nom_h.GetEntries() == test_h.GetEntries()
    assert nom_h.GetNbinsX() == test_h.GetNbinsX()
    for i in range(nom_h.GetNbinsX()):
        assert utils.almost_equal_relative_and_abs(nom_h.GetBinContent(i),
                                                   test_h.GetBinContent(i))


def get_input_type_str(process_code):
    """Returns the input type string for output filenaming. To
    distinguish between outputs from oaanalinputs, flattreeinputs, or
    minitreeinputs
    """
    if process_code == 1:
        return 'flattreeinput'
    elif process_code == 2:
        return 'minitreeinput'
    else:
        return 'oaanalinput'


def get_syst_type_str(par_path):
    """Returns the systematic type string for output filenaming. To
    distinguish between nominal and systematics
    """

    if 'systematics' in par_path:
        return 'syst'
    elif 'p0danalyses' in par_path:
        return 'syst'
    else:
        return 'nom'


def run_check_roovtxlink(highland_output, tree):
    """Runs CheckRooTrackerVtxLink.exe when appropriate. Pipe stdout to
    /dev/null
    """
    tree_to_flag = {'truth':'-t',
                    'default':'-d',
                    'all_syst':'-s'}
    with open(os.devnull) as fnull:
        if highland_output.rootfile.Get('NRooTrackerVtx'):
            if highland_output.rootfile.Get(tree):
                subprocess.check_call(['CheckRooTrackerVtxLink.exe', '-m',
                                       highland_output.path,
                                       tree_to_flag[tree]],
                                      stdout=fnull)
