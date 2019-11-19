
# This script sets up a .rootmap file in the directory that includes the highlandTools library.
# The .rootmap file means that ROOT will automatically load the required libraries, so calls like
# gSystem->Load("libhighlandTools.so") are not required in analysis macros.
#
# There is a slight caveat that OSX generates a .dylib file, rather that a .so file. The rlibmap
# utility assumes a .so file, so we must make a symlink between the two.
set base=$NUECCANALYSISROOT/$NUECCANALYSISCONFIG
set base1=$HIGHLANDTOOLSROOT/$HIGHLANDTOOLSCONFIG
set base2=$PSYCHECOREROOT/$PSYCHECORECONFIG
set base3=$PSYCHEUTILSROOT/$PSYCHEUTILSCONFIG


if ( -d $base ) then
  set output=$base/libDrawingToolsNueCC.rootmap
  set hl_lib=$base/libDrawingToolsNueCC.so
  set root_libs="$base1/libhighlandTools.so $base2/libpsycheCore.so $base3/libpsycheUtils.so libRIO.so libHistPainter.so libMatrix.so libPhysics.so libTree.so libGeom.so" 
  set includes=$HNUECCANALYSISROOT/src/*_LinkDef.h

  rlibmap -f -o $output -l $hl_lib -d $root_libs -c $includes

  if ( $HIGHLANDTOOLSCONFIG == "Darwin" ) then
    ln -fs $base/libDrawingToolsNueCC.dylib $base/libDrawingToolsNueCC.so
    ln -fs $base1/libhighlandTools.dylib $base1/libhighlandTools.so
    ln -fs $base2/libpsycheCore.dylib $base2/libpsycheCore.so
    ln -fs $base3/libpsycheUtils.dylib $base3/libpsycheUtils.so
  endif
endif
