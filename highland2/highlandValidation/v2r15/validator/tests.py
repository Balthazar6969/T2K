"""Write test functions here (e.g. compare the number of entries in
the default tree).

These are passed to the tester, which generates tests for each
nominal/test version pair found in the output storage directory and
runs each function passed.
"""
import os
import ROOT as rt
import validation_utils
import config
import math
from tianlu import utils, rootutils
from array import array
from collections import namedtuple, defaultdict

__author__ = 'Tianlu Yuan'
__email__ = 'tianlu.yuan [at] colorado.edu'


Br = namedtuple('Br', 'name binning cut')

# branches common to 'default' and 'truth' trees
BRANCHES = [Br('topology',            array('d', range(11)),''),
            Br('mectopology',         array('d', range(11)),''),
            Br('reaction',            array('d', range(11)),''),
            Br('reactionCC',          array('d', range(11)),''),
            Br('reactionnofv',        array('d', range(11)),''),
            Br('particle',            array('d', range(-1000, 3000)),''),
            Br('accum_level',         array('d', range(20)),''),
            Br('nu_trueE',            array('d', utils.frange( 0,    1e4,   100)),''),
            Br('truelepton_mom',      array('d', utils.frange( 0,    1e4,   100)),''),
#            Br('truelepton_costheta', array('d', utils.frange(-1,    1.,    0.01)),''),   In nue is called differently
            Br('TruthVertexID',       array('d', range(50000)),''),
            Br('RooVtxEntry',         array('d', range(1000)),'')]
if config.PROCESS_CODE != 2:
    BRANCHES.append(Br('RooVtxIndex',         array('d', range(50)),''))

# branches in the default tree
DEFAULT_BRANCHES = BRANCHES + [Br('selmu_mom',                array('d', utils.frange(0, 1e4, 100)),''),
                               Br('acos(selmu_costheta)',     array('d', utils.frange(0, math.pi, 0.01)),''),
                               Br('selmu_mom-selmu_truemom',  array('d', utils.frange(-1e3, 1e3,50)),''),
                               Br('target',                   array('d', range(100)),'')]


# branches in the default tree for nue and gamma analyses
DEFAULT_NUE_BRANCHES = BRANCHES + [Br('selelec_mom',                   array('d', utils.frange(0, 1e4, 100)),''),
                                   Br('acos(selelec_costheta)',        array('d', utils.frange(0, math.pi, 0.01)),''),
                                   Br('selelec_mom-selelec_true_mom',  array('d', utils.frange(-1e3, 1e3,50)),''),
                                   Br('TargetPDG',                     array('d', range(100)),'')]


# branches in data
# data files don't need truth comparisons
DATA_BRANCHES = [Br('selmu_mom',            array('d', utils.frange(0, 1e4, 100)),''),
                 Br('acos(selmu_costheta)', array('d', utils.frange(0, math.pi, 0.01)),''),
                 Br('accum_level',          array('d', range(20)),''),]


DATA_NUE_BRANCHES = [Br('selelec_mom',            array('d', utils.frange(0, 1e4, 100)),''),
                     Br('acos(selelec_costheta)', array('d', utils.frange(0, math.pi, 0.01)),''),
                     Br('accum_level',            array('d', range(20)),''),]


# additional variables to check for P0D analyses
P0D_BRANCHES = BRANCHES + [Br('truep_mom',       array('d', utils.frange(0, 5e3, 100)),''),
                           Br('truelepton_momt', array('d', utils.frange(0, 1e3, 100)),''),
                           Br('truep_momt',      array('d', utils.frange(0, 1e3, 100)),''),
                           Br('true_dphi',       array('d', utils.frange(0, 180, 1)),'')]


P0D_DEFAULT_BRANCHES = P0D_BRANCHES + [Br('nmichel',      array('d', range(11)),''),
                                       Br('np0d',      array('d', range(11)),''),
                                       Br('sel_dphi',      array('d', utils.frange(0, 180, 1)),''),]

P0D_DATA_BRANCHES = DATA_BRANCHES + [Br('nmichel',      array('d', range(11)),''),
                                     Br('np0d',      array('d', range(11)),''),
                                     Br('sel_dphi',      array('d', utils.frange(0, 180, 1)),''),]

# branches in the all_syst tree
SYST_BRANCHES = [Br('selmu_mom',            array('d', utils.frange(0, 1e4, 100)),''),
                 Br('acos(selmu_costheta)', array('d', utils.frange(0, math.pi, 0.01)),''),
                 Br('accum_level',          array('d', range(20)),''),
                 Br('TruthVertexID',        array('d', range(50000)),''),
                 Br('RooVtxEntry',          array('d', range(1000)),'')]
