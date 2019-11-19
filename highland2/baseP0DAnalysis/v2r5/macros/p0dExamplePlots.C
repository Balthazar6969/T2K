{
  
  DrawingTools draw("test8.root",false);

  //-------- Draw X vs Z position of all hits for a given event with color corresponding to the hit charge 
  default.Draw("HitInParticleInVertexPosition[][][0]:HitInParticleInVertexPosition[][][2]","(HitInParticleInVertexType==0 && evt==4403975)*HitInParticleInVertexCharge","colz");

  gPad->Update();
  getchar();

  //-------- Draw Y vs Z position of all hits for a given event with color corresponding to the hit charge 
  default.Draw("HitInParticleInVertexPosition[][][1]:HitInParticleInVertexPosition[][][2]","(HitInParticleInVertexType==1 && evt==4403975)*HitInParticleInVertexCharge","colz");

  gPad->Update();
  getchar();

  // Create a DrawingTools instance with a file as input
  //  DrawingTools draw("test8.root");

  //-------- Draw the number of showers after quality cut (accum_level>0)
  draw.SetTitleX("# showers");
  draw.Draw(default,"nShowers",10,0,10,"reactionnofv","accum_level>0 && nShowers>0");

  // Draw vertical lines with arrows (true) indicating the cut
  draw.DrawCutLineVertical(2,true,"l");
  draw.DrawCutLineVertical(3,true,"r");

  gPad->Update();
  getchar();

  //-------- Draw the number of tracks after "2 showers cut" (accum_level>1)
  draw.SetTitleX("# tracks");
  draw.Draw(default,"nParticlesInVertex-nShowers",10,0,10,"reactionnofv","accum_level>1");

  // Draw vertical lines with arrows (true) indicating the cut
  draw.DrawCutLineVertical(1,true,"r");

  gPad->Update();
  getchar();

  //-------- Draw the number of delayed clusters after "0 tracks cut" (accum_level>2)
  draw.SetTitleX("# delayed clusters");
  draw.Draw(default,"nMichel",15,0,15,"reactionnofv","accum_level>2");

  // Draw vertical lines with arrows (true) indicating the cut
  draw.DrawCutLineVertical(1,true,"r");

  gPad->Update();
  getchar();

  //-------- Draw the energy of the most energetic shower after "0 delayed clsusters cut" (accum_level>3)
  draw.SetTitleX("energy of most energetic shower");
  draw.Draw(default,"Shower1EDeposit",10,0,10000,"reactionnofv","accum_level>3");

  getchar();

  //-------- Draw the energy of the second most energetic shower after "0 delayed clsusters cut" (accum_level>3)
  draw.SetTitleX("energy of second most energetic shower");
  draw.Draw(default,"Shower2EDeposit",10,0,10000,"reactionnofv","accum_level>3");

  getchar();

  //-------- Draw the efficiency and purity for selecting NC (reactionnofv==4) as a function of the cut

  // Create first a DataSample such that default (needed for purity) and truth (needed for efficiency) trees can be used simultaneously
  DataSample mc("test8.root");

  draw.DrawEffPurVSCut(mc,"reactionnofv==4");
}
