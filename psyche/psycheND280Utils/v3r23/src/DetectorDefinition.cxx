#include <DetectorDefinition.hxx>
#include <iomanip>

//dumping of array
#define dump_array3(par) std::cout << std::fixed    << \
  std::setw(10) << std::left   << #par   << ": \t"  << \
  std::setw(10) << std::right  << par[0] << " "     << \
  std::setw(10) << std::right  << par[1] << " "     << \
  std::setw(10) << std::right  << par[2] << std::endl;

#define dump_var(par)  std::cout << std::fixed << std::setw(20) << std::left << #par << ":\t" << \
  std::setw(10) << std::right << par << std::endl;

namespace DetDef {

  // Detector definitions
  // Can be overridden in yourAnalysis::Initialize()
  // with e.g. DetDef::fgd1min[0] = x;
  // (note that DetDef::fgd1min = {x,y,z} works only with old compilers)
  // Fiducial volume is defined by removing the edge volumes in FiducialVolumeDefinition.cxx

// new defintion of FGD volumes after definition of bar positions (more below)
//  Float_t fgd1min[3] = {-932.17,-877.17,115.95};
//  Float_t fgd1max[3] = {932.17,987.17,446.955};
//  Float_t fgd2min[3] = {-932.17,-877.17,1473.95};
//  Float_t fgd2max[3] = {932.17,987.17,1807.05};


  Float_t p0dmin[3] = {-1092.79,-1107.39,-3296.48};
  Float_t p0dmax[3] = {1012.45,1130.99,-938.753};

  //from RECPACK simplified geometry: active DsECal volume
  Float_t dsecalmin[3] = {-1019.97,-1009.97,2855.03};
  Float_t dsecalmax[3] = {1019.97,1029.97,3273.17};

  //active Barrel ECal from RECPACK simplified geomtry
  Float_t tecalLmin[3] = {1385.03,-1063.47,-663.97};
  Float_t tecalLmax[3] = {1766.27,1236.47,3215.97};
  Float_t tecalRmin[3] = {-1766.27,-1063.47,-663.97};
  Float_t tecalRmax[3] = {-1385.03,1237.47,3215.97};

  Float_t tecalTLmin[3] = {36.03,1310.53,-663.97};
  Float_t tecalTLmax[3] = {1575.97,1691.77,3215.97};
  Float_t tecalTRmin[3] = {-1575.97,1310.53,-663.97};
  Float_t tecalTRmax[3] = {-36.03,1691.77,3215.97};

  Float_t tecalBLmin[3] = {136.03,-1711.77,-663.97};
  Float_t tecalBLmax[3] = {1675.97,-1330.53,3215.97};
  Float_t tecalBRmin[3] = {-1675.03,-1711.77,-663.97};
  Float_t tecalBRmax[3] = {-136.03,-1330.53,3215.97};

  //active P0D ECal from RECPACK simplified geomtry
  Float_t pecalLmin[3] = {1570.00,  -1344.00,   -3270.00};
  Float_t pecalLmax[3] = {1657.00,  1436.00,    -890.00};
  Float_t pecalRmin[3] = {-1657.00, -1344.00,   -3270.00};
  Float_t pecalRmax[3] = {-1570.00, 1436.00,    -890.00};


  Float_t pecalTLmin[3] = {36.00,     1490.00,  -3270.00};
  Float_t pecalTLmax[3] = {1576.00,   1577.00,  -890.00};
  Float_t pecalTRmin[3] = {-1576.00,  1490.03,  -3269.97};
  Float_t pecalTRmax[3] = {-36.00,    1577.97,  -890.03};

  Float_t pecalBLmin[3] = {136.00,    -1597.00, -3270.00};
  Float_t pecalBLmax[3] = {1676.00,   -1510.00, -890.00};
  Float_t pecalBRmin[3] = {-1676.00,  -1597.00, -3270.00};
  Float_t pecalBRmax[3] = {136.00,    -1510.00, -890.00};


  //from RECPACK simplified geometry: SMRD volumes
  Float_t smrd15Lmin[3] = {1832.03,-2029.98, -3813.98};
  Float_t smrd15Lmax[3] = {2021.97,2009.98,915.47};

  Float_t smrd6Lmin[3] = {1832.03,-2029.98, 996.03};
  Float_t smrd6Lmax[3] = {2086.97,2009.98,1871.97};

