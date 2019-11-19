#include "Stage2GasSelection.hxx"
#include <iostream>
#include "libReadoaAnalysisProjectHeaders.h"

int main(int argc, char** argv) {
  char* inFile;
  char* outFile;

  if (argc != 3) {
    std::cout << "Incorrect arguments: please give input filename and output "
                 "filename."
              << std::endl;
    return 1;
  }
  if (argc == 3) {
    std::cout << "Reading from input file(s): " << argv[1] << std::endl;
    std::cout << "Writing to output file: " << argv[2] << std::endl;

    inFile = argv[1];
    outFile = argv[2];
  }

  Stage2GasSelection(inFile, outFile);

  return 0;
}
