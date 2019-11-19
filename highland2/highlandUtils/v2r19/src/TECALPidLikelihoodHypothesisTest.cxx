#include "TECALPidLikelihoodHypothesisTest.hxx"

#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooKeysPdf.h"
#include "RooHistPdf.h"
#include "RooDataHist.h"

#include <iostream>

//#include "TGenericClassInfo.h"

//for test code only
//#include "TFile.h"
//#include "TCanvas.h"
//#include "TH1F.h"

//using namespace ND;
//ClassImp(TECALPidLikelihoodHypothesisTest);
//ClassImp(TECALPidLikelihoodHypothesis);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// TECALPidLikelihoodHypothesis
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//ecalPid::TECALPidLikelihoodHypothesis::TECALPidLikelihoodHypothesis()
//{
//	// for root IO
//}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ecalPid::TECALPidLikelihoodHypothesis::TECALPidLikelihoodHypothesis(std::string inAlgorithmName, Hypothesis inHypothesis) :
		algorithmName(inAlgorithmName),
		hypothesis(inHypothesis)
{
	//nothing to do.
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ecalPid::TECALPidLikelihoodHypothesis::~TECALPidLikelihoodHypothesis()
{
    //std::cout << "ecalPid::TECALPidLikelihoodHypothesis::~TECALPidLikelihoodHypothesis()" << std::endl;
  for (std::map<Variable, RooRealVar*>::iterator it = m_variables.begin(); it != m_variables.end(); it++) {
    if (it->second) {
      delete it->second;
    }
  }
  for (std::map<Variable, std::vector<RooRealVar*> >::iterator it = m_dependentVariables.begin(); it != m_dependentVariables.end(); it++) {
    for (unsigned int i = 0; i < it->second.size(); i++) {
      if (it->second[i]) {
        delete it->second[i];
      }
    }
  }
  for (std::map<Variable, RooDataSet*>::iterator it = m_datasets.begin(); it != m_datasets.end(); it++) {
    if (it->second) {
      delete it->second;
    }
  }
  for (std::map<Variable, RooKeysPdf*>::iterator it = m_kernels.begin(); it != m_kernels.end(); it++) {
    if (it->second) {
      delete it->second;
    }
  }
  for (std::map<Variable, RooHistPdf*>::iterator it = m_histPdfs.begin(); it != m_histPdfs.end(); it++) {
    if (it->second) {
      delete it->second;
    }
  }
  for (std::map<Variable, RooDataHist*>::iterator it = m_dataHists.begin(); it != m_dataHists.end(); it++) {
    if (it->second) {
      delete it->second;
    }
  }
  for (std::map<Variable, TH1*>::iterator it = m_rawHists.begin(); it != m_rawHists.end(); it++) {
    if (it->second) {
      delete it->second;
    }
  }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodHypothesis::AddPdf(TH1F* hist, Variable variable)
{
	std::string variableName = this->GetVariableName(variable);
	if(hist)
	{
		hist = static_cast<TH1F*>(hist->Clone());
		this->PreventEmptyBins(hist);
		std::string moddedVariableName = algorithmName+"_"+hypothesisName + "_" + variableName;
		//std::cout << "ecalPid::TECALPidLikelihoodHypothesis::addPdf() adding pdf hist:" << hist->GetName() << ", var="<<moddedVariableName<<std::endl;
		//create pdf
		double binMin = hist->GetXaxis()->GetXmin();
		double binMax = hist->GetXaxis()->GetXmax();
		RooRealVar* var = new RooRealVar((variableName).c_str(),("observable "+variableName).c_str(),(binMin-binMax)/2., binMin, binMax);
		RooDataHist* datahist = new RooDataHist(("datahist_"+moddedVariableName).c_str(),("datahist_"+variableName).c_str(),*var, hist);
		RooHistPdf* histpdf = new RooHistPdf(("histpdf_"+moddedVariableName).c_str(),("hist pdf "+variableName).c_str(), *var, *datahist, 1);
		//store pointers
		//std::cout << "storng pointers" << std::endl;
		m_dataValues[variable] = 0.;
		m_variables[variable] = var;
		m_dataHists[variable] = datahist;
		m_histPdfs[variable] = histpdf;
		m_rawHists[variable] = hist;
		//std::cout << "done." << std::endl;
		return;
	}
	else
	{
		throw BadVariableException(variableName+" given null histogram ");
	}
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodHypothesis::AddPdf(TH2F* hist, Variable variable, Variable dependentVariable)
{
	std::string variableName = this->GetVariableName(variable);
	std::string depdendentVariableName = this->GetVariableName(dependentVariable);
	if(hist)
	{
		hist = static_cast<TH2F*>(hist->Clone());
		this->PreventEmptyBins(hist);
		std::string moddedVariableName = algorithmName+"_"+hypothesisName + "_" + variableName;
		//std::cout << "ecalPid::TECALPidLikelihoodHypothesis::addPdf() adding pdf hist:" << hist->GetName() << ", var="<<moddedVariableName<<std::endl;
		//create pdf
		double xBinMin = hist->GetXaxis()->GetXmin();
		double xBinMax = hist->GetXaxis()->GetXmax();
		double yBinMin = hist->GetYaxis()->GetXmin();
		double yBinMax = hist->GetYaxis()->GetXmax();
		RooRealVar* var = new RooRealVar((variableName).c_str(),("observable "+variableName).c_str(),(yBinMin-yBinMax)/2., yBinMin, yBinMax);
		RooRealVar* dependentVariableVar = new RooRealVar((variableName+"_"+depdendentVariableName).c_str(),(depdendentVariableName+" "+variableName).c_str(),(xBinMin-xBinMax)/2., xBinMin, xBinMax);
		RooDataHist* datahist = new RooDataHist(("datahist_"+moddedVariableName).c_str(),("datahist_"+variableName).c_str(),RooArgList(*dependentVariableVar,*var), hist);
		RooHistPdf* histpdf = new RooHistPdf(("histpdf_"+moddedVariableName).c_str(),("hist pdf "+variableName).c_str(), RooArgList(*dependentVariableVar,*var), *datahist, 1);
		//store pointers
		//std::cout << "storng pointers" << std::endl;
		m_dataValues[variable] = 0.;
		m_variables[variable] = var;
		m_dataHists[variable] = datahist;
		m_histPdfs[variable] = histpdf;
		m_rawHists[variable] = hist;
		m_dataValues[dependentVariable] = 0.;
		m_dependentVariables[dependentVariable].push_back(dependentVariableVar);
		//std::cout << "done." << std::endl;
		return;
	}
	else
	{
		throw BadVariableException("ecalPid::TECALPidLikelihoodHypothesis::addPdf given null histogram hypothesis="+this->hypothesisName+", variableName="+variableName+", dependentVariable="+depdendentVariableName);
	}
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodHypothesis::PreventEmptyBins(TH1F* hist)
{
	if(hist)
	{
		for(int i = 0; i < hist->GetNbinsX()+2; ++i)
		{
			double binContent = hist->GetBinContent(i);
			if (binContent <= 0.)
			{
				hist->SetBinContent(i,0.5);//set bin content to half of an event
			}
		}
	}
	return;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodHypothesis::PreventEmptyBins(TH2F* hist)
{
	if(hist)
	{
		for(int i = 0; i < hist->GetNbinsX()+2; ++i)
		{
			for(int j = 0; j < hist->GetNbinsY()+2; ++j)
			{
				int binNum = hist->GetBin(i,j);
				double binContent = hist->GetBinContent(binNum);
				if (binContent <= 0.)
				{
					hist->SetBinContent(binNum,0.5);//set bin content to half of an event
				}
			}
		}
	}
	return;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodHypothesis::AddPdf(TTree* tree, Variable variable, double binMin, double binMax)
{
	std::string variableName = this->GetVariableName(variable);
	std::string moddedVariableName = algorithmName+"_"+hypothesisName + "_" + variableName;
	//std::cout << "ecalPid::TECALPidLikelihoodHypothesis::addPdf() adding pdf tree:" << tree->GetName() << ", var="<<moddedVariableName<<", binMin="<<binMin<<", binMax="<< binMax << std::endl;
	//check tree contains variable
	if(tree)
	{
		if(tree->FindBranch(variableName.c_str()))
		{
			//std::cout << "found branch" << std::endl;
			//create pdf
			RooRealVar* var = new RooRealVar((variableName).c_str(),("observable "+variableName).c_str(),(binMin-binMax)/2., binMin, binMax);
			RooDataSet* dataset = new RooDataSet(("dataset_"+moddedVariableName).c_str(),("dataset_"+variableName).c_str(),tree, *var);
			RooKeysPdf* kernel = new RooKeysPdf(("kernel_"+moddedVariableName).c_str(),("kernel "+variableName).c_str(), *var, *dataset);
			//store pointers
			//std::cout << "storng pointers" << std::endl;
			m_dataValues[variable] = 0.;
			m_variables[variable] = var;
			m_datasets[variable] = dataset;
			m_kernels[variable] = kernel;
			//std::cout << "done." << std::endl;
		}
		else
		{
			throw BadVariableException("ecalPid::TECALPidLikelihoodHypothesis::addPdf : "+variableName+" not in tree "+tree->GetName());
		}
	}
	else
	{
		throw BadVariableException("ecalPid::TECALPidLikelihoodHypothesis::addPdf given null tree : hypothesis="+this->hypothesisName+", variableName="+variableName);
	}
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
double ecalPid::TECALPidLikelihoodHypothesis::GetLikelihood()
{
    double result = 1.;
    
    //set data values
    for(std::map<Variable, double>::iterator it = m_dataValues.begin(); it != m_dataValues.end(); ++it)
    {
        double value = it->second;
        //try discriminating variables
        std::map<Variable, RooRealVar*>::iterator it_var = m_variables.find(it->first);
        if(it_var != m_variables.end())
        {
        	RooRealVar* var = it_var->second;
        	var->setVal(value);
        }
        else
        {//try dependent variables
        	std::map<Variable, std::vector<RooRealVar*> >::iterator it_varMap = m_dependentVariables.find(it->first);
        	if(it_varMap != m_dependentVariables.end())
        	{
        		for(std::vector<RooRealVar*>::iterator it_depvar = it_varMap->second.begin();
        				it_depvar != it_varMap->second.end();
        				++it_depvar)
        		{
        			RooRealVar* var = *it_depvar;
        			var->setVal(value);
        		}
        	}
        }
    }
    //calculate likelihood
    for(std::map<Variable, RooKeysPdf*>::iterator it = m_kernels.begin(); it != m_kernels.end(); ++it)
    {
        RooKeysPdf* pdf = it->second;
        double value = pdf->getVal();
        //std::cout << "hypothesis = " << hypothesisName << ", variable = " << it->first << ", data = " << m_variables[it->first]->getVal() << ", likelihood = " << value << std::endl;
        result *= value;
    }
    for(std::map<Variable, RooHistPdf*>::iterator it = m_histPdfs.begin(); it != m_histPdfs.end(); ++it)
    {
        RooHistPdf* pdf = it->second;
        double value = pdf->getVal();
        //std::cout << "hypothesis = " << hypothesisName << ", variable = " << it->first << ", data = " << m_variables[it->first]->getVal() << ", likelihood = " << value << std::endl;
        result *= value;
    }
    //std::cout << "hypothesis = " << hypothesisName << ", combinedLikelihood = " << result << std::endl;
    return result;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
double ecalPid::TECALPidLikelihoodHypothesis::GetLogLikelihood(double truncateValue)
{
    double likeliHood = this->GetLikelihood();
    double minLikelihood = TMath::Exp(-2.*truncateValue);
    if( likeliHood<minLikelihood  )
    {
        likeliHood = minLikelihood;
    }
    return -2.*TMath::Log(likeliHood);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodHypothesis::SetDataValue(Variable variable, double value)
{
    std::map<Variable,double>::iterator it = m_dataValues.find(variable);
    if(it!=m_dataValues.end())
    {
        it->second = value;
    }
    else
    {
        throw BadVariableException("ecalPid::TECALPidLikelihoodHypothesis::setDataValue given bad variable. hypothesis="+this->hypothesisName+", variable="+this->GetVariableName(variable));
    }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
double ecalPid::TECALPidLikelihoodHypothesis::GetDataValue(Variable variable)
{
    double result = 0.;
    std::map<Variable,double>::iterator it = m_dataValues.find(variable);
    if(it!=m_dataValues.end())
    {
        result = it->second;
    }
    else
    {
        throw BadVariableException("ecalPid::TECALPidLikelihoodHypothesis::getDataValue hypothesis="+this->hypothesisName+", variable="+this->GetVariableName(variable));
    }
    return result;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::string ecalPid::TECALPidLikelihoodHypothesis::GetInfoString()
{
	std::stringstream info;
	info << "ecalPid::TECALPidLikelihoodHypothesis[" << hypothesisName << "," << m_variables.size() << " entries" << "]" << std::endl;
	int i = 0;
	for(std::map<Variable, double>::iterator it = m_dataValues.begin(); it != m_dataValues.end(); ++it)
	{
		info << "Values : var" << i << " = " << it->first << ", value=" << it->second << std::endl;
		++i;
	}
	i = 0;
	for(std::map<Variable, RooRealVar*>::iterator it = m_variables.begin(); it != m_variables.end(); ++it)
	{
		info << "RooRealVar : var" << i << " = " << it->first << ", RooRealVar=" << it->second->GetName() << std::endl;
		++i;
	}
	i = 0;
	for(std::map<Variable, TH1*>::iterator it = m_rawHists.begin(); it != m_rawHists.end(); ++it)
	{
		info << "RawHists : var" << i << " = " << it->first << ", pdf=" << it->second->GetName() << std::endl;
		++i;
	}
	i = 0;
	for(std::map<Variable, RooDataSet*>::iterator it = m_datasets.begin(); it != m_datasets.end(); ++it)
	{
		info << "RooDataSet : var" << i << " = " << it->first << ", pdf=" << it->second->GetName() << std::endl;
		++i;
	}
	i = 0;
	for(std::map<Variable, RooKeysPdf*>::iterator it = m_kernels.begin(); it != m_kernels.end(); ++it)
	{
		info << "RooKeysPdf : var" << i << " = " << it->first << ", pdf=" << it->second->GetName() << std::endl;
		++i;
	}
	i = 0;
	for(std::map<Variable, RooDataHist*>::iterator it = m_dataHists.begin(); it != m_dataHists.end(); ++it)
	{
		info << "RooDataHist : var" << i << " = " << it->first << ", pdf=" << it->second->GetName() << std::endl;
		++i;
	}
	i = 0;
	for(std::map<Variable, RooHistPdf*>::iterator it = m_histPdfs.begin(); it != m_histPdfs.end(); ++it)
	{
		info << "HistPdfs : var" << i << " = " << it->first << ", pdf=" << it->second->GetName() << std::endl;
		++i;
	}
	return info.str();
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::string ecalPid::TECALPidLikelihoodHypothesis::GetVariableName(Variable variable)
{
	return TECALPidDefs::Get().GetVariableName(variable);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// TECALPidLikelihoodHypothesis Tester
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//ecalPid::TECALPidLikelihoodHypothesisTest::TECALPidLikelihoodHypothesisTest()
//{
//	// for root IO
//}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ecalPid::TECALPidLikelihoodHypothesisTest::TECALPidLikelihoodHypothesisTest(std::string inName)
: name(inName)
{
	//nothing to do.
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ecalPid::TECALPidLikelihoodHypothesisTest::~TECALPidLikelihoodHypothesisTest()
{
	//delete the recently created TECALPidLikelihoodHypothesisTests
	for(std::map<Hypothesis,TECALPidLikelihoodHypothesis*>::iterator it = m_hypothesis.begin();
			it != m_hypothesis.end();
			++it
	)
	{
		TECALPidLikelihoodHypothesis* hypothesis = it->second;
		if(hypothesis)
		{
			delete hypothesis;
			it->second = 0;
		}
	}
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodHypothesisTest::AddPdf(Hypothesis hypothesis, TH1F* hist, Variable variable)
{
    std::map<Hypothesis,TECALPidLikelihoodHypothesis*>::iterator it = m_hypothesis.find(hypothesis);
    TECALPidLikelihoodHypothesis* likelihood = 0;
    if(it == m_hypothesis.end())
    {
        likelihood = new TECALPidLikelihoodHypothesis(name,hypothesis);
        m_hypothesis[hypothesis] = likelihood;
    }
    else
    {
        likelihood = it->second;
    }
    likelihood->AddPdf(hist,variable);
    return;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodHypothesisTest::AddPdf(Hypothesis hypothesis, TH2F* hist, Variable variable, Variable dependentVariable)
{
    std::map<Hypothesis,TECALPidLikelihoodHypothesis*>::iterator it = m_hypothesis.find(hypothesis);
    TECALPidLikelihoodHypothesis* likelihood = 0;
    if(it == m_hypothesis.end())
    {
        likelihood = new TECALPidLikelihoodHypothesis(name,hypothesis);
        m_hypothesis[hypothesis] = likelihood;
    }
    else
    {
        likelihood = it->second;
    }
    likelihood->AddPdf(hist,variable,dependentVariable);
    return;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
double ecalPid::TECALPidLikelihoodHypothesisTest::GetLikelihood(Hypothesis hypothesis)
{
    double result = -1.;
    std::map<Hypothesis,TECALPidLikelihoodHypothesis*>::iterator it = m_hypothesis.find(hypothesis);
    if(it != m_hypothesis.end())
    {
        result = it->second->GetLikelihood();
    }
    else
    {
        throw BadHypothesisException("TECALPidLikelihoodHypothesisTest::getLikelihood : bad hypothesis:"+hypothesis);
    }
    return result;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
double ecalPid::TECALPidLikelihoodHypothesisTest::GetLogLikelihood(Hypothesis hypothesis, double truncateValue)
{
    double result = -1.;
    std::map<Hypothesis,TECALPidLikelihoodHypothesis*>::iterator it = m_hypothesis.find(hypothesis);
    if(it != m_hypothesis.end())
    {
        result = it->second->GetLogLikelihood(truncateValue);
    }
    else
    {
        throw BadHypothesisException("TECALPidLikelihoodHypothesisTest::getLogLikelihood : bad hypothesis:"+hypothesis);
    }
    return result;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
double ecalPid::TECALPidLikelihoodHypothesisTest::GetLogLikelihoodRatio(Hypothesis hypothesis1, Hypothesis hypothesis2, double truncateValue)
{
	double llr1 = this->GetLogLikelihood(hypothesis1,truncateValue);
	double llr2 = this->GetLogLikelihood(hypothesis2,truncateValue);
	return llr1-llr2;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodHypothesisTest::SetDataValue(Variable variable, double value)
{
    for(std::map<Hypothesis,TECALPidLikelihoodHypothesis*>::iterator it = m_hypothesis.begin();
        it != m_hypothesis.end();
        ++it
        )
    {
        it->second->SetDataValue(variable,value);
    }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ecalPid::TECALPidLikelihoodHypothesisTest::AddPdf(Hypothesis hypothesis, TTree* tree, Variable variable, double binMin, double binMax)
{
    std::map<Hypothesis,TECALPidLikelihoodHypothesis*>::iterator it = m_hypothesis.find(hypothesis);
    TECALPidLikelihoodHypothesis* likelihood = 0;
    if(it == m_hypothesis.end())
    {
        likelihood = new TECALPidLikelihoodHypothesis(name,hypothesis);
        m_hypothesis[hypothesis] = likelihood;
    }
    else
    {
        likelihood = it->second;
    }
    likelihood->AddPdf(tree,variable,binMin,binMax);
    return;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::string ecalPid::TECALPidLikelihoodHypothesisTest::GetInfoString()
{
	std::stringstream info;
	info << "ecalPid::TECALPidLikelihoodHypothesisTest(" << m_hypothesis.size() << " entries)" << std::endl;
	for(std::map<Hypothesis, TECALPidLikelihoodHypothesis*>::iterator it_hyp = m_hypothesis.begin(); it_hyp != m_hypothesis.end(); ++it_hyp)
	{
		info << "(" << it_hyp->first << ", " << it_hyp->second << "," << (it_hyp->second?it_hyp->second->GetInfoString():std::string("")) << ")" << std::endl;
	}
	return info.str();
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *






// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//for testing
//int main()
//{
//
//    ecalPid::TECALPidLikelihoodHypothesisTest likelihood;
//
//    std::cout << "loading electron pdfs" << std::endl;
//    TFile* electronFile = new TFile("test_electron.root","READ");
//    TTree* electronTree = dynamic_cast<TTree*>( electronFile->Get("EcalReconPidAnalysis") );
//    likelihood.addPdf("electron",electronTree,"PID_AMR",0,301);
//    likelihood.addPdf("electron",electronTree,"PID_Max_Ratio",0,450);
//    //likelihood.addPdf("electron",electronTree,"PID_QRMS",0,450);//not in my ntuple yet.
//
//    std::cout << "loading muon pdfs" << std::endl;
//    TFile* muonFile = new TFile("test_muon.root","READ");
//    TTree* muonTree = dynamic_cast<TTree*>( muonFile->Get("EcalReconPidAnalysis") );
//    likelihood.addPdf("muon",muonTree,"PID_AMR",0,301);
//    likelihood.addPdf("muon",muonTree,"PID_Max_Ratio",0,450);
//    //likelihood.addPdf("electron",electronTree,"PID_QRMS",0,450);//not in my ntuple yet.
//
//        std::cout << "processing trees" << std::endl;
//    double PID_AMR=0,PID_Max_Ratio=0;
//    electronTree->SetBranchAddress("PID_AMR",&PID_AMR);
//    electronTree->SetBranchAddress("PID_Max_Ratio",&PID_Max_Ratio);
//
//    TCanvas* canv = new TCanvas("testLikelihood","testLikelihood",800,600);
//    TH1F* elecHist = new TH1F("elecHist","electron",100,-100.,100.);
//    TH1F* muonHist = new TH1F("muonHist","muon",100,-100.,100.);
//
//    for(int i = 0; i < electronTree->GetEntries(); ++i)
//    {
//        electronTree->GetEntry(i);
//        likelihood.setDataValue("PID_AMR", PID_AMR);
//        likelihood.setDataValue("PID_Max_Ratio", PID_Max_Ratio);
//        double el = likelihood.getLogLikelihood("electron");
//        double mul = likelihood.getLogLikelihood("muon");
//        double ratio = el - mul;
//        elecHist->Fill(ratio);
//        //std::cout << "AMR=" << PID_AMR << ", Max_Ratio=" << PID_Max_Ratio << ", el=" << el << ", mul=" << mul << ", ratio=" << ratio << std::endl;
//    }
//
//    muonTree->SetBranchAddress("PID_AMR",&PID_AMR);
//    muonTree->SetBranchAddress("PID_Max_Ratio",&PID_Max_Ratio);
//    for(int i = 0; i < muonTree->GetEntries(); ++i)
//    {
//        muonTree->GetEntry(i);
//        //std::cout << PID_AMR << " " << PID_Max_Ratio << std::endl;
//        likelihood.setDataValue("PID_AMR", PID_AMR);
//        likelihood.setDataValue("PID_Max_Ratio", PID_Max_Ratio);
//        double el = likelihood.getLogLikelihood("electron");
//        double mul = likelihood.getLogLikelihood("muon");
//        double ratio = el - mul;
//        muonHist->Fill(ratio);
//        std::cout << "AMR=" << PID_AMR << ", Max_Ratio=" << PID_Max_Ratio << ", el=" << el << ", mul=" << mul << ", ratio=" << ratio << std::endl;
//    }
//
//    muonHist->SetLineColor(kRed);
//    elecHist->SetLineColor(kBlue);
//    muonHist->Draw();
//    elecHist->Draw("SAME");
//    canv->SaveAs((std::string(canv->GetName())+".eps").c_str());
//
//    return 0;
//}
