#-- start of make_header -----------------

#====================================
#  Application FixMergedRooVtxLink
#
#   Generated Mon Nov  4 15:26:58 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_FixMergedRooVtxLink_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_FixMergedRooVtxLink_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_FixMergedRooVtxLink

highlandIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_FixMergedRooVtxLink = /tmp/CMT_$(highlandIO_tag)_FixMergedRooVtxLink.make$(cmt_lock_pid)
else
#cmt_local_tagfile_FixMergedRooVtxLink = $(highlandIO_tag)_FixMergedRooVtxLink.make
cmt_local_tagfile_FixMergedRooVtxLink = $(bin)$(highlandIO_tag)_FixMergedRooVtxLink.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_FixMergedRooVtxLink = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_FixMergedRooVtxLink = $(highlandIO_tag).make
cmt_local_tagfile_FixMergedRooVtxLink = $(bin)$(highlandIO_tag).make
endif

endif

-include $(cmt_local_tagfile_FixMergedRooVtxLink)

ifdef cmt_FixMergedRooVtxLink_has_target_tag

ifdef READONLY
cmt_final_setup_FixMergedRooVtxLink = /tmp/CMT_highlandIO_FixMergedRooVtxLinksetup.make
cmt_local_FixMergedRooVtxLink_makefile = /tmp/CMT_FixMergedRooVtxLink$(cmt_lock_pid).make
else
cmt_final_setup_FixMergedRooVtxLink = $(bin)highlandIO_FixMergedRooVtxLinksetup.make
cmt_local_FixMergedRooVtxLink_makefile = $(bin)FixMergedRooVtxLink.make
endif

else

ifdef READONLY
cmt_final_setup_FixMergedRooVtxLink = /tmp/CMT_highlandIOsetup.make
cmt_local_FixMergedRooVtxLink_makefile = /tmp/CMT_FixMergedRooVtxLink$(cmt_lock_pid).make
else
cmt_final_setup_FixMergedRooVtxLink = $(bin)highlandIOsetup.make
cmt_local_FixMergedRooVtxLink_makefile = $(bin)FixMergedRooVtxLink.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandIOsetup.make
else
cmt_final_setup = $(bin)highlandIOsetup.make
endif

FixMergedRooVtxLink ::


ifdef READONLY
FixMergedRooVtxLink ::
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
	$(echo) 'FixMergedRooVtxLink'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = FixMergedRooVtxLink/
FixMergedRooVtxLink::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

FixMergedRooVtxLink :: dirs  $(bin)FixMergedRooVtxLink${application_suffix}
	$(echo) "FixMergedRooVtxLink ok"

#-- end of application_header
#-- start of application

$(bin)FixMergedRooVtxLink${application_suffix} :: $(bin)FixMergedRooVtxLink.o $(use_stamps) $(FixMergedRooVtxLinkstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)FixMergedRooVtxLink.o $(cmt_installarea_linkopts) $(FixMergedRooVtxLink_use_linkopts) $(FixMergedRooVtxLinklinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
FixMergedRooVtxLinkinstallname = FixMergedRooVtxLink${application_suffix}

FixMergedRooVtxLink :: FixMergedRooVtxLinkinstall

install :: FixMergedRooVtxLinkinstall

FixMergedRooVtxLinkinstall :: $(install_dir)/$(FixMergedRooVtxLinkinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(FixMergedRooVtxLinkinstallname) :: $(bin)$(FixMergedRooVtxLinkinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(FixMergedRooVtxLinkinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##FixMergedRooVtxLinkclean :: FixMergedRooVtxLinkuninstall

uninstall :: FixMergedRooVtxLinkuninstall

FixMergedRooVtxLinkuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(FixMergedRooVtxLinkinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (FixMergedRooVtxLink.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),FixMergedRooVtxLinkclean)

#$(bin)FixMergedRooVtxLink_dependencies.make :: dirs

ifndef QUICK
$(bin)FixMergedRooVtxLink_dependencies.make : ../app/FixMergedRooVtxLink.cxx $(use_requirements) $(cmt_final_setup_FixMergedRooVtxLink)
	$(echo) "(FixMergedRooVtxLink.make) Rebuilding $@"; \
	  $(build_dependencies) FixMergedRooVtxLink -all_sources -out=$@ ../app/FixMergedRooVtxLink.cxx
endif

#$(FixMergedRooVtxLink_dependencies)

-include $(bin)FixMergedRooVtxLink_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)FixMergedRooVtxLink_dependencies.make : $(FixMergedRooVtxLink_cxx_dependencies)

$(bin)$(binobj)FixMergedRooVtxLink.o : $(FixMergedRooVtxLink_cxx_dependencies)
	$(cpp_echo) ../app/FixMergedRooVtxLink.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(FixMergedRooVtxLink_pp_cppflags) $(app_FixMergedRooVtxLink_pp_cppflags) $(FixMergedRooVtxLink_pp_cppflags) $(use_cppflags) $(FixMergedRooVtxLink_cppflags) $(app_FixMergedRooVtxLink_cppflags) $(FixMergedRooVtxLink_cppflags) $(FixMergedRooVtxLink_cxx_cppflags) -I../app ../app/FixMergedRooVtxLink.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: FixMergedRooVtxLinkclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(FixMergedRooVtxLink.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(FixMergedRooVtxLink.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_FixMergedRooVtxLink)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(FixMergedRooVtxLink.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(FixMergedRooVtxLink.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(FixMergedRooVtxLink.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

FixMergedRooVtxLinkclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) FixMergedRooVtxLink${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f FixMergedRooVtxLink${application_suffix}

#	@echo "------> (FixMergedRooVtxLink.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)FixMergedRooVtxLink.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf FixMergedRooVtxLink_deps FixMergedRooVtxLink_dependencies.make
#-- end of cleanup_objects ------