if config.PROCESS_CODE != 2:
    SYST_BRANCHES.append(Br('RooVtxIndex',         array('d', range(50)),''))

# branches in the all_syst tree
ALLSYST_BRANCHES = SYST_BRANCHES + [Br('weight_syst[][0]',  array('d', utils.frange(0.5,  1.5,  0.005)), 'weight_syst[][0] !=1'),
                                    Br('weight_syst[][1]',  array('d', utils.frange(0.99, 1.01, 0.0001)),'weight_syst[][1] !=1'),
                                    Br('weight_syst[][2]',  array('d', utils.frange(0.8,  1.5,  0.002)), 'weight_syst[][2] !=1'),
                                    Br('weight_syst[][3]',  array('d', utils.frange(0.8,  1.2,  0.0005)),'weight_syst[][3] !=1'),
                                    Br('weight_syst[][4]',  array('d', utils.frange(0,    2.,   0.01)),  'weight_syst[][4] !=1'),
                                    Br('weight_syst[][5]',  array('d', utils.frange(0,    2.,   0.01)),  'weight_syst[][5] !=1'),
                                    Br('weight_syst[][6]',  array('d', utils.frange(0.8,  1.2,  0.0005)),'weight_syst[][6] !=1'),
                                    Br('weight_syst[][7]',  array('d', utils.frange(0.98, 1.02, 0.0002)),'weight_syst[][7] !=1'),
                                    Br('weight_syst[][8]',  array('d', utils.frange(0.9,  1.1,  0.001)), 'weight_syst[][8] !=1'),
                                    Br('weight_syst[][9]',  array('d', utils.frange(0,    2.,   0.01)),  'weight_syst[][9] !=1'),
                                    Br('weight_syst[][10]', array('d', utils.frange(0,    2.5,  0.01)),  'weight_syst[][10]!=1'),
                                    Br('weight_syst[][11]', array('d', utils.frange(0.5,  1.5,  0.005)), 'weight_syst[][11] !=1')]

# branches in flattree
FLATTREE_DATA_BRANCHES = [Br('sEvt',                    array('d', utils.frange(0., 1e4, 1.)),''),
                          Br('NTracks',                 array('d', range(50)),'')]


# branches in flattree
FLATTREE_BRANCHES = [Br('sNTotalTrueTracks',         array('d', utils.frange(0, 500, 10)),''),
                     Br('sNTotalTrueVertices',       array('d', utils.frange(0, 50, 1)),''),
                     Br('sEvt',                      array('d', utils.frange(2.5e6, 4e6, 1e3)),''),
                     Br('sNTrueVertices',            array('d', range(25)),''),
                     Br('sTrueVertexLeptonDir',      array('d', utils.frange(-1.2, 1.2, 0.01)),''),
                     Br('sTrueVertexLeptonMom',      array('d', utils.frange(0, 1e4, 100)),''),
                     Br('sTrueVertexLeptonPDG',      array('d', range(-15, 15)),''),
                     Br('sTrueVertexNReconTracks',   array('d', range(50)),''),
                     Br('sTrueVertexNReconVertices', array('d', range(10)),''),
                     Br('sNTrueTracks',              array('d', range(500)),''),
                     Br('sTrueTrackNReconTracks',    array('d', range(10)), ''),
                     Br('NTracks',                   array('d', range(50)), '')]


# branches in minitree
MINITREE_BRANCHES = [Br('AnaSpillB.NTotalTrueParticles', array('d', utils.frange(0, 500, 10)),''),
                     Br('AnaSpillB.NTotalTrueVertices', array('d', utils.frange(0, 50, 1)),'')]


# branches in RooTrackerVtx tree
ROOVTX_BRANCHES = [Br('NVtx',                     array('d', range(30)),'')]


# variation systematic trees
VARIATION_SYSTEMATICS = ['bfield',
                         'momscale',
                         'momresol',
                         'tpcpid',
                         'fgdpid']