  Float_t smrd78Lmin[3] = {1832.03,-2029.98, 1952.03};
  Float_t smrd78Lmax[3] = {2216.97,2009.98,3813.48};

  Float_t smrd15Rmin[3] = {-2021.97,-2029.98,-3813.98};
  Float_t smrd15Rmax[3] = {-1832.02,2009.98,915.47};

  Float_t smrd6Rmin[3] = {-2086.97,-2029.98,996.03};
  Float_t smrd6Rmax[3] = {-1832.02,2009.98,1871.97};

  Float_t smrd78Rmin[3] = {-2216.97,-2029.98,1952.03};
  Float_t smrd78Rmax[3] = {-1832.02,2009.98,3813.47};

  Float_t smrdTLmin[3] = {32.02,2010.02,-3813.98};
  Float_t smrdTLmax[3] = {1896.98,2197.48,3818.98};

  Float_t smrdTRmin[3] = {-1896.98,2010.02,-3813.98};
  Float_t smrdTRmax[3] = {-32.02,2197.48,3818.98};

  Float_t smrdBLmin[3] = {32.02,-2222.48,-3813.98};
  Float_t smrdBLmax[3] = {1896.98,-2030.02,3813.98};

  Float_t smrdBRmin[3] = {-1896.98,-2222.48,-3813.98};
  Float_t smrdBRmax[3] = {-32.02,-2030.02,3813.98};


  Float_t tpc1min[3] = {-1150.00, -1170.0, -885.0};
  Float_t tpc1max[3] = { 1150.00,  1230.0,   89.0};
  Float_t tpc2min[3] = {-1150.00, -1170.0,  474.0};
  Float_t tpc2max[3] = { 1150.00,  1230.0, 1448.0};
  Float_t tpc3min[3] = {-1150.00, -1170.0, 1833.0};
  Float_t tpc3max[3] = { 1150.00,  1230.0, 2807.0};

  Float_t trackerAllmin[3] = {-1160.00, -1256.0,  -885.0};
  Float_t trackerAllmax[3] = { 1160.00,  1230.0,  2807.0};


  // FGDs

  // center values found using oaApplyAlign/v2r1/Linux/ViewCenters.exe
  // on a prod5F neut reco file of run2, different for DATA and MC
  Float_t fgd2BarCenterMC[14] = {1478.85,1488.65,1531.1,1540.9,1583.35,1593.15,1635.6,1645.4,1687.85,1697.65,1740.1,1749.9,1792.35,1802.15};
  Float_t fgd2BarCenterDATA[14] = {1479.937,1489.737,1532.187,1541.987,1584.437,1594.237,1636.687,1646.487,1688.937,1698.737,1741.187,1750.987,1793.437,1803.237};
  Float_t fgd2WaterCenterMC[6] = {1509.84,1562.09,1614.34,1666.59,1718.84,1771.09};
  Float_t fgd2WaterCenterDATA[6] = {1510.93,1563.18,1615.43,1667.68,1719.93,1772.18};

  // This will be filled automatically depending from the geometry available so no need to split MC and Data
  Float_t fgd2WaterCenter[6]  = {1509.84, 1562.09, 1614.34, 1666.59, 1718.84, 1771.09};
  Float_t fgd2BarCenter[14]   = {1478.85, 1488.65, 1531.1, 1540.9, 1583.35, 1593.15, 1635.6, 1645.4, 1687.85, 1697.65, 1740.1, 1749.9, 1792.35, 1802.15};
  Float_t fgd1BarCenter[30] = {
    120.85,
    130.65,
    143.10,
    152.90,
    165.35,
    175.15,
    187.60,
    197.40,
    209.85,
    219.65,
    232.10,
    241.90,
    254.35,
    264.15,
    276.60,
    286.40,
    298.85,
    308.65,
    321.10,
    330.90,
    343.35,
    353.15,
    365.60,
    375.40,
    387.85,
    397.65,
    410.10,
    419.90,
    432.35,
    442.15
  };

  Float_t fgdXYModuleWidth = 20.25;
  Float_t fgdXYAirWidth = 2.0;
  Float_t fgdXYBarWidth = 9.61;
  Float_t fgdXYMiddleGlueWidth = 0.190;
  Float_t fgdXYGlueWidth = 0.188;
  Float_t fgdXYSkinWidth = 0.232;

