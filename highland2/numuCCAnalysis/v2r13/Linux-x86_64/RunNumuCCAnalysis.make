#-- start of make_header -----------------

#====================================
#  Application RunNumuCCAnalysis
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

cmt_RunNumuCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunNumuCCAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunNumuCCAnalysis

numuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunNumuCCAnalysis = /tmp/CMT_$(numuCCAnalysis_tag)_RunNumuCCAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunNumuCCAnalysis = $(numuCCAnalysis_tag)_RunNumuCCAnalysis.make
cmt_local_tagfile_RunNumuCCAnalysis = $(bin)$(numuCCAnalysis_tag)_RunNumuCCAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

numuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunNumuCCAnalysis = /tmp/CMT_$(numuCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunNumuCCAnalysis = $(numuCCAnalysis_tag).make
cmt_local_tagfile_RunNumuCCAnalysis = $(bin)$(numuCCAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunNumuCCAnalysis)

ifdef cmt_RunNumuCCAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunNumuCCAnalysis = /tmp/CMT_numuCCAnalysis_RunNumuCCAnalysissetup.make
cmt_local_RunNumuCCAnalysis_makefile = /tmp/CMT_RunNumuCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunNumuCCAnalysis = $(bin)numuCCAnalysis_RunNumuCCAnalysissetup.make
cmt_local_RunNumuCCAnalysis_makefile = $(bin)RunNumuCCAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunNumuCCAnalysis = /tmp/CMT_numuCCAnalysissetup.make
cmt_local_RunNumuCCAnalysis_makefile = /tmp/CMT_RunNumuCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunNumuCCAnalysis = $(bin)numuCCAnalysissetup.make
cmt_local_RunNumuCCAnalysis_makefile = $(bin)RunNumuCCAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_numuCCAnalysissetup.make
else
cmt_final_setup = $(bin)numuCCAnalysissetup.make
endif

RunNumuCCAnalysis ::


ifdef READONLY
RunNumuCCAnalysis ::
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
	$(echo) 'RunNumuCCAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunNumuCCAnalysis/
RunNumuCCAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunNumuCCAnalysis :: dirs  $(bin)RunNumuCCAnalysis${application_suffix}
	$(echo) "RunNumuCCAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunNumuCCAnalysis${application_suffix} :: $(bin)RunNumuCCAnalysis.o $(use_stamps) $(RunNumuCCAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunNumuCCAnalysis.o $(cmt_installarea_linkopts) $(RunNumuCCAnalysis_use_linkopts) $(RunNumuCCAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunNumuCCAnalysisinstallname = RunNumuCCAnalysis${application_suffix}

RunNumuCCAnalysis :: RunNumuCCAnalysisinstall

install :: RunNumuCCAnalysisinstall

RunNumuCCAnalysisinstall :: $(install_dir)/$(RunNumuCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunNumuCCAnalysisinstallname) :: $(bin)$(RunNumuCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunNumuCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunNumuCCAnalysisclean :: RunNumuCCAnalysisuninstall

uninstall :: RunNumuCCAnalysisuninstall

RunNumuCCAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunNumuCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunNumuCCAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunNumuCCAnalysisclean)

#$(bin)RunNumuCCAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunNumuCCAnalysis_dependencies.make : ../app/RunNumuCCAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunNumuCCAnalysis)
	$(echo) "(RunNumuCCAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunNumuCCAnalysis -all_sources -out=$@ ../app/RunNumuCCAnalysis.cxx
endif

#$(RunNumuCCAnalysis_dependencies)

-include $(bin)RunNumuCCAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunNumuCCAnalysis_dependencies.make : $(RunNumuCCAnalysis_cxx_dependencies)

$(bin)$(binobj)RunNumuCCAnalysis.o : $(RunNumuCCAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunNumuCCAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunNumuCCAnalysis_pp_cppflags) $(app_RunNumuCCAnalysis_pp_cppflags) $(RunNumuCCAnalysis_pp_cppflags) $(use_cppflags) $(RunNumuCCAnalysis_cppflags) $(app_RunNumuCCAnalysis_cppflags) $(RunNumuCCAnalysis_cppflags) $(RunNumuCCAnalysis_cxx_cppflags) -I../app ../app/RunNumuCCAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunNumuCCAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunNumuCCAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunNumuCCAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunNumuCCAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNumuCCAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNumuCCAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNumuCCAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunNumuCCAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunNumuCCAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunNumuCCAnalysis${application_suffix}

#	@echo "------> (RunNumuCCAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunNumuCCAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunNumuCCAnalysis_deps RunNumuCCAnalysis_dependencies.make
#-- end of cleanup_objects ------
