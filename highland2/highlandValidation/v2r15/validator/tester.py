""" This is the base tester module.  run this with `nosetests`
nosetest recognizes only functions that start with 'test_'
"""
import tests
import loader
import config
import ROOT as rt

# turn off graphics. validation plots are saved as pdf
rt.gROOT.SetBatch()


__author__ = 'Tianlu Yuan'
__email__ = 'tianlu.yuan [at] colorado.edu'


HIGHLAND_LOADER = loader.HighlandLoader()
HIGHLAND_LOADER2 = loader.HighlandLoader(True)

def analysis_generator(outputs,  analysis_funcs):
    """ generator function for running tests some set of outputs
    """
    nominal = outputs['nom']
    toverify= outputs['toverify'] if outputs.has_key('toverify') else []

    # loop over each testcase (i.e. different version)
    for testcase in toverify:
        # loop over basic test functions
        for func in analysis_funcs:
            # yield is used by nosetests for looping through tests
            # each iteration will be a filed as a separate test
            yield func, nominal, testcase


def test_common_generator():
    """ For each analysis output, loop over the non-nominal versions and
    for each (nom, testcase) pair run every func in common_test_funcs.
    """
    all_outputs = HIGHLAND_LOADER.get_outputs()
    # loop over each highland analysis
    for prog in all_outputs:
        for input_file in all_outputs[prog]:
            outputs = all_outputs[prog][input_file]
            for generated_test in analysis_generator(outputs,
                                                     tests.common_test_funcs):
                yield generated_test


def test_numu_generator():
    """ For RunNumuCCAnalysis output, loop over the non-nominal versions and
    for each (nom, testcase) pair run every func in numu_test_funcs.
    """
    numucc_outputs = HIGHLAND_LOADER.get_outputs()['RunNumuCCAnalysis']
    for input_file in numucc_outputs:
        if 'beam' in input_file or 'rdp' in input_file:
            test_funcs = tests.numu_test_funcs_rdp
        else:
            test_funcs = tests.numu_test_funcs

        for generated_test in analysis_generator(numucc_outputs[input_file],
                                                 test_funcs):
            yield generated_test


def test_antiNumuCC_generator():
    """ For RunAntiNumuCCAnalysis output, loop over the non-nominal versions and
    for each (nom, testcase) pair run every func in antiNumuCC_test_funcs.
    """
    antinumucc_outputs = HIGHLAND_LOADER.get_outputs()['RunAntiNumuCCAnalysis']
    for input_file in antinumucc_outputs:
        if 'beam' in input_file or 'rdp' in input_file:
            test_funcs = tests.antiNumuCC_test_funcs_rdp
        else:
            test_funcs = tests.antiNumuCC_test_funcs

        for generated_test in analysis_generator(antinumucc_outputs[input_file],
                                                 test_funcs):
            yield generated_test


def test_numuBkgInAntiNuMode_generator():
    """ For RunNumuBkgInAntiNuModeAnalysis output, loop over the non-nominal versions and
    for each (nom, testcase) pair run every func in numuBkgInAntiNuMode_test_funcs
    """
    numubkginantinumode_outputs = HIGHLAND_LOADER.get_outputs()['RunNumuBkgInAntiNuModeAnalysis']
    for input_file in numubkginantinumode_outputs:
        if 'beam' in input_file or 'rdp' in input_file:
            test_funcs = tests.numuBkgInAntiNuMode_test_funcs_rdp
        else:
            test_funcs = tests.numuBkgInAntiNuMode_test_funcs

        for generated_test in analysis_generator(numubkginantinumode_outputs[input_file],
                                                 test_funcs):
            yield generated_test


def test_numuCCMultiPi_generator():
    """ For RunNumuCCMultiPiAnalysis output, loop over the non-nominal versions and
    for each (nom, testcase) pair run every func in numu_test_funcs.
    """
    numuccmultipi_outputs = HIGHLAND_LOADER.get_outputs()['RunNumuCCMultiPiAnalysis']
    for input_file in numuccmultipi_outputs:
        if 'beam' in input_file or 'rdp' in input_file:
            test_funcs = tests.numuCCMultiPi_test_funcs_rdp
        else:
            test_funcs = tests.numuCCMultiPi_test_funcs

        for generated_test in analysis_generator(numuccmultipi_outputs[input_file],
                                                 test_funcs):
            yield generated_test


