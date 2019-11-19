
# This script sets up a .rootmap file in the directory that includes the highlandTools library.
# The .rootmap file means that ROOT will automatically load the required libraries, so calls like
# gSystem->Load("libpsycheEventModel.so") are not required in analysis macros.
#
# There is a slight caveat that OSX generates a .dylib file, rather that a .so file. The rlibmap
# utility assumes a .so file, so we must make a symlink between the two.
base=$PSYCHEEVENTMODELROOT/$PSYCHEEVENTMODELCONFIG 

if [[ -d $base ]]; then
  output=$base/libpsycheEventModel.rootmap
  hl_lib=$base/libpsycheEventModel.so
  root_libs="libRIO.so libHistPainter.so libMatrix.so libPhysics.so libTree.so libEG.so" 
  includes=$PSYCHEEVENTMODELROOT/src/*_LinkDef.h

  rlibmap -f -o $output -l $hl_lib -d $root_libs -c $includes


  if [[ $PSYCHEEVENTMODELCONFIG == "Darwin"* ]]; then
    ln -fs $base/libpsycheEventModel.dylib $base/libpsycheEventModel.so
  fi
fi
