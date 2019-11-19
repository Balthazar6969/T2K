#-- start of make_header -----------------

#====================================
#  Application RunRooTrackerVtxExample
#
#   Generated Mon Nov  4 15:27:00 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunRooTrackerVtxExample_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunRooTrackerVtxExample_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunRooTrackerVtxExample

baseAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunRooTrackerVtxExample = /tmp/CMT_$(baseAnalysis_tag)_RunRooTrackerVtxExample.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunRooTrackerVtxExample = $(baseAnalysis_tag)_RunRooTrackerVtxExample.make
cmt_local_tagfile_RunRooTrackerVtxExample = $(bin)$(baseAnalysis_tag)_RunRooTrackerVtxExample.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

baseAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunRooTrackerVtxExample = /tmp/CMT_$(baseAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunRooTrackerVtxExample = $(baseAnalysis_tag).make
cmt_local_tagfile_RunRooTrackerVtxExample = $(bin)$(baseAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunRooTrackerVtxExample)

ifdef cmt_RunRooTrackerVtxExample_has_target_tag

ifdef READONLY
cmt_final_setup_RunRooTrackerVtxExample = /tmp/CMT_baseAnalysis_RunRooTrackerVtxExamplesetup.make
cmt_local_RunRooTrackerVtxExample_makefile = /tmp/CMT_RunRooTrackerVtxExample$(cmt_lock_pid).make
else
cmt_final_setup_RunRooTrackerVtxExample = $(bin)baseAnalysis_RunRooTrackerVtxExamplesetup.make
cmt_local_RunRooTrackerVtxExample_makefile = $(bin)RunRooTrackerVtxExample.make
endif

else

ifdef READONLY
cmt_final_setup_RunRooTrackerVtxExample = /tmp/CMT_baseAnalysissetup.make
cmt_local_RunRooTrackerVtxExample_makefile = /tmp/CMT_RunRooTrackerVtxExample$(cmt_lock_pid).make
else
cmt_final_setup_RunRooTrackerVtxExample = $(bin)baseAnalysissetup.make
cmt_local_RunRooTrackerVtxExample_makefile = $(bin)RunRooTrackerVtxExample.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_baseAnalysissetup.make
else
cmt_final_setup = $(bin)baseAnalysissetup.make
endif

RunRooTrackerVtxExample ::


ifdef READONLY
RunRooTrackerVtxExample ::
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
	$(echo) 'RunRooTrackerVtxExample'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunRooTrackerVtxExample/
RunRooTrackerVtxExample::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunRooTrackerVtxExample :: dirs  $(bin)RunRooTrackerVtxExample${application_suffix}
	$(echo) "RunRooTrackerVtxExample ok"

#-- end of application_header
#-- start of application

$(bin)RunRooTrackerVtxExample${application_suffix} :: $(bin)RunRooTrackerVtxExample.o $(use_stamps) $(RunRooTrackerVtxExamplestamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunRooTrackerVtxExample.o $(cmt_installarea_linkopts) $(RunRooTrackerVtxExample_use_linkopts) $(RunRooTrackerVtxExamplelinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunRooTrackerVtxExampleinstallname = RunRooTrackerVtxExample${application_suffix}

RunRooTrackerVtxExample :: RunRooTrackerVtxExampleinstall

install :: RunRooTrackerVtxExampleinstall

RunRooTrackerVtxExampleinstall :: $(install_dir)/$(RunRooTrackerVtxExampleinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunRooTrackerVtxExampleinstallname) :: $(bin)$(RunRooTrackerVtxExampleinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunRooTrackerVtxExampleinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunRooTrackerVtxExampleclean :: RunRooTrackerVtxExampleuninstall

uninstall :: RunRooTrackerVtxExampleuninstall

RunRooTrackerVtxExampleuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunRooTrackerVtxExampleinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunRooTrackerVtxExample.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunRooTrackerVtxExampleclean)

#$(bin)RunRooTrackerVtxExample_dependencies.make :: dirs

ifndef QUICK
$(bin)RunRooTrackerVtxExample_dependencies.make : ../app/RunRooTrackerVtxExample.cxx $(use_requirements) $(cmt_final_setup_RunRooTrackerVtxExample)
	$(echo) "(RunRooTrackerVtxExample.make) Rebuilding $@"; \
	  $(build_dependencies) RunRooTrackerVtxExample -all_sources -out=$@ ../app/RunRooTrackerVtxExample.cxx
endif

#$(RunRooTrackerVtxExample_dependencies)

-include $(bin)RunRooTrackerVtxExample_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunRooTrackerVtxExample_dependencies.make : $(RunRooTrackerVtxExample_cxx_dependencies)

$(bin)$(binobj)RunRooTrackerVtxExample.o : $(RunRooTrackerVtxExample_cxx_dependencies)
	$(cpp_echo) ../app/RunRooTrackerVtxExample.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunRooTrackerVtxExample_pp_cppflags) $(app_RunRooTrackerVtxExample_pp_cppflags) $(RunRooTrackerVtxExample_pp_cppflags) $(use_cppflags) $(RunRooTrackerVtxExample_cppflags) $(app_RunRooTrackerVtxExample_cppflags) $(RunRooTrackerVtxExample_cppflags) $(RunRooTrackerVtxExample_cxx_cppflags) -I../app ../app/RunRooTrackerVtxExample.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunRooTrackerVtxExampleclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunRooTrackerVtxExample.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunRooTrackerVtxExample.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunRooTrackerVtxExample)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunRooTrackerVtxExample.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunRooTrackerVtxExample.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunRooTrackerVtxExample.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunRooTrackerVtxExampleclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunRooTrackerVtxExample${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunRooTrackerVtxExample${application_suffix}

#	@echo "------> (RunRooTrackerVtxExample.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunRooTrackerVtxExample.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunRooTrackerVtxExample_deps RunRooTrackerVtxExample_dependencies.make
#-- end of cleanup_objects ------
