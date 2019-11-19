{  
  std::string dir =  gSystem->Getenv("PSYCHECOREROOT");
  gSystem->Exec("rm -f src");
  std::string ln = "ln -fs "+dir+"/src src";
  gSystem->Exec(ln.c_str());
  _file0.MakeProject("highland2Classes","*","+ RECREATE");
  gSystem->Exec("rm -f src");
  gSystem->Exec("rm -fr ../src/highland2Classes");
  gSystem->Exec("mv highland2Classes ../src");
}
