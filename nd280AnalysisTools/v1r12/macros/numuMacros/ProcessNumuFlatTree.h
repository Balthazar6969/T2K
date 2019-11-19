#include "TRandom3.h"
#include <TSystem.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TTree.h>


int p_Module_g10a;
int p_Module_bbb;
int p_Module_ccc;
int p_Module_ddd;

// Contains very general variables, needed by everyone
// Prefix should be p_ for every variable in this common.

bool p_isMC;
bool p_silent;
TTree *p_numutree;

TRandom3 p_fRandomGenerator;


// Very common constant:
//------------------------------------------------------------------------- 
double p_muMass = 105.658367; //MeV (muon mass)                                
double p_pMass = 938.27203; //MeV (proton mass)                                
double p_nMass = 939.56536; //MeV (neutron mass)                               
double p_NucPot = 25.; //MeV (nuclear potential)          


//General variables from oaAnalysis output
//-------------------------------------------------------------------------                                               
int p_NPIDs;                                       
int p_NTPC;                                        
int p_NTPCIso;                                       
int p_NDsECALIso;                                    
int p_NTrECALIso;                                    
int p_NP0DIso;                                      
int p_NSMRDIso;                                    
int p_NFGDIso;                                     
int p_NFGD1Unused, p_NFGD2Unused;                      
int p_NDelayedClusters;                            
double p_EarliestTrackTime;
int p_EventID;       
int p_RunID;

TClonesArray *p_PIDs;
TClonesArray *p_DelayedClusters;



// Dimensions of arrays:
//_______________________________________________________________________
const int p_NMAXTRAJ = 200;
const int p_NMAXTRUEVERTICES = 50;
const int p_NMAXCLUSTERS = 50;
const int p_NVTXTYPES = 11;
const int p_NTRAJTYPES = 7;
const int p_NBUNCH = 9;
const int p_NRUNS = 4;
const int p_NHYPO = 5;
const int p_NMAXTRACKS = 10;
const int p_NOPTIONS = 2;
//_______________________________________________________________________


//Cuts
const double p_michelCut[5] = {400., 600., 800., 1200., 1600.};


//_______________________________________________________________________
// Some Truth variables:

int p_NTruthVertices[p_NVTXTYPES];
int p_NTruthTrajs[p_NTRAJTYPES];

// Set the branches for the truth information.
TClonesArray *p_TruthVertices[p_NVTXTYPES];
TClonesArray *p_TruthTrajs[p_NTRAJTYPES];


// set a few string arrays
string p_vtxnames[p_NVTXTYPES] = {"VtxFGD1","VtxFGD2","VtxP0D","VtxTPC","VtxMRD","VtxDsECal","VtxBrlECal","VtxRestOfOffAxis","VtxOther","VtxINGRID","VtxP0DECal"};

int p_TruthVertexDet[p_NVTXTYPES] = {4, 5, 8, 1, 9, 6, 7, 10, 11, 12, 7};
string p_trajnames[p_NTRAJTYPES] = {"TrajsLepton","TrajsBaryon","TrajsMeson","TrajsPhoton","TrajsOtherCharged","TrajsOtherNeutral","TrajsOther"};
string p_ntrajnames[p_NTRAJTYPES] = {"NTrajsLepton","NTrajsBaryon","NTrajsMeson","NTrajsPhoton","NTrajsOtherCharged","NTrajsOtherNeutral","NTrajsOther"};

//________________________________________________________________________
// Constant related to bunch time

// Bin zero is always 0
// Bin 1-6 correpond to the peak of the timing distribution for bunch 1-6 (given by gaussian fit)
// Bin 7 is for very high time
// Bin 8 is for other times

// Values ok for MCP1
const double p_time_mc[p_NBUNCH] = {0, 87.5, 671.6, 1252.9, 1837.3, 2416.1, 2998.6, 10000.0, -5000.0};
//and RDP1
const double p_time_data[p_NBUNCH] = {0, 3357.2, 3940.8, 4523.9, 5105.9, 5689.6, 6272.3, 10000.0, -5000.0};

/* //For MCP2: */
/* const double p_time_mc[p_NBUNCH] = {0, 2780.2, 3363.0, 3944.8, 4527.5, 5108.4,5690.4, 10000.0, -5000.0}; */
/* //For RDP3: */
/* const double p_time_data[p_NBUNCH] = { 0, 2834, 2780, 3418, 4000, 5166, 5747,  10000.0, -5000.0}; */
