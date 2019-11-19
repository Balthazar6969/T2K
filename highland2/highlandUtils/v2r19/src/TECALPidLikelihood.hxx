#ifndef ND280_ND280ANALYSISTOOLS_ANALYSISTOOLS_TECALPIDLIKELIHOOD_HXX
#define ND280_ND280ANALYSISTOOLS_ANALYSISTOOLS_TECALPIDLIKELIHOOD_HXX

#include <TObject.h>
#include "Versioning.hxx"


//ND280 includes
#include "TECALPidDefs.hxx"

class TH1F;

namespace ecalPid
{//begin ND namespace

//forward declaration
class TECALPidLikelihoodHypothesisTest;
class TECALPidLikelihoodAlgorithm;
class TECALPidLikelihoodPdfManager;

//declare exceptions
class EECALPidLikelihoodException : public std::exception
{
  virtual const char* what() const throw()
  {
    return "EECALPidLikelihoodException";
  }
};

class EECALPidLikelihoodUnknownAlgorithm : public std::exception
{
  virtual const char* what() const throw()
  {
    return "EECALPidLikelihoodUnknownAlgorithm";
  }
};

class EECALPidLikelihoodUnknownVariable : public std::exception
{
  virtual const char* what() const throw()
  {
    return "EECALPidLikelihoodUnknownVariable";
  }
};

class EECALLikelihoodPdfManagerSetupFailure : public EECALPidLikelihoodException
{
  virtual const char* what() const throw()
  {
    return "EECALLikelihoodPdfManagerSetupFailure";
  }
};

class EECALLikelihoodPdfManagerUnknownPdf : public EECALPidLikelihoodException
{
  virtual const char* what() const throw()
  {
    return "EECALLikelihoodPdfManagerUnknownPdf";
  }
};

class EECALLikelihoodAlgorithmMissingVariable : public EECALPidLikelihoodException
{
  virtual const char* what() const throw()
  {
    return "EECALLikelihoodAlgorithmMissingVariable";
  }
};

/** @brief Runs ECAL Likelihood based PID.
 *
 *  Calculates Likelihood ratios from PID variables.
 *
 *  This class reads the ecalRecon parameters file, sets up the defined
 *  algorithms and calculates the likelihood ratio values. Finally it can add
 *  the results as TRealDatums to a TReconBase object. There is a reasonable
 *  overhead to create the object (setting up pdfs, manipulating histograms
 *  etc). It is recommended to construct the object once and then use the same
 *  object for all events.
 *
 *  The parameters must contain the following settings:
 *  @arg ecalRecon.Pid.DoLikelihoodSwitch = 1 (or = 0)
 *  @arg ecalRecon.Pid.Likelihood.PdfFile = v0000.0001 (specify the pdf file to use)
 *
 *  The pdf files are stored at repo.nd280.org/ecalRecon/likelihoodPidPdfs
 *  These files should be automatically downloaded when setting up ecalRecon
 *  (see the ecalrecon-get-likelihood-pdfs script).
 *
 *  To specify an algorithm, additional options must be set in the parameters
 *  file:
 *
 *  @arg ecalRecon.Pid.Likelihood.Algorithm1.Name set the name of the algorithm.
 *  You may set as many algorithms as required (Algorithm1,Algorithm2...).
 *  However, the numberings must be sequential i.e. 1,2,3,4..N with no missing
 *  integers. When a missing number is found, the code stops looking for new
 *  parameters.
 *
 *  @arg ecalRecon.Pid.Likelihood.Algorithm1.InputVariable1 specifies the input
 *  variables to use. Note that if the pdfs for this variable cannot be found
 *  then an exception will be thrown. When adding new PID algorithms then you
 *  should test with the pdf file that you intend to use. Note that you may set
 *  any number of variables (following the numbering rules for AlgorithmN).
 *
 *  @arg ecalRecon.Pid.Likelihood.Algorithm1.Hypothesis1 and Hypothesis2 specify
 *  the hypotheses that will be used to calculate the likelihood ratio. i.e. the
 *  algorithm output will be L(hypothesis1|data) / L(hypothesis2|data).
 *
 *  @see TECALPidDefs for possible variables and hypothesis settings.
 *
 *  For example, one possible setup would be:
 *  @code
 *-  < ecalRecon.Pid.Likelihood.Algorithm1.Name = TrackShower >
 *   < ecalRecon.Pid.Likelihood.Algorithm1.InputVariable1 = AMR >
 *   < ecalRecon.Pid.Likelihood.Algorithm1.InputVariable2 = Max_Ratio >
 *   < ecalRecon.Pid.Likelihood.Algorithm1.InputVariable3 = EMLikelihood >
 *   < ecalRecon.Pid.Likelihood.Algorithm1.InputVariable4 = ShowerAngle >
 *   < ecalRecon.Pid.Likelihood.Algorithm1.InputVariable4 = ShowerWidth >
 *   < ecalRecon.Pid.Likelihood.Algorithm1.Hypothesis1 = muon >
 *   < ecalRecon.Pid.Likelihood.Algorithm1.Hypothesis2 = electron >
 *
 *   < ecalRecon.Pid.Likelihood.Algorithm2.Name = EMHadronicShower >
 *   < ecalRecon.Pid.Likelihood.Algorithm2.InputVariable1 = Asymmetry >
 *   < ecalRecon.Pid.Likelihood.Algorithm2.InputVariable2 = MeanPosition >
 *   < ecalRecon.Pid.Likelihood.Algorithm2.InputVariable3 = Qskew >
 *   < ecalRecon.Pid.Likelihood.Algorithm2.InputVariable4 = Max_Ratio >
 *   < ecalRecon.Pid.Likelihood.Algorithm2.Hypothesis1 = electron >
 *   < ecalRecon.Pid.Likelihood.Algorithm2.Hypothesis2 = pion >
 *  @endcode
 *
 *  To use this class:
 *  @code
 *  //in initialisation
 *  ecalPid::TECALPidLikelihood* likelihoodCalculator = new ecalPid::TECALPidLikelihood();
 *  likelihoodCalculator->SetupAlgorithms();//reads parameters file and sets up defined algorithms.
 *
 *  //in the event loop
 *  ND::TTrackerECALReconModule::TECALReconObject& pidVarCalc = ... //get pid var calc
 *  likelihoodCalculator->CalculateLikelihoodValues(pidVarCalc);
 *  ecalPid::THandle<TReconBase> ecalPidObject = ... //get output object
 *  likelihoodCalculator->AddLikelihoodValuesToObject(ecalPidObject);
 *  @endcode
 *
 */
class TECALPidLikelihood
{
public:

