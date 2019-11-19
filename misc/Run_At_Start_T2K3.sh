 #!/bin/bash
export CVSROOT=:ext:conorf@repo.nd280.org:/home/trt2kmgr/ND280Repository

 export CMTPATH=/mnt/data/T2K/frozen
 .  $CMTPATH/nd280/*/cmt/setup.sh

 export CMTPATH=/mnt/data/backup/home/cfrancois/T2K2/:$CMTPATH

 export CVS_RSH=ssh
 unset CVS_SERVER

