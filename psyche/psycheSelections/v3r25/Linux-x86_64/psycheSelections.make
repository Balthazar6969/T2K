#-- start of make_header -----------------

#====================================
#  Library psycheSelections
#
#   Generated Mon Nov  4 15:26:52 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_psycheSelections_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_psycheSelections_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_psycheSelections

psycheSelections_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheSelections = /tmp/CMT_$(psycheSelections_tag)_psycheSelections.make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheSelections = $(psycheSelections_tag)_psycheSelections.make
cmt_local_tagfile_psycheSelections = $(bin)$(psycheSelections_tag)_psycheSelections.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheSelections_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheSelections = /tmp/CMT_$(psycheSelections_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheSelections = $(psycheSelections_tag).make
cmt_local_tagfile_psycheSelections = $(bin)$(psycheSelections_tag).make
endif

endif

-include $(cmt_local_tagfile_psycheSelections)

ifdef cmt_psycheSelections_has_target_tag

ifdef READONLY
cmt_final_setup_psycheSelections = /tmp/CMT_psycheSelections_psycheSelectionssetup.make
cmt_local_psycheSelections_makefile = /tmp/CMT_psycheSelections$(cmt_lock_pid).make
else
cmt_final_setup_psycheSelections = $(bin)psycheSelections_psycheSelectionssetup.make
cmt_local_psycheSelections_makefile = $(bin)psycheSelections.make
endif

else

ifdef READONLY
cmt_final_setup_psycheSelections = /tmp/CMT_psycheSelectionssetup.make
cmt_local_psycheSelections_makefile = /tmp/CMT_psycheSelections$(cmt_lock_pid).make
else
cmt_final_setup_psycheSelections = $(bin)psycheSelectionssetup.make
cmt_local_psycheSelections_makefile = $(bin)psycheSelections.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheSelectionssetup.make
else
cmt_final_setup = $(bin)psycheSelectionssetup.make
endif

psycheSelections ::


ifdef READONLY
psycheSelections ::
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
	$(echo) 'psycheSelections'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = psycheSelections/
psycheSelections::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

psycheSelectionslibname   = $(bin)$(library_prefix)psycheSelections$(library_suffix)
psycheSelectionslib       = $(psycheSelectionslibname).a
psycheSelectionsstamp     = $(bin)psycheSelections.stamp
psycheSelectionsshstamp   = $(bin)psycheSelections.shstamp

psycheSelections :: dirs  psycheSelectionsLIB
	$(echo) "psycheSelections ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

psycheSelectionsLIB :: $(psycheSelectionslib) $(psycheSelectionsshstamp)
	$(echo) "psycheSelections : library ok"

$(psycheSelectionslib) :: $(bin)SystematicTuning.o $(bin)ToyBoxND280.o $(bin)ToyBoxTracker.o $(bin)antiNueCCFGD2Selection.o $(bin)p0dNumuCCSelection.o $(bin)antiNueCCSelection.o $(bin)trackerSelectionUtils.o $(bin)antiNumuCCFGD2Selection.o $(bin)antiNumuCCMultiPiFGD2Selection.o $(bin)numuCCFGD2Selection.o $(bin)antiNumuCCMultiPiSelection.o $(bin)numuCCMultiPiFGD2Selection.o $(bin)antiNumuCCMultiTrackFGD2Selection.o $(bin)numuCCMultiPiSelection.o $(bin)antiNumuCCMultiTrackSelection.o $(bin)numuCCSelection.o $(bin)antiNumuCCSelection.o $(bin)baseSelection.o $(bin)gammaFGD2Selection.o $(bin)gammaSelection.o $(bin)nueCCFGD2Selection.o $(bin)nueCCSelection.o $(bin)numuBkgInAntiNuModeCCFGD2Selection.o $(bin)numuCC4piFGD2Selection.o $(bin)numuBkgInAntiNuModeCCMultiPiFGD2Selection.o $(bin)numuCC4piMultiPiFGD2Selection.o $(bin)numuBkgInAntiNuModeCCMultiPiSelection.o $(bin)numuCC4piMultiPiSelection.o $(bin)numuBkgInAntiNuModeCCMultiTrackFGD2Selection.o $(bin)numuBkgInAntiNuModeCCSelection.o $(bin)numuBkgInAntiNuModeCCMultiTrackSelection.o $(bin)numuCC4piSelection.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(psycheSelectionslib) $?
	$(lib_silent) $(ranlib) $(psycheSelectionslib)
	$(lib_silent) cat /dev/null >$(psycheSelectionsstamp)

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

$(psycheSelectionslibname).$(shlibsuffix) :: $(psycheSelectionslib) requirements $(use_requirements) $(psycheSelectionsstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" psycheSelections $(psycheSelections_shlibflags)

$(psycheSelectionsshstamp) :: $(psycheSelectionslibname).$(shlibsuffix)
	$(lib_silent) if test -f $(psycheSelectionslibname).$(shlibsuffix) ; then cat /dev/null >$(psycheSelectionsshstamp) ; fi

psycheSelectionsclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)SystematicTuning.o $(bin)ToyBoxND280.o $(bin)ToyBoxTracker.o $(bin)antiNueCCFGD2Selection.o $(bin)p0dNumuCCSelection.o $(bin)antiNueCCSelection.o $(bin)trackerSelectionUtils.o $(bin)antiNumuCCFGD2Selection.o $(bin)antiNumuCCMultiPiFGD2Selection.o $(bin)numuCCFGD2Selection.o $(bin)antiNumuCCMultiPiSelection.o $(bin)numuCCMultiPiFGD2Selection.o $(bin)antiNumuCCMultiTrackFGD2Selection.o $(bin)numuCCMultiPiSelection.o $(bin)antiNumuCCMultiTrackSelection.o $(bin)numuCCSelection.o $(bin)antiNumuCCSelection.o $(bin)baseSelection.o $(bin)gammaFGD2Selection.o $(bin)gammaSelection.o $(bin)nueCCFGD2Selection.o $(bin)nueCCSelection.o $(bin)numuBkgInAntiNuModeCCFGD2Selection.o $(bin)numuCC4piFGD2Selection.o $(bin)numuBkgInAntiNuModeCCMultiPiFGD2Selection.o $(bin)numuCC4piMultiPiFGD2Selection.o $(bin)numuBkgInAntiNuModeCCMultiPiSelection.o $(bin)numuCC4piMultiPiSelection.o $(bin)numuBkgInAntiNuModeCCMultiTrackFGD2Selection.o $(bin)numuBkgInAntiNuModeCCSelection.o $(bin)numuBkgInAntiNuModeCCMultiTrackSelection.o $(bin)numuCC4piSelection.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf psycheSelections_deps psycheSelections_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
psycheSelectionsinstallname = $(library_prefix)psycheSelections$(library_suffix).$(shlibsuffix)

psycheSelections :: psycheSelectionsinstall

install :: psycheSelectionsinstall

psycheSelectionsinstall :: $(install_dir)/$(psycheSelectionsinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(psycheSelectionsinstallname) :: $(bin)$(psycheSelectionsinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheSelectionsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##psycheSelectionsclean :: psycheSelectionsuninstall

uninstall :: psycheSelectionsuninstall

psycheSelectionsuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheSelectionsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),psycheSelectionsclean)

#$(bin)psycheSelections_dependencies.make :: dirs

ifndef QUICK
$(bin)psycheSelections_dependencies.make : ../src/SystematicTuning.cxx ../src/ToyBoxND280.cxx ../src/ToyBoxTracker.cxx ../src/antiNueCCFGD2Selection.cxx ../src/p0dNumuCCSelection.cxx ../src/antiNueCCSelection.cxx ../src/trackerSelectionUtils.cxx ../src/antiNumuCCFGD2Selection.cxx ../src/antiNumuCCMultiPiFGD2Selection.cxx ../src/numuCCFGD2Selection.cxx ../src/antiNumuCCMultiPiSelection.cxx ../src/numuCCMultiPiFGD2Selection.cxx ../src/antiNumuCCMultiTrackFGD2Selection.cxx ../src/numuCCMultiPiSelection.cxx ../src/antiNumuCCMultiTrackSelection.cxx ../src/numuCCSelection.cxx ../src/antiNumuCCSelection.cxx ../src/baseSelection.cxx ../src/gammaFGD2Selection.cxx ../src/gammaSelection.cxx ../src/nueCCFGD2Selection.cxx ../src/nueCCSelection.cxx ../src/numuBkgInAntiNuModeCCFGD2Selection.cxx ../src/numuCC4piFGD2Selection.cxx ../src/numuBkgInAntiNuModeCCMultiPiFGD2Selection.cxx ../src/numuCC4piMultiPiFGD2Selection.cxx ../src/numuBkgInAntiNuModeCCMultiPiSelection.cxx ../src/numuCC4piMultiPiSelection.cxx ../src/numuBkgInAntiNuModeCCMultiTrackFGD2Selection.cxx ../src/numuBkgInAntiNuModeCCSelection.cxx ../src/numuBkgInAntiNuModeCCMultiTrackSelection.cxx ../src/numuCC4piSelection.cxx $(use_requirements) $(cmt_final_setup_psycheSelections)
	$(echo) "(psycheSelections.make) Rebuilding $@"; \
	  $(build_dependencies) psycheSelections -all_sources -out=$@ ../src/SystematicTuning.cxx ../src/ToyBoxND280.cxx ../src/ToyBoxTracker.cxx ../src/antiNueCCFGD2Selection.cxx ../src/p0dNumuCCSelection.cxx ../src/antiNueCCSelection.cxx ../src/trackerSelectionUtils.cxx ../src/antiNumuCCFGD2Selection.cxx ../src/antiNumuCCMultiPiFGD2Selection.cxx ../src/numuCCFGD2Selection.cxx ../src/antiNumuCCMultiPiSelection.cxx ../src/numuCCMultiPiFGD2Selection.cxx ../src/antiNumuCCMultiTrackFGD2Selection.cxx ../src/numuCCMultiPiSelection.cxx ../src/antiNumuCCMultiTrackSelection.cxx ../src/numuCCSelection.cxx ../src/antiNumuCCSelection.cxx ../src/baseSelection.cxx ../src/gammaFGD2Selection.cxx ../src/gammaSelection.cxx ../src/nueCCFGD2Selection.cxx ../src/nueCCSelection.cxx ../src/numuBkgInAntiNuModeCCFGD2Selection.cxx ../src/numuCC4piFGD2Selection.cxx ../src/numuBkgInAntiNuModeCCMultiPiFGD2Selection.cxx ../src/numuCC4piMultiPiFGD2Selection.cxx ../src/numuBkgInAntiNuModeCCMultiPiSelection.cxx ../src/numuCC4piMultiPiSelection.cxx ../src/numuBkgInAntiNuModeCCMultiTrackFGD2Selection.cxx ../src/numuBkgInAntiNuModeCCSelection.cxx ../src/numuBkgInAntiNuModeCCMultiTrackSelection.cxx ../src/numuCC4piSelection.cxx
endif

#$(psycheSelections_dependencies)

-include $(bin)psycheSelections_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(SystematicTuning_cxx_dependencies)

$(bin)$(binobj)SystematicTuning.o : $(SystematicTuning_cxx_dependencies)
	$(cpp_echo) $(src)SystematicTuning.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(SystematicTuning_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(SystematicTuning_cppflags) $(SystematicTuning_cxx_cppflags)  $(src)SystematicTuning.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(ToyBoxND280_cxx_dependencies)

$(bin)$(binobj)ToyBoxND280.o : $(ToyBoxND280_cxx_dependencies)
	$(cpp_echo) $(src)ToyBoxND280.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(ToyBoxND280_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(ToyBoxND280_cppflags) $(ToyBoxND280_cxx_cppflags)  $(src)ToyBoxND280.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(ToyBoxTracker_cxx_dependencies)

$(bin)$(binobj)ToyBoxTracker.o : $(ToyBoxTracker_cxx_dependencies)
	$(cpp_echo) $(src)ToyBoxTracker.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(ToyBoxTracker_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(ToyBoxTracker_cppflags) $(ToyBoxTracker_cxx_cppflags)  $(src)ToyBoxTracker.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(antiNueCCFGD2Selection_cxx_dependencies)

$(bin)$(binobj)antiNueCCFGD2Selection.o : $(antiNueCCFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)antiNueCCFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(antiNueCCFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(antiNueCCFGD2Selection_cppflags) $(antiNueCCFGD2Selection_cxx_cppflags)  $(src)antiNueCCFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(p0dNumuCCSelection_cxx_dependencies)

$(bin)$(binobj)p0dNumuCCSelection.o : $(p0dNumuCCSelection_cxx_dependencies)
	$(cpp_echo) $(src)p0dNumuCCSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(p0dNumuCCSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(p0dNumuCCSelection_cppflags) $(p0dNumuCCSelection_cxx_cppflags)  $(src)p0dNumuCCSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(antiNueCCSelection_cxx_dependencies)

$(bin)$(binobj)antiNueCCSelection.o : $(antiNueCCSelection_cxx_dependencies)
	$(cpp_echo) $(src)antiNueCCSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(antiNueCCSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(antiNueCCSelection_cppflags) $(antiNueCCSelection_cxx_cppflags)  $(src)antiNueCCSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(trackerSelectionUtils_cxx_dependencies)

$(bin)$(binobj)trackerSelectionUtils.o : $(trackerSelectionUtils_cxx_dependencies)
	$(cpp_echo) $(src)trackerSelectionUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(trackerSelectionUtils_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(trackerSelectionUtils_cppflags) $(trackerSelectionUtils_cxx_cppflags)  $(src)trackerSelectionUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(antiNumuCCFGD2Selection_cxx_dependencies)

$(bin)$(binobj)antiNumuCCFGD2Selection.o : $(antiNumuCCFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)antiNumuCCFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(antiNumuCCFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(antiNumuCCFGD2Selection_cppflags) $(antiNumuCCFGD2Selection_cxx_cppflags)  $(src)antiNumuCCFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(antiNumuCCMultiPiFGD2Selection_cxx_dependencies)

$(bin)$(binobj)antiNumuCCMultiPiFGD2Selection.o : $(antiNumuCCMultiPiFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)antiNumuCCMultiPiFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(antiNumuCCMultiPiFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(antiNumuCCMultiPiFGD2Selection_cppflags) $(antiNumuCCMultiPiFGD2Selection_cxx_cppflags)  $(src)antiNumuCCMultiPiFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuCCFGD2Selection_cxx_dependencies)

$(bin)$(binobj)numuCCFGD2Selection.o : $(numuCCFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)numuCCFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuCCFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuCCFGD2Selection_cppflags) $(numuCCFGD2Selection_cxx_cppflags)  $(src)numuCCFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(antiNumuCCMultiPiSelection_cxx_dependencies)

$(bin)$(binobj)antiNumuCCMultiPiSelection.o : $(antiNumuCCMultiPiSelection_cxx_dependencies)
	$(cpp_echo) $(src)antiNumuCCMultiPiSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(antiNumuCCMultiPiSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(antiNumuCCMultiPiSelection_cppflags) $(antiNumuCCMultiPiSelection_cxx_cppflags)  $(src)antiNumuCCMultiPiSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuCCMultiPiFGD2Selection_cxx_dependencies)

$(bin)$(binobj)numuCCMultiPiFGD2Selection.o : $(numuCCMultiPiFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)numuCCMultiPiFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuCCMultiPiFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuCCMultiPiFGD2Selection_cppflags) $(numuCCMultiPiFGD2Selection_cxx_cppflags)  $(src)numuCCMultiPiFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(antiNumuCCMultiTrackFGD2Selection_cxx_dependencies)

$(bin)$(binobj)antiNumuCCMultiTrackFGD2Selection.o : $(antiNumuCCMultiTrackFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)antiNumuCCMultiTrackFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(antiNumuCCMultiTrackFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(antiNumuCCMultiTrackFGD2Selection_cppflags) $(antiNumuCCMultiTrackFGD2Selection_cxx_cppflags)  $(src)antiNumuCCMultiTrackFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuCCMultiPiSelection_cxx_dependencies)

$(bin)$(binobj)numuCCMultiPiSelection.o : $(numuCCMultiPiSelection_cxx_dependencies)
	$(cpp_echo) $(src)numuCCMultiPiSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuCCMultiPiSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuCCMultiPiSelection_cppflags) $(numuCCMultiPiSelection_cxx_cppflags)  $(src)numuCCMultiPiSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(antiNumuCCMultiTrackSelection_cxx_dependencies)

$(bin)$(binobj)antiNumuCCMultiTrackSelection.o : $(antiNumuCCMultiTrackSelection_cxx_dependencies)
	$(cpp_echo) $(src)antiNumuCCMultiTrackSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(antiNumuCCMultiTrackSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(antiNumuCCMultiTrackSelection_cppflags) $(antiNumuCCMultiTrackSelection_cxx_cppflags)  $(src)antiNumuCCMultiTrackSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuCCSelection_cxx_dependencies)

$(bin)$(binobj)numuCCSelection.o : $(numuCCSelection_cxx_dependencies)
	$(cpp_echo) $(src)numuCCSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuCCSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuCCSelection_cppflags) $(numuCCSelection_cxx_cppflags)  $(src)numuCCSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(antiNumuCCSelection_cxx_dependencies)

$(bin)$(binobj)antiNumuCCSelection.o : $(antiNumuCCSelection_cxx_dependencies)
	$(cpp_echo) $(src)antiNumuCCSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(antiNumuCCSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(antiNumuCCSelection_cppflags) $(antiNumuCCSelection_cxx_cppflags)  $(src)antiNumuCCSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(baseSelection_cxx_dependencies)

$(bin)$(binobj)baseSelection.o : $(baseSelection_cxx_dependencies)
	$(cpp_echo) $(src)baseSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(baseSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(baseSelection_cppflags) $(baseSelection_cxx_cppflags)  $(src)baseSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(gammaFGD2Selection_cxx_dependencies)

$(bin)$(binobj)gammaFGD2Selection.o : $(gammaFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)gammaFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(gammaFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(gammaFGD2Selection_cppflags) $(gammaFGD2Selection_cxx_cppflags)  $(src)gammaFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(gammaSelection_cxx_dependencies)

$(bin)$(binobj)gammaSelection.o : $(gammaSelection_cxx_dependencies)
	$(cpp_echo) $(src)gammaSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(gammaSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(gammaSelection_cppflags) $(gammaSelection_cxx_cppflags)  $(src)gammaSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(nueCCFGD2Selection_cxx_dependencies)

$(bin)$(binobj)nueCCFGD2Selection.o : $(nueCCFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)nueCCFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(nueCCFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(nueCCFGD2Selection_cppflags) $(nueCCFGD2Selection_cxx_cppflags)  $(src)nueCCFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(nueCCSelection_cxx_dependencies)

$(bin)$(binobj)nueCCSelection.o : $(nueCCSelection_cxx_dependencies)
	$(cpp_echo) $(src)nueCCSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(nueCCSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(nueCCSelection_cppflags) $(nueCCSelection_cxx_cppflags)  $(src)nueCCSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuBkgInAntiNuModeCCFGD2Selection_cxx_dependencies)

$(bin)$(binobj)numuBkgInAntiNuModeCCFGD2Selection.o : $(numuBkgInAntiNuModeCCFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)numuBkgInAntiNuModeCCFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuBkgInAntiNuModeCCFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuBkgInAntiNuModeCCFGD2Selection_cppflags) $(numuBkgInAntiNuModeCCFGD2Selection_cxx_cppflags)  $(src)numuBkgInAntiNuModeCCFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuCC4piFGD2Selection_cxx_dependencies)

$(bin)$(binobj)numuCC4piFGD2Selection.o : $(numuCC4piFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)numuCC4piFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuCC4piFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuCC4piFGD2Selection_cppflags) $(numuCC4piFGD2Selection_cxx_cppflags)  $(src)numuCC4piFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuBkgInAntiNuModeCCMultiPiFGD2Selection_cxx_dependencies)

$(bin)$(binobj)numuBkgInAntiNuModeCCMultiPiFGD2Selection.o : $(numuBkgInAntiNuModeCCMultiPiFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)numuBkgInAntiNuModeCCMultiPiFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuBkgInAntiNuModeCCMultiPiFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuBkgInAntiNuModeCCMultiPiFGD2Selection_cppflags) $(numuBkgInAntiNuModeCCMultiPiFGD2Selection_cxx_cppflags)  $(src)numuBkgInAntiNuModeCCMultiPiFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuCC4piMultiPiFGD2Selection_cxx_dependencies)

$(bin)$(binobj)numuCC4piMultiPiFGD2Selection.o : $(numuCC4piMultiPiFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)numuCC4piMultiPiFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuCC4piMultiPiFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuCC4piMultiPiFGD2Selection_cppflags) $(numuCC4piMultiPiFGD2Selection_cxx_cppflags)  $(src)numuCC4piMultiPiFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuBkgInAntiNuModeCCMultiPiSelection_cxx_dependencies)

$(bin)$(binobj)numuBkgInAntiNuModeCCMultiPiSelection.o : $(numuBkgInAntiNuModeCCMultiPiSelection_cxx_dependencies)
	$(cpp_echo) $(src)numuBkgInAntiNuModeCCMultiPiSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuBkgInAntiNuModeCCMultiPiSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuBkgInAntiNuModeCCMultiPiSelection_cppflags) $(numuBkgInAntiNuModeCCMultiPiSelection_cxx_cppflags)  $(src)numuBkgInAntiNuModeCCMultiPiSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuCC4piMultiPiSelection_cxx_dependencies)

