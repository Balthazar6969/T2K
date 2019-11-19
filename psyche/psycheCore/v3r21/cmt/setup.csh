# echo "Setting psycheCore v3r21 in /mnt/data/backup/home/cfrancois/T2K2/psyche"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /mnt/data/T2K/CMT/v1r20p20081118
endif
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=psycheCore -version=v3r21 -path=/mnt/data/backup/home/cfrancois/T2K2/psyche  -no_cleanup $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