# simple test functions
@validation_utils.info
def compare_pot(nom, test):
    def getpotgoodbeamgoodnd280(potinfo):
        """ Accessor function to get the actual pot value from potinfo
        """
        return potinfo.GetPOTGoodBeamGoodND280()

    # outputs from older versions of highland2 analyses stored the POT
    # info in a flattened float format. Current version in POTInfo container
    try:
        nom_pot = rootutils.sum_branch(nom.rootfile.header,
                                       'POT_GoodBeamGoodND280')
    except ReferenceError:
        nom_pot = rootutils.sum_branch(nom.rootfile.header,
                                       'POTInfo',
                                       rt.Header,
                                       getpotgoodbeamgoodnd280)

    test_pot = rootutils.sum_branch(test.rootfile.header,
                                    'POTInfo',
                                    rt.Header,
                                    getpotgoodbeamgoodnd280)

    print '{0}-{1} \t Good PoT: {2}'.format(nom.input_type,
                                            nom.version,
                                            nom_pot)
    print '{0}-{1} \t Good PoT: {2}'.format(test.input_type,
                                            test.version,
                                            test_pot)

    # allow 1 per mil difference to account for numerical precision
    # (POT counting uses floats)
    assert utils.almost_equal_relative_and_abs(nom_pot, test_pot,
                                               max_rel_diff=0.001)


@validation_utils.info
def compare_drawing_tools_pot(nom, test):
    nom_pot = nom.tool.GetGoodPOT(nom.sample)
    test_pot = test.tool.GetGoodPOT(test.sample)

    print '{0}-{1} \t Good PoT: {2}'.format(nom.input_type,
                                            nom.version,
                                            nom_pot)
    print '{0}-{1} \t Good PoT: {2}'.format(test.input_type,
                                            test.version,
                                            test_pot)

    assert utils.almost_equal_relative_and_abs(nom_pot,
                                               test_pot,
                                               max_rel_diff=0.001)


@validation_utils.info
def compare_drawing_tools_spills(nom, test):
    nom_spill = nom.tool.GetGoodSpills(nom.sample)
    test_spill = test.tool.GetGoodSpills(test.sample)

    print '{0}-{1} \t Good Spills: {2}'.format(nom.input_type,
                                              nom.version,
                                              nom_spill)
    print '{0}-{1} \t Good Spills: {2}'.format(test.input_type,
                                              test.version,
                                              test_spill)

    assert nom_spill == test_spill


# Functions that create test functions. This is necessary because the
# test functions can take only two parameters: nom and test, so if we
# want to pass other arguments (e.g. tree, cut...) we must build them
# individually using a wrapper function.
def create_entries_comp(tree, cut):
    """ Returns function to test the number of entries with the applied cut
    """
    @validation_utils.info
    def compare_entries(nom, test):
        print 'tree:', tree
        print 'cut:', cut

        nom_tree  = nom.rootfile.Get(tree)
        test_tree = test.rootfile.Get(tree)
        validation_utils.entries_compare(nom_tree, test_tree, cut)

    return compare_entries


def create_histograms_comp(tree, branch, cut, use_draw_tool=False):
    """ Returns a function to test the branch variable with the applied cut
    """
    @validation_utils.info
    def compare_histogram(nom, test):

        cut2=cut
        if branch.cut!='':
            cut2 = cut +' && ' + branch.cut

        print 'tree:', tree
        print 'branch:', branch.name
        print 'cut:', cut2
        print 'use drawing tool:', use_draw_tool

        if config.PROCESS_CODE == 0:
            nom_leg = nom.version
            test_leg = test.version

        else:
            nom_leg = nom.input_type
            test_leg = test.input_type

        dt_str = 'drawtool' if use_draw_tool else 'ttreedraw'
        title = '{0}, {1}, {2} w/cut: {3}, {4}'.format(nom.name,
                                                       nom.input_file,
                                                       tree,
                                                       cut2,
                                                       dt_str)
        plots_file = os.path.join(config.PLOTS_DIR,
                                  '{0}_{1}_{2}_{3}_{4}_{5}_{6}_{7}.png'.format(
                                      nom.name,
                                      nom.input_file,
                                      nom_leg,
                                      test_leg,
                                      tree,
                                      rootutils.escape_for_filename(branch.name),
                                      rootutils.escape_for_filename(cut),
                                      dt_str))

        validation_utils.histograms_compare(nom,
                                            test,
                                            tree,
                                            title,
                                            branch.name,
                                            cut2,
                                            branch.binning,
                                            nom_leg,
                                            test_leg,
                                            plots_file,
                                            use_draw_tool)

    return compare_histogram