$(bin)$(binobj)numuCC4piMultiPiSelection.o : $(numuCC4piMultiPiSelection_cxx_dependencies)
	$(cpp_echo) $(src)numuCC4piMultiPiSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuCC4piMultiPiSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuCC4piMultiPiSelection_cppflags) $(numuCC4piMultiPiSelection_cxx_cppflags)  $(src)numuCC4piMultiPiSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuBkgInAntiNuModeCCMultiTrackFGD2Selection_cxx_dependencies)

$(bin)$(binobj)numuBkgInAntiNuModeCCMultiTrackFGD2Selection.o : $(numuBkgInAntiNuModeCCMultiTrackFGD2Selection_cxx_dependencies)
	$(cpp_echo) $(src)numuBkgInAntiNuModeCCMultiTrackFGD2Selection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuBkgInAntiNuModeCCMultiTrackFGD2Selection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuBkgInAntiNuModeCCMultiTrackFGD2Selection_cppflags) $(numuBkgInAntiNuModeCCMultiTrackFGD2Selection_cxx_cppflags)  $(src)numuBkgInAntiNuModeCCMultiTrackFGD2Selection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuBkgInAntiNuModeCCSelection_cxx_dependencies)

$(bin)$(binobj)numuBkgInAntiNuModeCCSelection.o : $(numuBkgInAntiNuModeCCSelection_cxx_dependencies)
	$(cpp_echo) $(src)numuBkgInAntiNuModeCCSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuBkgInAntiNuModeCCSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuBkgInAntiNuModeCCSelection_cppflags) $(numuBkgInAntiNuModeCCSelection_cxx_cppflags)  $(src)numuBkgInAntiNuModeCCSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuBkgInAntiNuModeCCMultiTrackSelection_cxx_dependencies)

