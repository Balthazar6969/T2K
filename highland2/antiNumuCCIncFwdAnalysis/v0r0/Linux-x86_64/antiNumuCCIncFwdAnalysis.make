#-- start of make_header -----------------

#====================================
#  Library antiNumuCCIncFwdAnalysis
#
#   Generated Mon Nov  4 15:27:08 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_antiNumuCCIncFwdAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_antiNumuCCIncFwdAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_antiNumuCCIncFwdAnalysis

antiNumuCCIncFwdAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_antiNumuCCIncFwdAnalysis = /tmp/CMT_$(antiNumuCCIncFwdAnalysis_tag)_antiNumuCCIncFwdAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_antiNumuCCIncFwdAnalysis = $(antiNumuCCIncFwdAnalysis_tag)_antiNumuCCIncFwdAnalysis.make
cmt_local_tagfile_antiNumuCCIncFwdAnalysis = $(bin)$(antiNumuCCIncFwdAnalysis_tag)_antiNumuCCIncFwdAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

antiNumuCCIncFwdAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_antiNumuCCIncFwdAnalysis = /tmp/CMT_$(antiNumuCCIncFwdAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_antiNumuCCIncFwdAnalysis = $(antiNumuCCIncFwdAnalysis_tag).make
cmt_local_tagfile_antiNumuCCIncFwdAnalysis = $(bin)$(antiNumuCCIncFwdAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_antiNumuCCIncFwdAnalysis)

ifdef cmt_antiNumuCCIncFwdAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_antiNumuCCIncFwdAnalysis = /tmp/CMT_antiNumuCCIncFwdAnalysis_antiNumuCCIncFwdAnalysissetup.make
cmt_local_antiNumuCCIncFwdAnalysis_makefile = /tmp/CMT_antiNumuCCIncFwdAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_antiNumuCCIncFwdAnalysis = $(bin)antiNumuCCIncFwdAnalysis_antiNumuCCIncFwdAnalysissetup.make
cmt_local_antiNumuCCIncFwdAnalysis_makefile = $(bin)antiNumuCCIncFwdAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_antiNumuCCIncFwdAnalysis = /tmp/CMT_antiNumuCCIncFwdAnalysissetup.make
cmt_local_antiNumuCCIncFwdAnalysis_makefile = /tmp/CMT_antiNumuCCIncFwdAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_antiNumuCCIncFwdAnalysis = $(bin)antiNumuCCIncFwdAnalysissetup.make
cmt_local_antiNumuCCIncFwdAnalysis_makefile = $(bin)antiNumuCCIncFwdAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_antiNumuCCIncFwdAnalysissetup.make
else
cmt_final_setup = $(bin)antiNumuCCIncFwdAnalysissetup.make
endif

antiNumuCCIncFwdAnalysis ::


ifdef READONLY
antiNumuCCIncFwdAnalysis ::
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
	$(echo) 'antiNumuCCIncFwdAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = antiNumuCCIncFwdAnalysis/
antiNumuCCIncFwdAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

antiNumuCCIncFwdAnalysislibname   = $(bin)$(library_prefix)antiNumuCCIncFwdAnalysis$(library_suffix)
antiNumuCCIncFwdAnalysislib       = $(antiNumuCCIncFwdAnalysislibname).a
antiNumuCCIncFwdAnalysisstamp     = $(bin)antiNumuCCIncFwdAnalysis.stamp
antiNumuCCIncFwdAnalysisshstamp   = $(bin)antiNumuCCIncFwdAnalysis.shstamp

antiNumuCCIncFwdAnalysis :: dirs  antiNumuCCIncFwdAnalysisLIB
	$(echo) "antiNumuCCIncFwdAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

antiNumuCCIncFwdAnalysisLIB :: $(antiNumuCCIncFwdAnalysislib) $(antiNumuCCIncFwdAnalysisshstamp)
	$(echo) "antiNumuCCIncFwdAnalysis : library ok"

$(antiNumuCCIncFwdAnalysislib) :: $(bin)ECalMIPPionVariationSystematic.o $(bin)ToFDSECalCorrection.o $(bin)ToFDSECalVariationSystematic.o $(bin)ToFECalOtherVariationSystematic.o $(bin)antiNumuCCIncFwdAnalysis.o $(bin)antiNumuCCIncFwdSelection.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(antiNumuCCIncFwdAnalysislib) $?
	$(lib_silent) $(ranlib) $(antiNumuCCIncFwdAnalysislib)
	$(lib_silent) cat /dev/null >$(antiNumuCCIncFwdAnalysisstamp)

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

$(antiNumuCCIncFwdAnalysislibname).$(shlibsuffix) :: $(antiNumuCCIncFwdAnalysislib) requirements $(use_requirements) $(antiNumuCCIncFwdAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" antiNumuCCIncFwdAnalysis $(antiNumuCCIncFwdAnalysis_shlibflags)

$(antiNumuCCIncFwdAnalysisshstamp) :: $(antiNumuCCIncFwdAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(antiNumuCCIncFwdAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(antiNumuCCIncFwdAnalysisshstamp) ; fi

antiNumuCCIncFwdAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)ECalMIPPionVariationSystematic.o $(bin)ToFDSECalCorrection.o $(bin)ToFDSECalVariationSystematic.o $(bin)ToFECalOtherVariationSystematic.o $(bin)antiNumuCCIncFwdAnalysis.o $(bin)antiNumuCCIncFwdSelection.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf antiNumuCCIncFwdAnalysis_deps antiNumuCCIncFwdAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
antiNumuCCIncFwdAnalysisinstallname = $(library_prefix)antiNumuCCIncFwdAnalysis$(library_suffix).$(shlibsuffix)

antiNumuCCIncFwdAnalysis :: antiNumuCCIncFwdAnalysisinstall

install :: antiNumuCCIncFwdAnalysisinstall

antiNumuCCIncFwdAnalysisinstall :: $(install_dir)/$(antiNumuCCIncFwdAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(antiNumuCCIncFwdAnalysisinstallname) :: $(bin)$(antiNumuCCIncFwdAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(antiNumuCCIncFwdAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##antiNumuCCIncFwdAnalysisclean :: antiNumuCCIncFwdAnalysisuninstall

uninstall :: antiNumuCCIncFwdAnalysisuninstall

antiNumuCCIncFwdAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(antiNumuCCIncFwdAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),antiNumuCCIncFwdAnalysisclean)

#$(bin)antiNumuCCIncFwdAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)antiNumuCCIncFwdAnalysis_dependencies.make : ../src/ECalMIPPionVariationSystematic.cxx ../src/ToFDSECalCorrection.cxx ../src/ToFDSECalVariationSystematic.cxx ../src/ToFECalOtherVariationSystematic.cxx ../src/antiNumuCCIncFwdAnalysis.cxx ../src/antiNumuCCIncFwdSelection.cxx $(use_requirements) $(cmt_final_setup_antiNumuCCIncFwdAnalysis)
	$(echo) "(antiNumuCCIncFwdAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) antiNumuCCIncFwdAnalysis -all_sources -out=$@ ../src/ECalMIPPionVariationSystematic.cxx ../src/ToFDSECalCorrection.cxx ../src/ToFDSECalVariationSystematic.cxx ../src/ToFECalOtherVariationSystematic.cxx ../src/antiNumuCCIncFwdAnalysis.cxx ../src/antiNumuCCIncFwdSelection.cxx
endif

#$(antiNumuCCIncFwdAnalysis_dependencies)

-include $(bin)antiNumuCCIncFwdAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)antiNumuCCIncFwdAnalysis_dependencies.make : $(ECalMIPPionVariationSystematic_cxx_dependencies)

$(bin)$(binobj)ECalMIPPionVariationSystematic.o : $(ECalMIPPionVariationSystematic_cxx_dependencies)
	$(cpp_echo) $(src)ECalMIPPionVariationSystematic.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(antiNumuCCIncFwdAnalysis_pp_cppflags) $(lib_antiNumuCCIncFwdAnalysis_pp_cppflags) $(ECalMIPPionVariationSystematic_pp_cppflags) $(use_cppflags) $(antiNumuCCIncFwdAnalysis_cppflags) $(lib_antiNumuCCIncFwdAnalysis_cppflags) $(ECalMIPPionVariationSystematic_cppflags) $(ECalMIPPionVariationSystematic_cxx_cppflags)  $(src)ECalMIPPionVariationSystematic.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)antiNumuCCIncFwdAnalysis_dependencies.make : $(ToFDSECalCorrection_cxx_dependencies)

$(bin)$(binobj)ToFDSECalCorrection.o : $(ToFDSECalCorrection_cxx_dependencies)
	$(cpp_echo) $(src)ToFDSECalCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(antiNumuCCIncFwdAnalysis_pp_cppflags) $(lib_antiNumuCCIncFwdAnalysis_pp_cppflags) $(ToFDSECalCorrection_pp_cppflags) $(use_cppflags) $(antiNumuCCIncFwdAnalysis_cppflags) $(lib_antiNumuCCIncFwdAnalysis_cppflags) $(ToFDSECalCorrection_cppflags) $(ToFDSECalCorrection_cxx_cppflags)  $(src)ToFDSECalCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)antiNumuCCIncFwdAnalysis_dependencies.make : $(ToFDSECalVariationSystematic_cxx_dependencies)

$(bin)$(binobj)ToFDSECalVariationSystematic.o : $(ToFDSECalVariationSystematic_cxx_dependencies)
	$(cpp_echo) $(src)ToFDSECalVariationSystematic.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(antiNumuCCIncFwdAnalysis_pp_cppflags) $(lib_antiNumuCCIncFwdAnalysis_pp_cppflags) $(ToFDSECalVariationSystematic_pp_cppflags) $(use_cppflags) $(antiNumuCCIncFwdAnalysis_cppflags) $(lib_antiNumuCCIncFwdAnalysis_cppflags) $(ToFDSECalVariationSystematic_cppflags) $(ToFDSECalVariationSystematic_cxx_cppflags)  $(src)ToFDSECalVariationSystematic.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)antiNumuCCIncFwdAnalysis_dependencies.make : $(ToFECalOtherVariationSystematic_cxx_dependencies)

$(bin)$(binobj)ToFECalOtherVariationSystematic.o : $(ToFECalOtherVariationSystematic_cxx_dependencies)
	$(cpp_echo) $(src)ToFECalOtherVariationSystematic.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(antiNumuCCIncFwdAnalysis_pp_cppflags) $(lib_antiNumuCCIncFwdAnalysis_pp_cppflags) $(ToFECalOtherVariationSystematic_pp_cppflags) $(use_cppflags) $(antiNumuCCIncFwdAnalysis_cppflags) $(lib_antiNumuCCIncFwdAnalysis_cppflags) $(ToFECalOtherVariationSystematic_cppflags) $(ToFECalOtherVariationSystematic_cxx_cppflags)  $(src)ToFECalOtherVariationSystematic.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)antiNumuCCIncFwdAnalysis_dependencies.make : $(antiNumuCCIncFwdAnalysis_cxx_dependencies)