def create_check_roovtxlink(tree, use_nom):
    """Returns a function that runs highland2 program
    CheckRooTrackerVtxLink.exe when the necessary trees exist.
    """
    @validation_utils.info
    def check_roovtxlink(nom, test):
        version_str = nom.version if use_nom else test.version
        print 'Running CheckRooTrackerVtxLink.exe for {0} tree using {1}...'.format(
            tree, version_str)
        if use_nom:
            validation_utils.run_check_roovtxlink(nom, tree)
        else:
            validation_utils.run_check_roovtxlink(test, tree)

    return check_roovtxlink


def get_events(filename):
    datafile = file(filename)

    events = defaultdict(lambda: defaultdict(dict))
    found = False
    for line in datafile:
        # found the selection name
        if 'Statistics for selection' in line:
            sel = (line.partition('Statistics for selection \'')[2]).partition('\'')[0]

        # found the branch name and the number of events for this branch
        if '# events = ' in line:
            branch = (line.partition('\' -->')[0]).partition('branch \'')[2]
            nevt = (line.partition('# events = ')[2]).partition('\n')[0]

            events[sel][branch]=nevt;
            
        
    return events


def get_time(filename):
    datafile = file(filename)

    for line in datafile:
        # found the processing time
        if 'time: ' in line:
          time = line.partition('time: ')[2]
    return float(time)

def create_check_psycheSteering_selections():
    """Returns a function that runs psyche program
    RunExample.exe when the necessary output file exist
    """
    @validation_utils.info
    def check_psycheSteering_selections(nom, test):

        events_nom  = get_events(nom[0])
        events_test = get_events(test[0]) 

        success=True
        if (len(events_nom) != len(events_test)):
            print 'different number of selections and/or branches being run'
            print "NOMINAL: ", events_nom
            print "TEST:    ", events_test
            success=False
            
        for s in events_nom:
            for b in events_nom[s]:
                if (events_nom[s][b]!=events_test[s][b]):
                    print "number of events differ for branch", "'"+b+"'", "in selection", "'"+s+"'.", "nom:", events_nom[s][b],", test:", events_test[s][b]
                    success=False


        assert success

    return check_psycheSteering_selections


def create_check_psycheSteering_performance():
    """Returns a function that runs psyche program
    RunExample.exe when the necessary output file exist
    """
    @validation_utils.info
    def check_psycheSteering_performance(nom, test):

        events_nom  = get_events(nom[0])
        events_test = get_events(test[0]) 

        time_nom  = get_time(nom[0])
        time_test = get_time(test[0])

        
        success=True
        if (len(events_nom) != len(events_test)):
            print 'different number of selections and/or branches being run'
            print "NOMINAL: ", events_nom
            print "TEST:    ", events_test
            success=False

        change = (time_test-time_nom)/time_nom

        if (change > 0.2):
            print 'performance have been degraded', round(change*100,1), "%", ", limit is 20%"  
            print "NOMINAL: ", time_nom,  "s"
            print "TEST:    ", time_test, "s"
            success=False

        assert success

    return check_psycheSteering_performance

            

common_test_funcs = [compare_pot,
                     compare_drawing_tools_pot,
                     compare_drawing_tools_spills,
                     create_check_roovtxlink('truth', False),
                     create_check_roovtxlink('default', False)]


# numu tests
numu_test_funcs = [create_entries_comp('truth', ''),
                   create_entries_comp('default', 'accum_level>5')]
numu_test_funcs.extend([create_histograms_comp('default',
                                               BRANCH,
                                               'accum_level>5',
                                               bool(use_tool))
                        for BRANCH in DEFAULT_BRANCHES for use_tool in range(2)])
numu_test_funcs.extend([create_histograms_comp('truth',
                                               BRANCH,
                                               '',
                                               bool(use_tool))
                        for BRANCH in BRANCHES for use_tool in range(2)])

numu_test_funcs.extend([create_histograms_comp('NRooTrackerVtx',BRANCH,'',False) for BRANCH in ROOVTX_BRANCHES])

numu_test_funcs_rdp = [create_entries_comp('default', 'accum_level>5')]
numu_test_funcs_rdp.extend([create_histograms_comp('default',
                                                   BRANCH,
                                                   'accum_level>5',
                                                   bool(use_tool))
                            for BRANCH in DATA_BRANCHES for use_tool in range(2)])


# p0dnumu tests
p0dnumu_test_funcs = [create_entries_comp('truth', ''),
                      create_entries_comp('default', 'accum_level>2')]
p0dnumu_test_funcs.extend([create_histograms_comp('default',
                                                  BRANCH,
                                                  'accum_level>2')
                           for BRANCH in P0D_DEFAULT_BRANCHES])