  Float_t fgdWaterModuleWidth = 27.9379;
  Float_t fgdWaterAirWidth = 2.0621;
  Float_t fgdWaterPCWidth = 25.0632;
  Float_t fgdWaterGlueWidth = 0.6329;
  Float_t fgdWaterSkinWidth = 0.80445;

  // FGD1 = XY module + air/G10_spacer
  //        20.25     + 2.0
  // FGD2 = XY module + air/G10_spacer + water module + air/G10_spacer
  //        20.25     + 2.0            + 27.9379      + 2.0621          = 52.25 mm

  // XY module = G10_sheets + glue  + X layer + glue  + Y layer + glue  + G10_sheets
  // 20.25 mm  = 0.232      + 0.188 + 9.61    + 0.190 + 9.61    + 0.188 + 0.232

  // water module = polypropylene + glue   + water/PC(polycarbonate) + glue   + polypropylene
  // 27.9379 mm   = 0.80445       + 0.6329 + 25.0632                 + 0.6329 + 0.80445
  // Actually the more trustable value is 27.9379, the others are calulated through the averaged areal desities  (see TN 122)

  // reference
  // https://repo.nd280.org/viewvc/ND280/nd280mc/src/ND280FGDConstructor.cc?revision=1.19.2.1&view=markup
  // http://www.t2k.org/nd280/software/alignment/asbuilt/fgd-geometry-status/FGD_geometry_checks

  // We have to include also skin and glue since mass studies are done on the whole modules (TN 91, 122, 198)
  Float_t fgd1minZ = fgd1BarCenter[0] - (fgdXYBarWidth/2.) - fgdXYSkinWidth - fgdXYGlueWidth;
  Float_t fgd2minZ = fgd2BarCenter[0] - (fgdXYBarWidth/2.) - fgdXYSkinWidth - fgdXYGlueWidth;
  Float_t fgd1maxZ = fgd1BarCenter[29] + (fgdXYBarWidth/2.) + fgdXYSkinWidth + fgdXYGlueWidth;
  Float_t fgd2maxZ = fgd2BarCenter[13] + (fgdXYBarWidth/2.) + fgdXYSkinWidth + fgdXYGlueWidth;
  Float_t fgd1min[3] = {-932.17, -877.17, fgd1minZ};
  Float_t fgd2min[3] = {-932.17, -877.17, fgd2minZ};
  Float_t fgd1max[3] = {932.17, 987.17, fgd1maxZ};
  Float_t fgd2max[3] = {932.17, 987.17, fgd2maxZ};



  // active Barrel ECal from RECPACK simplified geometry
  // prod6 data with alignment applied
  Float_t tecalLmin_p6_data[3]  = {  1342.03,  -1051.47, -703.97 };
  Float_t tecalLmax_p6_data[3]  = {  1723.27,  1248.47,  3175.97 };
  Float_t tecalRmin_p6_data[3]  = {  -1751.27, -1053.47, -693.97 };
  Float_t tecalRmax_p6_data[3]  = {  -1370.03, 1247.47,  3185.97 };

  Float_t tecalTLmin_p6_data[3] = { 4.03,     1320.53,  -693.97 };
  Float_t tecalTLmax_p6_data[3] = { 1543.97,  1701.77,  3185.97 };
  Float_t tecalTRmin_p6_data[3] = { -1569.97, 1320.53,  -693.97 };
  Float_t tecalTRmax_p6_data[3] = { -30.03,   1701.77,  3185.97 };

  Float_t tecalBLmin_p6_data[3] = { 96.03,    -1695.77, -698.97 };
  Float_t tecalBLmax_p6_data[3] = { 1635.97,  -1314.53, 3180.97 };
  Float_t tecalBRmin_p6_data[3] = {-1658.03,  -1693.77, -688.97 };
  Float_t tecalBRmax_p6_data[3] = {-119.03,   -1312.53, 3190.97 };



