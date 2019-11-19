#-- start of make_header -----------------

#====================================
#  Application RunAntiNumuCCIncFwdAnalysis
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

cmt_RunAntiNumuCCIncFwdAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunAntiNumuCCIncFwdAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunAntiNumuCCIncFwdAnalysis

antiNumuCCIncFwdAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunAntiNumuCCIncFwdAnalysis = /tmp/CMT_$(antiNumuCCIncFwdAnalysis_tag)_RunAntiNumuCCIncFwdAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunAntiNumuCCIncFwdAnalysis = $(antiNumuCCIncFwdAnalysis_tag)_RunAntiNumuCCIncFwdAnalysis.make
cmt_local_tagfile_RunAntiNumuCCIncFwdAnalysis = $(bin)$(antiNumuCCIncFwdAnalysis_tag)_RunAntiNumuCCIncFwdAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

antiNumuCCIncFwdAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunAntiNumuCCIncFwdAnalysis = /tmp/CMT_$(antiNumuCCIncFwdAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunAntiNumuCCIncFwdAnalysis = $(antiNumuCCIncFwdAnalysis_tag).make
cmt_local_tagfile_RunAntiNumuCCIncFwdAnalysis = $(bin)$(antiNumuCCIncFwdAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunAntiNumuCCIncFwdAnalysis)

ifdef cmt_RunAntiNumuCCIncFwdAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunAntiNumuCCIncFwdAnalysis = /tmp/CMT_antiNumuCCIncFwdAnalysis_RunAntiNumuCCIncFwdAnalysissetup.make
cmt_local_RunAntiNumuCCIncFwdAnalysis_makefile = /tmp/CMT_RunAntiNumuCCIncFwdAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunAntiNumuCCIncFwdAnalysis = $(bin)antiNumuCCIncFwdAnalysis_RunAntiNumuCCIncFwdAnalysissetup.make
cmt_local_RunAntiNumuCCIncFwdAnalysis_makefile = $(bin)RunAntiNumuCCIncFwdAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunAntiNumuCCIncFwdAnalysis = /tmp/CMT_antiNumuCCIncFwdAnalysissetup.make
cmt_local_RunAntiNumuCCIncFwdAnalysis_makefile = /tmp/CMT_RunAntiNumuCCIncFwdAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunAntiNumuCCIncFwdAnalysis = $(bin)antiNumuCCIncFwdAnalysissetup.make
cmt_local_RunAntiNumuCCIncFwdAnalysis_makefile = $(bin)RunAntiNumuCCIncFwdAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_antiNumuCCIncFwdAnalysissetup.make
else
cmt_final_setup = $(bin)antiNumuCCIncFwdAnalysissetup.make
endif

RunAntiNumuCCIncFwdAnalysis ::


ifdef READONLY
RunAntiNumuCCIncFwdAnalysis ::
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
	$(echo) 'RunAntiNumuCCIncFwdAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunAntiNumuCCIncFwdAnalysis/
RunAntiNumuCCIncFwdAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunAntiNumuCCIncFwdAnalysis :: dirs  $(bin)RunAntiNumuCCIncFwdAnalysis${application_suffix}
	$(echo) "RunAntiNumuCCIncFwdAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunAntiNumuCCIncFwdAnalysis${application_suffix} :: $(bin)RunAntiNumuCCIncFwdAnalysis.o $(use_stamps) $(RunAntiNumuCCIncFwdAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunAntiNumuCCIncFwdAnalysis.o $(cmt_installarea_linkopts) $(RunAntiNumuCCIncFwdAnalysis_use_linkopts) $(RunAntiNumuCCIncFwdAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunAntiNumuCCIncFwdAnalysisinstallname = RunAntiNumuCCIncFwdAnalysis${application_suffix}

RunAntiNumuCCIncFwdAnalysis :: RunAntiNumuCCIncFwdAnalysisinstall

install :: RunAntiNumuCCIncFwdAnalysisinstall

RunAntiNumuCCIncFwdAnalysisinstall :: $(install_dir)/$(RunAntiNumuCCIncFwdAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunAntiNumuCCIncFwdAnalysisinstallname) :: $(bin)$(RunAntiNumuCCIncFwdAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunAntiNumuCCIncFwdAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunAntiNumuCCIncFwdAnalysisclean :: RunAntiNumuCCIncFwdAnalysisuninstall

uninstall :: RunAntiNumuCCIncFwdAnalysisuninstall

RunAntiNumuCCIncFwdAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunAntiNumuCCIncFwdAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunAntiNumuCCIncFwdAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunAntiNumuCCIncFwdAnalysisclean)

#$(bin)RunAntiNumuCCIncFwdAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunAntiNumuCCIncFwdAnalysis_dependencies.make : ../app/RunAntiNumuCCIncFwdAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunAntiNumuCCIncFwdAnalysis)
	$(echo) "(RunAntiNumuCCIncFwdAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunAntiNumuCCIncFwdAnalysis -all_sources -out=$@ ../app/RunAntiNumuCCIncFwdAnalysis.cxx
endif

#$(RunAntiNumuCCIncFwdAnalysis_dependencies)

-include $(bin)RunAntiNumuCCIncFwdAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunAntiNumuCCIncFwdAnalysis_dependencies.make : $(RunAntiNumuCCIncFwdAnalysis_cxx_dependencies)

$(bin)$(binobj)RunAntiNumuCCIncFwdAnalysis.o : $(RunAntiNumuCCIncFwdAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunAntiNumuCCIncFwdAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunAntiNumuCCIncFwdAnalysis_pp_cppflags) $(app_RunAntiNumuCCIncFwdAnalysis_pp_cppflags) $(RunAntiNumuCCIncFwdAnalysis_pp_cppflags) $(use_cppflags) $(RunAntiNumuCCIncFwdAnalysis_cppflags) $(app_RunAntiNumuCCIncFwdAnalysis_cppflags) $(RunAntiNumuCCIncFwdAnalysis_cppflags) $(RunAntiNumuCCIncFwdAnalysis_cxx_cppflags) -I../app ../app/RunAntiNumuCCIncFwdAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunAntiNumuCCIncFwdAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunAntiNumuCCIncFwdAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunAntiNumuCCIncFwdAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunAntiNumuCCIncFwdAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNumuCCIncFwdAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNumuCCIncFwdAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNumuCCIncFwdAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunAntiNumuCCIncFwdAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunAntiNumuCCIncFwdAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunAntiNumuCCIncFwdAnalysis${application_suffix}

#	@echo "------> (RunAntiNumuCCIncFwdAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunAntiNumuCCIncFwdAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunAntiNumuCCIncFwdAnalysis_deps RunAntiNumuCCIncFwdAnalysis_dependencies.make
#-- end of cleanup_objects ------
