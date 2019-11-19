#ifndef ND280_ND280ANALYSISTOOLS_ANALYSISTOOLS_TECALPIDLIKELIHOODHYPOTHESISTEST_HXX
#define ND280_ND280ANALYSISTOOLS_ANALYSISTOOLS_TECALPIDLIKELIHOODHYPOTHESISTEST_HXX

#include <map>
#include <stdexcept>
#include <string>
#include <sstream>

#include "TROOT.h"
#include "TRint.h"
#include "TObject.h"

#include "TECALPidDefs.hxx"

class TTree;
class TH1F;
class RooRealVar;
class RooDataSet;
class RooDataHist;
class RooKeysPdf;
class RooHistPdf;
class TH1;
class TH1F;
class TH2F;


namespace ecalPid
{

//forward decalrations
class TECALPidLikelihoodHypothesis;

/** @brief Calculates log likelihood ratios used by ECAL PID
 *
 * This is the main class that is used to calculate log likelihood ratios for
 * the ECAL PID. To use this class:
 * @code
 * //setup the algorithm
 * TECALPidLikelihoodHypothesisTest likelihoodCalc("TrackShowerLikelihood");
 * //for each particle type/topology classification and each PID variable you must
 * //provide a pdf. eg
 * likelihoodCalc.AddPdf(TECALPidDefs::kECALPidHypothesis_EMShower, histogram, TECALPidDefs::kECALPidVar_AMR);
 *
 * //in the event loop
 * double pidVarAMR = ... // get PID variable
 * likelihoodCalc.SetDataValue(TECALPidDefs::kECALPidVar_AMR, pidVarAMR);
 *
 * //get the result
 * double llr = likelihoodCalc.GetLogLikelihoodRatio(TECALPidDefs::kECALPidHypothesis_EMShower,TECALPidDefs::kECALPidHypothesis_HadronicShower);
 * @endcode
 *
 * It is used by wrapper classes in ECAL Recon. The idea is that this has
 * minimal dependency on any other ECAL Recon classes to allow it to be ported
 * to oaAnalysis easily. This will permit updated PID weights to be recalculated
 * with new PDFs or a different variable set.
 *
 */
class TECALPidLikelihoodHypothesisTest //: public TObject
{
public:
	//define the key types used in the std::maps
	typedef TECALPidDefs::PidVariableId_t Variable;
	typedef TECALPidDefs::PidHypothesisId_t Hypothesis;
	//TECALPidLikelihoodHypothesisTest();//for root IO

	/** @brief Create a new ECAL Pid Likelihood calculator
	 *  @param[in] name needs the name of the algorithm to make object names unique.
	 */
	TECALPidLikelihoodHypothesisTest(std::string name);

	/** @brief Destructor
	 *
	 *  Deletes the TECALPidLikelihoodHypothesis objects that were created.
	 */
	~TECALPidLikelihoodHypothesisTest();

	/** @brief add a 1D histogram pdf
	 *
	 * Adds this pdf to the TECALPidLikelihoodHypothesis object. If no object
	 * exists for this hypothesis then one is created.
	 *
	 * There is no need to normalise the histogram as this is done
	 * automatically.
	 *
	 * @param[in] hypothesis the hypothesis this pdf is for
	 * @param[in] hist the histogram from which to create a pdf.
	 * @param[in] variable corresponding variable identifier for this pdf.
	 *
	 * @see TECALPidLikelihoodHypothesis::AddPdf
	 *
	 */
    void AddPdf(Hypothesis hypothesis, TH1F* hist, Variable variable);

	/** @brief add a 2D histogram pdf
	 *
	 * Adds this pdf to the TECALPidLikelihoodHypothesis object. If no object
	 * exists for this hypothesis then one is created.
	 *
	 * There is no need to normalise the histogram as this is done
	 * automatically.
	 *
	 * @param[in] hypothesis the hypothesis this pdf is for
	 * @param[in] hist the histogram from which to create a pdf.
	 * @param[in] variable variable for the Y dimension.
	 * @param[in] dependentVariable variable for the X dimension.
	 *
	 * @see TECALPidLikelihoodHypothesis::AddPdf
	 *
	 */
    void AddPdf(Hypothesis hypothesis, TH2F* hist, Variable variable, Variable dependentVariable);