	/** @brief Constructor
	 *
	 *  No setup is done in the constructor. Remember to call SetupAlgorithms
	 *  before attempting to calculating likelihoods.
	 *
	 *  @see SetupAlgorithms
	 */
	TECALPidLikelihood();

	/** @brief Destructor
	 *
	 * Deletes the TECALPidPdfManager and all of the instances of
	 * TECALPidLikelihoodAlgorithm that were created.
	 *
	 */
	~TECALPidLikelihood();

	/** @brief Setup up the algorithms.
	 *
	 * Sets up the algorithms. At present no algorithms are hard coded and all
	 * are loaded from the run time parameters. You must call this once before
	 * attempting to calculate likelihood values.
	 * @see LoadAlgorithmsFromRunTimeParameters
	 *
	 */
	void SetupAlgorithms();

	/** @brief gets variable values and calculates likelihood values.
	 *
	 *  Calculates the likelihoods from the variables stored in the
	 *  TECALPidVarCalc. Hence, you must have called
	 *  ND::TTrackerECALReconModule::TECALReconObject::Process before calling this method.
	 *
	 *  @param[in] inputVariables the object that contains the input variables.
	 *
	 */
	void CalculateLikelihoodValues(ND::TTrackerECALReconModule::TECALReconObject& inputVariables);

	/** @brief stores the reults inside the inputObject.
	 *
	 *  Adds the calculated likelihood ratios as TRealDatums in inputObject.
	 *
	 *  @param[in] inputObject to store the results.
	 *
	 */
	//void AddLikelihoodValuesToObject(THandle<TReconBase> inputObject);

	double GetLikelihoodValue(const std::string& algorithmId);

private:

	/** @brief Read parameters file and setup algorithms.
	 *
	 *  Sets up the algorithms as defined in the parameters file.
	 *
	 */
	//void LoadAlgorithmsFromRunTimeParameters();

	/** brief Convert integer to string
	 *
	 * A convenience method to convert integers to strings.
	 *
	 */
	std::string ConvertToString(int i);

	std::map<std::string,ecalPid::TECALPidLikelihoodAlgorithm*> fListOfAlgorithms;
	std::map<std::string,double> fLikelihoodRatioResults;
	TECALPidLikelihoodPdfManager* fPdfManager;

