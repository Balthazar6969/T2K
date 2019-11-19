# echo "Setting nd280Highland2 v2r21 in /home/cfrancois/T2K2"

if test "${CMTROOT}" = ""; then
  CMTROOT=/home/cfrancois/CMT/v1r20p20081118; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=nd280Highland2 -version=v2r21 -path=/home/cfrancois/T2K2  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}
