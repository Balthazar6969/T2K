#!/bin/sh

# Example of script to chain all MC together in one file:
#
# F.Dufour (February 2011)


GEN=neut

# Set your path correctly
WORKDIR=/atlas/users/mravonel/t2k/nd280/oaAnalysis/v4r22/macros
OUTPUTDIR=/neutrino/data2/t2k/mcp1/$GEN/numuflattree
TARGETDIR=/neutrino/data2/t2k/minitree_geneva

#LOWER RUN LIMIT (from LOW)
LOW=0000
#UPPER RUN LIMIT (goes to HIGH-1)
HIGH=0100
numMIN=90000000
numMAX=90000001

ANAVERSION=v4r22
rm -rf $TARGETDIR/numuflattree_${GEN}_rev966_usedfile.txt

for FILE in `ls $OUTPUTDIR`
  do

# Depending on the file name, you might have to play with this   
  FILEB=`basename $FILE`
  FILEBASE=${FILEB%\.root}
  filechop=${FILEBASE:13:8}
  filechopsub=${FILEBASE:22:4}

# Check for broken files 
  FILESIZE=$(stat -c%s $OUTPUTDIR/$FILE)
  if [ $FILESIZE -lt 500 ]
      then
      echo file $filechop $filechopsub is broken: $FILESIZE
      continue
  fi


 if [ $filechop -lt $numMIN ]
      then
      continue
  fi
  if [ $filechop -ge $numMAX ]
      then
      continue
  fi
  if [ $filechopsub -lt $LOW ]
      then
      continue
  fi
  if [ $filechopsub -ge $HIGH ]
      then
      continue
  fi

      
      OUTPUTFILE=${OUTPUTDIR}/numuflattree_${filechop}_${filechopsub}.root
    
      echo $OUTPUTFILE > $FILE.txt
      cat $FILE.txt >> $TARGETDIR/numuflattree_${GEN}_rev966_usedfile.txt
      rm -rf $FILE.txt
          

done



root -b -q 'chainTree.C("numuflattree_'${GEN}'_rev966_usedfile.txt","'$TARGETDIR'/numuflattree_'${GEN}'_rev966.root")'

# Change mode if you are sharing files with colleagues and want them to use them too.
# (Obviously not needed)

chmod 775 $TARGETDIR/numuflattree_${GEN}_rev966.root
