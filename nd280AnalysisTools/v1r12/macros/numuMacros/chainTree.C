void chainTree(string inputname,string outputname){

    TString fileName(inputname);
    ifstream inputFile(fileName.Data(), ios::in);
    TChain *chain = new TChain("numutree");
    if (!inputFile) {
        cerr << "Cannot open input file '" << fileName.Data() << "'. Exit!" << endl;
        return;
    }
    
    string inputString;
    while (inputFile >> inputString) {
        std::cout << "Adding file: " << inputString << std::endl;
        chain->Add(inputString.c_str());
    }    
    TString name(outputname);
    cout<<" writing files to "<<name<<endl;
    chain->Merge(name);
    cout<<".... done "<<endl;
}
