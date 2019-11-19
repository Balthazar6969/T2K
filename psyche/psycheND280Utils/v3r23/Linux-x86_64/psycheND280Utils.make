#-- start of make_header -----------------

#====================================
#  Library psycheND280Utils
#
#   Generated Mon Nov  4 15:26:46 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_psycheND280Utils_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_psycheND280Utils_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_psycheND280Utils

psycheND280Utils_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheND280Utils = /tmp/CMT_$(psycheND280Utils_tag)_psycheND280Utils.make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheND280Utils = $(psycheND280Utils_tag)_psycheND280Utils.make
cmt_local_tagfile_psycheND280Utils = $(bin)$(psycheND280Utils_tag)_psycheND280Utils.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheND280Utils_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheND280Utils = /tmp/CMT_$(psycheND280Utils_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheND280Utils = $(psycheND280Utils_tag).make
cmt_local_tagfile_psycheND280Utils = $(bin)$(psycheND280Utils_tag).make
endif

endif

-include $(cmt_local_tagfile_psycheND280Utils)

ifdef cmt_psycheND280Utils_has_target_tag

ifdef READONLY
cmt_final_setup_psycheND280Utils = /tmp/CMT_psycheND280Utils_psycheND280Utilssetup.make
cmt_local_psycheND280Utils_makefile = /tmp/CMT_psycheND280Utils$(cmt_lock_pid).make
else
cmt_final_setup_psycheND280Utils = $(bin)psycheND280Utils_psycheND280Utilssetup.make
cmt_local_psycheND280Utils_makefile = $(bin)psycheND280Utils.make
endif

else

ifdef READONLY
cmt_final_setup_psycheND280Utils = /tmp/CMT_psycheND280Utilssetup.make
cmt_local_psycheND280Utils_makefile = /tmp/CMT_psycheND280Utils$(cmt_lock_pid).make
else
cmt_final_setup_psycheND280Utils = $(bin)psycheND280Utilssetup.make
cmt_local_psycheND280Utils_makefile = $(bin)psycheND280Utils.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheND280Utilssetup.make
else
cmt_final_setup = $(bin)psycheND280Utilssetup.make
endif

psycheND280Utils ::


ifdef READONLY
psycheND280Utils ::
	@echo tags=$(tags)
	@echo cmt_local_tagfile=$(cmt_local_tagfile)
endif


dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'psycheND280Utils'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = psycheND280Utils/
psycheND280Utils::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

psycheND280Utilslibname   = $(bin)$(library_prefix)psycheND280Utils$(library_suffix)
psycheND280Utilslib       = $(psycheND280Utilslibname).a
psycheND280Utilsstamp     = $(bin)psycheND280Utils.stamp
psycheND280Utilsshstamp   = $(bin)psycheND280Utils.shstamp

psycheND280Utils :: dirs  psycheND280UtilsLIB
	$(echo) "psycheND280Utils ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

psycheND280UtilsLIB :: $(psycheND280Utilslib) $(psycheND280Utilsshstamp)
	$(echo) "psycheND280Utils : library ok"

$(psycheND280Utilslib) :: $(bin)ConstituentsUtils.o $(bin)CutUtils.o $(bin)DetectorDefinition.o $(bin)EventBoxTracker.o $(bin)EventBoxUtils.o $(bin)FiducialVolumeDefinition.o $(bin)FluxWeighting.o $(bin)GeometryManager.o $(bin)MomentumResolVariation.o $(bin)ND280AnalysisUtils.o $(bin)ND280BeamBunching.o $(bin)PIDUtils.o $(bin)PionCrossSections.o $(bin)PionInteractionSystematic.o $(bin)ProtonInteractionSystematic.o $(bin)SecondaryInteractionSystematic.o $(bin)TPCPIDVariation.o $(bin)ToFSenseCorrector.o $(bin)TreeConverterUtils.o $(bin)TruthUtils.o $(bin)VersioningUtils.o $(bin)nueCCUtils.o $(bin)nueCutUtils.o $(bin)psycheND280Tools_Dict.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(psycheND280Utilslib) $?
	$(lib_silent) $(ranlib) $(psycheND280Utilslib)
	$(lib_silent) cat /dev/null >$(psycheND280Utilsstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

#
# We add one level of dependency upon the true shared library 
# (rather than simply upon the stamp file)
# this is for cases where the shared library has not been built
# while the stamp was created (error??) 
#

$(psycheND280Utilslibname).$(shlibsuffix) :: $(psycheND280Utilslib) requirements $(use_requirements) $(psycheND280Utilsstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" psycheND280Utils $(psycheND280Utils_shlibflags)

$(psycheND280Utilsshstamp) :: $(psycheND280Utilslibname).$(shlibsuffix)
	$(lib_silent) if test -f $(psycheND280Utilslibname).$(shlibsuffix) ; then cat /dev/null >$(psycheND280Utilsshstamp) ; fi

psycheND280Utilsclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)ConstituentsUtils.o $(bin)CutUtils.o $(bin)DetectorDefinition.o $(bin)EventBoxTracker.o $(bin)EventBoxUtils.o $(bin)FiducialVolumeDefinition.o $(bin)FluxWeighting.o $(bin)GeometryManager.o $(bin)MomentumResolVariation.o $(bin)ND280AnalysisUtils.o $(bin)ND280BeamBunching.o $(bin)PIDUtils.o $(bin)PionCrossSections.o $(bin)PionInteractionSystematic.o $(bin)ProtonInteractionSystematic.o $(bin)SecondaryInteractionSystematic.o $(bin)TPCPIDVariation.o $(bin)ToFSenseCorrector.o $(bin)TreeConverterUtils.o $(bin)TruthUtils.o $(bin)VersioningUtils.o $(bin)nueCCUtils.o $(bin)nueCutUtils.o $(bin)psycheND280Tools_Dict.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf psycheND280Utils_deps psycheND280Utils_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
psycheND280Utilsinstallname = $(library_prefix)psycheND280Utils$(library_suffix).$(shlibsuffix)

psycheND280Utils :: psycheND280Utilsinstall

install :: psycheND280Utilsinstall

psycheND280Utilsinstall :: $(install_dir)/$(psycheND280Utilsinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(psycheND280Utilsinstallname) :: $(bin)$(psycheND280Utilsinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheND280Utilsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##psycheND280Utilsclean :: psycheND280Utilsuninstall

uninstall :: psycheND280Utilsuninstall

psycheND280Utilsuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheND280Utilsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),psycheND280Utilsclean)

#$(bin)psycheND280Utils_dependencies.make :: dirs

ifndef QUICK
$(bin)psycheND280Utils_dependencies.make : ../src/ConstituentsUtils.cxx ../src/CutUtils.cxx ../src/DetectorDefinition.cxx ../src/EventBoxTracker.cxx ../src/EventBoxUtils.cxx ../src/FiducialVolumeDefinition.cxx ../src/FluxWeighting.cxx ../src/GeometryManager.cxx ../src/MomentumResolVariation.cxx ../src/ND280AnalysisUtils.cxx ../src/ND280BeamBunching.cxx ../src/PIDUtils.cxx ../src/PionCrossSections.cxx ../src/PionInteractionSystematic.cxx ../src/ProtonInteractionSystematic.cxx ../src/SecondaryInteractionSystematic.cxx ../src/TPCPIDVariation.cxx ../src/ToFSenseCorrector.cxx ../src/TreeConverterUtils.cxx ../src/TruthUtils.cxx ../src/VersioningUtils.cxx ../src/nueCCUtils.cxx ../src/nueCutUtils.cxx ../dict/psycheND280Tools_Dict.cxx $(use_requirements) $(cmt_final_setup_psycheND280Utils)
	$(echo) "(psycheND280Utils.make) Rebuilding $@"; \
	  $(build_dependencies) psycheND280Utils -all_sources -out=$@ ../src/ConstituentsUtils.cxx ../src/CutUtils.cxx ../src/DetectorDefinition.cxx ../src/EventBoxTracker.cxx ../src/EventBoxUtils.cxx ../src/FiducialVolumeDefinition.cxx ../src/FluxWeighting.cxx ../src/GeometryManager.cxx ../src/MomentumResolVariation.cxx ../src/ND280AnalysisUtils.cxx ../src/ND280BeamBunching.cxx ../src/PIDUtils.cxx ../src/PionCrossSections.cxx ../src/PionInteractionSystematic.cxx ../src/ProtonInteractionSystematic.cxx ../src/SecondaryInteractionSystematic.cxx ../src/TPCPIDVariation.cxx ../src/ToFSenseCorrector.cxx ../src/TreeConverterUtils.cxx ../src/TruthUtils.cxx ../src/VersioningUtils.cxx ../src/nueCCUtils.cxx ../src/nueCutUtils.cxx ../dict/psycheND280Tools_Dict.cxx
endif

#$(psycheND280Utils_dependencies)

-include $(bin)psycheND280Utils_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(ConstituentsUtils_cxx_dependencies)

$(bin)$(binobj)ConstituentsUtils.o : $(ConstituentsUtils_cxx_dependencies)
	$(cpp_echo) $(src)ConstituentsUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(ConstituentsUtils_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(ConstituentsUtils_cppflags) $(ConstituentsUtils_cxx_cppflags)  $(src)ConstituentsUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(CutUtils_cxx_dependencies)

$(bin)$(binobj)CutUtils.o : $(CutUtils_cxx_dependencies)
	$(cpp_echo) $(src)CutUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(CutUtils_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(CutUtils_cppflags) $(CutUtils_cxx_cppflags)  $(src)CutUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(DetectorDefinition_cxx_dependencies)

$(bin)$(binobj)DetectorDefinition.o : $(DetectorDefinition_cxx_dependencies)
	$(cpp_echo) $(src)DetectorDefinition.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(DetectorDefinition_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(DetectorDefinition_cppflags) $(DetectorDefinition_cxx_cppflags)  $(src)DetectorDefinition.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(EventBoxTracker_cxx_dependencies)

$(bin)$(binobj)EventBoxTracker.o : $(EventBoxTracker_cxx_dependencies)
	$(cpp_echo) $(src)EventBoxTracker.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(EventBoxTracker_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(EventBoxTracker_cppflags) $(EventBoxTracker_cxx_cppflags)  $(src)EventBoxTracker.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(EventBoxUtils_cxx_dependencies)

$(bin)$(binobj)EventBoxUtils.o : $(EventBoxUtils_cxx_dependencies)
	$(cpp_echo) $(src)EventBoxUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(EventBoxUtils_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(EventBoxUtils_cppflags) $(EventBoxUtils_cxx_cppflags)  $(src)EventBoxUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(FiducialVolumeDefinition_cxx_dependencies)

$(bin)$(binobj)FiducialVolumeDefinition.o : $(FiducialVolumeDefinition_cxx_dependencies)
	$(cpp_echo) $(src)FiducialVolumeDefinition.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(FiducialVolumeDefinition_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(FiducialVolumeDefinition_cppflags) $(FiducialVolumeDefinition_cxx_cppflags)  $(src)FiducialVolumeDefinition.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(FluxWeighting_cxx_dependencies)

$(bin)$(binobj)FluxWeighting.o : $(FluxWeighting_cxx_dependencies)
	$(cpp_echo) $(src)FluxWeighting.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(FluxWeighting_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(FluxWeighting_cppflags) $(FluxWeighting_cxx_cppflags)  $(src)FluxWeighting.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(GeometryManager_cxx_dependencies)

$(bin)$(binobj)GeometryManager.o : $(GeometryManager_cxx_dependencies)
	$(cpp_echo) $(src)GeometryManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(GeometryManager_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(GeometryManager_cppflags) $(GeometryManager_cxx_cppflags)  $(src)GeometryManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(MomentumResolVariation_cxx_dependencies)

$(bin)$(binobj)MomentumResolVariation.o : $(MomentumResolVariation_cxx_dependencies)
	$(cpp_echo) $(src)MomentumResolVariation.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(MomentumResolVariation_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(MomentumResolVariation_cppflags) $(MomentumResolVariation_cxx_cppflags)  $(src)MomentumResolVariation.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(ND280AnalysisUtils_cxx_dependencies)

$(bin)$(binobj)ND280AnalysisUtils.o : $(ND280AnalysisUtils_cxx_dependencies)
	$(cpp_echo) $(src)ND280AnalysisUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(ND280AnalysisUtils_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(ND280AnalysisUtils_cppflags) $(ND280AnalysisUtils_cxx_cppflags)  $(src)ND280AnalysisUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(ND280BeamBunching_cxx_dependencies)

$(bin)$(binobj)ND280BeamBunching.o : $(ND280BeamBunching_cxx_dependencies)
	$(cpp_echo) $(src)ND280BeamBunching.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(ND280BeamBunching_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(ND280BeamBunching_cppflags) $(ND280BeamBunching_cxx_cppflags)  $(src)ND280BeamBunching.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(PIDUtils_cxx_dependencies)

$(bin)$(binobj)PIDUtils.o : $(PIDUtils_cxx_dependencies)
	$(cpp_echo) $(src)PIDUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(PIDUtils_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(PIDUtils_cppflags) $(PIDUtils_cxx_cppflags)  $(src)PIDUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(PionCrossSections_cxx_dependencies)

$(bin)$(binobj)PionCrossSections.o : $(PionCrossSections_cxx_dependencies)
	$(cpp_echo) $(src)PionCrossSections.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(PionCrossSections_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(PionCrossSections_cppflags) $(PionCrossSections_cxx_cppflags)  $(src)PionCrossSections.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(PionInteractionSystematic_cxx_dependencies)

$(bin)$(binobj)PionInteractionSystematic.o : $(PionInteractionSystematic_cxx_dependencies)
	$(cpp_echo) $(src)PionInteractionSystematic.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(PionInteractionSystematic_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(PionInteractionSystematic_cppflags) $(PionInteractionSystematic_cxx_cppflags)  $(src)PionInteractionSystematic.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(ProtonInteractionSystematic_cxx_dependencies)

$(bin)$(binobj)ProtonInteractionSystematic.o : $(ProtonInteractionSystematic_cxx_dependencies)
	$(cpp_echo) $(src)ProtonInteractionSystematic.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(ProtonInteractionSystematic_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(ProtonInteractionSystematic_cppflags) $(ProtonInteractionSystematic_cxx_cppflags)  $(src)ProtonInteractionSystematic.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(SecondaryInteractionSystematic_cxx_dependencies)

$(bin)$(binobj)SecondaryInteractionSystematic.o : $(SecondaryInteractionSystematic_cxx_dependencies)
	$(cpp_echo) $(src)SecondaryInteractionSystematic.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(SecondaryInteractionSystematic_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(SecondaryInteractionSystematic_cppflags) $(SecondaryInteractionSystematic_cxx_cppflags)  $(src)SecondaryInteractionSystematic.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(TPCPIDVariation_cxx_dependencies)

$(bin)$(binobj)TPCPIDVariation.o : $(TPCPIDVariation_cxx_dependencies)
	$(cpp_echo) $(src)TPCPIDVariation.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(TPCPIDVariation_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(TPCPIDVariation_cppflags) $(TPCPIDVariation_cxx_cppflags)  $(src)TPCPIDVariation.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(ToFSenseCorrector_cxx_dependencies)

$(bin)$(binobj)ToFSenseCorrector.o : $(ToFSenseCorrector_cxx_dependencies)
	$(cpp_echo) $(src)ToFSenseCorrector.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(ToFSenseCorrector_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(ToFSenseCorrector_cppflags) $(ToFSenseCorrector_cxx_cppflags)  $(src)ToFSenseCorrector.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(TreeConverterUtils_cxx_dependencies)

$(bin)$(binobj)TreeConverterUtils.o : $(TreeConverterUtils_cxx_dependencies)
	$(cpp_echo) $(src)TreeConverterUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(TreeConverterUtils_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(TreeConverterUtils_cppflags) $(TreeConverterUtils_cxx_cppflags)  $(src)TreeConverterUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(TruthUtils_cxx_dependencies)

$(bin)$(binobj)TruthUtils.o : $(TruthUtils_cxx_dependencies)
	$(cpp_echo) $(src)TruthUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(TruthUtils_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(TruthUtils_cppflags) $(TruthUtils_cxx_cppflags)  $(src)TruthUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(VersioningUtils_cxx_dependencies)

$(bin)$(binobj)VersioningUtils.o : $(VersioningUtils_cxx_dependencies)
	$(cpp_echo) $(src)VersioningUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(VersioningUtils_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(VersioningUtils_cppflags) $(VersioningUtils_cxx_cppflags)  $(src)VersioningUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(nueCCUtils_cxx_dependencies)

$(bin)$(binobj)nueCCUtils.o : $(nueCCUtils_cxx_dependencies)
	$(cpp_echo) $(src)nueCCUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(nueCCUtils_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(nueCCUtils_cppflags) $(nueCCUtils_cxx_cppflags)  $(src)nueCCUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(nueCutUtils_cxx_dependencies)

$(bin)$(binobj)nueCutUtils.o : $(nueCutUtils_cxx_dependencies)
	$(cpp_echo) $(src)nueCutUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(nueCutUtils_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(nueCutUtils_cppflags) $(nueCutUtils_cxx_cppflags)  $(src)nueCutUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheND280Utils_dependencies.make : $(psycheND280Tools_Dict_cxx_dependencies)

$(bin)$(binobj)psycheND280Tools_Dict.o : $(psycheND280Tools_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/psycheND280Tools_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheND280Utils_pp_cppflags) $(lib_psycheND280Utils_pp_cppflags) $(psycheND280Tools_Dict_pp_cppflags) $(use_cppflags) $(psycheND280Utils_cppflags) $(lib_psycheND280Utils_cppflags) $(psycheND280Tools_Dict_cppflags) $(psycheND280Tools_Dict_cxx_cppflags) -I../dict ../dict/psycheND280Tools_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: psycheND280Utilsclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(psycheND280Utils.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(psycheND280Utils.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_psycheND280Utils)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheND280Utils.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheND280Utils.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheND280Utils.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

psycheND280Utilsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)psycheND280Utils$(library_suffix).a $(binobj)$(library_prefix)psycheND280Utils$(library_suffix).s? $(binobj)psycheND280Utils.stamp $(binobj)psycheND280Utils.shstamp
#-- end of cleanup_library ---------------

