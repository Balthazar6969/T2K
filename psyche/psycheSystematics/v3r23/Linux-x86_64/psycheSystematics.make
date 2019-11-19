#-- start of make_header -----------------

#====================================
#  Library psycheSystematics
#
#   Generated Mon Nov  4 15:26:54 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_psycheSystematics_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_psycheSystematics_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_psycheSystematics

psycheSystematics_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheSystematics = /tmp/CMT_$(psycheSystematics_tag)_psycheSystematics.make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheSystematics = $(psycheSystematics_tag)_psycheSystematics.make
cmt_local_tagfile_psycheSystematics = $(bin)$(psycheSystematics_tag)_psycheSystematics.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheSystematics_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheSystematics = /tmp/CMT_$(psycheSystematics_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheSystematics = $(psycheSystematics_tag).make
cmt_local_tagfile_psycheSystematics = $(bin)$(psycheSystematics_tag).make
endif

endif

-include $(cmt_local_tagfile_psycheSystematics)

ifdef cmt_psycheSystematics_has_target_tag

ifdef READONLY
cmt_final_setup_psycheSystematics = /tmp/CMT_psycheSystematics_psycheSystematicssetup.make
cmt_local_psycheSystematics_makefile = /tmp/CMT_psycheSystematics$(cmt_lock_pid).make
else
cmt_final_setup_psycheSystematics = $(bin)psycheSystematics_psycheSystematicssetup.make
cmt_local_psycheSystematics_makefile = $(bin)psycheSystematics.make
endif

else

ifdef READONLY
cmt_final_setup_psycheSystematics = /tmp/CMT_psycheSystematicssetup.make
cmt_local_psycheSystematics_makefile = /tmp/CMT_psycheSystematics$(cmt_lock_pid).make
else
cmt_final_setup_psycheSystematics = $(bin)psycheSystematicssetup.make
cmt_local_psycheSystematics_makefile = $(bin)psycheSystematics.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheSystematicssetup.make
else
cmt_final_setup = $(bin)psycheSystematicssetup.make
endif

psycheSystematics ::


ifdef READONLY
psycheSystematics ::
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
	$(echo) 'psycheSystematics'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = psycheSystematics/
psycheSystematics::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

psycheSystematicslibname   = $(bin)$(library_prefix)psycheSystematics$(library_suffix)
psycheSystematicslib       = $(psycheSystematicslibname).a
psycheSystematicsstamp     = $(bin)psycheSystematics.stamp
psycheSystematicsshstamp   = $(bin)psycheSystematics.shstamp

psycheSystematics :: dirs  psycheSystematicsLIB
	$(echo) "psycheSystematics ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

psycheSystematicsLIB :: $(psycheSystematicslib) $(psycheSystematicsshstamp)
	$(echo) "psycheSystematics : library ok"

$(psycheSystematicslib) :: $(bin)BFieldDistortionSystematics.o $(bin)MomentumScaleSystematics.o $(bin)TPCECalMatchEffSystematics.o $(bin)ChargeIDEffSystematics.o $(bin)ECalEMEnergyResolSystematics.o $(bin)OOFVSystematics.o $(bin)ECalEMEnergyScaleSystematics.o $(bin)P0DELossResolSystematics.o $(bin)PileUpSystematics.o $(bin)ECalEMEnergySystematicsBase.o $(bin)TPCFGDMatchEffSystematics.o $(bin)ECalEmHipPIDSystematics.o $(bin)TPCP0DMatchEffSystematics.o $(bin)ECalPIDSystematics.o $(bin)TPCTrackEffSystematics.o $(bin)ECalTrackEffSystematics.o $(bin)FGD2ShowerSystematics.o $(bin)ToFResolSystematics.o $(bin)SIPionSystematics.o $(bin)FGDECalMatchEffSystematics.o $(bin)SIProtonSystematics.o $(bin)FGDECalSMRDMatchEffSystematics.o $(bin)SandMuonsSystematics.o $(bin)FGDHybridTrackEffSystematics.o $(bin)FGDMassSystematics.o $(bin)nueECalPileUpSystematics.o $(bin)FGDPIDSystematics.o $(bin)FGDTrackEffSystematics.o $(bin)nueOOFVSystematics.o $(bin)FluxWeightSystematics.o $(bin)nueP0DPileUpSystematics.o $(bin)TPCClusterEffSystematics.o $(bin)MichelElectronEffSystematics.o $(bin)MomBiasFromVertexMigrationSystematics.o $(bin)MomentumResolSystematics.o $(bin)MomRangeResolSystematics.o $(bin)nueTPCPileUpSystematics.o $(bin)P0DELossScaleSystematics.o $(bin)P0DVetoSystematics.o $(bin)TPCPIDSystematics.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(psycheSystematicslib) $?
	$(lib_silent) $(ranlib) $(psycheSystematicslib)
	$(lib_silent) cat /dev/null >$(psycheSystematicsstamp)

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

$(psycheSystematicslibname).$(shlibsuffix) :: $(psycheSystematicslib) requirements $(use_requirements) $(psycheSystematicsstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" psycheSystematics $(psycheSystematics_shlibflags)

$(psycheSystematicsshstamp) :: $(psycheSystematicslibname).$(shlibsuffix)
	$(lib_silent) if test -f $(psycheSystematicslibname).$(shlibsuffix) ; then cat /dev/null >$(psycheSystematicsshstamp) ; fi

psycheSystematicsclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)BFieldDistortionSystematics.o $(bin)MomentumScaleSystematics.o $(bin)TPCECalMatchEffSystematics.o $(bin)ChargeIDEffSystematics.o $(bin)ECalEMEnergyResolSystematics.o $(bin)OOFVSystematics.o $(bin)ECalEMEnergyScaleSystematics.o $(bin)P0DELossResolSystematics.o $(bin)PileUpSystematics.o $(bin)ECalEMEnergySystematicsBase.o $(bin)TPCFGDMatchEffSystematics.o $(bin)ECalEmHipPIDSystematics.o $(bin)TPCP0DMatchEffSystematics.o $(bin)ECalPIDSystematics.o $(bin)TPCTrackEffSystematics.o $(bin)ECalTrackEffSystematics.o $(bin)FGD2ShowerSystematics.o $(bin)ToFResolSystematics.o $(bin)SIPionSystematics.o $(bin)FGDECalMatchEffSystematics.o $(bin)SIProtonSystematics.o $(bin)FGDECalSMRDMatchEffSystematics.o $(bin)SandMuonsSystematics.o $(bin)FGDHybridTrackEffSystematics.o $(bin)FGDMassSystematics.o $(bin)nueECalPileUpSystematics.o $(bin)FGDPIDSystematics.o $(bin)FGDTrackEffSystematics.o $(bin)nueOOFVSystematics.o $(bin)FluxWeightSystematics.o $(bin)nueP0DPileUpSystematics.o $(bin)TPCClusterEffSystematics.o $(bin)MichelElectronEffSystematics.o $(bin)MomBiasFromVertexMigrationSystematics.o $(bin)MomentumResolSystematics.o $(bin)MomRangeResolSystematics.o $(bin)nueTPCPileUpSystematics.o $(bin)P0DELossScaleSystematics.o $(bin)P0DVetoSystematics.o $(bin)TPCPIDSystematics.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf psycheSystematics_deps psycheSystematics_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
psycheSystematicsinstallname = $(library_prefix)psycheSystematics$(library_suffix).$(shlibsuffix)

psycheSystematics :: psycheSystematicsinstall

install :: psycheSystematicsinstall

psycheSystematicsinstall :: $(install_dir)/$(psycheSystematicsinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(psycheSystematicsinstallname) :: $(bin)$(psycheSystematicsinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheSystematicsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##psycheSystematicsclean :: psycheSystematicsuninstall

uninstall :: psycheSystematicsuninstall

psycheSystematicsuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheSystematicsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),psycheSystematicsclean)

#$(bin)psycheSystematics_dependencies.make :: dirs

ifndef QUICK
$(bin)psycheSystematics_dependencies.make : ../src/BFieldDistortionSystematics.cxx ../src/MomentumScaleSystematics.cxx ../src/TPCECalMatchEffSystematics.cxx ../src/ChargeIDEffSystematics.cxx ../src/ECalEMEnergyResolSystematics.cxx ../src/OOFVSystematics.cxx ../src/ECalEMEnergyScaleSystematics.cxx ../src/P0DELossResolSystematics.cxx ../src/PileUpSystematics.cxx ../src/ECalEMEnergySystematicsBase.cxx ../src/TPCFGDMatchEffSystematics.cxx ../src/ECalEmHipPIDSystematics.cxx ../src/TPCP0DMatchEffSystematics.cxx ../src/ECalPIDSystematics.cxx ../src/TPCTrackEffSystematics.cxx ../src/ECalTrackEffSystematics.cxx ../src/FGD2ShowerSystematics.cxx ../src/ToFResolSystematics.cxx ../src/SIPionSystematics.cxx ../src/FGDECalMatchEffSystematics.cxx ../src/SIProtonSystematics.cxx ../src/FGDECalSMRDMatchEffSystematics.cxx ../src/SandMuonsSystematics.cxx ../src/FGDHybridTrackEffSystematics.cxx ../src/FGDMassSystematics.cxx ../src/nueECalPileUpSystematics.cxx ../src/FGDPIDSystematics.cxx ../src/FGDTrackEffSystematics.cxx ../src/nueOOFVSystematics.cxx ../src/FluxWeightSystematics.cxx ../src/nueP0DPileUpSystematics.cxx ../src/TPCClusterEffSystematics.cxx ../src/MichelElectronEffSystematics.cxx ../src/MomBiasFromVertexMigrationSystematics.cxx ../src/MomentumResolSystematics.cxx ../src/MomRangeResolSystematics.cxx ../src/nueTPCPileUpSystematics.cxx ../src/P0DELossScaleSystematics.cxx ../src/P0DVetoSystematics.cxx ../src/TPCPIDSystematics.cxx $(use_requirements) $(cmt_final_setup_psycheSystematics)
	$(echo) "(psycheSystematics.make) Rebuilding $@"; \
	  $(build_dependencies) psycheSystematics -all_sources -out=$@ ../src/BFieldDistortionSystematics.cxx ../src/MomentumScaleSystematics.cxx ../src/TPCECalMatchEffSystematics.cxx ../src/ChargeIDEffSystematics.cxx ../src/ECalEMEnergyResolSystematics.cxx ../src/OOFVSystematics.cxx ../src/ECalEMEnergyScaleSystematics.cxx ../src/P0DELossResolSystematics.cxx ../src/PileUpSystematics.cxx ../src/ECalEMEnergySystematicsBase.cxx ../src/TPCFGDMatchEffSystematics.cxx ../src/ECalEmHipPIDSystematics.cxx ../src/TPCP0DMatchEffSystematics.cxx ../src/ECalPIDSystematics.cxx ../src/TPCTrackEffSystematics.cxx ../src/ECalTrackEffSystematics.cxx ../src/FGD2ShowerSystematics.cxx ../src/ToFResolSystematics.cxx ../src/SIPionSystematics.cxx ../src/FGDECalMatchEffSystematics.cxx ../src/SIProtonSystematics.cxx ../src/FGDECalSMRDMatchEffSystematics.cxx ../src/SandMuonsSystematics.cxx ../src/FGDHybridTrackEffSystematics.cxx ../src/FGDMassSystematics.cxx ../src/nueECalPileUpSystematics.cxx ../src/FGDPIDSystematics.cxx ../src/FGDTrackEffSystematics.cxx ../src/nueOOFVSystematics.cxx ../src/FluxWeightSystematics.cxx ../src/nueP0DPileUpSystematics.cxx ../src/TPCClusterEffSystematics.cxx ../src/MichelElectronEffSystematics.cxx ../src/MomBiasFromVertexMigrationSystematics.cxx ../src/MomentumResolSystematics.cxx ../src/MomRangeResolSystematics.cxx ../src/nueTPCPileUpSystematics.cxx ../src/P0DELossScaleSystematics.cxx ../src/P0DVetoSystematics.cxx ../src/TPCPIDSystematics.cxx
endif

#$(psycheSystematics_dependencies)

-include $(bin)psycheSystematics_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(BFieldDistortionSystematics_cxx_dependencies)

$(bin)$(binobj)BFieldDistortionSystematics.o : $(BFieldDistortionSystematics_cxx_dependencies)
	$(cpp_echo) $(src)BFieldDistortionSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(BFieldDistortionSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(BFieldDistortionSystematics_cppflags) $(BFieldDistortionSystematics_cxx_cppflags)  $(src)BFieldDistortionSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(MomentumScaleSystematics_cxx_dependencies)

$(bin)$(binobj)MomentumScaleSystematics.o : $(MomentumScaleSystematics_cxx_dependencies)
	$(cpp_echo) $(src)MomentumScaleSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(MomentumScaleSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(MomentumScaleSystematics_cppflags) $(MomentumScaleSystematics_cxx_cppflags)  $(src)MomentumScaleSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(TPCECalMatchEffSystematics_cxx_dependencies)

$(bin)$(binobj)TPCECalMatchEffSystematics.o : $(TPCECalMatchEffSystematics_cxx_dependencies)
	$(cpp_echo) $(src)TPCECalMatchEffSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(TPCECalMatchEffSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(TPCECalMatchEffSystematics_cppflags) $(TPCECalMatchEffSystematics_cxx_cppflags)  $(src)TPCECalMatchEffSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(ChargeIDEffSystematics_cxx_dependencies)

$(bin)$(binobj)ChargeIDEffSystematics.o : $(ChargeIDEffSystematics_cxx_dependencies)
	$(cpp_echo) $(src)ChargeIDEffSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(ChargeIDEffSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(ChargeIDEffSystematics_cppflags) $(ChargeIDEffSystematics_cxx_cppflags)  $(src)ChargeIDEffSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(ECalEMEnergyResolSystematics_cxx_dependencies)

$(bin)$(binobj)ECalEMEnergyResolSystematics.o : $(ECalEMEnergyResolSystematics_cxx_dependencies)
	$(cpp_echo) $(src)ECalEMEnergyResolSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(ECalEMEnergyResolSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(ECalEMEnergyResolSystematics_cppflags) $(ECalEMEnergyResolSystematics_cxx_cppflags)  $(src)ECalEMEnergyResolSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(OOFVSystematics_cxx_dependencies)

$(bin)$(binobj)OOFVSystematics.o : $(OOFVSystematics_cxx_dependencies)
	$(cpp_echo) $(src)OOFVSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(OOFVSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(OOFVSystematics_cppflags) $(OOFVSystematics_cxx_cppflags)  $(src)OOFVSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(ECalEMEnergyScaleSystematics_cxx_dependencies)

$(bin)$(binobj)ECalEMEnergyScaleSystematics.o : $(ECalEMEnergyScaleSystematics_cxx_dependencies)
	$(cpp_echo) $(src)ECalEMEnergyScaleSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(ECalEMEnergyScaleSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(ECalEMEnergyScaleSystematics_cppflags) $(ECalEMEnergyScaleSystematics_cxx_cppflags)  $(src)ECalEMEnergyScaleSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(P0DELossResolSystematics_cxx_dependencies)

$(bin)$(binobj)P0DELossResolSystematics.o : $(P0DELossResolSystematics_cxx_dependencies)
	$(cpp_echo) $(src)P0DELossResolSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(P0DELossResolSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(P0DELossResolSystematics_cppflags) $(P0DELossResolSystematics_cxx_cppflags)  $(src)P0DELossResolSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(PileUpSystematics_cxx_dependencies)

$(bin)$(binobj)PileUpSystematics.o : $(PileUpSystematics_cxx_dependencies)
	$(cpp_echo) $(src)PileUpSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(PileUpSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(PileUpSystematics_cppflags) $(PileUpSystematics_cxx_cppflags)  $(src)PileUpSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(ECalEMEnergySystematicsBase_cxx_dependencies)

$(bin)$(binobj)ECalEMEnergySystematicsBase.o : $(ECalEMEnergySystematicsBase_cxx_dependencies)
	$(cpp_echo) $(src)ECalEMEnergySystematicsBase.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(ECalEMEnergySystematicsBase_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(ECalEMEnergySystematicsBase_cppflags) $(ECalEMEnergySystematicsBase_cxx_cppflags)  $(src)ECalEMEnergySystematicsBase.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(TPCFGDMatchEffSystematics_cxx_dependencies)

$(bin)$(binobj)TPCFGDMatchEffSystematics.o : $(TPCFGDMatchEffSystematics_cxx_dependencies)
	$(cpp_echo) $(src)TPCFGDMatchEffSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(TPCFGDMatchEffSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(TPCFGDMatchEffSystematics_cppflags) $(TPCFGDMatchEffSystematics_cxx_cppflags)  $(src)TPCFGDMatchEffSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(ECalEmHipPIDSystematics_cxx_dependencies)

$(bin)$(binobj)ECalEmHipPIDSystematics.o : $(ECalEmHipPIDSystematics_cxx_dependencies)
	$(cpp_echo) $(src)ECalEmHipPIDSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(ECalEmHipPIDSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(ECalEmHipPIDSystematics_cppflags) $(ECalEmHipPIDSystematics_cxx_cppflags)  $(src)ECalEmHipPIDSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(TPCP0DMatchEffSystematics_cxx_dependencies)

$(bin)$(binobj)TPCP0DMatchEffSystematics.o : $(TPCP0DMatchEffSystematics_cxx_dependencies)
	$(cpp_echo) $(src)TPCP0DMatchEffSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(TPCP0DMatchEffSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(TPCP0DMatchEffSystematics_cppflags) $(TPCP0DMatchEffSystematics_cxx_cppflags)  $(src)TPCP0DMatchEffSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(ECalPIDSystematics_cxx_dependencies)

$(bin)$(binobj)ECalPIDSystematics.o : $(ECalPIDSystematics_cxx_dependencies)
	$(cpp_echo) $(src)ECalPIDSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(ECalPIDSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(ECalPIDSystematics_cppflags) $(ECalPIDSystematics_cxx_cppflags)  $(src)ECalPIDSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(TPCTrackEffSystematics_cxx_dependencies)

$(bin)$(binobj)TPCTrackEffSystematics.o : $(TPCTrackEffSystematics_cxx_dependencies)
	$(cpp_echo) $(src)TPCTrackEffSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(TPCTrackEffSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(TPCTrackEffSystematics_cppflags) $(TPCTrackEffSystematics_cxx_cppflags)  $(src)TPCTrackEffSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(ECalTrackEffSystematics_cxx_dependencies)

$(bin)$(binobj)ECalTrackEffSystematics.o : $(ECalTrackEffSystematics_cxx_dependencies)
	$(cpp_echo) $(src)ECalTrackEffSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(ECalTrackEffSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(ECalTrackEffSystematics_cppflags) $(ECalTrackEffSystematics_cxx_cppflags)  $(src)ECalTrackEffSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(FGD2ShowerSystematics_cxx_dependencies)

$(bin)$(binobj)FGD2ShowerSystematics.o : $(FGD2ShowerSystematics_cxx_dependencies)
	$(cpp_echo) $(src)FGD2ShowerSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(FGD2ShowerSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(FGD2ShowerSystematics_cppflags) $(FGD2ShowerSystematics_cxx_cppflags)  $(src)FGD2ShowerSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(ToFResolSystematics_cxx_dependencies)

$(bin)$(binobj)ToFResolSystematics.o : $(ToFResolSystematics_cxx_dependencies)
	$(cpp_echo) $(src)ToFResolSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(ToFResolSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(ToFResolSystematics_cppflags) $(ToFResolSystematics_cxx_cppflags)  $(src)ToFResolSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(SIPionSystematics_cxx_dependencies)

$(bin)$(binobj)SIPionSystematics.o : $(SIPionSystematics_cxx_dependencies)
	$(cpp_echo) $(src)SIPionSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(SIPionSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(SIPionSystematics_cppflags) $(SIPionSystematics_cxx_cppflags)  $(src)SIPionSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(FGDECalMatchEffSystematics_cxx_dependencies)

$(bin)$(binobj)FGDECalMatchEffSystematics.o : $(FGDECalMatchEffSystematics_cxx_dependencies)
	$(cpp_echo) $(src)FGDECalMatchEffSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(FGDECalMatchEffSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(FGDECalMatchEffSystematics_cppflags) $(FGDECalMatchEffSystematics_cxx_cppflags)  $(src)FGDECalMatchEffSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(SIProtonSystematics_cxx_dependencies)

$(bin)$(binobj)SIProtonSystematics.o : $(SIProtonSystematics_cxx_dependencies)
	$(cpp_echo) $(src)SIProtonSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(SIProtonSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(SIProtonSystematics_cppflags) $(SIProtonSystematics_cxx_cppflags)  $(src)SIProtonSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(FGDECalSMRDMatchEffSystematics_cxx_dependencies)

$(bin)$(binobj)FGDECalSMRDMatchEffSystematics.o : $(FGDECalSMRDMatchEffSystematics_cxx_dependencies)
	$(cpp_echo) $(src)FGDECalSMRDMatchEffSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(FGDECalSMRDMatchEffSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(FGDECalSMRDMatchEffSystematics_cppflags) $(FGDECalSMRDMatchEffSystematics_cxx_cppflags)  $(src)FGDECalSMRDMatchEffSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(SandMuonsSystematics_cxx_dependencies)

$(bin)$(binobj)SandMuonsSystematics.o : $(SandMuonsSystematics_cxx_dependencies)
	$(cpp_echo) $(src)SandMuonsSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(SandMuonsSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(SandMuonsSystematics_cppflags) $(SandMuonsSystematics_cxx_cppflags)  $(src)SandMuonsSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(FGDHybridTrackEffSystematics_cxx_dependencies)

$(bin)$(binobj)FGDHybridTrackEffSystematics.o : $(FGDHybridTrackEffSystematics_cxx_dependencies)
	$(cpp_echo) $(src)FGDHybridTrackEffSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(FGDHybridTrackEffSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(FGDHybridTrackEffSystematics_cppflags) $(FGDHybridTrackEffSystematics_cxx_cppflags)  $(src)FGDHybridTrackEffSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(FGDMassSystematics_cxx_dependencies)

$(bin)$(binobj)FGDMassSystematics.o : $(FGDMassSystematics_cxx_dependencies)
	$(cpp_echo) $(src)FGDMassSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(FGDMassSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(FGDMassSystematics_cppflags) $(FGDMassSystematics_cxx_cppflags)  $(src)FGDMassSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(nueECalPileUpSystematics_cxx_dependencies)

$(bin)$(binobj)nueECalPileUpSystematics.o : $(nueECalPileUpSystematics_cxx_dependencies)
	$(cpp_echo) $(src)nueECalPileUpSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(nueECalPileUpSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(nueECalPileUpSystematics_cppflags) $(nueECalPileUpSystematics_cxx_cppflags)  $(src)nueECalPileUpSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(FGDPIDSystematics_cxx_dependencies)

$(bin)$(binobj)FGDPIDSystematics.o : $(FGDPIDSystematics_cxx_dependencies)
	$(cpp_echo) $(src)FGDPIDSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(FGDPIDSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(FGDPIDSystematics_cppflags) $(FGDPIDSystematics_cxx_cppflags)  $(src)FGDPIDSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(FGDTrackEffSystematics_cxx_dependencies)

$(bin)$(binobj)FGDTrackEffSystematics.o : $(FGDTrackEffSystematics_cxx_dependencies)
	$(cpp_echo) $(src)FGDTrackEffSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(FGDTrackEffSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(FGDTrackEffSystematics_cppflags) $(FGDTrackEffSystematics_cxx_cppflags)  $(src)FGDTrackEffSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(nueOOFVSystematics_cxx_dependencies)

$(bin)$(binobj)nueOOFVSystematics.o : $(nueOOFVSystematics_cxx_dependencies)
	$(cpp_echo) $(src)nueOOFVSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(nueOOFVSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(nueOOFVSystematics_cppflags) $(nueOOFVSystematics_cxx_cppflags)  $(src)nueOOFVSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(FluxWeightSystematics_cxx_dependencies)

$(bin)$(binobj)FluxWeightSystematics.o : $(FluxWeightSystematics_cxx_dependencies)
	$(cpp_echo) $(src)FluxWeightSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(FluxWeightSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(FluxWeightSystematics_cppflags) $(FluxWeightSystematics_cxx_cppflags)  $(src)FluxWeightSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(nueP0DPileUpSystematics_cxx_dependencies)

$(bin)$(binobj)nueP0DPileUpSystematics.o : $(nueP0DPileUpSystematics_cxx_dependencies)
	$(cpp_echo) $(src)nueP0DPileUpSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(nueP0DPileUpSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(nueP0DPileUpSystematics_cppflags) $(nueP0DPileUpSystematics_cxx_cppflags)  $(src)nueP0DPileUpSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(TPCClusterEffSystematics_cxx_dependencies)

$(bin)$(binobj)TPCClusterEffSystematics.o : $(TPCClusterEffSystematics_cxx_dependencies)
	$(cpp_echo) $(src)TPCClusterEffSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(TPCClusterEffSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(TPCClusterEffSystematics_cppflags) $(TPCClusterEffSystematics_cxx_cppflags)  $(src)TPCClusterEffSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(MichelElectronEffSystematics_cxx_dependencies)

$(bin)$(binobj)MichelElectronEffSystematics.o : $(MichelElectronEffSystematics_cxx_dependencies)
	$(cpp_echo) $(src)MichelElectronEffSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(MichelElectronEffSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(MichelElectronEffSystematics_cppflags) $(MichelElectronEffSystematics_cxx_cppflags)  $(src)MichelElectronEffSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(MomBiasFromVertexMigrationSystematics_cxx_dependencies)

$(bin)$(binobj)MomBiasFromVertexMigrationSystematics.o : $(MomBiasFromVertexMigrationSystematics_cxx_dependencies)
	$(cpp_echo) $(src)MomBiasFromVertexMigrationSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(MomBiasFromVertexMigrationSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(MomBiasFromVertexMigrationSystematics_cppflags) $(MomBiasFromVertexMigrationSystematics_cxx_cppflags)  $(src)MomBiasFromVertexMigrationSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(MomentumResolSystematics_cxx_dependencies)

$(bin)$(binobj)MomentumResolSystematics.o : $(MomentumResolSystematics_cxx_dependencies)
	$(cpp_echo) $(src)MomentumResolSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(MomentumResolSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(MomentumResolSystematics_cppflags) $(MomentumResolSystematics_cxx_cppflags)  $(src)MomentumResolSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(MomRangeResolSystematics_cxx_dependencies)

$(bin)$(binobj)MomRangeResolSystematics.o : $(MomRangeResolSystematics_cxx_dependencies)
	$(cpp_echo) $(src)MomRangeResolSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(MomRangeResolSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(MomRangeResolSystematics_cppflags) $(MomRangeResolSystematics_cxx_cppflags)  $(src)MomRangeResolSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(nueTPCPileUpSystematics_cxx_dependencies)

$(bin)$(binobj)nueTPCPileUpSystematics.o : $(nueTPCPileUpSystematics_cxx_dependencies)
	$(cpp_echo) $(src)nueTPCPileUpSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(nueTPCPileUpSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(nueTPCPileUpSystematics_cppflags) $(nueTPCPileUpSystematics_cxx_cppflags)  $(src)nueTPCPileUpSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(P0DELossScaleSystematics_cxx_dependencies)

$(bin)$(binobj)P0DELossScaleSystematics.o : $(P0DELossScaleSystematics_cxx_dependencies)
	$(cpp_echo) $(src)P0DELossScaleSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(P0DELossScaleSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(P0DELossScaleSystematics_cppflags) $(P0DELossScaleSystematics_cxx_cppflags)  $(src)P0DELossScaleSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(P0DVetoSystematics_cxx_dependencies)

$(bin)$(binobj)P0DVetoSystematics.o : $(P0DVetoSystematics_cxx_dependencies)
	$(cpp_echo) $(src)P0DVetoSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(P0DVetoSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(P0DVetoSystematics_cppflags) $(P0DVetoSystematics_cxx_cppflags)  $(src)P0DVetoSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSystematics_dependencies.make : $(TPCPIDSystematics_cxx_dependencies)

$(bin)$(binobj)TPCPIDSystematics.o : $(TPCPIDSystematics_cxx_dependencies)
	$(cpp_echo) $(src)TPCPIDSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSystematics_pp_cppflags) $(lib_psycheSystematics_pp_cppflags) $(TPCPIDSystematics_pp_cppflags) $(use_cppflags) $(psycheSystematics_cppflags) $(lib_psycheSystematics_cppflags) $(TPCPIDSystematics_cppflags) $(TPCPIDSystematics_cxx_cppflags)  $(src)TPCPIDSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: psycheSystematicsclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(psycheSystematics.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(psycheSystematics.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_psycheSystematics)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheSystematics.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheSystematics.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheSystematics.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

psycheSystematicsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)psycheSystematics$(library_suffix).a $(binobj)$(library_prefix)psycheSystematics$(library_suffix).s? $(binobj)psycheSystematics.stamp $(binobj)psycheSystematics.shstamp
#-- end of cleanup_library ---------------
