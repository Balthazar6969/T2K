#include "TMath.h"
#include "TNUANCEAnalysisUtils.hxx"

/*
 *DO DICH
#.  ID   Nu_i  targ   lep_f
#. Default to MC integration of RS cross-section
#. Alternative is to override this card with
#. the value 'ROMB' instead of 'STRA' (~10 times slower)
*DO RSAL
  'STRA'
*DO DIAL
  'STRA'
#. Description of resonance reactions
*DO RSCH
#.  ID  IsoPar Branch Nu_i targ   Lep_f Bary_f  N_f X_f
+------------------------------------------------------------------------+
|      1      |               Charged current quasi-elastic              |
#.  ID     Up_ckm  Dn_ckm  Nu_i     targ   lep_f   Nucl_f
    1       1        1      12      2112    11      2212           CC
    1       1        1     -12      2212   -11      2112           CC
|-------------+----------------------------------------------------------|
|      2      |               Neutral current quasi-elastic              |
    2       1        1      12      2112    12      2112           NC
    2       1        1      12      2212    12      2212           NC
    2       1        1     -12      2112   -12      2112           NC
    2       1        1     -12      2212   -12      2212           NC
|-------------+----------------------------------------------------------|
|    3-16     |              Single-pion resonant production             |
     3    1      1     12  2212    11   2212     1  211            CC
     4    2      0     12  2112    11   2212     1  111            CC
     4    2      1     12  2112    11   2212     1  111            CC
     5    3      0     12  2112    11   2112     1  211            CC
     5    3      1     12  2112    11   2112     1  211            CC
     6    2      0     12  2212    12   2212     1  111            NC
     6    2      1     12  2212    12   2212     1  111            NC
     7    3      0     12  2212    12   2112     1  211            NC
     7    3      1     12  2212    12   2112     1  211            NC
     8    4      0     12  2112    12   2112     1  111            NC
     8    4      1     12  2112    12   2112     1  111            NC
     9    5      0     12  2112    12   2212     1 -211            NC
     9    5      1     12  2112    12   2212     1 -211            NC
    10    1      1    -12  2112   -11   2112     1 -211            CC
    11    4      0    -12  2212   -11   2112     1  111            CC
    11    4      1    -12  2212   -11   2112     1  111            CC
    12    5      0    -12  2212   -11   2212     1 -211            CC
    12    5      1    -12  2212   -11   2212     1 -211            CC
    13    2      0    -12  2212   -12   2212     1  111            NC
    13    2      1    -12  2212   -12   2212     1  111            NC
    14    3      0    -12  2212   -12   2112     1  211            NC
    14    3      1    -12  2212   -12   2112     1  211            NC
    15    4      0    -12  2112   -12   2112     1  111            NC
    15    4      1    -12  2112   -12   2112     1  111            NC
    16    5      0    -12  2112   -12   2212     1 -211            NC
    16    5      1    -12  2112   -12   2212     1 -211            NC
|-------------+----------------------------------------------------------|
|    17-90    |   Additional resonant channels with other final states   |
#. Delta+pi production
    17    6      2     12  2212    11   2214     1  211             CC
    18    7      2     12  2212    11   2224     1  111             CC
    19    8      2     12  2112    11   2214     1  111             CC
    20    9      2     12  2112    11   2114     1  211             CC
    21   10      2     12  2112    11   2224     1 -211             CC
    22    8      2     12  2212    12   2214     1  111             NC
    23    9      2     12  2212    12   2114     1  211             NC
    24   10      2     12  2212    12   2224     1 -211             NC
    25   11      2     12  2112    12   2214     1 -211             NC
    26   12      2     12  2112    12   2114     1  111             NC
    27   13      2     12  2112    12   1114     1  211             NC
    28   14      2    -12  2112   -11   2114     1 -211             CC
    29   15      2    -12  2112   -11   1114     1  111             CC
    30   11      2    -12  2212   -11   2214     1 -211             CC
    31   12      2    -12  2212   -11   2114     1  111             CC
    32   13      2    -12  2212   -11   1114     1  211             CC
    33    8      2    -12  2212   -12   2214     1  111             NC
    34    9      2    -12  2212   -12   2114     1  211             NC
    35   10      2    -12  2212   -12   2224     1 -211             NC
    36   11      2    -12  2112   -12   2214     1 -211             NC
    37   12      2    -12  2112   -12   2114     1  111             NC
    38   13      2    -12  2112   -12   1114     1  211             NC
#. Rho production
    39   1       3     12  2212    11   2212     1  213             CC
    40   2       3     12  2112    11   2212     1  113             CC
    41   3       3     12  2112    11   2112     1  213             CC
    42   2       3     12  2212    12   2212     1  113             NC
    43   3       3     12  2212    12   2112     1  213             NC
    44   4       3     12  2112    12   2112     1  113             NC
    45   5       3     12  2112    12   2212     1 -213             NC
    46   1       3    -12  2112   -11   2112     1 -213             CC
    47   4       3    -12  2212   -11   2112     1  113             CC
    48   5       3    -12  2212   -11   2212     1 -213             CC
    49   2       3    -12  2212   -12   2212     1  113             NC
    50   3       3    -12  2212   -12   2112     1  213             NC
    51   4       3    -12  2112   -12   2112     1  113             NC
    52   5       3    -12  2112   -12   2212     1 -213             NC
#. Sigma+K production
    53   1       4     12  2212    11   3222     1  321             CC
    54   2       4     12  2112    11   3212     1  321             CC
    55   3       4     12  2112    11   3222     1  311             CC
    56   2       4     12  2212    12   3212     1  321             NC
    57   3       4     12  2212    12   3222     1  311             NC
    58   4       4     12  2112    12   3212     1  311             NC
    59   5       4     12  2112    12   3112     1  321             NC
    60   1       4    -12  2112   -11   3112     1  311             CC
    61   4       4    -12  2212   -11   3212     1  311             CC
    62   5       4    -12  2212   -11   3112     1  321             CC
    63   2       4    -12  2212   -12   3212     1  321             NC
    64   3       4    -12  2212   -12   3222     1  311             NC
    65   4       4    -12  2112   -12   3212     1  311             NC
    66   5       4    -12  2112   -12   3112     1  321             NC
#.Eta production
    67  16       5     12  2112    11   2212     1  221             CC
    68  16       5     12  2212    12   2212     1  221             NC
    69  16       5     12  2112    12   2112     1  221             NC
    70  16       5    -12  2212   -11   2112     1  221             CC
    71  16       5    -12  2212   -12   2212     1  221             NC
    72  16       5    -12  2112   -12   2112     1  221             NC
#.Lambda+K production
    73  16       6     12  2112    11   3122     1  321             CC
    74  16       6     12  2212    12   3122     1  321             NC
    75  16       6     12  2112    12   3122     1  311             NC
    76  16       6    -12  2212   -11   3122     1  311             CC
    77  16       6    -12  2212   -12   3122     1  321             NC
    78  16       6    -12  2112   -12   3122     1  311             NC
#.Two pion production
    79  17       7     12  2112    11   2212     2  211 -211             CC
    80  18       7     12  2112    11   2212     2  111  111             CC
    81  17       7     12  2212    12   2212     2  211 -211             NC
    82  18       7     12  2212    12   2212     2  111  111             NC
    83  17       7     12  2112    12   2112     2  211 -211             NC
    84  18       7     12  2112    12   2112     2  111  111             NC
    85  17       7    -12  2212   -11   2112     2  211 -211             CC
    86  18       7    -12  2212   -11   2112     2  111  111             CC
    87  17       7    -12  2212   -12   2212     2  211 -211             NC
    88  18       7    -12  2212   -12   2212     2  111  111             NC
    89  17       7    -12  2112   -12   2112     2  211 -211             NC
    90  18       7    -12  2112   -12   2112     2  111  111             NC
|-------------+----------------------------------------------------------|
|     91      |              Charged current deep-inelastic              |
    91    12   2212    11             CC
    91    12   2112    11             CC
    91   -12   2212   -11             CC
    91   -12   2112   -11             CC
|-------------+----------------------------------------------------------|
|     92      |              Neutral current deep-inelastic              |
    92    12   2212    12             NC
    92    12   2112    12             NC
    92   -12   2212   -12             NC
    92   -12   2112   -12             NC
|-------------+----------------------------------------------------------|
|    93-94    |                         Not used                         |
|-------------+----------------------------------------------------------|
|     95      |        Cabibbo-suppressed quasi-elastic scattering       |
   95       1        2     -12      2212   -11      3122             CC
   95       1        2     -12      2112   -11      3112             CC
   95       1        2     -12      2212   -11      3212             CC
|-------------+----------------------------------------------------------|
|     96      |   Neutral current coherent/diffractive pion production   |
|-------------+----------------------------------------------------------|
|     97      |   Charged current coherent/diffractive pion production   |
|-------------+----------------------------------------------------------|
|     98      |             Elastic scattering from electrons            |
|-------------+----------------------------------------------------------|
|     99      |           Inverse muon decay (electron target)           |
+------------------------------------------------------------------------+
*/

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::GetReactionCode(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  return std::atoi(vertex->ReactionCode.substr(strlen("NUANCE ")).c_str());
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a CC event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 1 || TMath::Abs(number) == 3 ||
      TMath::Abs(number) == 4 || TMath::Abs(number) == 5 ||
      TMath::Abs(number) == 10 || TMath::Abs(number) == 11 ||
      TMath::Abs(number) == 12 || TMath::Abs(number) == 18 ||
      TMath::Abs(number) == 19 || TMath::Abs(number) == 20 ||
      TMath::Abs(number) == 21 || TMath::Abs(number) == 28 ||
      TMath::Abs(number) == 29 || TMath::Abs(number) == 30 ||
      TMath::Abs(number) == 31 || TMath::Abs(number) == 32 ||
      TMath::Abs(number) == 40 || TMath::Abs(number) == 41 ||
      TMath::Abs(number) == 46 || TMath::Abs(number) == 47 ||
      TMath::Abs(number) == 48 || TMath::Abs(number) == 53 ||
      TMath::Abs(number) == 54 || TMath::Abs(number) == 55 ||
      TMath::Abs(number) == 60 || TMath::Abs(number) == 61 ||
      TMath::Abs(number) == 62 || TMath::Abs(number) == 67 ||
      TMath::Abs(number) == 70 || TMath::Abs(number) == 73 ||
      TMath::Abs(number) == 76 || TMath::Abs(number) == 79 ||
      TMath::Abs(number) == 80 || TMath::Abs(number) == 85 ||
      TMath::Abs(number) == 86 || TMath::Abs(number) == 91 ||
      TMath::Abs(number) == 95 || TMath::Abs(number) == 97) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a NC event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 2 || TMath::Abs(number) == 6 ||
      TMath::Abs(number) == 7 || TMath::Abs(number) == 8 ||
      TMath::Abs(number) == 9 || TMath::Abs(number) == 13 ||
      TMath::Abs(number) == 14 || TMath::Abs(number) == 15 ||
      TMath::Abs(number) == 16 || TMath::Abs(number) == 22 ||
      TMath::Abs(number) == 23 || TMath::Abs(number) == 24 ||
      TMath::Abs(number) == 25 || TMath::Abs(number) == 26 ||
      TMath::Abs(number) == 27 || TMath::Abs(number) == 33 ||
      TMath::Abs(number) == 34 || TMath::Abs(number) == 35 ||
      TMath::Abs(number) == 36 || TMath::Abs(number) == 37 ||
      TMath::Abs(number) == 38 || TMath::Abs(number) == 42 ||
      TMath::Abs(number) == 43 || TMath::Abs(number) == 44 ||
      TMath::Abs(number) == 45 || TMath::Abs(number) == 49 ||
      TMath::Abs(number) == 50 || TMath::Abs(number) == 51 ||
      TMath::Abs(number) == 52 || TMath::Abs(number) == 56 ||
      TMath::Abs(number) == 57 || TMath::Abs(number) == 58 ||
      TMath::Abs(number) == 59 || TMath::Abs(number) == 63 ||
      TMath::Abs(number) == 64 || TMath::Abs(number) == 65 ||
      TMath::Abs(number) == 66 || TMath::Abs(number) == 68 ||
      TMath::Abs(number) == 69 || TMath::Abs(number) == 71 ||
      TMath::Abs(number) == 72 || TMath::Abs(number) == 74 ||
      TMath::Abs(number) == 75 || TMath::Abs(number) == 77 ||
      TMath::Abs(number) == 78 || TMath::Abs(number) == 81 ||
      TMath::Abs(number) == 82 || TMath::Abs(number) == 83 ||
      TMath::Abs(number) == 84 || TMath::Abs(number) == 87 ||
      TMath::Abs(number) == 88 || TMath::Abs(number) == 89 ||
      TMath::Abs(number) == 90 || TMath::Abs(number) == 92) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsQELCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a QEL CC event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 1) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsELNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it an Elastic NC event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 2) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsRES(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a Resonance (CC+NC) event?
  Int_t number = this->GetReactionCode(vertex);
  if ((TMath::Abs(number) >= 3 && TMath::Abs(number) <= 90)) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsRESCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a Resonance CC event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 3 || TMath::Abs(number) == 4 ||
      TMath::Abs(number) == 5 || TMath::Abs(number) == 10 ||
      TMath::Abs(number) == 11 || TMath::Abs(number) == 12 ||
      TMath::Abs(number) == 18 || TMath::Abs(number) == 19 ||
      TMath::Abs(number) == 20 || TMath::Abs(number) == 21 ||
      TMath::Abs(number) == 28 || TMath::Abs(number) == 29 ||
      TMath::Abs(number) == 30 || TMath::Abs(number) == 31 ||
      TMath::Abs(number) == 32 || TMath::Abs(number) == 40 ||
      TMath::Abs(number) == 41 || TMath::Abs(number) == 46 ||
      TMath::Abs(number) == 47 || TMath::Abs(number) == 48 ||
      TMath::Abs(number) == 53 || TMath::Abs(number) == 54 ||
      TMath::Abs(number) == 55 || TMath::Abs(number) == 60 ||
      TMath::Abs(number) == 61 || TMath::Abs(number) == 62 ||
      TMath::Abs(number) == 67 || TMath::Abs(number) == 70 ||
      TMath::Abs(number) == 73 || TMath::Abs(number) == 76 ||
      TMath::Abs(number) == 79 || TMath::Abs(number) == 80 ||
      TMath::Abs(number) == 85 || TMath::Abs(number) == 86) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsRESNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a Resonance NC event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 6 || TMath::Abs(number) == 7 ||
      TMath::Abs(number) == 8 || TMath::Abs(number) == 9 ||
      TMath::Abs(number) == 13 || TMath::Abs(number) == 14 ||
      TMath::Abs(number) == 15 || TMath::Abs(number) == 16 ||
      TMath::Abs(number) == 22 || TMath::Abs(number) == 23 ||
      TMath::Abs(number) == 24 || TMath::Abs(number) == 25 ||
      TMath::Abs(number) == 26 || TMath::Abs(number) == 27 ||
      TMath::Abs(number) == 33 || TMath::Abs(number) == 34 ||
      TMath::Abs(number) == 35 || TMath::Abs(number) == 36 ||
      TMath::Abs(number) == 37 || TMath::Abs(number) == 38 ||
      TMath::Abs(number) == 42 || TMath::Abs(number) == 43 ||
      TMath::Abs(number) == 44 || TMath::Abs(number) == 45 ||
      TMath::Abs(number) == 49 || TMath::Abs(number) == 50 ||
      TMath::Abs(number) == 51 || TMath::Abs(number) == 52 ||
      TMath::Abs(number) == 56 || TMath::Abs(number) == 57 ||
      TMath::Abs(number) == 58 || TMath::Abs(number) == 59 ||
      TMath::Abs(number) == 63 || TMath::Abs(number) == 64 ||
      TMath::Abs(number) == 65 || TMath::Abs(number) == 66 ||
      TMath::Abs(number) == 68 || TMath::Abs(number) == 69 ||
      TMath::Abs(number) == 71 || TMath::Abs(number) == 72 ||
      TMath::Abs(number) == 74 || TMath::Abs(number) == 75 ||
      TMath::Abs(number) == 77 || TMath::Abs(number) == 78 ||
      TMath::Abs(number) == 81 || TMath::Abs(number) == 82 ||
      TMath::Abs(number) == 83 || TMath::Abs(number) == 84 ||
      TMath::Abs(number) == 87 || TMath::Abs(number) == 88 ||
      TMath::Abs(number) == 89 || TMath::Abs(number) == 90) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsDIS(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a DIS (CC+NC) event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 91 || TMath::Abs(number) == 92) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsDISCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a DIS CC event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 91) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsDISNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a DIS NC event?
  /// Note: Excludes DIS charm production -
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 92) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsCOH(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a COH (CC+NC, elastic + pi prod + ...)  event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 96 || TMath::Abs(number) == 97) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsCOHPiCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a COH CC pion production event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 96) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsCOHPiNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a COH NC pion production event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 97) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsCOHEl(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a COH elastic event?
  Int_t number = this->GetReactionCode(vertex);
  std::cerr << "TNUANCEAnalysisUtils::IsCOHEl: no reaction code for this"
            << std::endl;
  if (TMath::Abs(number) == -9999) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsAMNuGamma(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it an anomaly-mediated nu-gamma interaction?
  Int_t number = this->GetReactionCode(vertex);
  std::cerr << "TNUANCEAnalysisUtils::IsCOHEl: no reaction code for this"
            << std::endl;
  if (TMath::Abs(number) == -9999) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsNuEEL(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a (anti)neutrino - electron elastic event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 98) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsIMD(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it an inverse muon decay event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 99) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsCharmProd(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a charm production event?
  Int_t number = this->GetReactionCode(vertex);
  std::cerr << "TNUANCEAnalysisUtils::IsCOHEl: no reaction code for this"
            << std::endl;
  if (TMath::Abs(number) == -9999) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsQELCharm(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a QEL charm production event?
  Int_t number = this->GetReactionCode(vertex);
  std::cerr << "TNUANCEAnalysisUtils::IsCOHEl: no reaction code for this"
            << std::endl;
  if (TMath::Abs(number) == -9999) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::IsDISCharm(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a DIS charm production event?
  Int_t number = this->GetReactionCode(vertex);
  std::cerr << "TNUANCEAnalysisUtils::IsCOHEl: no reaction code for this"
            << std::endl;
  if (TMath::Abs(number) == -9999) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NeutrinoPdg(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the input neutrino pdg code
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::TargetPdg(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit target pdg code
  /// Note: Using PDG-2006 ion code convention 10LZZZAAAI (example: Fe56 ->
  /// 1000260560)
  ///       Free nucleon targets 1000010010(proton), 1000000010(neutron) are
  ///       automatically converted to
  ///        the more familiar codes (2212, 2112 respectivelly)

  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::TargetZ(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit target atomic number
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::TargetA(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit target mass number
  return -1;
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::HitNucleon(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Is there a hit nucleon? (may not be set, eg for COH or ve- events)
  return false;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::HitNucleonPdg(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit nucleon pdg code? (if the hit nucleon is set)
  return -1;
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::HitQuark(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Is there a hit quark? (a hit quark can only be set in DIS events)
  return false;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::HitQuarkPdg(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit quark pdg code (if the hit quark is set)
  return -1;
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::HitSeaQuark(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Is the hit quark from the sea? (if it is set at all)
  return false;
}

//____________________________________________________________________________________________________
Bool_t TNUANCEAnalysisUtils::HitValenceQuark(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Is the hit quark a valence one? (if it is set at all)
  return false;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::ResonanceId(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return produced baryon resonance id
  ///   no resonance => -1
  ///   P33_1232     =>  0
  ///   S11_1535     =>  1
  ///   D13_1520     =>  2
  ///   S11_1650     =>  3
  ///   D13_1700     =>  4
  ///   D15_1675     =>  5
  ///   S31_1620     =>  6
  ///   D33_1700     =>  7
  ///   P11_1440     =>  8
  ///   P33_1600     =>  9
  ///   P13_1720     => 10
  ///   F15_1680     => 11
  ///   P31_1910     => 12
  ///   P33_1920     => 13
  ///   F35_1905     => 14
  ///   F37_1950     => 15
  ///   P11_1710     => 16
  ///   F17_1970     => 17

  return -1;
}

//____________________________________________________________________________________________________
std::string TNUANCEAnalysisUtils::ResonanceName(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  // Int_t resid = this->ResonanceId(vertex);

  /* switch(resid) {
    case ( -1 ) :  return "no_resonance"; break;
    case (  0 ) :  return "P33_1232";     break;
    case (  1 ) :  return "S11_1535";     break;
    case (  2 ) :  return "D13_1520";     break;
    case (  3 ) :  return "S11_1650";     break;
    case (  4 ) :  return "D13_1700";     break;
    case (  5 ) :  return "D15_1675";     break;
    case (  6 ) :  return "S31_1620";     break;
    case (  7 ) :  return "D33_1700";     break;
    case (  8 ) :  return "P11_1440";     break;
    case (  9 ) :  return "P33_1600";     break;
    case ( 10 ) :  return "P13_1720";     break;
    case ( 11 ) :  return "F15_1680";     break;
    case ( 12 ) :  return "P31_1910";     break;
    case ( 13 ) :  return "P33_1920";     break;
    case ( 14 ) :  return "F35_1905";     break;
    case ( 15 ) :  return "F37_1950";     break;
    case ( 16 ) :  return "P11_1710";     break;
    case ( 17 ) :  return "F17_1970";     break;
    default     :  return "no_resonance"; break;
  } */
  return "??";
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NProton_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of protons in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NNeutron_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of neutrons in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NPi0_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi0 in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NPip_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi+ in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NPim_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi- in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NK0_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K0+\bar{K0} in primary hadronic shower (before intranuclear
  /// hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NKp_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K+ in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NKm_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K- in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NProton_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of protons in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NNeutron_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of neutrons in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NPi0_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi0 in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NPip_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi+ in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NPim_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi- in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NK0_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K0+\bar{K0} in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NKp_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K+ in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNUANCEAnalysisUtils::NKm_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K- in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Double_t TNUANCEAnalysisUtils::Kinematics_x(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the Bjorken x kinematical variablefor the current event
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TNUANCEAnalysisUtils::Kinematics_v(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the energy transfer to the target for the current event (GeV)
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TNUANCEAnalysisUtils::Kinematics_y(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the inelasticity y for the current event
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TNUANCEAnalysisUtils::Kinematics_t(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the energy transfer to the nucleus for COHPi events
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TNUANCEAnalysisUtils::Kinematics_W(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the hadronic invariant mass W for the current event (in GeV)
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TNUANCEAnalysisUtils::Kinematics_Q2(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the momentum transfer Q2 (>0) for the current event (in GeV^2)
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TNUANCEAnalysisUtils::Kinematics_q2(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Return the momentum transfer q2 (<0) for the current event (in GeV^2)
  return -1 * this->Kinematics_Q2(vertex);
}