$(bin)$(binobj)antiNumuCCIncFwdAnalysis.o : $(antiNumuCCIncFwdAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)antiNumuCCIncFwdAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(antiNumuCCIncFwdAnalysis_pp_cppflags) $(lib_antiNumuCCIncFwdAnalysis_pp_cppflags) $(antiNumuCCIncFwdAnalysis_pp_cppflags) $(use_cppflags) $(antiNumuCCIncFwdAnalysis_cppflags) $(lib_antiNumuCCIncFwdAnalysis_cppflags) $(antiNumuCCIncFwdAnalysis_cppflags) $(antiNumuCCIncFwdAnalysis_cxx_cppflags)  $(src)antiNumuCCIncFwdAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)antiNumuCCIncFwdAnalysis_dependencies.make : $(antiNumuCCIncFwdSelection_cxx_dependencies)

$(bin)$(binobj)antiNumuCCIncFwdSelection.o : $(antiNumuCCIncFwdSelection_cxx_dependencies)
	$(cpp_echo) $(src)antiNumuCCIncFwdSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(antiNumuCCIncFwdAnalysis_pp_cppflags) $(lib_antiNumuCCIncFwdAnalysis_pp_cppflags) $(antiNumuCCIncFwdSelection_pp_cppflags) $(use_cppflags) $(antiNumuCCIncFwdAnalysis_cppflags) $(lib_antiNumuCCIncFwdAnalysis_cppflags) $(antiNumuCCIncFwdSelection_cppflags) $(antiNumuCCIncFwdSelection_cxx_cppflags)  $(src)antiNumuCCIncFwdSelection.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: antiNumuCCIncFwdAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(antiNumuCCIncFwdAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(antiNumuCCIncFwdAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_antiNumuCCIncFwdAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNumuCCIncFwdAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNumuCCIncFwdAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNumuCCIncFwdAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

antiNumuCCIncFwdAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)antiNumuCCIncFwdAnalysis$(library_suffix).a $(binobj)$(library_prefix)antiNumuCCIncFwdAnalysis$(library_suffix).s? $(binobj)antiNumuCCIncFwdAnalysis.stamp $(binobj)antiNumuCCIncFwdAnalysis.shstamp
#-- end of cleanup_library ---------------

