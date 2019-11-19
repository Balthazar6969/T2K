#-- start of make_header -----------------

#====================================
#  Library numuCCAnalysis
#
#   Generated Mon Nov  4 15:27:04 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_numuCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_numuCCAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_numuCCAnalysis

numuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_numuCCAnalysis = /tmp/CMT_$(numuCCAnalysis_tag)_numuCCAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_numuCCAnalysis = $(numuCCAnalysis_tag)_numuCCAnalysis.make
cmt_local_tagfile_numuCCAnalysis = $(bin)$(numuCCAnalysis_tag)_numuCCAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

numuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_numuCCAnalysis = /tmp/CMT_$(numuCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_numuCCAnalysis = $(numuCCAnalysis_tag).make
cmt_local_tagfile_numuCCAnalysis = $(bin)$(numuCCAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_numuCCAnalysis)

ifdef cmt_numuCCAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_numuCCAnalysis = /tmp/CMT_numuCCAnalysis_numuCCAnalysissetup.make
cmt_local_numuCCAnalysis_makefile = /tmp/CMT_numuCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_numuCCAnalysis = $(bin)numuCCAnalysis_numuCCAnalysissetup.make
cmt_local_numuCCAnalysis_makefile = $(bin)numuCCAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_numuCCAnalysis = /tmp/CMT_numuCCAnalysissetup.make
cmt_local_numuCCAnalysis_makefile = /tmp/CMT_numuCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_numuCCAnalysis = $(bin)numuCCAnalysissetup.make
cmt_local_numuCCAnalysis_makefile = $(bin)numuCCAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_numuCCAnalysissetup.make
else
cmt_final_setup = $(bin)numuCCAnalysissetup.make
endif

numuCCAnalysis ::


ifdef READONLY
numuCCAnalysis ::
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
	$(echo) 'numuCCAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = numuCCAnalysis/
numuCCAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

numuCCAnalysislibname   = $(bin)$(library_prefix)numuCCAnalysis$(library_suffix)
numuCCAnalysislib       = $(numuCCAnalysislibname).a
numuCCAnalysisstamp     = $(bin)numuCCAnalysis.stamp
numuCCAnalysisshstamp   = $(bin)numuCCAnalysis.shstamp

numuCCAnalysis :: dirs  numuCCAnalysisLIB
	$(echo) "numuCCAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

numuCCAnalysisLIB :: $(numuCCAnalysislib) $(numuCCAnalysisshstamp)
	$(echo) "numuCCAnalysis : library ok"

$(numuCCAnalysislib) :: $(bin)numuCCAnalysis.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(numuCCAnalysislib) $?
	$(lib_silent) $(ranlib) $(numuCCAnalysislib)
	$(lib_silent) cat /dev/null >$(numuCCAnalysisstamp)

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

$(numuCCAnalysislibname).$(shlibsuffix) :: $(numuCCAnalysislib) requirements $(use_requirements) $(numuCCAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" numuCCAnalysis $(numuCCAnalysis_shlibflags)

$(numuCCAnalysisshstamp) :: $(numuCCAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(numuCCAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(numuCCAnalysisshstamp) ; fi

numuCCAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)numuCCAnalysis.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf numuCCAnalysis_deps numuCCAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
numuCCAnalysisinstallname = $(library_prefix)numuCCAnalysis$(library_suffix).$(shlibsuffix)

numuCCAnalysis :: numuCCAnalysisinstall

install :: numuCCAnalysisinstall

numuCCAnalysisinstall :: $(install_dir)/$(numuCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(numuCCAnalysisinstallname) :: $(bin)$(numuCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(numuCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##numuCCAnalysisclean :: numuCCAnalysisuninstall

uninstall :: numuCCAnalysisuninstall

numuCCAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(numuCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),numuCCAnalysisclean)

#$(bin)numuCCAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)numuCCAnalysis_dependencies.make : ../src/numuCCAnalysis.cxx $(use_requirements) $(cmt_final_setup_numuCCAnalysis)
	$(echo) "(numuCCAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) numuCCAnalysis -all_sources -out=$@ ../src/numuCCAnalysis.cxx
endif

#$(numuCCAnalysis_dependencies)

-include $(bin)numuCCAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)numuCCAnalysis_dependencies.make : $(numuCCAnalysis_cxx_dependencies)

$(bin)$(binobj)numuCCAnalysis.o : $(numuCCAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)numuCCAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(numuCCAnalysis_pp_cppflags) $(lib_numuCCAnalysis_pp_cppflags) $(numuCCAnalysis_pp_cppflags) $(use_cppflags) $(numuCCAnalysis_cppflags) $(lib_numuCCAnalysis_cppflags) $(numuCCAnalysis_cppflags) $(numuCCAnalysis_cxx_cppflags)  $(src)numuCCAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: numuCCAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(numuCCAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(numuCCAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_numuCCAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(numuCCAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(numuCCAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(numuCCAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

numuCCAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)numuCCAnalysis$(library_suffix).a $(binobj)$(library_prefix)numuCCAnalysis$(library_suffix).s? $(binobj)numuCCAnalysis.stamp $(binobj)numuCCAnalysis.shstamp
#-- end of cleanup_library ---------------

