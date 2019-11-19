if test "${CMTROOT}" = ""; then
  CMTROOT=/mnt/data/T2K/CMT/v1r20p20081118; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=highlandEventModel -version=v2r19 -path=/mnt/data/backup/home/cfrancois/T2K2/highland2 $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

