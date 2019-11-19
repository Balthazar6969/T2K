 #!/bin/bash 
export CVSROOT=:ext:conorf@repo.nd280.org:/home/trt2kmgr/ND280Repository

 source ~/CMT/setup.sh


 export CMTPATH=~/T2K/v12r7
   $CMTPATH/nd280/v12r7/cmt/setup.sh

 export CVS_RSH=ssh
 unset CVS_SERVER