$(bin)$(binobj)numuBkgInAntiNuModeCCMultiTrackSelection.o : $(numuBkgInAntiNuModeCCMultiTrackSelection_cxx_dependencies)
	$(cpp_echo) $(src)numuBkgInAntiNuModeCCMultiTrackSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuBkgInAntiNuModeCCMultiTrackSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuBkgInAntiNuModeCCMultiTrackSelection_cppflags) $(numuBkgInAntiNuModeCCMultiTrackSelection_cxx_cppflags)  $(src)numuBkgInAntiNuModeCCMultiTrackSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSelections_dependencies.make : $(numuCC4piSelection_cxx_dependencies)

$(bin)$(binobj)numuCC4piSelection.o : $(numuCC4piSelection_cxx_dependencies)
	$(cpp_echo) $(src)numuCC4piSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSelections_pp_cppflags) $(lib_psycheSelections_pp_cppflags) $(numuCC4piSelection_pp_cppflags) $(use_cppflags) $(psycheSelections_cppflags) $(lib_psycheSelections_cppflags) $(numuCC4piSelection_cppflags) $(numuCC4piSelection_cxx_cppflags)  $(src)numuCC4piSelection.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: psycheSelectionsclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(psycheSelections.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(psycheSelections.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_psycheSelections)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheSelections.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheSelections.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheSelections.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

psycheSelectionsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)psycheSelections$(library_suffix).a $(binobj)$(library_prefix)psycheSelections$(library_suffix).s? $(binobj)psycheSelections.stamp $(binobj)psycheSelections.shstamp
#-- end of cleanup_library ---------------
