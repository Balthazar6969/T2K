#!/usr/bin/python
"""
This module implements timing cuts from the NuMu selection.
Specifically, they are taken from Brian Kirby's analysis.
Cuts are implemented as mean +/-  (4 * width).
"""

peakTimeRdp4XXX = [2839.7,3423.5,4005.4,4588.6,5172.2,5754.6,-1000.,-1000.]
"""Real data mean times: runs 4XXX, 2010 - 6 bunches"""
peakWidthRdp4XXX = [ 15., 15., 15., 15., 15., 15., 0.0, 0.0]
"""Real data widths: runs 4XXX, 2010 - 6 bunches"""

peakTimeRdp5XXX = [2839.7,3423.5,4005.4,4588.6,5172.2,5754.6,-1000.,-1000.]
"""Real data mean times: runs 5XXX, 2010 - 6 bunches"""
peakWidthRdp5XXX = [ 15., 15., 15., 15., 15., 15., 0.0, 0.0]
"""Real data widths: runs 5XXX, 2010 - 6 bunches"""

peakTimeRdp6XXX = [2853.95,3444.15,4030.41,4620.34,5180.28,5770.12,6343.77,6924.67]
"""Real data mean times: runs 6XXX, 2011 - P0D water IN"""
peakWidthRdp6XXX = [ 15., 15., 15., 15., 15., 15., 15., 15.]
"""Real data widths: runs 6XXX, 2011 - P0D water IN"""

peakTimeRdp7XXX = [3019.11,3597.74,4180.73,4763.93,5346.49,5927.83,6508.5,7093.56]
"""Real data mean times: runs 7XXX, 2011 - P0D water IN/OUT"""
peakWidthRdp7XXX = [ 15., 15., 15., 15., 15., 15., 15., 15.] 
"""Real data widths: runs 7XXX, 2011 - P0D water IN/OUT"""

peakTimeMC = [2750.2,3332.0,3914.7,4497.0,5078.4,5659.7,6243.4,6824.2]
"""Monte Carlo mean times"""
peakWidthMC = [ 15., 15., 15., 15., 15., 15., 15., 15.]  
"""Monte Carlo widths"""

def GetWidth(isMC, run):
    """Get the 1-sigma widths for a specific MC and run combination."""
    if isMC:
        return peakWidthMC
    if 4000 <= run <= 4999:
        return peakWidthRdp4XXX
    if 5000 <= run <= 5999:
        return peakWidthRdp5XXX
    if 6000 <= run <= 6999:
        return peakWidthRdp6XXX
    if 7000 <= run <= 7999:
        return peakWidthRdp7XXX

    return []

def GetTimes(isMC, run):
    """Get the bunch times for a specific MC and run combination."""
    if isMC:
        return peakTimeMC
    if 4000 <= run <= 4999:
        return peakTimeRdp4XXX
    if 5000 <= run <= 5999:
        return peakTimeRdp5XXX
    if 6000 <= run <= 6999:
        return peakTimeRdp6XXX
    if 7000 <= run <= 7999:
        return peakTimeRdp7XXX

    return []

def PassesCut(isMC, run, objtime):
    """
    Check whether a time falls within one of the windows from the NuMu analysis.

    Keyword arguments:
    isMC    - Whether this is MC or Data.
    run     - The run number of the event.
    objtime - The time to test.
    """

    width = GetWidth(isMC, run)
    times = GetTimes(isMC, run)

    for ii, width in enumerate(width):
        time = times[ii]
        low  = time - 4. * width
        high = time + 4. * width

        if low < objtime < high:
            return True

    return False

        