	/** @brief add a 1D kernel histogram pdf
	 *
	 * Adds this pdf to the TECALPidLikelihoodHypothesis object. If no object
	 * exists for this hypothesis then one is created.
	 *
	 * The name of the variable must be the same name as the corresponding
	 * branch in the input TTree.
	 *
	 * @param[in] hypothesis the hypothesis this pdf is for
	 * @param[in] tree the TTree containing the variable.
	 * @param[in] variable variable for the Y dimension.
	 * @param[in] binMin min range of variable values.
	 * @param[in] binMax max range of variable values.
	 *
	 * @see TECALPidLikelihoodHypothesis::AddPdf
	 *
	 */
	void AddPdf(Hypothesis hypothesis, TTree* tree, Variable variable, double binMin, double binMax);

	/** @brief returns the combined likelihood.
	 *  This should return the likelihood value which should be a value between
	 *  0 and 1. For the return value to have meaning you must have set the
	 *  data values. Raises and exception if the hypothesis doesnt exist.
	 *
	 *  @param[in] hypothesis specifies the hypothesis.
	 *  @return likelihood value
	 *
	 */
	double GetLikelihood(Hypothesis hypothesis);

	/** @brief returns the -2*log-likelihood.
	 *
	 *	Returns (-2*logLikelihood_hypothesis1) - (-2*logLikelihood_hypothesis2).
	 *
	 *  @param[in] hypothesis specifies the hypothesis.
	 *  @param[in] truncateValue specifies a limit to prevent taking the log of zero.
	 *  @return -2*log-likelihood value
	 *
	 *  @see TECALPidLikelihoodHypothesis::GetLogLikelihood
	 *
	 */
    double GetLogLikelihood(Hypothesis hypothesis, double truncateValue=1E9);

    /** @brief get the log likelihood ratio of two hypotheses.
     *  @param[in] hypothesis1 specifies hypothesis1
     *  @param[in] hypothesis2 specifies hypothesis2
     *  @param[in] truncateValue prevents taking the log of zero.
     *  @return log likelihood ratio value
     *
     *  @see TECALPidLikelihoodHypothesis::GetLogLikelihood
     */
    double GetLogLikelihoodRatio(Hypothesis hypothesis1, Hypothesis hypothesis2, double truncateValue=1E9);

    /** @brief set data value for specified variable.
     *
     *  You should call SetDataValue for each input variable before attempting
     *  to calculate likelihoods.
     *
     *  @param[in] variable variable ID
     *  @param[in] value variable value
     */
	void SetDataValue(Variable variable, double value);

	/** @brief get information about the object.
	 *
	 * 	Returns information about the object used for debugging.
	 *
	 * 	@return string containing information about the state of the object.
	 *
	 */
	std::string GetInfoString();

	typedef std::runtime_error BadHypothesisException;//TODO, implement exceptions.

private:
	std::string name;
    std::map<Hypothesis, TECALPidLikelihoodHypothesis*> m_hypothesis;

public:
    //ClassDef(TECALPidLikelihoodHypothesisTest,1);

};

/** @brief Calculates log likelihood ratios used by TECALPidLikelihoodHypothesisTest
 *
 *  This object is used by TECALPidLikelihoodHypothesisTest to calculate
 *  likelihoods. This class is not meant to be used directly by users. Instead
 *  use TECALPidLikelihoodHypothesis instead.
 *
 */
class TECALPidLikelihoodHypothesis //: public TObject
{
public:

	typedef std::runtime_error BadVariableException;//TODO, implement exceptions.
	typedef TECALPidLikelihoodHypothesisTest::Variable Variable;
	typedef TECALPidLikelihoodHypothesisTest::Hypothesis Hypothesis;

