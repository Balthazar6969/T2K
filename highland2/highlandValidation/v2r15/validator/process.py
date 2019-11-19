#!/bin/env python
"""
This module is for processing the validation data using highland executables
"""
import config as cfg
import shlex
import validation_utils
from tianlu import utils
import os
from subprocess import Popen
from multiprocessing import Pool
from distutils.spawn import find_executable


__author__ = 'Tianlu Yuan'
__email__ = 'tianlu.yuan [at] colorado.edu'


def check_output(output_file, prompt_remove):
    """ highland will not process files if the output_file exists.  Prompt
    for removal.  Create PROCESS_OUT_DIR if it doesn't exist.

    returns True if the output is clear for highland to proceed
    """
    if output_file is None:
        return False

    # Check if output file already exists and prompt for removal
    if os.path.exists(output_file):
        print output_file, 'exists\n'

        if prompt_remove:
            if utils.prompt_yes_no('Remove output file: '+output_file+'?\n'):
                os.remove(output_file)
            else:
                print 'Output file exists!  Will not run:'+output_file+'\n'
                return False
        else:
            print 'Removing...\n'
            os.remove(output_file)

    return True


def get_analysis_version(prog):
    """Returns the program ver match using the shell cmd 'which prog'. If
    no match is found return 'HEAD'
    """
    fpath = find_executable(prog+'.exe')
    if fpath is None:
        print 'process.get_analysis_version: Cannot find program', prog
    else:
        return validation_utils.re_search_analysis_version(fpath)


def get_highland_version(prog):
    """ Returns the highland ver match using the shell cmd 'which prog'.
    If no match is found return None.
    """
    fpath = find_executable(prog+'.exe')
    if fpath is None:
        print 'process.get_highland_version: Cannot find program', prog
    else:
        return validation_utils.re_search_highland_version(fpath)


def get_in_paths(prog, input_code=0):
    """ return full input file path for prog
    """
    process_dict = cfg.PROCESS_DICT

    if input_code == 0:
        # oa input need to be joined
        return [os.path.join(cfg.PROCESS_IN_DIR, inp_file)
                for inp_file in process_dict[prog]['inp']]
    else:
        # flattree minitree input already full path
        return process_dict[prog]['inp']


def get_par_path(prog):
    """ return full parameter file path for prog
    """
    return cfg.PROCESS_DICT[prog]['par_path']


def get_out_paths(prog, input_code=0):
    """ return full output file path for prog

    input_code: 0 oaAnalysis, 1 flattree output, 2 minitree output
    """
    ver = get_analysis_version(prog)
    highland_ver = get_highland_version(prog)

    par_path = cfg.PROCESS_DICT[prog]['par_path']
    syst_type = validation_utils.get_syst_type_str(par_path)
    inp_type  = validation_utils.get_input_type_str(input_code)
    
    if ver and highland_ver:
        outputs = []
        for filepath in cfg.PROCESS_DICT[prog]['inp']:
            inp_file = validation_utils.re_search_input_file(
                os.path.basename(filepath))

            outputs.append(os.path.join(cfg.PROCESS_OUT_DIR,
                                        validation_utils.OUTPUT_TEMPLATE.format(prog,
                                                                                syst_type,
                                                                                inp_type,
                                                                                inp_file,
                                                                                ver,
                                                                                highland_ver)))

        return outputs

def get_io_paths(prog, input_code=0):
    """ Returns a tuple of the form (input, output) for each program prog
    """
    inputs = get_in_paths(prog, input_code)
    outputs = get_out_paths(prog, input_code)

    if len(inputs) != len(outputs):
        raise RuntimeError('process.get_io_paths: Mismatch between inputs and outputs!')

    return zip(inputs, outputs)


def get_to_process(prompt_remove, input_code=0):
    """Checks for preexisting outputs and returns a dict of programs that
    are to be processed. This tries to be smart about what input-files
    to process for each analysis/input-files combo specified in config
    for flattree/minitree validation, optimizing the processing
    somewhat so that we don't process what we don't validate.

    code: optional argument for speciying the input type which enters
    into the output filename

    This function became a little messy. Probably shouldn't mess with it.
    """
    flattree = 'RunCreateFlatTree'
    minitree = 'RunCreateMiniTree'
    psyche   = 'RunExample'


    to_process = cfg.PROCESS_DICT.copy()

    # Highland Validation
    if   cfg.PROCESS_CODE == 0 and input_code!=3:
        if psyche in to_process: to_process.pop(psyche)
    # FlatTree Validation
    elif cfg.PROCESS_CODE == 1:
        if psyche   in to_process:                   to_process.pop(psyche)
        if minitree in to_process:                   to_process.pop(minitree)
        if flattree in to_process and input_code==1: to_process.pop(flattree)
    # MiniTree Validation
    elif cfg.PROCESS_CODE == 2:
        if psyche   in to_process:                   to_process.pop(psyche)
        if flattree in to_process:                   to_process.pop(flattree)
        if minitree in to_process and input_code==2: to_process.pop(minitree)
    # psyche-FlatTree Validation
    elif cfg.PROCESS_CODE == 3 or input_code==3:
        if input_code==0:            
            for prog in cfg.PROCESS_DICT:
                if prog!=flattree:
                    to_process.pop(prog)
        else:
            for prog in cfg.PROCESS_DICT:
                if prog!=psyche:
                    to_process.pop(prog)
    # psyche-MiniTree Validation
    elif cfg.PROCESS_CODE == 4:
        if input_code==0:            
            for prog in cfg.PROCESS_DICT:
                if prog!=minitree:
                    to_process.pop(prog)
        else:
            for prog in cfg.PROCESS_DICT:
                if prog!=psyche:
                    to_process.pop(prog)


    # some optimization based on process_code
    # the input_code defaults to 0 here because the flattree output itself was
    # produced from oa files

    # FlatTree (1) or psycheSteering-FlatTree (3) validation
    if cfg.PROCESS_CODE == 1 or cfg.PROCESS_CODE == 3 or input_code==3:
