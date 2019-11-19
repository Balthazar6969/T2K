#! /bin/csh -f 

##
## Code to run over the root files produced from ccAnalysis.C 
## and extract the neutrino energy distribution.
## The output files of the fit are stored in the directory
## that will be created by this script and called 'output'.
## to run this script: source do_ccAnalysis_fit.csh
##
## This code and method are under evolution.
## Francesca Di Lodovico (October 2009)
##

rm -rf output_old;
mv output output_old;
mkdir output;

set i=0
set MAvariation=49
while (( $i < ${MAvariation} )); 
    echo i=$i
    echo -n "$i"
    root -b -q 'run_ccAnalysis_fit.C('true','false','${i}')' >& output/inputtree_$i.log
    @ i++
end

set i=0
while (( $i < 1 )); 
    echo i=$i
    echo -n "$i"
    root -b -q 'run_ccAnalysis_fit.C('false,true,${i}')' >& output/log_$i.log
    @ i++
end

