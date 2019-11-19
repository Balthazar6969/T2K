
#include <sstream>
//ROOT includes
#include "TKey.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TPRegexp.h"
#include "TObjString.h"
#include "TObjArray.h"
#include "TString.h"

//ND280 includes
#include "TECALPidLikelihood.hxx"
#include "TECALPidLikelihoodHypothesisTest.hxx"
//#include "TOARuntimeParameters.hxx"
//#include "TRealDatum.hxx"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ecalPid::TECALPidLikelihood::TECALPidLikelihood() : fPdfManager(0)
{
	//nothing to do.
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ecalPid::TECALPidLikelihood::~TECALPidLikelihood()
{
	//delete the algorithms that we created
	for(AlgorithmIterator it = fListOfAlgorithms.begin();
			it != fListOfAlgorithms.end();
			++it)
	{
		TECALPidLikelihoodAlgorithm* alg = it->second;
		if(alg) { delete alg; it->second = 0; }
	}
	//delete the pdf manager
	if(fPdfManager) { delete fPdfManager; fPdfManager = 0; }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihood::CalculateLikelihoodValues(ND::TTrackerECALReconModule::TECALReconObject& inputObject)
{
	for(AlgorithmIterator it = fListOfAlgorithms.begin();
			it != fListOfAlgorithms.end();
			++it)
	{
		TECALPidLikelihoodAlgorithm* alg = it->second;
		double logLikelihoodRatio = alg->CalculateLogLikelihoodRatio(inputObject);
		fLikelihoodRatioResults[it->first] = logLikelihoodRatio;
	}
	return;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihood::SetupAlgorithms()
{
	//check that pdf manager has been loaded
	if(!fPdfManager)
	{
		fPdfManager = new TECALPidLikelihoodPdfManager();
	}

	//load hard-coded algorithms
	//for now we want to keep things flexible and just laod them from the
	//parameters file.

	//get algorithms defined in runtime parameters
	//LoadAlgorithmsFromRunTimeParameters();
	//create a list of likelihood calculators
	std::string algName;
	int nVars = 0;
	ecalPid::TECALPidDefs::PidHypothesisId_t hypothesis1(ecalPid::TECALPidDefs::kECALPidHypothesis_Invalid);
	ecalPid::TECALPidDefs::PidHypothesisId_t hypothesis2(ecalPid::TECALPidDefs::kECALPidHypothesis_Invalid);
	std::vector<ecalPid::TECALPidDefs::PidVariableId_t> listOfInputVariables;
	ecalPid::TECALPidLikelihoodAlgorithm* alg(0);
	//setup track shower algorithm
	algName = "MipEm";
	hypothesis1 = ecalPid::TECALPidDefs::kECALPidHypothesis_Mip;
	hypothesis2 = ecalPid::TECALPidDefs::kECALPidHypothesis_EmShower;
	nVars = 4;
	ecalPid::TECALPidDefs::PidVariableId_t varsTrackShower[] = {
			ecalPid::TECALPidDefs::kECALPidVar_Circularity,
			ecalPid::TECALPidDefs::kECALPidVar_QRMS,
			//ecalPid::TECALPidDefs::kECALPidVar_TransverseChargeRatio,
			ecalPid::TECALPidDefs::kECALPidVar_TruncatedMaxRatio,
			ecalPid::TECALPidDefs::kECALPidVar_FrontBackRatio,
	};
	listOfInputVariables = std::vector<ecalPid::TECALPidDefs::PidVariableId_t>(varsTrackShower, varsTrackShower+nVars);
	alg = new ecalPid::TECALPidLikelihoodAlgorithm(algName, hypothesis1, hypothesis2, listOfInputVariables, *fPdfManager);
	fListOfAlgorithms[algName] = alg;
	fLikelihoodRatioResults[algName] = 0.0;
	//setup MIP Track+Shower
	algName = "MipPion";
	hypothesis1 = ecalPid::TECALPidDefs::kECALPidHypothesis_Mip;
	hypothesis2 = ecalPid::TECALPidDefs::kECALPidHypothesis_ShoweringPion;
	nVars = 4;
	ecalPid::TECALPidDefs::PidVariableId_t varsMipPion[] = {
			ecalPid::TECALPidDefs::kECALPidVar_Circularity,
			ecalPid::TECALPidDefs::kECALPidVar_QRMS,
			//ecalPid::TECALPidDefs::kECALPidVar_TransverseChargeRatio,
			ecalPid::TECALPidDefs::kECALPidVar_TruncatedMaxRatio,
			ecalPid::TECALPidDefs::kECALPidVar_FrontBackRatio,
	};
	listOfInputVariables = std::vector<ecalPid::TECALPidDefs::PidVariableId_t>(varsMipPion, varsMipPion+nVars);
	alg = new ecalPid::TECALPidLikelihoodAlgorithm(algName, hypothesis1, hypothesis2, listOfInputVariables, *fPdfManager);
	fListOfAlgorithms[algName] = alg;
	fLikelihoodRatioResults[algName] = 0.0;
	//setup electron/proton tag
	algName = "EmHip";
	hypothesis1 = ecalPid::TECALPidDefs::kECALPidHypothesis_EmShower;
	hypothesis2 = ecalPid::TECALPidDefs::kECALPidHypothesis_Hip;
	nVars = 4;
	ecalPid::TECALPidDefs::PidVariableId_t varsEmHip[] = {
			ecalPid::TECALPidDefs::kECALPidVar_Circularity,
			ecalPid::TECALPidDefs::kECALPidVar_QRMS,
			//ecalPid::TECALPidDefs::kECALPidVar_TransverseChargeRatio,
			ecalPid::TECALPidDefs::kECALPidVar_TruncatedMaxRatio,
			ecalPid::TECALPidDefs::kECALPidVar_FrontBackRatio,
	};
	listOfInputVariables = std::vector<ecalPid::TECALPidDefs::PidVariableId_t>(varsEmHip, varsEmHip+nVars);
	alg = new ecalPid::TECALPidLikelihoodAlgorithm(algName, hypothesis1, hypothesis2, listOfInputVariables, *fPdfManager);
	fListOfAlgorithms[algName] = alg;
	fLikelihoodRatioResults[algName] = 0.0;
	//Print out information about the implemented algorthms.
	for(AlgorithmIterator it = fListOfAlgorithms.begin();
			it != fListOfAlgorithms.end();
			++it)
	{
		it->second->Print();
	}
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//void ecalPid::TECALPidLikelihood::LoadAlgorithmsFromRunTimeParameters()
//{
//	//retrieve settings from the ecalRecon parameters file
//	ecalPid::TOARuntimeParameters& runtimeParameters = ecalPid::TOARuntimeParameters::Get();
//	bool breakFlag = 0;
//	int i = -1;//counter for algorithm number.
//	while(!breakFlag)
//	{
//		++i;//increment algorithm number counter
//		std::string iStr = this->ConvertToString(i);
//		//look for an algorithm
//		std::string parRoot = "ecalRecon.Pid.Likelihood.Algorithm"+iStr;
//		std::string parAlgName = parRoot + ".Name";
//		bool hasAlg = runtimeParameters.HasParameter(parAlgName);
//		if(hasAlg)
//		{
//			//get algorithm name
//			std::string algName = runtimeParameters.GetParameterS(parAlgName);
//			//get the selected hypotheses
//			std::string parHypothesis1 = parRoot + ".Hypothesis1";
//			std::string parHypothesis2 = parRoot + ".Hypothesis2";
//			//get hypotheses names
//			//an exception will be intentionally thrown if the parameters file is incorrectly configured
//			std::string hypothesis1Name = runtimeParameters.GetParameterS(parHypothesis1);
//			std::string hypothesis2Name = runtimeParameters.GetParameterS(parHypothesis2);
//			//convert string to PidHypothesisId_t enum.
//			ecalPid::TECALPidDefs::PidHypothesisId_t hypothesis1 = ecalPid::TECALPidDefs::Get().GetHypothesisId(hypothesis1Name);
//			ecalPid::TECALPidDefs::PidHypothesisId_t hypothesis2 = ecalPid::TECALPidDefs::Get().GetHypothesisId(hypothesis2Name);
//			//get the selected input variables
//			std::vector<ecalPid::TECALPidDefs::PidVariableId_t> listOfInputVariables;
//			std::string parInputVarRoot = parRoot+".InputVariable";
//			int j = -1;//counter for variable number
//			bool inputVariablesBreakFlag = 0;
//			while(!inputVariablesBreakFlag)
//			{
//				++j; //increment variable number counter
//				std::string jStr = this->ConvertToString(j);
//				std::string parVariable = parInputVarRoot + jStr;
//				bool hasVariable = runtimeParameters.HasParameter(parVariable);
//				if (hasVariable)
//				{
//					//get variable name
//					std::string variableName = runtimeParameters.GetParameterS(parVariable);
//					//convert string to PidVariableId_t enum.
//					//an exception will be intentionally thrown if the variable is unknown.
//					ecalPid::TECALPidDefs::PidVariableId_t variable = ecalPid::TECALPidDefs::Get().GetVariableId(variableName);
//					listOfInputVariables.push_back(variable);
//				}
//				else if(j>1)
//				{
//					//couldn't find an variable, stop looking!
//					//note: we keep looking if j<=0 in case the user started from counting
//					//from 1.
//					inputVariablesBreakFlag = 1;//break out of the loop.
//				}
//			}
//			//check that pdf manager has been loaded
//			if(!fPdfManager)
//			{
//				fPdfManager = new TECALPidLikelihoodPdfManager();
//			}
//			//we now have all of the information required to create the
//			//create a list of likelihood calculator
//			ecalPid::TECALPidLikelihoodAlgorithm* alg = new ecalPid::TECALPidLikelihoodAlgorithm(algName, hypothesis1, hypothesis2, listOfInputVariables, *fPdfManager);
//			alg->Print();
//			fListOfAlgorithms.push_back(alg);
//			//add default value into the vector to store results
//			fLikelihoodRatioResults.push_back(0.);
//		}
//		else if(i>1)
//		{
//			//couldn't find an algorithms, stop looking!
//			//note: we keep looking if i<=0 in case the user started from counting
//			//from 1.
//			breakFlag = 1;
//		}
//	}
//	return;
//}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::string ecalPid::TECALPidLikelihood::ConvertToString(int i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
double ecalPid::TECALPidLikelihood::GetLikelihoodValue(const std::string& algorithmName)
{
	ResultIterator it = fLikelihoodRatioResults.find(algorithmName);
	if (it != fLikelihoodRatioResults.end())
	{
		return it->second;
	}
	else
	{//unknown algorithm, raise an exception
		throw EECALPidLikelihoodUnknownAlgorithm();
	}
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *





// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ecalPid::TECALPidLikelihoodAlgorithm::TECALPidLikelihoodAlgorithm(
		const std::string& algorithmName,
		ecalPid::TECALPidDefs::PidHypothesisId_t hypothesis1,
		ecalPid::TECALPidDefs::PidHypothesisId_t hypothesis2,
		const std::vector<ecalPid::TECALPidDefs::PidVariableId_t>& listOfInputVariables,
		TECALPidLikelihoodPdfManager& pdfManager
)
: fAlgorithmName(algorithmName),
  fHypothesis1(hypothesis1),
  fHypothesis2(hypothesis2),
  fListOfInputVariables(listOfInputVariables),
  fLikelihood(0)
{
	this->Configure(pdfManager);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ecalPid::TECALPidLikelihoodAlgorithm::~TECALPidLikelihoodAlgorithm()
{
	if(fLikelihood) { delete fLikelihood; fLikelihood = 0; }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodAlgorithm::Configure(TECALPidLikelihoodPdfManager& pdfManager)
{
	fLikelihood = new TECALPidLikelihoodHypothesisTest(fAlgorithmName);
	ecalPid::TECALPidDefs::PidVariableId_t angleVariable = ecalPid::TECALPidDefs::kECALPidVar_Angle;
	//setup the likelihood hypothesis test
	for(std::vector<ecalPid::TECALPidDefs::PidVariableId_t>::iterator it = fListOfInputVariables.begin();
			it != fListOfInputVariables.end();
			++it)
	{
		ecalPid::TECALPidDefs::PidVariableId_t variable = *it;
		TH2F* histPdf1 = dynamic_cast<TH2F*>(pdfManager.GetPdf(fHypothesis1,variable, 2));
		fLikelihood->AddPdf(fHypothesis1, histPdf1, variable, angleVariable);
		TH2F* histPdf2 = dynamic_cast<TH2F*>(pdfManager.GetPdf(fHypothesis2,variable, 2));
		fLikelihood->AddPdf(fHypothesis2, histPdf2, variable, angleVariable);
	}
	return;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
double ecalPid::TECALPidLikelihoodAlgorithm::CalculateLogLikelihoodRatio(ND::TTrackerECALReconModule::TECALReconObject& inputObject)
{
	//set the current values
	for(std::vector<ecalPid::TECALPidDefs::PidVariableId_t>::iterator it = fListOfInputVariables.begin();
			it != fListOfInputVariables.end();
			++it)
	{
		ecalPid::TECALPidDefs::PidVariableId_t variableId = *it;
		double variableValue = this->GetVariableValue(variableId,inputObject);
		fLikelihood->SetDataValue(variableId, variableValue);
	}
	//get the log likelihood ratio
	double logLikelihoodRatio = fLikelihood->GetLogLikelihoodRatio(this->fHypothesis1, this->fHypothesis2);
	return logLikelihoodRatio;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
double ecalPid::TECALPidLikelihoodAlgorithm::GetVariableValue(ecalPid::TECALPidDefs::PidVariableId_t variableId, ND::TTrackerECALReconModule::TECALReconObject& inputObject)
{
//	double result = 0.;
//	THandle<TRealDatum> datum = inputObject->Get<TRealDatum>(variableName.c_str());
//	if(datum)
//	{
//		result = datum->GetValue();
//	}
//	else
//	{
//		inputObject->ls();
//		ND280Error("TECALPidLikelihoodAlgorithm cannot find variable named:"<<variableName<<" inside input object.");
//		throw EECALLikelihoodAlgorithmMissingVariable();
//	}
//	return result;
	switch(variableId)
	{
    
#if !VERSION_HAS_REMOVED_OBSOLETE_ECAL_PID_VARS 
	case ecalPid::TECALPidDefs::kECALPidVar_AMR:
		return inputObject.PID_AMR;

//	case ecalPid::TECALPidDefs::kECALPidVar_Track_Length:
//		return inputObject.PID_Track_Width;
//	case ecalPid::TECALPidDefs::kECALPidVar_NumHits:
//		return inputObject.PID_NumHits;
//	case ecalPid::TECALPidDefs::kECALPidVar_ChargeRatio:
//		return inputObject.PID_ChargeRatio;
//	case ecalPid::TECALPidDefs::kECALPidVar_MeanCharge:
//		return inputObject.PID_MeanCharge;
	case ecalPid::TECALPidDefs::kECALPidVar_Max_Ratio:
		return inputObject.PID_Max_Ratio;
#endif
    
	case ecalPid::TECALPidDefs::kECALPidVar_TruncatedMaxRatio:
		return inputObject.PID_TruncatedMaxRatio;
//	case ecalPid::TECALPidDefs::kECALPidVar_MaxWidth:
//		return inputObject.PID_MaxWidth;
//	case ecalPid::TECALPidDefs::kECALPidVar_ChargeStandardDeviation:
//		return inputObject.PID_ChargeStandardDeviation;
//	case ecalPid::TECALPidDefs::kECALPidVar_WidthDiff:
//		return inputObject.PID_WidthDiff;
//	case ecalPid::TECALPidDefs::kECALPidVar_MinWidth:
//		return inputObject.PID_MinWidth;

    
#if! VERSION_HAS_REMOVED_OBSOLETE_ECAL_PID_VARS 
	case ecalPid::TECALPidDefs::kECALPidVar_Qskew:
		return inputObject.PID_Qskew;
	case ecalPid::TECALPidDefs::kECALPidVar_EMLikelihood:
		return inputObject.PID_EMLikelihood;
#endif
    
  case ecalPid::TECALPidDefs::kECALPidVar_TransverseChargeRatio:
		return inputObject.PID_TransverseChargeRatio;
	case ecalPid::TECALPidDefs::kECALPidVar_Asymmetry:
		return inputObject.PID_Asymmetry;
	case ecalPid::TECALPidDefs::kECALPidVar_ShowerAngle:
		return inputObject.PID_ShowerAngle;
    
    
#if! VERSION_HAS_REMOVED_OBSOLETE_ECAL_PID_VARS 
	case ecalPid::TECALPidDefs::kECALPidVar_MeanPosition:
		return inputObject.PID_MeanPosition;
	case ecalPid::TECALPidDefs::kECALPidVar_ShowerWidth:
		return inputObject.PID_ShowerWidth;
//	case ecalPid::TECALPidDefs::kECALPidVar_KSTest:
//		return inputObject.PID_KSTest;
	case ecalPid::TECALPidDefs::kECALPidVar_KFParameter:
		return inputObject.KFParameter;
	case ecalPid::TECALPidDefs::kECALPidVar_KFNNodes:
		return inputObject.KFNNodes;
	case ecalPid::TECALPidDefs::kECALPidVar_KFParameterNodes:
		return inputObject.KFParameterNodes;
	case ecalPid::TECALPidDefs::kECALPidVar_KFHitQuality:
		return inputObject.KFHitQuality;
	case ecalPid::TECALPidDefs::kECALPidVar_NormalizedMipChi2:
		return inputObject.PID_NormalizedMipChi2;
	case ecalPid::TECALPidDefs::kECALPidVar_NormalizedMipChi2AllLayers:
		return inputObject.PID_NormalizedMipChi2AllLayers;
    
#endif
	case ecalPid::TECALPidDefs::kECALPidVar_Circularity:
		return inputObject.PID_Circularity;
	case ecalPid::TECALPidDefs::kECALPidVar_QRMS:
		return inputObject.EMEnergyFit_Para_QRMS;
	case ecalPid::TECALPidDefs::kECALPidVar_FrontBackRatio:
		return inputObject.PID_FrontBackRatio;
	case ecalPid::TECALPidDefs::kECALPidVar_Angle:
		return inputObject.PID_Angle;
	default:
		std::cerr << "TECALPidLikelihoodAlgorithm::GetVariableValue unknown variable " << variableId << std::endl;
		throw EECALPidLikelihoodUnknownVariable();
	}
	return 0.0;//prevent compiler warnings.
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const std::string& ecalPid::TECALPidLikelihoodAlgorithm::GetName()
{
	return fAlgorithmName;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodAlgorithm::Print()
{
	std::string hyp1Name = ecalPid::TECALPidDefs::Get().GetHypothesisName(fHypothesis1);
	std::string hyp2Name = ecalPid::TECALPidDefs::Get().GetHypothesisName(fHypothesis2);
	std::cout << "TECALPidLikelihoodAlgorithm("
			<< "algName=" << fAlgorithmName
			<< ", hypothesis1=" << fHypothesis1 << " (" << hyp1Name << ")"
			<< ", hypothesis2="<< fHypothesis2 << " (" << hyp2Name << ")"
			<< ")"
			<< std::endl;
	int i = 0;
	for(std::vector<ecalPid::TECALPidDefs::PidVariableId_t>::iterator it = fListOfInputVariables.begin();
			it != fListOfInputVariables.end();
			++it)
	{
		ecalPid::TECALPidDefs::PidVariableId_t varId = *it;
		std::string varName = ecalPid::TECALPidDefs::Get().GetVariableName(varId);
		std::cout << "variable" << i << "= " << varId << " (" << varId << ")" << std::endl;
		++i;
	}
	return;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *







// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ecalPid::TECALPidLikelihoodPdfManager::TECALPidLikelihoodPdfManager() : fHasLoadedPdfs(0)
{
	//nothing to do.
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ecalPid::TECALPidLikelihoodPdfManager::~TECALPidLikelihoodPdfManager()
{
	//need to delete my histograms
	for(std::map<ecalPid::TECALPidLikelihoodPdfManager::PdfKey_t, TH1*>::iterator it = fPdfs.begin();
			it != fPdfs.end();
			++it)
	{
		TH1* hist = it->second;
		if(hist) { delete hist; it->second = 0; }
	}
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
TH1* ecalPid::TECALPidLikelihoodPdfManager::GetPdf(ecalPid::TECALPidDefs::PidHypothesisId_t hypothesis, ecalPid::TECALPidDefs::PidVariableId_t& variable, int nDimensions)
{
	std::string hypothesisName = ecalPid::TECALPidDefs::Get().GetHypothesisName(hypothesis);
	std::string variableName = ecalPid::TECALPidDefs::Get().GetVariableName(variable);
	return this->GetPdf(hypothesisName, variableName, nDimensions);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
TH1* ecalPid::TECALPidLikelihoodPdfManager::GetPdf(const std::string& hypothesisName, const std::string& variableName, int nDimensions)
{
	if(!fHasLoadedPdfs)
	{
		this->LoadPdfs();
	}
	TH1* result = 0;
	ecalPid::TECALPidLikelihoodPdfManager::PdfKey_t key(hypothesisName,variableName, nDimensions);
	std::map<ecalPid::TECALPidLikelihoodPdfManager::PdfKey_t, TH1*>::const_iterator it = fPdfs.find(key);
	if(it!=fPdfs.end())
	{
		result = it->second;
	}
	if(!result)
	{
		std::cerr << "TECALPidLikelihoodPdfManager unable to find pdf for hypothesis:" << hypothesisName << ", variable:"<<variableName << std::endl;
		//print out names of all pdfs
		for(it = fPdfs.begin(); it != fPdfs.end(); ++it)
		{
			std::cerr << "pdfKey=" << it->first.info() << ", hist=" << it->second << std::endl;
		}
		throw EECALLikelihoodPdfManagerUnknownPdf();
	}
	return result;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodPdfManager::LoadPdfs()
{
	bool isSuccess = 0;
	//prevent further calls to load pdfs.
	fHasLoadedPdfs = 1;
	//remember the global tfile as our shenanigans here will change it
	TFile* initialGlobalFile = gFile;
	//load pdf files
	std::string fileName = this->FindPdfFile();
	TFile pdfFile(fileName.c_str(),"read");
	if(pdfFile.IsOpen())
	{
		//load in histograms
		TList* listOfKeys = pdfFile.GetListOfKeys();
		//std::cout  << "found N_keys=" << listOfKeys->GetSize() << std::endl;
		TIter next(listOfKeys);
		TObject* keyObj = 0;
		while( (keyObj=next()) )
		{
			//std::cout << "found key=" << keyObj->GetName() << ", class=" << keyObj->ClassName() << std::endl;
			TKey* key = dynamic_cast<TKey*>(keyObj);
			//std::cout << "found object=" << key->GetName() << ", class=" << key->GetClassName() << std::endl;
			TObject* obj = key->ReadObj();
			TH1* hist = dynamic_cast<TH1*>(obj);
			if(hist)
			{
				//parse hist name to get hypothesis and variablename.
				std::string hypothesis;
				std::string variableName;
				int dimensions;
				this->ParseHistogramName(hist,hypothesis,variableName, dimensions);
				//make a copy to store in pdf manager
				hist = static_cast<TH1F*>(hist->Clone());
				hist->SetDirectory(0);//I will take ownership.
				//fill container
				ecalPid::TECALPidLikelihoodPdfManager::PdfKey_t k(hypothesis,variableName, dimensions);
				fPdfs[k] = hist;
				//found a valid histogram so it must be valid file
				isSuccess = 1;
			}
		}
		//close file
		pdfFile.Close();
	}
	//reset global variable to it's original value
	gFile = initialGlobalFile;
	//throw exception if failed
	if(!isSuccess)
	{
		std::cerr << "TECALPidLikelihoodPdfManager ECAL Likelihood PDF file failed to open." << std::endl;
		throw EECALLikelihoodPdfManagerSetupFailure();
	}
	return;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::string ecalPid::TECALPidLikelihoodPdfManager::FindPdfFile() const
{
	std::string result;
	const char* envEcalReconRoot = gSystem->Getenv("HIGHLANDUTILSROOT");
	if (!envEcalReconRoot)
	{
		std::cerr << "TECALPidLikelihoodPdfManager ND280ANALYSISTOOLSROOT environment variable is not set."
				<< " I don't know where to look for input PDF files!"
				<< std::endl;
		throw EECALLikelihoodPdfManagerSetupFailure();
	}
	std::string ecalReconRoot(envEcalReconRoot);
	std::string ecalReconConfig("AnalysisTools/ecalPidPdfs");
	std::string ecalReconDir = ecalReconRoot + "/src";// + ecalReconConfig;
	void* dirp = gSystem->OpenDirectory(ecalReconDir.c_str());
	if(dirp)
	{
		//search in this directory for the file we are after.
		//find all files matching pattern ecalRecon_likelihood_pdfs_(.*).root
		std::vector<std::string> fullPaths;
		std::vector<std::string> matches;
		//TPRegexp regExp("ecalRecon_likelihood_pdfs_prod(.*)_version(.*)\\.root");
		TPRegexp regExp("ecalRecon_likelihood_pdfs_(.*)\\.root");
		while (const char* f = gSystem->GetDirEntry(dirp)) {
			std::string fileName(f);
			if (TString(fileName).Contains(regExp))
			{
				fullPaths.push_back(ecalReconDir + "/" + fileName);
				//get matches
				TObjArray* objArray = regExp.MatchS(fileName);
				TObjString* matchValue = static_cast<TObjString*>(objArray->At(1));
				matches.push_back( std::string(matchValue->GetString().Data()) );
				delete objArray;
			}
		}
		gSystem->FreeDirectory(dirp);
// TODO : implement a method for the user to manually choose an input PDF file.
//        in ecalRecon this was dne through the parameters file. However, nd280AnalysisTools has no parameters file (that I know of).
//		//we may have found multiple files
//		//need to pick a specific one
//		//first try file specified by ecalRecon parameters file
//		std::string parPdfFile = "ecalRecon.Pid.Likelihood.PdfFile";
//		bool setInParameterFile = ecalPid::TOARuntimeParameters::Get().HasParameter(parPdfFile);
//		if (setInParameterFile)
//		{
//			std::string selectedFile = ecalPid::TOARuntimeParameters::Get().GetParameterS(parPdfFile);
//			for(unsigned int i = 0; i < matches.size(); ++i)
//			{
//				std::string match = matches.at(i);
//				if(match == selectedFile)
//				{
//					result = fullPaths.at(i);
//					break;
//				}
//			}
//		}
//		//try picking the most recent file
//		else if(fullPaths.size()>0)
		if(fullPaths.size()>0)
		{
			//naming convention should ensure that the latest version
			//is alphabetically last.
			result = *(std::max_element(fullPaths.begin(),fullPaths.end()));
		}
	}
	if(result.empty())
	{
		std::cerr << "TECALPidLikelihoodPdfManager ECAL Likelihood PDF file not available:" \
                << "  Did you run ${ND280ANALYSISTOOLSROOT}/AnalysisTools/nd280AnalysisTools-get-likelihood-pdfs.sh ?" << std::endl;
		throw EECALLikelihoodPdfManagerSetupFailure();
	}
	return result;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodPdfManager::ParseHistogramName(TH1* hist, std::string& hypothesis,std::string& variableName, int& dimensions) const
{
	//reset inputs
	hypothesis = "";
	variableName = "";
	dimensions = 0;
	size_t pos = std::string::npos;
	//parse histogram name
	std::string histName = hist->GetName();
	//strip prefix from name
	std::string prefix = "";
	pos = histName.find(prefix);
	if(pos!=std::string::npos)
	{
		histName = histName.substr(pos+prefix.size(),histName.size());
	}
	//split name up into <hypothesis>_<variable> components.
	std::vector<std::string> tokens = this->TokenizeString(histName,".");
	if(tokens.size()==3)
	{
		hypothesis = tokens.at(0);
		variableName = tokens.at(1);
		std::string dimensionsStr = tokens.at(2);
		if (dimensionsStr=="1D")
		{
			dimensions = 1;
		}
		else if (dimensionsStr=="2D")
		{
			dimensions = 2;
		}
		else if (dimensionsStr=="3D")
		{
			dimensions = 3;
		}
		//std::cout << "found hist="<< histName
		//		<< ", hypothesis=" << hypothesis
		//		<< ", variableName=" << variableName
		//		<< std::endl;
	}
	//check that output has been set correctly
	if(hypothesis.empty() || variableName.empty() || dimensions==0)
	{
		std::cerr << "TECALPidLikelihoodPdfManager ParseHistogramName failed:"
				<< "histName =" << histName << std::endl;
		throw ecalPid::EECALLikelihoodPdfManagerSetupFailure();
	}
	return;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::vector<std::string> ecalPid::TECALPidLikelihoodPdfManager::TokenizeString(const std::string& text, const std::string& sep) const {
	std::vector<std::string> tokens;
  size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}








// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ecalPid::TECALPidLikelihoodPdfManager::PdfKey_t::PdfKey_t() :
	hypothesisName(""), variableName(""), dimensions(0)
{
	//nothing to do.
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ecalPid::TECALPidLikelihoodPdfManager::PdfKey_t::PdfKey_t(const std::string& hyp, const std::string& var, int d) :
	hypothesisName(hyp), variableName(var), dimensions(d)
{
	//nothing to do.
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
bool ecalPid::TECALPidLikelihoodPdfManager::PdfKey_t::operator<(const ecalPid::TECALPidLikelihoodPdfManager::PdfKey_t& rhs) const
{
    if (this->hypothesisName != rhs.hypothesisName)
    {
        return this->hypothesisName < rhs.hypothesisName;
    }
    else if(this->variableName != rhs.variableName)
    {
        return this->variableName < rhs.variableName;
    }
    else
    {
        return this->dimensions < rhs.dimensions;
    }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::string ecalPid::TECALPidLikelihoodPdfManager::PdfKey_t::info() const
{
	std::stringstream ss;
	ss << "(" << hypothesisName << ","
			<< variableName << ","
			<< dimensions << ")";
	return ss.str();
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
