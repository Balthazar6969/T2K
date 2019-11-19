"""
This module can be modified to suit the users environment.
"""
import os
from tianlu.analysis import utils as tautils

### [basic_setup] ###
# 0: compare oaanalysis highland2 outputs from different versions
# 1: compare oaanalysis highland2 output against flattree output
# 2: compare oaanalysis highland2 output against minitree output
PROCESS_CODE = 0

HIGHLANDVALIDATIONROOT = os.getenv('HIGHLANDVALIDATIONROOT')

# Path to the validation data-sample input-file list
PROCESS_IN_DIR = os.path.join(HIGHLANDVALIDATIONROOT, 'data')

# Path to directory where the processed highland output will be
# saved. This is the default for comparing outputs from highland2
PROCESS_OUT_DIR = os.path.join(HIGHLANDVALIDATIONROOT,
                               'outputs', str(PROCESS_CODE))

PROCESS_OUT_PSYCHE_DIR = os.path.join(HIGHLANDVALIDATIONROOT,
                                      'outputs', "0")

# Path to directory where validation plots are saved
PLOTS_DIR = os.path.join(PROCESS_OUT_DIR, 'plots')

# Path to parameter override files
PARAM_FILE_SYST = os.path.join(HIGHLANDVALIDATIONROOT,
                               'parameters','systematics.parameters.dat')
PARAM_FILE_P0D = os.path.join(HIGHLANDVALIDATIONROOT,
                              'parameters','p0danalyses.parameters.dat')
PARAM_FILE_FLATTREE = os.path.join(HIGHLANDVALIDATIONROOT,
                                   'parameters','flattree.parameters.dat')
PARAM_FILE_PSYCHE = os.path.join(HIGHLANDVALIDATIONROOT,
                                 'parameters','psycheSteering.parameters.dat')


### [advanced_setup] ###
# PROCESS_DICT
# <key>: 'analysis executable name'
# 'inp': oa input file list stored in PROCESS_IN_DIR
# 'nom': nominal version used as baseline for comparison between different
#        versions of highland2. For flattree/minitree comparison with oaanal,
#        files processed using oa directly are treated as nominal. These MUST
#        correspond to the package versions in the used nd280Highland2 version
# 'par_path': path to parameters overrides file. '' means no override
#
# The .exe should already be in your $PATH if you've sourced the cmt
# setup script for highlandValidation
if tautils.nd280analysis_prod() == 5:
    PROCESS_DICT = {'RunP0DNumuCCAnalysis':{'inp':['input-file-neut-p5.list',
                                                   'input-file-beam-p5.list'],
                                            'nom':'highland2-v2r11',
                                            'par_path':PARAM_FILE_P0D},
                    'RunP0DNumuCCQEAnalysis':{'inp':['input-file-neut-p5.list',
                                                     'input-file-beam-p5.list'],
                                              'nom':'highland2-v2r9',
                                              'par_path':PARAM_FILE_P0D},
                    'RunNumuCCAnalysis':{'inp':['input-file-neut-p5.list',
                                                'input-file-beam-p5.list'],
                                         'nom':'highland2-v2r13',
                                         'par_path':PARAM_FILE_FLATTREE},  # TODO
                    'RunNueCCAnalysis':{'inp':['input-file-neut-p5.list',
                                               'input-file-beam-p5.list'],
                                        'nom':'highland2-v2r15',
                                        'par_path':PARAM_FILE_FLATTREE},
                    'RunAntiNumuCCAnalysis':{'inp':['input-file-neut-p5.list'],
                                             'nom':'highland2-v2r3',
                                             'par_path':PARAM_FILE_SYST},
                    'RunNumuCCMultiPiAnalysis':{'inp':['input-file-neut-p5.list'],
                                                'nom':'highland2-v2r5',
                                                'par_path':PARAM_FILE_SYST},
                    'RunNumuBkgInAntiNuModeAnalysis':{'inp':['input-file-neut-p5.list'],
                                                      'nom':'highland2-v2r3',
                                                      'par_path':PARAM_FILE_SYST},
                    'RunCreateFlatTree':{'inp':['input-file-neut-p5.list',
                                                'input-file-beam-p5.list'],
                                         'nom':'highland2-v2r17',
                                         'par_path':PARAM_FILE_FLATTREE},
                    'RunCreateMiniTree':{'inp':['input-file-neut-p5.list',
                                                'input-file-beam-p5.list'],
                                         'nom':'highland2-v2r17',
                                         'par_path':PARAM_FILE_FLATTREE},
                    'RunExample':{'inp':['input-file-neut-p5.list',
                                         'input-file-beam-p5.list'],
                                  'nom':'psyche-v3r13',
#                                  'par_path':PARAM_FILE_PSYCHE}
                                  'par_path':''}
    }

elif tautils.nd280analysis_prod() == 6:
    PROCESS_DICT = {'RunP0DNumuCCAnalysis':{'inp':['input-file-genie-p6.list',
                                                   'input-file-beam-p6.list'],
                                            'nom':'highland2-v2r11',
                                            'par_path':PARAM_FILE_P0D},
                    'RunP0DNumuCCQEAnalysis':{'inp':['input-file-genie-p6.list',
                                                     'input-file-beam-p6.list'],
                                              'nom':'highland2-v2r9',
                                              'par_path':PARAM_FILE_P0D},
                    'RunNumuCCAnalysis':{'inp':['input-file-neut-p6.list',
                                                'input-file-beam-p6.list'],
                                         'nom':'highland2-v2r13',
                                         'par_path':''},
                    'RunNueCCAnalysis':{'inp':['input-file-neut-p6.list',
                                               'input-file-beam-p6.list'],
                                        'nom':'highland2-v2r15',
                                        'par_path':PARAM_FILE_FLATTREE},
                    'RunAntiNumuCCAnalysis':{'inp':['input-file-neut-p6.list'],
                                             'nom':'highland2-v2r3',
                                             'par_path':PARAM_FILE_SYST},
                    'RunNumuCCMultiPiAnalysis':{'inp':['input-file-neut-p6.list'],
                                                'nom':'highland2-v2r5',
                                                'par_path':PARAM_FILE_SYST},
                    'RunNumuBkgInAntiNuModeAnalysis':{'inp':['input-file-neut-p6.list'],
                                                      'nom':'highland2-v2r3',
                                                      'par_path':PARAM_FILE_SYST},
                    'RunCreateFlatTree':{'inp':['input-file-neut-p6.list',
                                                'input-file-beam-p6.list'],
                                         'nom':'highland2-v2r17',
                                         'par_path':PARAM_FILE_FLATTREE},
                    'RunCreateMiniTree':{'inp':['input-file-neut-p6.list',
                                                'input-file-beam-p6.list'],
                                         'nom':'highland2-v2r17',
                                         'par_path':PARAM_FILE_FLATTREE},
                    'RunExample':{'inp':['input-file-neut-p6.list',
                                         'input-file-beam-p6.list'],
                                  'nom':'psyche-v3r13',
#                                  'par_path':PARAM_FILE_PSYCHE}
                                  'par_path':''}

    }

# SET the frozen version of nd280Highland2. This is parsed and used
# for Jenkins processing with the nominal package versions. If you
# wish to process a nominal version _locally_ you must manually checkout
# an nd280Highland2 frozen version then execute 'cmt show uses' in
# highlandValidation to ensure that those packages are actually being
# used before running ./process.py
#
# *** You must ensure that the 'nom' versions in PROCESS_DICT matches
# the packages in this version of nd280Highland2 ***
JENKINS_PROCESS_NOMINAL_ND280HIGHLAND2 = 'v2r17'