p0dnumu_test_funcs.extend([create_histograms_comp('truth',
                                              BRANCH,
                                              '')
                           for BRANCH in P0D_BRANCHES])

p0dnumu_test_funcs_rdp = [create_entries_comp('default', 'accum_level>2')]
p0dnumu_test_funcs_rdp.extend([create_histograms_comp('default',
                                                      BRANCH,
                                                      'accum_level>2')
                               for BRANCH in P0D_DATA_BRANCHES])


# p0dnumuccqe tests
p0dnumuccqe_test_funcs = [create_entries_comp('truth', ''),
                          create_entries_comp('default', 'accum_level[0][0]>3')]
p0dnumuccqe_test_funcs.extend([create_histograms_comp('default',
                                                      BRANCH,
                                                      'accum_level[0][0]>3')
                               for BRANCH in P0D_DEFAULT_BRANCHES])
p0dnumuccqe_test_funcs.extend([create_histograms_comp('truth',
                                                      BRANCH,
                                                      '')
                               for BRANCH in P0D_BRANCHES])

p0dnumuccqe_test_funcs_rdp = [create_entries_comp('default', 'accum_level[0][0]>3')]
p0dnumuccqe_test_funcs_rdp.extend([create_histograms_comp('default',
                                                          BRANCH,
                                                          'accum_level[0][0]>3')
                                   for BRANCH in P0D_DATA_BRANCHES])


# flattree tests
flattree_test_funcs = [create_entries_comp('flattree', '')]
flattree_test_funcs.extend([create_histograms_comp('flattree',
                                               BRANCH,
                                               '')
                            for BRANCH in FLATTREE_BRANCHES])
flattree_test_funcs.extend([create_histograms_comp('NRooTrackerVtx',BRANCH,'',False) for BRANCH in ROOVTX_BRANCHES])
flattree_test_funcs_rdp = [create_entries_comp('flattree', '')]
flattree_test_funcs_rdp.extend([create_histograms_comp('flattree',
                                                       BRANCH,
                                                       '')
                                for BRANCH in FLATTREE_DATA_BRANCHES])

# minitree tests
minitree_test_funcs = [create_entries_comp('MiniTree', '')]
minitree_test_funcs.extend([create_histograms_comp('MiniTree',
                                                   BRANCH,
                                                   '')
                            for BRANCH in MINITREE_BRANCHES])
minitree_test_funcs.extend([create_histograms_comp('NRooTrackerVtx',BRANCH,'',False) for BRANCH in ROOVTX_BRANCHES])
minitree_test_funcs_rdp = [create_entries_comp('MiniTree', '')]


# numuCCMultiPi tests
numuCCMultiPi_test_funcs = [create_entries_comp('truth', ''),
                            create_entries_comp('default', 'accum_level>6')]
numuCCMultiPi_test_funcs.extend([create_histograms_comp('default',
                                                        BRANCH,
                                                        'accum_level>6')
                                 for BRANCH in DEFAULT_BRANCHES])
numuCCMultiPi_test_funcs.extend([create_histograms_comp('all_syst',
                                                        BRANCH,
                                                        'accum_level>6')
                                 for BRANCH in ALLSYST_BRANCHES])
numuCCMultiPi_test_funcs.extend([create_histograms_comp('NRooTrackerVtx',BRANCH,'',False) for BRANCH in ROOVTX_BRANCHES])
for syst in VARIATION_SYSTEMATICS:
    numuCCMultiPi_test_funcs.extend([create_histograms_comp(syst+'_syst',
                                                            BRANCH,
                                                            'accum_level>6')
                                     for BRANCH in SYST_BRANCHES])

numuCCMultiPi_test_funcs.extend([create_histograms_comp('truth',
                                                    BRANCH,
                                                        '')
                                for BRANCH in BRANCHES])
numuCCMultiPi_test_funcs_rdp = [create_entries_comp('default', 'accum_level>6')]
numuCCMultiPi_test_funcs_rdp.extend([create_histograms_comp('default',
                                                            BRANCH,
                                                            'accum_level>6')
                                     for BRANCH in DATA_BRANCHES])

# antiNumuCC tests
antiNumuCC_test_funcs = [create_entries_comp('truth', ''),
                         create_entries_comp('default', 'accum_level>7')]
antiNumuCC_test_funcs.extend([create_histograms_comp('default',
                                                     BRANCH,
                                                     'accum_level>7')
                              for BRANCH in DEFAULT_BRANCHES])
