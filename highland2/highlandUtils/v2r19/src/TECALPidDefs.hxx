#ifndef ND_ND280ANALYSISTOOLS_ANALYSISTOOLS_TECALPIDDEFS_HXX
#define ND_ND280ANALYSISTOOLS_ANALYSISTOOLS_TECALPIDDEFS_HXX

#include <exception>
#include <string>
#include <map>

namespace ecalPid
{

class EECALPidDefException : public std::exception
{
  virtual const char* what() const throw()
  {
    return "EECALPidDefException";
  }
};

class EECALPidDefUnknownId : public EECALPidDefException
{
  virtual const char* what() const throw()
  {
    return "EECALPidDefUnknownId";
  }
};

class EECALPidDefUnknownName : public EECALPidDefException
{
  virtual const char* what() const throw()
  {
    return "EECALPidDefUnknownName";
  }
};

/** @brief Defines variable and hypothesis ID/names used for ECAL PID.
 *
 *  This class contains the enumeration types which are used to identify
 *  variables and hypotheses (i.e. particle type/object topology classifications)
 *  that are used in the ECAL PID. As well as the enums it also provides methods
 *  the convert between ID and string names.
 *
 *  It is implemented as a singleton which is accessed by the TECALPidDefs::Get
 *  method.
 *
 */
class TECALPidDefs
{
public:

//	/** @brief provides identifier for each PID hypothesis.
//	 *
//	 *  Provides unique identifier codes for each particle type/object
//	 *  topology classifications.
//	 *
//	 *  TODO: fill in all foreseen classifications.
//	 *
//	 */
//	enum PidAlgorithmId_t
//	{
//		kECALPidAlgorithm_Invalid=0,
//		kECALPidAlgorithm_MIP_EMShower=1,
//		kECALPidAlgorithm_HIP_EMShower=2,
//		kECALPidAlgorithm_MIP_TrackShower=2,
//	};

	/** @brief provides identifier for each PID hypothesis.
	 *
	 *  Provides unique identifier codes for each particle type/object
	 *  topology classifications.
	 *
	 *  TODO: fill in all foreseen classifications.
	 *
	 */
	enum PidHypothesisId_t
	{
		kECALPidHypothesis_Invalid=0,
		kECALPidHypothesis_Muon=1,
		kECALPidHypothesis_Electron=2,
		kECALPidHypothesis_Mip=3,
		kECALPidHypothesis_EmShower=4,
		kECALPidHypothesis_ShoweringPion=5,
		kECALPidHypothesis_Hip=5
	};

	/** @brief provides identifier for each PID variable.
	 *
	 *  Provides unique identifier codes for each ECAL PID variable.
	 *
	 */
	enum PidVariableId_t
	{
		kECALPidVar_Invalid = 					0,
		kECALPidVar_AMR =						1,
		kECALPidVar_Track_Length =				2,
		kECALPidVar_Track_Width =				3,
		kECALPidVar_NumHits =					4,
		kECALPidVar_ChargeRatio =				5,
		kECALPidVar_MeanCharge =				6,
		kECALPidVar_Max_Ratio =					7,
		kECALPidVar_TruncatedMaxRatio =			8,
		kECALPidVar_MaxWidth =					9,
		kECALPidVar_ChargeStandardDeviation =	10,
		kECALPidVar_WidthDiff =					11,
		kECALPidVar_MinWidth =					12,
		kECALPidVar_Qskew =						13,
		kECALPidVar_EMLikelihood =				14,
		kECALPidVar_TransverseChargeRatio = 	15,
		kECALPidVar_Asymmetry =					16,
		kECALPidVar_ShowerAngle =				17,
		kECALPidVar_MeanPosition =				18,
		kECALPidVar_ShowerWidth =				19,
		kECALPidVar_KSTest =					20,
		//kalman filter variables
		kECALPidVar_KFParameter =				21,
		kECALPidVar_KFNNodes =					22,
		kECALPidVar_KFParameterNodes =			23,
		kECALPidVar_KFHitQuality =				24,
		//ECAL dE/dx variables
		kECALPidVar_NormalizedMipChi2 =		25,
		kECALPidVar_NormalizedMipChi2AllLayers = 26,
		kECALPidVar_Circularity = 27,
		kECALPidVar_QRMS = 28,
		kECALPidVar_FrontBackRatio = 29,
		kECALPidVar_Angle = 30
	};

	/** @brief destructor
	 *
	 *  Does nothing as there is nothing to clean up.
	 *
	 */
	~TECALPidDefs();

	/** @brief provides access to an instance of this class.
	 *
	 */
	static const TECALPidDefs& Get();

	/** @brief converts variable ID to a string.
	 *
	 *  Raises a EECALPidDefUnknownId exception if the ID is unknown.
	 *
	 *  @param[in] variableId the variable ID.
	 *  @return the corresponding string for the variable.
	 */
	std::string GetVariableName(PidVariableId_t variableId) const;

	/** @brief converts a variable string to a variable ID.
	 *
	 * 	Raises a EECALPidDefUnknownId exception if the string is unknown.
	 *
	 *  @param[in] string for the variable.
	 *  @return the corresponding ID.
	 */
	PidVariableId_t GetVariableId(const std::string& variableName) const;

	/** @brief converts hypothesis ID to a string.
	 *
	 *  Raises a EECALPidDefUnknownId exception if the ID is unknown.
	 *
	 *  @param[in] hypothesisId the hypothesis ID.
	 *  @return the corresponding string for the hypothesis.
	 */
	std::string GetHypothesisName(PidHypothesisId_t hypothesisId) const;

	/** @brief converts a hypothesis string to a hypothesis ID.
	 *
	 * 	Raises a EECALPidDefUnknownId exception if the string is unknown.
	 *
	 *  @param[in] string for the hypothesis.
	 *  @return the corresponding ID.
	 */
	PidHypothesisId_t GetHypothesisId(const std::string& hypothesisName) const;

private:

	//we make these private to prevent the user creating more copies of
	//this singleton.
	TECALPidDefs();//private constructor
	TECALPidDefs(const TECALPidDefs&);//private copy constructor
	TECALPidDefs& operator=(const TECALPidDefs&);//private assignment operator

	/** @brief sets the variable names.
	 */
	void InitializeVariables();

	/** @brief sets the hypothesis names.
	 */
	void InitializeHypotheses();
	static TECALPidDefs* fInstance;
	std::map<PidVariableId_t,std::string> fVariableNames;
	std::map<std::string,PidVariableId_t> fVariableIds;
	std::map<PidHypothesisId_t,std::string> fHypothesisNames;
	std::map<std::string,PidHypothesisId_t> fHypothesisIds;
};

}//end ND namespace

#endif