#        if cfg.PROCESS_DICT.has_key(minitree):
#            cfg.PROCESS_DICT.pop(minitree)
        nooa_inputs = get_in_paths(flattree, 0)
        nooa_io = get_io_paths(flattree, 0)
    # MiniTree (2) or psycheSteering-MiniTree (4) validation
    elif cfg.PROCESS_CODE == 2 or cfg.PROCESS_CODE == 4:
#        if cfg.PROCESS_DICT.has_key(flattree):
#            cfg.PROCESS_DICT.pop(flattree)
        nooa_inputs = get_in_paths(minitree, 0)
        nooa_io = get_io_paths(minitree, 0)
    # Highland validation (0)
    else:
        nooa_inputs = None
        nooa_io = None

    # BE CAREFUL!!! This is a shallow copy of
    # cfg.PROCESS_DICT which means when we alter the
    # to_process copy at the second level, the PROCESS_DICT is itself
    # modified
    #    to_process = cfg.PROCESS_DICT.copy()
    #    for prog in cfg.PROCESS_DICT:
    for prog in to_process:
        iopaths = get_io_paths(prog, input_code)

        for iopath in iopaths:
            # check if output path is clear
            if not check_output(iopath[1], prompt_remove):
                to_process[prog]['inp'].remove(os.path.basename(iopath[0]))

        # check if this is flat/minitree validation and remove
        # those not processed by flat/minitreee exe file
        if cfg.PROCESS_CODE > 0:
            prog_inputs = get_in_paths(prog)
            for prog_input in prog_inputs:
                if prog_input not in nooa_inputs:
                    to_process[prog]['inp'].remove(os.path.basename(prog_input))


        # set flattree/minitree output as input for analyses
        if input_code > 0:
            # check if the orig_input used for flat/minitree
            # processing is in the current prog's list of
            # orig_inputs. If so, add that corresponding output to a
            # new_input list which will be used in the next stage
            # processing
            new_input = []
            for orig_input, orig_output in nooa_io:
                if os.path.basename(orig_input) in to_process[prog]['inp']:
                    new_input.append(orig_output)
                    
            to_process[prog]['inp'] = new_input

            
    return to_process


def run(prog, input_code=0):
    """ runs the highland program prog

    input_code: 0 oaAnalysis, 1 flattree output, 2 minitree output
    """

    cmd_template = '{prog}.exe {par0} {par} {out0} {out} {inp}'

    par_path   = get_par_path(prog)
    io_paths = get_io_paths(prog, input_code)

    if par_path == '':
        par_option= ''
    else:
        par_option= '-p'
        
        
    for io_path in io_paths:
        highland_ver = get_highland_version(prog)
        if highland_ver == 'psyche':
            out_option= ''        
            out_path = ''
        else:
            out_option= '-o'
            out_path = io_path[1]

        cmd = cmd_template.format(par0=par_option,
                                  par=par_path,
                                  prog=prog,
                                  out0=out_option, 
                                  out=out_path,
                                  inp=io_path[0])
        print cmd + '\n'

        if highland_ver == 'psyche':
            Popen(shlex.split(cmd),bufsize=0, executable=None, stdin=None, stdout=file(io_path[1]+".out","w")).communicate()
        else:
            Popen(shlex.split(cmd)).communicate()

def run_nooa(prog):
    """ runs the highland program prog over flattree/minitree outputs
    """
    run(prog, cfg.PROCESS_CODE)


def process(prompt_remove = True):
    """main method that uses a Pool to multithread and process multiple
    highland programs.

    prompt_remove: If output exists from a previous process script
    prompt for removal, otherwise remove automatically.
    """
    utils.make_dirs_if_needed(cfg.PROCESS_OUT_DIR)
    utils.make_dirs_if_needed(cfg.PLOTS_DIR)


    if cfg.PROCESS_CODE < 10:
        # 1. process oaAnalysis files, including FlatTree/MiniTree creation.
        #    Not done for psycheSteering validation (PROCESS_CODE>=3) since FlatTree/MiniTree should already exist
        to_process_oa = get_to_process(prompt_remove)
        utils.pool_wrapper(run, to_process_oa)
    
    if cfg.PROCESS_CODE > 0:
        # 2. process output from flattree/minitree
        to_process_nooa = get_to_process(prompt_remove, cfg.PROCESS_CODE)
        utils.pool_wrapper(run_nooa, to_process_nooa)
    else:
        # 2. Run psycheSteering flattree/minitree also for standard highland validation (PROCESS_CODE==0)
        to_process_nooa = get_to_process(prompt_remove, 3)
        utils.pool_wrapper(run_nooa, to_process_nooa)


if __name__ == '__main__':
    process(False)
