#-- start of make_header -----------------

#====================================
#  Application CheckRooTrackerVtxLink
#
#   Generated Mon Nov  4 15:26:48 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_CheckRooTrackerVtxLink_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_CheckRooTrackerVtxLink_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_CheckRooTrackerVtxLink

highlandUtils_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_CheckRooTrackerVtxLink = /tmp/CMT_$(highlandUtils_tag)_CheckRooTrackerVtxLink.make$(cmt_lock_pid)
else
#cmt_local_tagfile_CheckRooTrackerVtxLink = $(highlandUtils_tag)_CheckRooTrackerVtxLink.make
cmt_local_tagfile_CheckRooTrackerVtxLink = $(bin)$(highlandUtils_tag)_CheckRooTrackerVtxLink.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandUtils_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_CheckRooTrackerVtxLink = /tmp/CMT_$(highlandUtils_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_CheckRooTrackerVtxLink = $(highlandUtils_tag).make
cmt_local_tagfile_CheckRooTrackerVtxLink = $(bin)$(highlandUtils_tag).make
endif

endif

-include $(cmt_local_tagfile_CheckRooTrackerVtxLink)

ifdef cmt_CheckRooTrackerVtxLink_has_target_tag

ifdef READONLY
cmt_final_setup_CheckRooTrackerVtxLink = /tmp/CMT_highlandUtils_CheckRooTrackerVtxLinksetup.make
cmt_local_CheckRooTrackerVtxLink_makefile = /tmp/CMT_CheckRooTrackerVtxLink$(cmt_lock_pid).make
else
cmt_final_setup_CheckRooTrackerVtxLink = $(bin)highlandUtils_CheckRooTrackerVtxLinksetup.make
cmt_local_CheckRooTrackerVtxLink_makefile = $(bin)CheckRooTrackerVtxLink.make
endif

else

ifdef READONLY
cmt_final_setup_CheckRooTrackerVtxLink = /tmp/CMT_highlandUtilssetup.make
cmt_local_CheckRooTrackerVtxLink_makefile = /tmp/CMT_CheckRooTrackerVtxLink$(cmt_lock_pid).make
else
cmt_final_setup_CheckRooTrackerVtxLink = $(bin)highlandUtilssetup.make
cmt_local_CheckRooTrackerVtxLink_makefile = $(bin)CheckRooTrackerVtxLink.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandUtilssetup.make
else
cmt_final_setup = $(bin)highlandUtilssetup.make
endif

CheckRooTrackerVtxLink ::


ifdef READONLY
CheckRooTrackerVtxLink ::
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
	$(echo) 'CheckRooTrackerVtxLink'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = CheckRooTrackerVtxLink/
CheckRooTrackerVtxLink::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

CheckRooTrackerVtxLink :: dirs  $(bin)CheckRooTrackerVtxLink${application_suffix}
	$(echo) "CheckRooTrackerVtxLink ok"

#-- end of application_header
#-- start of application

$(bin)CheckRooTrackerVtxLink${application_suffix} :: $(bin)CheckRooTrackerVtxLink.o $(use_stamps) $(CheckRooTrackerVtxLinkstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)CheckRooTrackerVtxLink.o $(cmt_installarea_linkopts) $(CheckRooTrackerVtxLink_use_linkopts) $(CheckRooTrackerVtxLinklinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
CheckRooTrackerVtxLinkinstallname = CheckRooTrackerVtxLink${application_suffix}

CheckRooTrackerVtxLink :: CheckRooTrackerVtxLinkinstall

install :: CheckRooTrackerVtxLinkinstall

CheckRooTrackerVtxLinkinstall :: $(install_dir)/$(CheckRooTrackerVtxLinkinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(CheckRooTrackerVtxLinkinstallname) :: $(bin)$(CheckRooTrackerVtxLinkinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(CheckRooTrackerVtxLinkinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##CheckRooTrackerVtxLinkclean :: CheckRooTrackerVtxLinkuninstall

uninstall :: CheckRooTrackerVtxLinkuninstall

CheckRooTrackerVtxLinkuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(CheckRooTrackerVtxLinkinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (CheckRooTrackerVtxLink.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),CheckRooTrackerVtxLinkclean)

#$(bin)CheckRooTrackerVtxLink_dependencies.make :: dirs

ifndef QUICK
$(bin)CheckRooTrackerVtxLink_dependencies.make : ../app/CheckRooTrackerVtxLink.cxx $(use_requirements) $(cmt_final_setup_CheckRooTrackerVtxLink)
	$(echo) "(CheckRooTrackerVtxLink.make) Rebuilding $@"; \
	  $(build_dependencies) CheckRooTrackerVtxLink -all_sources -out=$@ ../app/CheckRooTrackerVtxLink.cxx
endif

#$(CheckRooTrackerVtxLink_dependencies)

-include $(bin)CheckRooTrackerVtxLink_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)CheckRooTrackerVtxLink_dependencies.make : $(CheckRooTrackerVtxLink_cxx_dependencies)

$(bin)$(binobj)CheckRooTrackerVtxLink.o : $(CheckRooTrackerVtxLink_cxx_dependencies)
	$(cpp_echo) ../app/CheckRooTrackerVtxLink.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(CheckRooTrackerVtxLink_pp_cppflags) $(app_CheckRooTrackerVtxLink_pp_cppflags) $(CheckRooTrackerVtxLink_pp_cppflags) $(use_cppflags) $(CheckRooTrackerVtxLink_cppflags) $(app_CheckRooTrackerVtxLink_cppflags) $(CheckRooTrackerVtxLink_cppflags) $(CheckRooTrackerVtxLink_cxx_cppflags) -I../app ../app/CheckRooTrackerVtxLink.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: CheckRooTrackerVtxLinkclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(CheckRooTrackerVtxLink.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(CheckRooTrackerVtxLink.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_CheckRooTrackerVtxLink)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(CheckRooTrackerVtxLink.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(CheckRooTrackerVtxLink.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(CheckRooTrackerVtxLink.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

CheckRooTrackerVtxLinkclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) CheckRooTrackerVtxLink${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f CheckRooTrackerVtxLink${application_suffix}

#	@echo "------> (CheckRooTrackerVtxLink.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)CheckRooTrackerVtxLink.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf CheckRooTrackerVtxLink_deps CheckRooTrackerVtxLink_dependencies.make
#-- end of cleanup_objects ------
