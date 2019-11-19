Author: Tianlu Yuan
Email: tianlu.yuan [at] colorado.edu

# highlandValidation
The idea behind this package is to ensure that new versions of highland produce outputs similar or identical to previous versions.  PoT, selected distributions, number of events, and other basic sanity checks should be performed.  A set of predetermined oaAnalysis files can be processed with different versions of highland/highland2.  Those outputs are then analyzed to ensure everything is behaving properly.

## Prerequisites
- python v2.6+
- [pyROOT](http://root.cern.ch/drupal/content/pyroot)
- [nose](https://nose.readthedocs.org/en/latest/)
- python utilities packaged under `src/pyutils.tar.gz`. Extract and add `src/Python` to your PYTHONPATH with `source src/setup_pyutils.sh`.

## Setup
All code resides in the `validator/` directory

- Modify `validator/config.py` to suit your environment
- Ensure that `data/input-file.list` contains paths to some set of oaAnal files that will remain constant

## Processing with highland[2]
You should use the validator tool to run highland[2] and process `input-file.list`.  The tests rely on the output file naming formats!

In order to test different versions of highland[2] you need to source CMT setup files as appropriate to each version.
To do so:

- Modify the `cmt/requirements` file within this package to use the appropriate highland[2] analyses and versions.
- Open a python shell inside `validator/` and `import process` then `process.process()` which will try to process input-files interactively
- Alternatively you can just do `python process.py` which will force processing

## Running Validation Checks
Once the analyses have been run, the outputs are used to do the validation tests.  This is handled by `loader.py` and `tester.py` but the user _only should ever need to modify_ `tests.py`.  In `tests.py` are where the actual comparisons are done between a nominal version (specified by `config.PROCESS_DICT[analysis]['nom']`) and a testcase version.  Some example functions are already defined.  The goal is to make it simple as possible to add additional test functions to do comparisons.

One thing to note is that this precludes using DrawingTools or other highlandTools to do comparisons, as they are not always cross-compatible between different highland versions.  Everything is done with the raw TTrees and ROOT commands.

- Run all tests with `nosetests` inside the `validator/` dir.
