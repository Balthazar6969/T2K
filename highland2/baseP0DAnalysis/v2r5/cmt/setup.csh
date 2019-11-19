# echo "Setting baseP0DAnalysis v2r5 in /home/cfrancois/T2K2/highland2"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /home/cfrancois/CMT/v1r20p20081118
endif
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=baseP0DAnalysis -version=v2r5 -path=/home/cfrancois/T2K2/highland2  -no_cleanup $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

