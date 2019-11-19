#!/usr/bin/python
import ROOT

volumes = { "P0D"  : { "Min" : ROOT.TVector3(-1030,   -950,    -3175),    "Max" : ROOT.TVector3(970,    850,    -1010)    },
            "FGD1" : { "Min" : ROOT.TVector3(-874.51, -819.51, 116.045),  "Max" : ROOT.TVector3(874.51, 929.51, 446.955)  },
            "FGD2" : { "Min" : ROOT.TVector3(-874.51, -819.51, 1474.045), "Max" : ROOT.TVector3(874.51, 929.51, 1806.955) },
            "DsECal":{ "Min" : ROOT.TVector3(-930,    -930,    2884),     "Max" : ROOT.TVector3(930,    930,    3249)     },
            "SMRD" : { "Min" : ROOT.TVector3(-1990,   -2170,   -2870),    "Max" : ROOT.TVector3(1990,   2170,   2870)     },
            "TPC1RP0" : { "Min" : ROOT.TVector3(-870.0, -930.0, -724.85), "Max" : ROOT.TVector3( -20.0, 1030.0,  -71.15)  },
            "TPC1RP1" : { "Min" : ROOT.TVector3(  20.0, -930.0, -724.85), "Max" : ROOT.TVector3( 870.0, 1030.0,  -71.15)  },
            "TPC2RP0" : { "Min" : ROOT.TVector3(-870.0, -930.0,  634.15), "Max" : ROOT.TVector3( -20.0, 1030.0, 1287.85)  },
            "TPC2RP1" : { "Min" : ROOT.TVector3(  20.0, -930.0,  634.15), "Max" : ROOT.TVector3( 870.0, 1030.0, 1287.85)  },
            "TPC3RP0" : { "Min" : ROOT.TVector3(-870.0, -930.0, 1993.15), "Max" : ROOT.TVector3( -20.0, 1030.0, 2646.85)  },
            "TPC3RP1" : { "Min" : ROOT.TVector3(  20.0, -930.0, 1993.15), "Max" : ROOT.TVector3( 870.0, 1030.0, 2646.85)  },
          }
"""
Fiducial volume definitions.

P0D volume definition from email from Erez
FGD volume definition from http://www.t2k.org/nd280/software/alignment/asbuilt/fgd-geometry-status/fgdfv/
DsECal volume excludes first/last 2 layers/bars
SMRD volume defined so that only the first two layers are included, and the first and last rings are excluded
"""

def IsInVolume(pos, str):
    """
    Whether a TVector3 is within a given module.

    Keyword arguments:
    pos - The TVector3 to test.
    str - The name of the volume to test. Should have corners defined in volumes.
    """

    if str not in volumes:
        print "ERROR : volume " + str + " does not have a FV defined"
        return False

    min = volumes[str]["Min"]
    max = volumes[str]["Max"]

    if (min.X() <= pos.X() <= max.X()) and (min.Y() <= pos.Y() <= max.Y()) and (min.Z() <= pos.Z() <= max.Z()):
        return True

    return False

def IsInFGD1(pos):
    """Handy function to test whether the TVector3 pos is in FGD1 FV."""
    return IsInVolume(pos, "FGD1")

def IsInFGD2(pos):
    """Handy function to test whether the TVector3 pos is in FGD2 FV."""
    return IsInVolume(pos, "FGD2")

def IsInP0D(pos):
    """Handy function to test whether the TVector3 pos is in P0D FV."""
    return IsInVolume(pos, "P0D")

def IsInDsECal(pos):
    """Handy function to test whether the TVector3 pos is in DsECal FV."""
    return IsInVolume(pos, "DsECal")

def IsInSMRD(pos):
    """Handy function to test whether the TVector3 pos is in the volume 
    enclosed by the SMRD FV (so a point in TPC1 would also return True)."""
    return IsInVolume(pos, "SMRD")

def IsContained(pos):
    """Whether the position is contianed in the ND280 volume. Must be in the
    volume defined by the outer extent of the SMRD FV, or the DsECal FV, or
    the P0D FV."""
    return IsInSMRD(pos) or IsInP0D(pos) or IsInDsECal(pos)

def IsObjectContained(obj):
    """Whether the first and last nodes of the object are contained in the
    volume defined by the outer extent of the SMRD FV, or the DsECal FV, or
    the P0D FV.

    obj must be a TGlobalReconObject."""

    nodesSaved = obj.NGlobalNodesSaved
    if nodesSaved == 0:
      return False

    startNode = obj.GlobalNodes.At(0)
    startPos = startNode.NodeState.Position
    endNode = obj.GlobalNodes.At(nodesSaved - 1)
    endPos = endNode.NodeState.Position 

    return (IsContained(endPos) and IsContained(startPos))
