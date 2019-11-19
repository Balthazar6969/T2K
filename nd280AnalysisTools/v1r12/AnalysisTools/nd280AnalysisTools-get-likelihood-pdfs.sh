#! /bin/bash
#
# Download the Likelihood PDFs from repo.nd280.org/ecalRecon/likelihoodPidPdfs
# using wget.  This is run without arguments.
# Note that this is based on the script $OAEVENTROOT/scripts/nd280-get-geometry

# The location of the input files.
if [ ${LIKELIHOODPDFFILES}x == x ]; then
    LIKELIHOODPDFFILES=http://repo.nd280.org/nd280files/ecalRecon/likelihoodPidPdfs/
fi

# This directory should ALWAYS exist, but check anyway.
if [ ! -d ${ND280ANALYSISTOOLSROOT} ]; then
    echo nd280AnalysisTools-get-likelihood=pdfs: nd280AnalysisTools directory is missing!
    echo    where is: ${ND280ANALYSISTOOLSROOT}
    exit 1
fi
cd ${ND280ANALYSISTOOLSROOT}

PDF_DIR=${ND280ANALYSISTOOLSROOT}/AnalysisTools/ecalPidPdfs

# This directory won't exist when the package is checkout the first  time.
if [ ! -d ${PDF_DIR} ]; then
    mkdir -p ${PDF_DIR}
fi
cd ${PDF_DIR}

# Get an index of pdf files.
wget -q -S -N ${LIKELIHOODPDFFILES} -o /dev/null

if [ ! -f index.html ]; then
    echo nd280AnalysisTools-get-likelihood-pdfs: Cannot check for new pdf files.
    exit 1
fi

# Build a list of the files download.
cat index.html | \
    egrep ecalRecon_likelihood_pdfs_.*\.root | \
    sed 's/<tr>.*<a[^>]*>'// | \
    sed 's/<.*//' | \
    cat > nd280AnalysisTools_likelihood_pdfs.index

# Remove the HTML index.
rm index.html

# Get the files.
for file in `cat nd280AnalysisTools_likelihood_pdfs.index`; do
    if [ ! -f $file ]; then
        wget -nv -S -N -c ${LIKELIHOODPDFFILES}/$file 2>&1 | grep URL;
    else echo "already have "$file
    fi
done

echo nd280AnalysisTools Likelihood PID PDFs up to date