	//TECALPidLikelihoodHypothesis() { }//for root io
	/** @brief Constructor
	 *
	 *  The algorithmName is needed to ensure that all of the histograms and
	 *  Roofit objects are given unique names.
	 *
	 *  @param[in] algorithmName the parent algorithm name
	 *  @param[in] hypothesis
	 */
	TECALPidLikelihoodHypothesis(std::string algorithmName, Hypothesis hypothesis);

	/** @brief Destructor
	 *
	 *  Deletes all of the RooFit and histogram objects it has created.
	 *
	 */
	virtual ~TECALPidLikelihoodHypothesis();
	
	/** @brief Add a 1D histogram pdf.
	 *
	 * @see TECALPidLikelihoodHypothesisTest::AddPdf
	 *
	 */
    void AddPdf(TH1F* tree, Variable variable);

	/** @brief Add a 2D histogram pdf.
	 *
	 * @see TECALPidLikelihoodHypothesisTest::AddPdf
	 *
	 */
    void AddPdf(TH2F* tree, Variable variable, Variable dependentVariable);

	/** @brief Add a 1D kernal pdf.
	 *
	 * @see TECALPidLikelihoodHypothesisTest::AddPdf
	 *
	 */
	void AddPdf(TTree* tree, Variable variable, double binMin, double binMax);

	/** @brief returns the current likelihood value.
	 *
	 * 	Remember that you must set the data values first.
	 *
	 *  @return likelihood value.
	 *
	 */
	double GetLikelihood();

	/** @brief returns the -2*log-likelihood.
	 * 	This returns the -2*log-likelihood value. For the return value to
	 *  have meaning you must have set the data values
	 *  The truncateValue parameter is used to prevent taking the log of 0.
	 *  The likelihood is not allowed to go below exp(-2*truncateValue).
	 *
	 *  @return -2*log-likelihood value
	 */
	double GetLogLikelihood(double truncateValue=1E9);

	/** @brief set data values
	 * @see  TECALPidLikelihoodHypothesisTest::SetDataValue
	 */
	void SetDataValue(Variable variable, double value);

	/** @brief get the current data value
	 * @param[in] variable the id of the variable.
	 * @return current value of the variable matching the input variable ID.
	 */
    double GetDataValue(Variable variable);

	/** @brief get information about the object.
	 * 	Returns information about the object used for debugging.
	 * 	@return string containing information about the state of the object.
	 */
    std::string GetInfoString();

	/** @brief ensures that the pdf has no empty bins.
	 *
	 * 	Due to limited MC statistics (or inaccurate MC modelling) is it
	 * 	possible to have an empty bin in a histogram, but still have a non-zero
	 * 	probability of finding a data event in that bin. This will cause
	 * 	problems when trying to construct a likelihood ratio.
	 *
	 * 	We protect against this by setting the bin content of empty bins to
	 * 	half of an event.
	 *
	 * 	@param[in] hist the input histogram
	 */
    void PreventEmptyBins(TH1F* hist);
	/** @brief ensures that the pdf has no empty bins.
	 *
	 *  See the documentation for the same method for 1D histograms.
	 *
	 * 	@param[in] hist the input histogram
	 */
    void PreventEmptyBins(TH2F* hist);

private:

    /** @brief get the name of a variable.
     *  @param[in] the id of the variable in question.
     * 	@return the string name of the variable.
     */
    std::string GetVariableName(Variable);

    std::string algorithmName;
    Hypothesis hypothesis;
    std::string hypothesisName;
    std::map<Variable, double> m_dataValues;
    std::map<Variable, RooRealVar*> m_variables;
    std::map<Variable, std::vector<RooRealVar*> > m_dependentVariables;
    std::map<Variable, RooDataSet*> m_datasets;
    std::map<Variable, RooKeysPdf*> m_kernels;
    std::map<Variable, RooHistPdf*> m_histPdfs;
    std::map<Variable, RooDataHist*> m_dataHists;
    std::map<Variable, TH1*> m_rawHists;

public:
    //ClassDef(TECALPidLikelihoodHypothesis,1);

};

}


#endif
