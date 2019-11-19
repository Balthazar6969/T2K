"""
This module is for loading the output highland produces and ensuring that
they match previous values
"""
import config as cfg
import validation_utils
import ROOT as rt
import glob
import os
from collections import namedtuple, defaultdict


__author__ = 'Tianlu Yuan'
__email__ = 'tianlu.yuan [at] colorado.edu'


HighlandOutput = namedtuple('HighlandOutput',
                            'path name input_type input_file version rootfile tool_base tool sample')

PsycheOutput = namedtuple('PsycheOutput',
                          'path name input_type input_file version')


class HighlandLoader(object):
    """ Loads all the files in the highlandValidation output directory and
    provides getter methods for reading.
    """

    def __init__(self,psyche=False):
        """ dict with the prog as keys and a dict as val.  subdict contains
        'nom' containing the HighlandOutput for the nominal version, and
        'toverify' which is a list of HighlandOutputs of the test versions.
        """
        # We filter out files that are not from highland analyses.
        if not psyche:
            output_files = filter(lambda path: validation_utils.is_highland_analysis(path),
                                  glob.glob(os.path.join(cfg.PROCESS_OUT_DIR,
                                                         '*.root')))
        else:
            output_files = filter(lambda path: validation_utils.is_highland_analysis(path),
                                  glob.glob(os.path.join(cfg.PROCESS_OUT_DIR,
                                                         '*.out')))
            output_files.extend(filter(lambda path: validation_utils.is_highland_analysis(path),
                                       glob.glob(os.path.join(cfg.PROCESS_OUT_PSYCHE_DIR,
                                                              '*.out'))))

                                
        self._outputs = defaultdict(lambda: defaultdict(dict))
        for path in output_files:
            output_filename = os.path.basename(path)

            # prog name (e.g. RunNumuCCAnalysis etc)
            prog = validation_utils.re_search_prog(output_filename)

            # Skip if this prog is not in cfg.PROCESS_DICT
            if not cfg.PROCESS_DICT.has_key(prog):
                print 'INFO: {0} is not defined in config.PROCESS_DICT. Skipping...'.format(prog)
                continue

            # Check input type and input file
            input_type = validation_utils.re_search_input_type(output_filename)
            input_file = validation_utils.re_search_input_file(output_filename)
            
            # Check version of analysis used to make this output
            version = '{0}-{1}'.format(
                validation_utils.re_search_highland_version(output_filename),
                validation_utils.re_search_analysis_version(output_filename))

            # save everything as a HighlandOutput namedtuple
            if not psyche:
                highland_output = HighlandOutput(path,
                                                 prog,
                                                 input_type,
                                                 input_file,
                                                 version,
                                                 rt.TFile(path),
                                                 rt.DrawingToolsBase(path, False),
                                                 rt.DrawingTools(path, False),
                                                 rt.DataSample(path))
            else:
                highland_output = PsycheOutput(path,
                                               prog,
                                               input_type,
                                               input_file,
                                               version)

            # determine if this is a the nominal or a test version
            # this depends on process_code. for flattree-oa
            # comparison, use oa as nominal. must also keep different
            # input_files separate
            if not psyche:
                if cfg.PROCESS_CODE == 0 and version == cfg.PROCESS_DICT[prog]['nom']:
                    self._outputs[prog][input_file]['nom'] = highland_output
                elif cfg.PROCESS_CODE > 0 and input_type == 'oaanalinput':
                    self._outputs[prog][input_file]['nom'] = highland_output
                else:
                    if self._outputs[prog].has_key('toverify'):
                        self._outputs[prog][input_file]['toverify'].append(highland_output)
                    else:
                        self._outputs[prog][input_file]['toverify'] = [highland_output]
            else:
                if version == cfg.PROCESS_DICT[prog]['nom']:
                    self._outputs[prog][input_file]['nom'] = highland_output
                else:
                    self._outputs[prog][input_file]['toverify'] = [highland_output]

                    
    def get_nominals(self):
        noms = [self._outputs[prog][input_file]['nom']
                for prog in self._outputs
                for input_file in self._outputs[prog]]

        return noms


    def get_outputs(self):
        return self._outputs


