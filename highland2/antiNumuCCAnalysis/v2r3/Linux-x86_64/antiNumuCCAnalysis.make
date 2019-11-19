#-- start of make_header -----------------

#====================================
#  Library antiNumuCCAnalysis
#
#   Generated Mon Nov  4 15:27:06 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_antiNumuCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_antiNumuCCAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_antiNumuCCAnalysis

antiNumuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_antiNumuCCAnalysis = /tmp/CMT_$(antiNumuCCAnalysis_tag)_antiNumuCCAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_antiNumuCCAnalysis = $(antiNumuCCAnalysis_tag)_antiNumuCCAnalysis.make
cmt_local_tagfile_antiNumuCCAnalysis = $(bin)$(antiNumuCCAnalysis_tag)_antiNumuCCAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

antiNumuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_antiNumuCCAnalysis = /tmp/CMT_$(antiNumuCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_antiNumuCCAnalysis = $(antiNumuCCAnalysis_tag).make
cmt_local_tagfile_antiNumuCCAnalysis = $(bin)$(antiNumuCCAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_antiNumuCCAnalysis)

ifdef cmt_antiNumuCCAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_antiNumuCCAnalysis = /tmp/CMT_antiNumuCCAnalysis_antiNumuCCAnalysissetup.make
cmt_local_antiNumuCCAnalysis_makefile = /tmp/CMT_antiNumuCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_antiNumuCCAnalysis = $(bin)antiNumuCCAnalysis_antiNumuCCAnalysissetup.make
cmt_local_antiNumuCCAnalysis_makefile = $(bin)antiNumuCCAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_antiNumuCCAnalysis = /tmp/CMT_antiNumuCCAnalysissetup.make
cmt_local_antiNumuCCAnalysis_makefile = /tmp/CMT_antiNumuCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_antiNumuCCAnalysis = $(bin)antiNumuCCAnalysissetup.make
cmt_local_antiNumuCCAnalysis_makefile = $(bin)antiNumuCCAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_antiNumuCCAnalysissetup.make
else
cmt_final_setup = $(bin)antiNumuCCAnalysissetup.make
endif

antiNumuCCAnalysis ::


ifdef READONLY
antiNumuCCAnalysis ::
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
	$(echo) 'antiNumuCCAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = antiNumuCCAnalysis/
antiNumuCCAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

antiNumuCCAnalysislibname   = $(bin)$(library_prefix)antiNumuCCAnalysis$(library_suffix)
antiNumuCCAnalysislib       = $(antiNumuCCAnalysislibname).a
antiNumuCCAnalysisstamp     = $(bin)antiNumuCCAnalysis.stamp
antiNumuCCAnalysisshstamp   = $(bin)antiNumuCCAnalysis.shstamp

antiNumuCCAnalysis :: dirs  antiNumuCCAnalysisLIB
	$(echo) "antiNumuCCAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

antiNumuCCAnalysisLIB :: $(antiNumuCCAnalysislib) $(antiNumuCCAnalysisshstamp)
	$(echo) "antiNumuCCAnalysis : library ok"

$(antiNumuCCAnalysislib) :: $(bin)antiNumuCCAnalysis.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(antiNumuCCAnalysislib) $?
	$(lib_silent) $(ranlib) $(antiNumuCCAnalysislib)
	$(lib_silent) cat /dev/null >$(antiNumuCCAnalysisstamp)

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

$(antiNumuCCAnalysislibname).$(shlibsuffix) :: $(antiNumuCCAnalysislib) requirements $(use_requirements) $(antiNumuCCAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" antiNumuCCAnalysis $(antiNumuCCAnalysis_shlibflags)

$(antiNumuCCAnalysisshstamp) :: $(antiNumuCCAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(antiNumuCCAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(antiNumuCCAnalysisshstamp) ; fi

antiNumuCCAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)antiNumuCCAnalysis.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf antiNumuCCAnalysis_deps antiNumuCCAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
antiNumuCCAnalysisinstallname = $(library_prefix)antiNumuCCAnalysis$(library_suffix).$(shlibsuffix)

antiNumuCCAnalysis :: antiNumuCCAnalysisinstall

install :: antiNumuCCAnalysisinstall

antiNumuCCAnalysisinstall :: $(install_dir)/$(antiNumuCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(antiNumuCCAnalysisinstallname) :: $(bin)$(antiNumuCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(antiNumuCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##antiNumuCCAnalysisclean :: antiNumuCCAnalysisuninstall

uninstall :: antiNumuCCAnalysisuninstall

antiNumuCCAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(antiNumuCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),antiNumuCCAnalysisclean)

#$(bin)antiNumuCCAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)antiNumuCCAnalysis_dependencies.make : ../src/antiNumuCCAnalysis.cxx $(use_requirements) $(cmt_final_setup_antiNumuCCAnalysis)
	$(echo) "(antiNumuCCAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) antiNumuCCAnalysis -all_sources -out=$@ ../src/antiNumuCCAnalysis.cxx
endif

#$(antiNumuCCAnalysis_dependencies)

-include $(bin)antiNumuCCAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)antiNumuCCAnalysis_dependencies.make : $(antiNumuCCAnalysis_cxx_dependencies)

$(bin)$(binobj)antiNumuCCAnalysis.o : $(antiNumuCCAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)antiNumuCCAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(antiNumuCCAnalysis_pp_cppflags) $(lib_antiNumuCCAnalysis_pp_cppflags) $(antiNumuCCAnalysis_pp_cppflags) $(use_cppflags) $(antiNumuCCAnalysis_cppflags) $(lib_antiNumuCCAnalysis_cppflags) $(antiNumuCCAnalysis_cppflags) $(antiNumuCCAnalysis_cxx_cppflags)  $(src)antiNumuCCAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: antiNumuCCAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(antiNumuCCAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(antiNumuCCAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_antiNumuCCAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNumuCCAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNumuCCAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNumuCCAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

antiNumuCCAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)antiNumuCCAnalysis$(library_suffix).a $(binobj)$(library_prefix)antiNumuCCAnalysis$(library_suffix).s? $(binobj)antiNumuCCAnalysis.stamp $(binobj)antiNumuCCAnalysis.shstamp
#-- end of cleanup_library ---------------

