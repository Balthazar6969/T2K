if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /home/cfrancois/CMT/v1r20p20081118
endif
source ${CMTROOT}/mgr/setup.csh
set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=antiNueCCAnalysis -version=v2r9 -path=/home/cfrancois/T2K2/highland2 $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

