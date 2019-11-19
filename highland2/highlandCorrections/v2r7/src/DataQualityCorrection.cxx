#include "DataQualityCorrection.hxx"
#include "DataClasses.hxx"
#include "Parameters.hxx"
#include <iostream>

//********************************************************************
DataQualityCorrection::DataQualityCorrection() :  CorrectionBase() {
//********************************************************************
  _time_periods.clear();
  _new_values.clear();

  std::string base = "highlandCorrections.DataQuality.";
  int pairing = 1;

  // Read in the periods to apply the correction for
  std::string curr = GetParamString(base + "NewValue.", pairing);

  while (ND::params().HasParameter(curr)) {
    int val = ND::params().GetParameterI(curr);

    if (val != 0 && val != 1) {
      std::cerr << "Invalid NewValue set for DataQuality correction in parameters file." << std::endl;
      std::cerr << "Values must be either 0 or 1 - correct highlandIO.parameters.dat!" << std::endl;
      exit(EXIT_FAILURE);
    }

    curr = GetParamString(base + "Time.Lower.", pairing);
    int lower = ND::params().GetParameterI(curr);
    curr = GetParamString(base + "Time.Upper.", pairing);
    int upper = ND::params().GetParameterI(curr);
    std::pair<int, int> p(lower, upper);
    _time_periods.push_back(p);
    _new_values.push_back(val);
    pairing++;
    curr = GetParamString(base + "NewValue.", pairing);
  }

}

//********************************************************************
std::string DataQualityCorrection::GetParamString(std::string base, int pairing) {
//********************************************************************
  std::stringstream ss;
  ss << base << pairing;
  return ss.str();
}

//********************************************************************
void DataQualityCorrection::Apply(AnaSpillC& spillBB) {
//********************************************************************

  // Correction was applied when making the flat tree.
  //  if (ND::input().InputIsFlatTree()) return;

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

  if (spill.GetIsMC()) {
    return;
  }

  int time = static_cast<AnaEventInfo*>(spill.EventInfo)->EventTime;
  int new_value = -999;

  for (unsigned int i = 0; i < _time_periods.size(); i++) {
    if (time >= _time_periods[i].first && time <= _time_periods[i].second) {
      new_value = _new_values[i];
      break;
    }
  }

  if (new_value < 0) {
    // We shouldn't apply the correction
    return;
  }

  spill.DataQuality->GoodDaq = new_value;
}