def test_nueCC_generator():
    """ For RunNueCCAnalysis output, loop over the non-nominal versions and
    for each (nom, testcase) pair run every func in nueCC_test_funcs.
    """
    nueCC_outputs = HIGHLAND_LOADER.get_outputs()['RunNueCCAnalysis']
    for input_file in nueCC_outputs:
        if 'beam' in input_file or 'rdp' in input_file:
            test_funcs = tests.nueCC_test_funcs_rdp
        else:
            test_funcs = tests.nueCC_test_funcs

        for generated_test in analysis_generator(nueCC_outputs[input_file],
                                                 test_funcs):
            yield generated_test


def test_flattree_generator():
    """ For RunCreateFlatTree output, loop over the non-nominal versions and
    for each (nom, testcase) pair run every func in p0dnumuccqe_test_funcs.
    """
    flattree_outputs = HIGHLAND_LOADER.get_outputs()['RunCreateFlatTree']
    for input_file in flattree_outputs:
        if 'beam' in input_file or 'rdp' in input_file:
            test_funcs = tests.flattree_test_funcs_rdp
        else:
            test_funcs = tests.flattree_test_funcs
        for generated_test in analysis_generator(flattree_outputs[input_file],
                                                 test_funcs):
            yield generated_test


def test_minitree_generator():
    """ For RunCreateMiniTree output, loop over the non-nominal versions and
    for each (nom, testcase) pair run every func in p0dnumuccqe_test_funcs.
    """
    minitree_outputs = HIGHLAND_LOADER.get_outputs()['RunCreateMiniTree']
    for input_file in minitree_outputs:
        if 'beam' in input_file or 'rdp' in input_file:
            test_funcs = tests.minitree_test_funcs_rdp
        else:
            test_funcs = tests.minitree_test_funcs
        for generated_test in analysis_generator(minitree_outputs[input_file],
                                                 test_funcs):
            yield generated_test

if config.PROCESS_CODE == 3 or config.PROCESS_CODE == 0:
    def test_psycheSteering_generator():
        """ For RunExample output, loop over the non-nominal versions and
        for each (nom, testcase) pair run every func in psycheSteering_test_funcs.
    """
        
        psycheSteering_outputs = HIGHLAND_LOADER2.get_outputs()['RunExample']
        for input_file in psycheSteering_outputs:
            if 'beam' in input_file or 'rdp' in input_file:
                test_funcs = tests.psycheSteering_test_funcs_rdp
            else:
                test_funcs = tests.psycheSteering_test_funcs
                for generated_test in analysis_generator(psycheSteering_outputs[input_file],
                                                         test_funcs):
                    yield generated_test

# Currently the flattree and minitree do not support p0d-based
# analyses. Turn on those tests only for oaanal input, version comparison.
if config.PROCESS_CODE == 0:
    def test_p0dnumu_generator():
        """ For RunP0DNumuCCAnalysis output, loop over the non-nominal versions and
        for each (nom, testcase) pair run every func in p0dnumu_test_funcs.
        """
        p0dnumucc_outputs = HIGHLAND_LOADER.get_outputs()['RunP0DNumuCCAnalysis']
        for input_file in p0dnumucc_outputs:
            if 'beam' in input_file or 'rdp' in input_file:
                test_funcs = tests.p0dnumu_test_funcs_rdp
            else:
                test_funcs = tests.p0dnumu_test_funcs

            for generated_test in analysis_generator(p0dnumucc_outputs[input_file],
                                                     test_funcs):
                yield generated_test


    def test_p0dnumuccqe_generator():
        """ For RunP0DNumuCCQEAnalysis output, loop over the non-nominal versions and
        for each (nom, testcase) pair run every func in p0dnumuccqe_test_funcs.
        """
        p0dnumuccqe_outputs = HIGHLAND_LOADER.get_outputs()['RunP0DNumuCCQEAnalysis']
        for input_file in p0dnumuccqe_outputs:
            if 'beam' in input_file or 'rdp' in input_file:
                test_funcs = tests.p0dnumuccqe_test_funcs_rdp
            else:
                test_funcs = tests.p0dnumuccqe_test_funcs

            for generated_test in analysis_generator(p0dnumuccqe_outputs[input_file],
                                                     test_funcs):
                yield generated_test
