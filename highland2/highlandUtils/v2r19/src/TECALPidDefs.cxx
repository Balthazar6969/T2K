#include "TECALPidDefs.hxx"
#include <iostream>

ecalPid::TECALPidDefs* ecalPid::TECALPidDefs::fInstance = 0;

ecalPid::TECALPidDefs::TECALPidDefs()
{
	InitializeVariables();
	InitializeHypotheses();
}

ecalPid::TECALPidDefs::~TECALPidDefs()
{
	//nothing to delete.
}

const ecalPid::TECALPidDefs& ecalPid::TECALPidDefs::Get()
{
	if(!fInstance)
	{
		fInstance = new ecalPid::TECALPidDefs();
	}
	return *fInstance;
}

std::string ecalPid::TECALPidDefs::GetVariableName(ecalPid::TECALPidDefs::PidVariableId_t variableId) const
{
	std::map<PidVariableId_t,std::string>::const_iterator it = fVariableNames.find(variableId);
	if(it==fVariableNames.end())
	{
		std::cerr << "TECALPidDefs::GetVariableName unknown variableId :" << variableId << std::endl;
		throw EECALPidDefUnknownId();
	}
	return it->second;
}

ecalPid::TECALPidDefs::PidVariableId_t ecalPid::TECALPidDefs::GetVariableId(const std::string& variableName) const
{
	std::map<std::string,PidVariableId_t>::const_iterator it = fVariableIds.find(variableName);
	if(it==fVariableIds.end())
	{
		std::cerr << "TECALPidDefs asked for variable ID for unknown variable:" << variableName << std::endl;
		throw EECALPidDefUnknownName();
	}
	return it->second;
}

std::string ecalPid::TECALPidDefs::GetHypothesisName(ecalPid::TECALPidDefs::PidHypothesisId_t hypothesisId) const
{
	std::map<PidHypothesisId_t,std::string>::const_iterator it = fHypothesisNames.find(hypothesisId);
	if(it==fHypothesisNames.end())
	{
		std::cerr << "TECALPidDefs::GetHypothesisName unknown hypothesisId :" << hypothesisId << std::endl;
		throw EECALPidDefUnknownId();
	}
	return it->second;
}

ecalPid::TECALPidDefs::PidHypothesisId_t ecalPid::TECALPidDefs::GetHypothesisId(const std::string& hypothesisName) const
{
	std::map<std::string,PidHypothesisId_t>::const_iterator it = fHypothesisIds.find(hypothesisName);
	if(it==fHypothesisIds.end())
	{
		std::cerr << "TECALPidDefs asked for hypothesis ID for unknown hypothesis:\""<<hypothesisName<<"\"" << std::endl;
		throw EECALPidDefUnknownName();
	}
	return it->second;
}

void ecalPid::TECALPidDefs::InitializeVariables()
{
	//fill the names
	fVariableNames[ kECALPidVar_Invalid ] = "Invalid";
	fVariableNames[ kECALPidVar_AMR ] = "AMR";
	fVariableNames[ kECALPidVar_Track_Length ] = "Track_Length";
	fVariableNames[ kECALPidVar_Track_Width ] = "Track_Width";
	fVariableNames[ kECALPidVar_NumHits ] = "NumHits";
	fVariableNames[ kECALPidVar_ChargeRatio ] = "ChargeRatio";
	fVariableNames[ kECALPidVar_MeanCharge ] = "MeanCharge";
	fVariableNames[ kECALPidVar_Max_Ratio ] = "Max_Ratio";
	fVariableNames[ kECALPidVar_TruncatedMaxRatio ] = "TruncatedMaxRatio";
	fVariableNames[ kECALPidVar_MaxWidth ] = "MaxWidth";
	fVariableNames[ kECALPidVar_ChargeStandardDeviation ] = "ChargeStandardDeviation";
	fVariableNames[ kECALPidVar_WidthDiff ] = "WidthDiff";
	fVariableNames[ kECALPidVar_MinWidth ] = "MinWidth";
	fVariableNames[ kECALPidVar_Qskew ] = "Qskew";
	fVariableNames[ kECALPidVar_EMLikelihood ] = "EMLikelihood";
	fVariableNames[ kECALPidVar_TransverseChargeRatio ] = "TransverseChargeRatio";
	fVariableNames[ kECALPidVar_Asymmetry ] = "Asymmetry";
	fVariableNames[ kECALPidVar_ShowerAngle ] = "ShowerAngle";
	fVariableNames[ kECALPidVar_MeanPosition ] = "MeanPosition";
	fVariableNames[ kECALPidVar_ShowerWidth ] = "ShowerWidth";
	fVariableNames[ kECALPidVar_KSTest ] = "KSTest";
	//Kalman Filter variables
	fVariableNames[ kECALPidVar_KFParameter ] = "KFParameter";
	fVariableNames[ kECALPidVar_KFNNodes ] = "KFNNodes";
	fVariableNames[ kECALPidVar_KFParameterNodes ] = "KFParameterNodes";
	fVariableNames[ kECALPidVar_KFHitQuality ] = "KFHitQuality";
	fVariableNames[ kECALPidVar_NormalizedMipChi2 ] = "NormalizedMipChi2";
	fVariableNames[ kECALPidVar_NormalizedMipChi2AllLayers ] = "NormalizedMipChi2AllLayers";

	fVariableNames[ kECALPidVar_Circularity ] = "Circularity";
	fVariableNames[ kECALPidVar_QRMS] = "QRMS";
	fVariableNames[ kECALPidVar_FrontBackRatio] = "FrontBackRatio";
	fVariableNames[ kECALPidVar_Angle ] = "Angle";

	//fill the inverted map
	for(std::map<PidVariableId_t,std::string>::iterator it = fVariableNames.begin();
			it != fVariableNames.end();
			++it)
	{
		fVariableIds[ it->second ] = it->first;
	}
	return;
}

void ecalPid::TECALPidDefs::InitializeHypotheses()
{
	//fill the names
	fHypothesisNames[ kECALPidHypothesis_Invalid ] = "invalid";
	fHypothesisNames[ kECALPidHypothesis_Muon ] = "muon";
	fHypothesisNames[ kECALPidHypothesis_Electron ] = "electron";

	fHypothesisNames[ kECALPidHypothesis_Mip ] = "MIP";
	fHypothesisNames[ kECALPidHypothesis_EmShower ] = "EMShower";
	fHypothesisNames[ kECALPidHypothesis_ShoweringPion ] = "ShoweringPion";
	fHypothesisNames[ kECALPidHypothesis_Hip ] = "HIP";

	//fill the inverted map
	for(std::map<PidHypothesisId_t,std::string>::iterator it = fHypothesisNames.begin();
			it != fHypothesisNames.end();
			++it)
	{
		fHypothesisIds[ it->second ] = it->first;
	}
	return;
}


