
# This script sets up a .rootmap file in the directory that includes the highlandCore library.
# The .rootmap file means that ROOT will automatically load the required libraries, so calls like
# gSystem->Load("libhighlandCore.so") are not required in analysis macros.
#
# There is a slight caveat that OSX generates a .dylib file, rather that a .so file. The rlibmap
# utility assumes a .so file, so we must make a symlink between the two.
base=$HIGHLANDCOREROOT/$HIGHLANDCORECONFIG 
base2=$PSYCHECOREROOT/$PSYCHECORECONFIG 

if [[ -d $base ]]; then
  output=$base/libhighlandCore.rootmap
  hl_lib="$base/libhighlandCore.so"
  root_libs="$base2/libpsycheCore.so   libRIO.so libHistPainter.so libMatrix.so libPhysics.so libTree.so libGeom.so libEG.so" 
  includes="$HIGHLANDCOREROOT/src/*_LinkDef.h"

  rlibmap -f -o $output -l $hl_lib -d $root_libs -c $includes

  if [[ $HIGHLANDCORECONFIG == "Darwin"* ]]; then
    ln -fs $base/libhighlandCore.dylib $base/libhighlandCore.so
    ln -fs $base2/libpsycheCore.dylib $base2/libpsycheCore.so
  fi
fi
