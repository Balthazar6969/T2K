{

  // The name of the micro-tree file (output of tutorialAnalysis) 
  std::string filename="test.root";

  // Create an instance of the DrawingTools and load the configuration specified in the input file
  DrawingTools draw(filename);
  
  // Create a DataSample with the input file
  DataSample sample(filename);

  //Make a very simple plot
  draw.SetTitleX("muon candidate momentum (MeV/c)");
  draw.Draw(sample,"selmu_mom",10,0,5000,"reaction","accum_level>5");
}
  































}
