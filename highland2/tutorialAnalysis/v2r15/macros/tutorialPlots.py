
from ROOT import *


#The name of the micro-tree file (output of tutorialAnalysis) 
filename='test.root'

# Create an instance of the DrawingTools and load the configuration specified in the input file
draw = DrawingTools(filename)
  
# Create a DataSample with the input file
sample = DataSample(filename)

# Make a very simple plot
draw.SetTitleX('muon candidate momentum (MeV/c)')
draw.Draw(sample,'selmu_mom',10,0,5000,'reaction','accum_level>5')

# hold open the canvas
gPad.WaitPrimitive()