antiNumuCC_test_funcs.extend([create_histograms_comp('all_syst',
                                                     BRANCH,
                                                     'accum_level>7')
                              for BRANCH in ALLSYST_BRANCHES])
antiNumuCC_test_funcs.extend([create_histograms_comp('NRooTrackerVtx',BRANCH,'',False) for BRANCH in ROOVTX_BRANCHES])
for syst in VARIATION_SYSTEMATICS:
    antiNumuCC_test_funcs.extend([create_histograms_comp(syst+'_syst',
                                                         BRANCH,
                                                         'accum_level>7')
                                  for BRANCH in SYST_BRANCHES])

antiNumuCC_test_funcs.extend([create_histograms_comp('truth',
                                                     BRANCH,
                                                     '')
                              for BRANCH in BRANCHES])
antiNumuCC_test_funcs_rdp = [create_entries_comp('default', 'accum_level>7')]
antiNumuCC_test_funcs_rdp.extend([create_histograms_comp('default',
                                                         BRANCH,
                                                         'accum_level>7')
                                     for BRANCH in DATA_BRANCHES])


# numuBkgInAntiNuMode tests
numuBkgInAntiNuMode_test_funcs = [create_entries_comp('truth', ''),
                                  create_entries_comp('default', 'accum_level>7')]
numuBkgInAntiNuMode_test_funcs.extend([create_histograms_comp('default',
                                                              BRANCH,
                                                              'accum_level>7')
                                       for BRANCH in DEFAULT_BRANCHES])
numuBkgInAntiNuMode_test_funcs.extend([create_histograms_comp('all_syst',
                                                              BRANCH,
                                                              'accum_level>7')
                                       for BRANCH in ALLSYST_BRANCHES])
for syst in VARIATION_SYSTEMATICS:
    numuBkgInAntiNuMode_test_funcs.extend([create_histograms_comp(syst+'_syst',
                                                                  BRANCH,
                                                                  'accum_level>7')
                                           for BRANCH in SYST_BRANCHES])

numuBkgInAntiNuMode_test_funcs.extend([create_histograms_comp('truth',
                                                              BRANCH,
                                                              '')
                                       for BRANCH in BRANCHES])
numuBkgInAntiNuMode_test_funcs.extend([create_histograms_comp('NRooTrackerVtx',BRANCH,'',False) for BRANCH in ROOVTX_BRANCHES])
numuBkgInAntiNuMode_test_funcs_rdp = [create_entries_comp('default', 'accum_level>7')]
numuBkgInAntiNuMode_test_funcs_rdp.extend([create_histograms_comp('default',
                                                                  BRANCH,
                                                                  'accum_level>7')
                                           for BRANCH in DATA_BRANCHES])


# nueCC tests
nueCC_test_funcs = [create_entries_comp('truth', ''),
                    create_entries_comp('default', 'accum_level>17')]
nueCC_test_funcs.extend([create_histograms_comp('default',
                                                BRANCH,
                                                'accum_level>17')
                         for BRANCH in DEFAULT_NUE_BRANCHES])
nueCC_test_funcs.extend([create_histograms_comp('NRooTrackerVtx',BRANCH,'',False) for BRANCH in ROOVTX_BRANCHES])
#nueCC_test_funcs.extend([create_histograms_comp('all_syst',
#                                                BRANCH,
#                                                'accum_level>7')
#                         for BRANCH in ALLSYST_BRANCHES])
#for syst in VARIATION_SYSTEMATICS:
#    nueCC_test_funcs.extend([create_histograms_comp(syst+'_syst',
#                                                    BRANCH,
#                                                    'accum_level>7')
#                             for BRANCH in SYST_BRANCHES])
#    
#    nueCC_test_funcs.extend([create_histograms_comp('truth',
#                                                    BRANCH,
#                                                    '')
#                             for BRANCH in BRANCHES])

nueCC_test_funcs_rdp = [create_entries_comp('default', 'accum_level>17')]
nueCC_test_funcs_rdp.extend([create_histograms_comp('default',
                                                    BRANCH,
                                                    'accum_level>17')
                             for BRANCH in DATA_NUE_BRANCHES])
    


# psycheSteering tests
psycheSteering_test_funcs     =  [create_check_psycheSteering_selections()]
psycheSteering_test_funcs.extend([create_check_psycheSteering_performance()])

psycheSteering_test_funcs_rdp = [create_check_psycheSteering_selections()]
psycheSteering_test_funcs_rdp.extend([create_check_psycheSteering_performance()])