  //**********************************
  void DumpVolumes(){
  //**********************************
    std::streamsize ss = std::cout.precision();
    std::cout.precision(3);

    std::cout << "\n***** DetectorDefinition: available volumes list: ***** \n" << std::endl;

    dump_array3(fgd1min);
    dump_array3(fgd1max);

    dump_array3(fgd2min);
    dump_array3(fgd2max);

    dump_array3(tpc1min);
    dump_array3(tpc1max);

    dump_array3(tpc2min);
    dump_array3(tpc2max);

    dump_array3(tpc3min);
    dump_array3(tpc3max);

    dump_array3(trackerAllmin);
    dump_array3(trackerAllmax);

    dump_array3(p0dmin);
    dump_array3(p0dmax);

    dump_array3(dsecalmin);
    dump_array3(dsecalmax);

    dump_array3(tecalLmin);
    dump_array3(tecalLmax);

    dump_array3(tecalRmin);
    dump_array3(tecalRmax);

    dump_array3(tecalTLmin);
    dump_array3(tecalTLmax);
    dump_array3(tecalTRmin);
    dump_array3(tecalTRmax);

    dump_array3(tecalBLmin);
    dump_array3(tecalBLmax);
    dump_array3(tecalBRmin);
    dump_array3(tecalBRmax);

    dump_array3(pecalLmin);
    dump_array3(pecalLmax);

    dump_array3(pecalRmin);
    dump_array3(pecalRmax);

    dump_array3(pecalTLmin);
    dump_array3(pecalTLmax);
    dump_array3(pecalTRmin);
    dump_array3(pecalTRmax);


    dump_array3(pecalBLmin);
    dump_array3(pecalBLmax);
    dump_array3(pecalBRmin);
    dump_array3(pecalBRmax);

    dump_array3(smrd15Lmin);
    dump_array3(smrd15Lmax);

    dump_array3(smrd6Lmin);
    dump_array3(smrd6Lmax);

    dump_array3(smrd78Lmin);
    dump_array3(smrd78Lmax);

    dump_array3(smrd15Rmin);
    dump_array3(smrd15Rmax);

    dump_array3(smrd6Rmin);
    dump_array3(smrd6Rmax);

    dump_array3(smrd78Rmin);
    dump_array3(smrd78Rmax);

    dump_array3(smrdTLmin);
    dump_array3(smrdTLmax);
    dump_array3(smrdTRmin);
    dump_array3(smrdTRmax);

    dump_array3(smrdBLmin);
    dump_array3(smrdBLmax);
    dump_array3(smrdBRmin);
    dump_array3(smrdBRmax);

    std::cout << "\n******************************************************* \n" << std::endl;

    std::cout.precision(ss);
  }

  //**********************************
  void DumpFGDInfo(){
  //**********************************
    std::streamsize ss = std::cout.precision();
    std::cout.precision(6);

    std::cout << " \n ***** DetectorDefinition: FGD Information: ***** \n" << std::endl;

    std::cout << " \n ***** FGD1 Bar Centers: ***** \n" << std::endl;

    for (int i=0; i<30; i++)
      std::cout << " FGD1 Bar Center_" << i <<":\t" << std::right << fgd1BarCenter[i] << std::endl;

    std::cout << " \n ***** \n" << std::endl;


    std::cout << " \n ***** FGD2 Bar Centers: ***** \n" << std::endl;

    for (int i=0; i<14; i++)
      std::cout << " FGD2 Bar Center_" << i <<":\t" << std::right << fgd2BarCenter[i] << std::endl;

    std::cout << " \n ***** \n" << std::endl;

    std::cout << " \n ***** FGD2 Water Centers: ***** \n" << std::endl;

    for (int i=0; i<6; i++)
      std::cout << " FGD2 Water Center_" << i <<":\t" << std::right << fgd2WaterCenter[i] << std::endl;


    std::cout << " \n ***** \n" << std::endl;

    std::cout.precision(3);

    dump_var(fgdXYModuleWidth);
    dump_var(fgdXYAirWidth);
    dump_var(fgdXYBarWidth);
    dump_var(fgdXYMiddleGlueWidth);
    dump_var(fgdXYGlueWidth);
    dump_var(fgdXYSkinWidth);
    dump_var(fgdWaterModuleWidth);
    dump_var(fgdWaterAirWidth);
    dump_var(fgdWaterPCWidth);
    dump_var(fgdWaterGlueWidth);
    dump_var(fgdWaterSkinWidth);

    std::cout << " \n ******************************************************* \n " << std::endl;

    std::cout.precision(ss);
  }

} //namespace