	typedef std::map<std::string,ecalPid::TECALPidLikelihoodAlgorithm*>::iterator AlgorithmIterator;
	typedef std::map<std::string,double>::iterator ResultIterator;

};

/** @brief Runs an Likelihood PID algorithm.
 *
 * The TECALPidLikelihood class creates one TECALPidLikelihoodAlgorithm object
 * per PID algorithm that is set in the parameters file.
 *
 * This class serves as a wrapper around TECALPidLikelihoodHypothesisTest.
 * The reason for this structure is because TECALPidLikelihoodHypothesisTest is
 * designed to be as stand alone as possible, and to have no dependency on
 * oaEvent. This will make it easier to port the likelihood calculation to
 * oaAnalysis so that Likelihoods can be recalculated with a different pdf set
 * in user analysis code.
 *
 * To use this class:
 * @code
 * //initialise
 * ecalPid::TECALPidLikelihoodAlgorithm alg("algName",
 *     ecalPid::TECALPidDefs::PidHypothesisId_t::emShower,
 *     ecalPid::TECALPidDefs::PidHypothesisId_t::hadronicShower,
 *     myListOfVariables,
 *     pdfManager);
 * //to calculate the likelihood
 * ND::TTrackerECALReconModule::TECALReconObject& inputVariables = ... //get pid variable calculator
 * //you need to have calculated the variables by calling ND::TTrackerECALReconModule::TECALReconObject::Process
 * double llr = ald.CalculateLogLikelihoodRatio(inputVariables);
 * );
 * @endcode
 *
 */
class TECALPidLikelihoodAlgorithm
{
public:

	/** @brief Create a new Likelihood Pid algorithm.
	 *
	 *	@param[in] algorithmName name of the algorithm, should be unique as it will be used to name the TDatum when saving.
	 *  @param[in] hypothesis1 sets particle type/classification to calculate likelihoods.
	 *  @param[in] hypothesis2 sets particle type/classification to calculate likelihoods.
	 *  @param[in] listOfInputVariables the variables/pdfs which will be combined to form the log likelihood.
	 *  @param[in] pdfManager needed to get pdfs.
	 *
	 */
	TECALPidLikelihoodAlgorithm(const std::string& algorithmName,
			ecalPid::TECALPidDefs::PidHypothesisId_t hypothesis1,
			ecalPid::TECALPidDefs::PidHypothesisId_t hypothesis2,
						const std::vector<ecalPid::TECALPidDefs::PidVariableId_t>& listOfInputVariables,
						TECALPidLikelihoodPdfManager& pdfManager
						);

	/** @brief Destructor
	 *
	 * Cleans up the TECALPidLikelihoodHypothesisTest that this class wraps around.
	 */
	~TECALPidLikelihoodAlgorithm();

	/** @brief return log likelihood value
	 *
	 * Gets the neccessary variable values from inputVariables, fills them in
	 * the contained TECALPidLikelihoodHypothesisTest object and calculates
	 * the log likelihood ratio of hypothesis1 and hypothesis2.
	 *
	 * @param[in] inputVariables used to fetch the variable values. Hence ND::TTrackerECALReconModule::TECALReconObject::Process must have been called first to get the right answer.
	 *
	 * @return the value of the log-likelihood ratio
	 */
	double CalculateLogLikelihoodRatio(ND::TTrackerECALReconModule::TECALReconObject& inputVariables);

	/** @brief Prints current state of the object to stdout
	 *
	 * Prints information about the algorithm to stdout for debugging purposes.
	 *
	 */
	void Print();

	/** @brief returns the algorithm name.
	 *  @return the name of this algorithm..
	 */
	const std::string& GetName();
private:

	/** @brief Sets up the algorithm.
	 *
	 *  Called when the object is constructed. , gets the required pdfs from the
	 *  pdf manager, fills them into a newly created instance of
	 *  ecalPid::TECALPidLikelihoodHypothesisTest.
	 *
	 *  @param[in] pdfManager used to get the pdf histograms needed by this algorithm.
	 *
	 */
	void Configure(TECALPidLikelihoodPdfManager& pdfManager);

	/** @brief gets the current value of a variable.
	 *
	 * @param[in] variableId variable whose value you want.
	 * @param[in] inputVariables the variable calculator object that stores variable values.
	 *
	 * @return current value of variableId stored in inputVariables
	 *
	 */
	double GetVariableValue(ecalPid::TECALPidDefs::PidVariableId_t variableId, ND::TTrackerECALReconModule::TECALReconObject& inputVariables);

	std::string fAlgorithmName;
	ecalPid::TECALPidDefs::PidHypothesisId_t fHypothesis1;
	ecalPid::TECALPidDefs::PidHypothesisId_t fHypothesis2;
	std::vector<ecalPid::TECALPidDefs::PidVariableId_t> fListOfInputVariables;
	ecalPid::TECALPidLikelihoodHypothesisTest* fLikelihood;
};

/** @brief Loads in PDFs from ROOT files in ecalRecon
 *
 *  Responsible for finding and loading in histogram pdfs from ROOT files stored
 *  in ecalRecon. If it cannot find a valid file or if it is asked for a pdf
 *  that does not exist then EECALLikelihoodPdfManagerSetupFailure and
 *  EECALLikelihoodPdfManagerUnknownPdf exceptions will be thrown, respectively.
 *
 */
class TECALPidLikelihoodPdfManager
{
public:
	/** @brief Constructor
	 *
	 *  Does nothing. The class only attempts to load the pdf file on the first
	 *  call of GetPdf.
	 *
	 */
	TECALPidLikelihoodPdfManager();

