if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /mnt/data/T2K/CMT/v1r20p20081118
endif
source ${CMTROOT}/mgr/setup.csh
set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=psycheSystematics -version=v3r23 -path=/mnt/data/backup/home/cfrancois/T2K2/psyche $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

