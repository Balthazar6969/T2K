#! /bin/tcsh -f 

##
## Code to run over the root files produced from ccAnalysis.C
## and extract the neutrino energy distribution.
## The output files of the fit are stored in the directory
## that will be created by this script and called 'output'.
## to run this script: source do_ccAnalysis_fit.sh
##
## This code and method are under evolution. 
## Francesca Di Lodovico (October 2009)
##

rm -rf output_old;
mv output output_old;
mkdir output;

i=0
MAvariation=49
while (( $i < ${MAvariation} )); do
    echo i=$i
    echo -n "$i"
    root -b -q 'run_ccAnalysis_fit.C('true','false','${i}')' >& output/inputtree_$i.log
    let i++
done

i=0
while (( $i < 1 )); do
    echo i=$i
    echo -n "$i"
    root -b -q 'run_ccAnalysis_fit.C('false,true,${i}')' >& output/log_$i.log
    let i++
done

