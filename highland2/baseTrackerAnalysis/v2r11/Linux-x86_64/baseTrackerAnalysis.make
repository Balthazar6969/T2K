#-- start of make_header -----------------

#====================================
#  Library baseTrackerAnalysis
#
#   Generated Mon Nov  4 15:27:02 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_baseTrackerAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_baseTrackerAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_baseTrackerAnalysis

baseTrackerAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_baseTrackerAnalysis = /tmp/CMT_$(baseTrackerAnalysis_tag)_baseTrackerAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_baseTrackerAnalysis = $(baseTrackerAnalysis_tag)_baseTrackerAnalysis.make
cmt_local_tagfile_baseTrackerAnalysis = $(bin)$(baseTrackerAnalysis_tag)_baseTrackerAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

baseTrackerAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_baseTrackerAnalysis = /tmp/CMT_$(baseTrackerAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_baseTrackerAnalysis = $(baseTrackerAnalysis_tag).make
cmt_local_tagfile_baseTrackerAnalysis = $(bin)$(baseTrackerAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_baseTrackerAnalysis)

ifdef cmt_baseTrackerAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_baseTrackerAnalysis = /tmp/CMT_baseTrackerAnalysis_baseTrackerAnalysissetup.make
cmt_local_baseTrackerAnalysis_makefile = /tmp/CMT_baseTrackerAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_baseTrackerAnalysis = $(bin)baseTrackerAnalysis_baseTrackerAnalysissetup.make
cmt_local_baseTrackerAnalysis_makefile = $(bin)baseTrackerAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_baseTrackerAnalysis = /tmp/CMT_baseTrackerAnalysissetup.make
cmt_local_baseTrackerAnalysis_makefile = /tmp/CMT_baseTrackerAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_baseTrackerAnalysis = $(bin)baseTrackerAnalysissetup.make
cmt_local_baseTrackerAnalysis_makefile = $(bin)baseTrackerAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_baseTrackerAnalysissetup.make
else
cmt_final_setup = $(bin)baseTrackerAnalysissetup.make
endif

baseTrackerAnalysis ::


ifdef READONLY
baseTrackerAnalysis ::
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
	$(echo) 'baseTrackerAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = baseTrackerAnalysis/
baseTrackerAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

baseTrackerAnalysislibname   = $(bin)$(library_prefix)baseTrackerAnalysis$(library_suffix)
baseTrackerAnalysislib       = $(baseTrackerAnalysislibname).a
baseTrackerAnalysisstamp     = $(bin)baseTrackerAnalysis.stamp
baseTrackerAnalysisshstamp   = $(bin)baseTrackerAnalysis.shstamp

baseTrackerAnalysis :: dirs  baseTrackerAnalysisLIB
	$(echo) "baseTrackerAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

baseTrackerAnalysisLIB :: $(baseTrackerAnalysislib) $(baseTrackerAnalysisshstamp)
	$(echo) "baseTrackerAnalysis : library ok"

$(baseTrackerAnalysislib) :: $(bin)baseTrackerAnalysis.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(baseTrackerAnalysislib) $?
	$(lib_silent) $(ranlib) $(baseTrackerAnalysislib)
	$(lib_silent) cat /dev/null >$(baseTrackerAnalysisstamp)

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

$(baseTrackerAnalysislibname).$(shlibsuffix) :: $(baseTrackerAnalysislib) requirements $(use_requirements) $(baseTrackerAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" baseTrackerAnalysis $(baseTrackerAnalysis_shlibflags)

$(baseTrackerAnalysisshstamp) :: $(baseTrackerAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(baseTrackerAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(baseTrackerAnalysisshstamp) ; fi

baseTrackerAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)baseTrackerAnalysis.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf baseTrackerAnalysis_deps baseTrackerAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
baseTrackerAnalysisinstallname = $(library_prefix)baseTrackerAnalysis$(library_suffix).$(shlibsuffix)

baseTrackerAnalysis :: baseTrackerAnalysisinstall

install :: baseTrackerAnalysisinstall

baseTrackerAnalysisinstall :: $(install_dir)/$(baseTrackerAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(baseTrackerAnalysisinstallname) :: $(bin)$(baseTrackerAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(baseTrackerAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##baseTrackerAnalysisclean :: baseTrackerAnalysisuninstall

uninstall :: baseTrackerAnalysisuninstall

baseTrackerAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(baseTrackerAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),baseTrackerAnalysisclean)

#$(bin)baseTrackerAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)baseTrackerAnalysis_dependencies.make : ../src/baseTrackerAnalysis.cxx $(use_requirements) $(cmt_final_setup_baseTrackerAnalysis)
	$(echo) "(baseTrackerAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) baseTrackerAnalysis -all_sources -out=$@ ../src/baseTrackerAnalysis.cxx
endif

#$(baseTrackerAnalysis_dependencies)

-include $(bin)baseTrackerAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)baseTrackerAnalysis_dependencies.make : $(baseTrackerAnalysis_cxx_dependencies)

$(bin)$(binobj)baseTrackerAnalysis.o : $(baseTrackerAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)baseTrackerAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(baseTrackerAnalysis_pp_cppflags) $(lib_baseTrackerAnalysis_pp_cppflags) $(baseTrackerAnalysis_pp_cppflags) $(use_cppflags) $(baseTrackerAnalysis_cppflags) $(lib_baseTrackerAnalysis_cppflags) $(baseTrackerAnalysis_cppflags) $(baseTrackerAnalysis_cxx_cppflags)  $(src)baseTrackerAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: baseTrackerAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(baseTrackerAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(baseTrackerAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_baseTrackerAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(baseTrackerAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(baseTrackerAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(baseTrackerAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

baseTrackerAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)baseTrackerAnalysis$(library_suffix).a $(binobj)$(library_prefix)baseTrackerAnalysis$(library_suffix).s? $(binobj)baseTrackerAnalysis.stamp $(binobj)baseTrackerAnalysis.shstamp
#-- end of cleanup_library ---------------

