{
  
  // T2K style definition
  // Adopted from BaBar collaboration
  // Add the following lines to the start of your rootlogon.C file
  TStyle *t2kStyle= new TStyle("T2K","T2K approved plots style");
  
  // use plain black on white colors
  t2kStyle->SetFrameBorderMode(0);
  t2kStyle->SetCanvasBorderMode(0);
  t2kStyle->SetPadBorderMode(0);
  t2kStyle->SetPadColor(0);
  t2kStyle->SetCanvasColor(0);
  t2kStyle->SetStatColor(0);
  t2kStyle->SetLineWidth(2);
  //t2kStyle->SetFillColor(0); ML
  t2kStyle->SetLegendBorderSize(1);
  TGaxis::SetMaxDigits(4); // restrict the number of digits in labels

  // set the paper & margin sizes
  t2kStyle->SetPaperSize(20,26);
  t2kStyle->SetPadTopMargin(0.05);
  t2kStyle->SetPadRightMargin(0.05);
  t2kStyle->SetPadBottomMargin(0.16);
  t2kStyle->SetPadLeftMargin(0.12);

  // use large Times-Roman fonts
  t2kStyle->SetTextFont(132);
  t2kStyle->SetTextSize(0.08);
  t2kStyle->SetLabelFont(132,"xyzt");
  t2kStyle->SetLabelSize(0.06,"xyzt"); // 0.05 ML
  t2kStyle->SetTitleSize(0.07,"xyzt"); // 0.06 ML
  t2kStyle->SetTitleFont(132,"xyzt");

  // Sort out the title incase you want to override the default not seeing the title
  t2kStyle->SetTitleFillColor(0);
  t2kStyle->SetTitleFontSize(0.06);
  t2kStyle->SetTitleFont(132,"pad");
  t2kStyle->SetTitleBorderSize(0);
  
  // Align to top left
  t2kStyle->SetTitleAlign(13);
  
  // use bold lines and markers
  // t2kStyle->SetMarkerStyle(20);
  t2kStyle->SetHistLineWidth(1.85);
  t2kStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
  
  // get rid of X error bars and y error bar caps
  // t2kStyle->SetErrorX(0.001);
  
  
  
  // do not display any of the standard histogram decorations
  t2kStyle->SetOptTitle(0);
  t2kStyle->SetOptStat(0); //ML
  t2kStyle->SetOptFit(0); //ML
  
  // put tick marks on top and RHS of plots
  t2kStyle->SetPadTickX(1);
  t2kStyle->SetPadTickY(1);
  t2kStyle->SetNdivisions(505, "XY");
  
  // Add a greyscale palette for 2D plots
  // int ncol=50;
  // double dcol = 1./float(ncol);
  // double gray = 1;
  // TColor **theCols = new TColor*[ncol];
  // for (int i=0;i<ncol;i++) theCols[i] = new TColor(999-i,0.0,0.7,0.7);
  // for (int j = 0; j < ncol; j++) {
  //   theCols[j]->SetRGB(gray,gray,gray);
  //   gray -= dcol;
  // }
  // int ColJul[100];
  // for  (int i=0; i<100; i++) ColJul[i]=999-i;
  // t2kStyle->SetPalette(ncol,ColJul);
  
  
  
  // Define a nicer color palette (red->blue)
  // Uncomment these lines for a color palette (default is B&W)
  t2kStyle->SetPalette(1, 0);  // use the nice red->blue palette
  const Int_t NRGBs = 5;
  const Int_t NCont = 200;
  
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  t2kStyle->SetNumberContours(NCont); 


  // Values for red to blue only ---> For covariance matrices etc
  const Int_t NRGBs = 3;
  const Int_t NCont = 200;
  Double_t stops[NRGBs] = { 0.00, 0.50, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 1.00, 1.00 };
  Double_t green[NRGBs] = { 0.00, 1.00, 0.00 };
  Double_t blue[NRGBs]  = { 1.00, 1.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  t2kStyle->SetNumberContours(NCont); 

  // const Int_t NRGBs = 2;
  // const Int_t NCont = 200;
  // Double_t stops[NRGBs] = { 0.00, 1.00 };
  // Double_t red[NRGBs]   = { 1.00, 1.00 };
  // Double_t green[NRGBs] = { 1.00, 0.00 };
  // Double_t blue[NRGBs]  = { 1.00, 0.00 };
  // TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  // t2kStyle->SetNumberContours(NCont);
  
  // End of definition of t2kStyle
  gROOT->Reset();
  gROOT->SetStyle("T2K");
  gROOT->ForceStyle();
  
}
