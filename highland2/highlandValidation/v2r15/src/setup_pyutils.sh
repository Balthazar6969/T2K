# usage: source setup.sh
# Extract and setup python module "tianlu"
cd $HIGHLANDVALIDATIONROOT/src
tar -xvzf pyutils.tar.gz
export PYTHONPATH=$HIGHLANDVALIDATIONROOT/src/Python:$PYTHONPATH
echo "Your PYTHONPATH is set!"
echo $PYTHONPATH