	/** @brief Destructor.
	 *
	 *  Deletes all of the histograms that were loaded.
	 */
	~TECALPidLikelihoodPdfManager();

	/** @brief return the pdf histogram for a specific hypothesis+variable
	 *
	 *  Finds the pdf histogram for corresponding to hypothesis and variable.
	 *  The TECALPidLikelihoodPdfManager owns the returned histogram.
	 *  If the pdf file has not been loaded yet, then LoadPdfs is called.
	 *  A EECALLikelihoodPdfManagerUnknownPdf exception will be thrown if there
	 *  does not exist a pdf for this hypothesis and variable combination.
	 *
	 *  @param[in] hypothesis particle type/object topology requested.
	 *  @param[in] variable pid variable requested.
	 *
	 *  @return histogram.
	 */
	TH1* GetPdf(ecalPid::TECALPidDefs::PidHypothesisId_t hypothesis, ecalPid::TECALPidDefs::PidVariableId_t& variable, int dimensions);
private:

	struct PdfKey_t
	{
		PdfKey_t();
		PdfKey_t(const std::string& hypothesisName, const std::string& variableName, int dimensions);
	    bool operator<(const PdfKey_t& rhs) const;
	    std::string info() const;
		std::string hypothesisName;
		std::string variableName;
		int dimensions;
	};

	/** @brief Loads the pdf files from ecal recon
	 *
	 * Opens the file named by LoadPdfs and reads in all histograms contained
	 * within. Copies of the histograms are made and the file is immediately
	 * closed. If no histograms are found or the file did not open then a
	 * EECALLikelihoodPdfManagerSetupFailure exception is thrown.
	 *
	 */
	void LoadPdfs();

	/** @brief Finds the ROOT file that stores PID pdfs.
	 *
	 *  Looks for files in the directory ${ECALRECONROOT}/${ECALRECONCONFIG}
	 *  matching the pattern .ecalRecon_likelihood_pdfs_*.root
	 *
	 *  If the ecalRecon parameter ecalRecon.Pid.Likelihood.PdfFile is set then
	 *  it will pick this file. Otherwise it will pick the file which is last
	 *  alphabetically (which should be the most recent file).
	 *
	 *  If no files are found or the file specified in the ecalRecon parameters
	 *  file does not exist then an EECALLikelihoodPdfManagerSetupFailure
	 *  exception will be thrown.
	 *
	 *  @return full path+file name of the pdf file.
	 */
	std::string FindPdfFile() const;

	/** @brief return the pdf histogram for a specific hypothesis+variable
	 *  Looks for a histogram matching this hyptothesis and variable
	 *  combination.This object retains ownership of the returned TH1. See
	 *  the corresponding public GetPdf method.
	 *  @see ParseHistogramName for the histogram naming format required.
	 *
	 *  @param[in] hypothesisName name of the particle type/object topology.
	 *  @param[in] variableName name of the PID variable.
	 *
	 * 	@return histogram
	 */
	TH1* GetPdf(const std::string& hypothesisName, const std::string& variableName, int nDimensions);

	/** @brief parses the input histogram name to determine hypothesis and variableName.
	 *
	 *  This method determines what hypothesis and variable an input pdf
	 *  histogram corresponds to from the histogram name. It assumes the format:
	 *  @code
	 *  likelihoodCalculator_hist_pid_<hypotheis>_<variable>
	 *  @endcode
	 *  or simply
	 *  @code
	 *  <hypotheis>_<variable>.
	 *  @endcode
	 *  Note that the latter form is preffered (the prefix
	 *  "likelihoodCalculator_hist_pid_" is allowed for historical reasons).
	 *
	 *  @param[in] the histogram.
	 *  @param[out] hypothesis resulting hypothesis name.
	 *  @param[out] variable resulting variable name.
	 *
	 */
	void ParseHistogramName(TH1* hist, std::string& hypothesis,std::string& variableName, int& dimensions) const;
	std::vector<std::string> TokenizeString(const std::string& inputText, const std::string& separator) const;

	bool fHasLoadedPdfs;
	std::map<PdfKey_t, TH1*> fPdfs;
};

}//end ND namespace

#endif
