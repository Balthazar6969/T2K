#include <iostream>
#include "libReadoaAnalysisProjectHeaders.h"
#include "TPCPOTCalculator.hxx"

int main(int argc, char ** argv){

  char* inFile;
  while (optind<argc) {
      inFile = argv[optind++];
  }

  TPCPOTCalculator(inFile);

  return 0;
}
