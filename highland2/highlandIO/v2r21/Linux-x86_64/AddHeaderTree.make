#-- start of make_header -----------------

#====================================
#  Application AddHeaderTree
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

cmt_AddHeaderTree_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_AddHeaderTree_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_AddHeaderTree

highlandIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_AddHeaderTree = /tmp/CMT_$(highlandIO_tag)_AddHeaderTree.make$(cmt_lock_pid)
else
#cmt_local_tagfile_AddHeaderTree = $(highlandIO_tag)_AddHeaderTree.make
cmt_local_tagfile_AddHeaderTree = $(bin)$(highlandIO_tag)_AddHeaderTree.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_AddHeaderTree = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_AddHeaderTree = $(highlandIO_tag).make
cmt_local_tagfile_AddHeaderTree = $(bin)$(highlandIO_tag).make
endif

endif

-include $(cmt_local_tagfile_AddHeaderTree)

ifdef cmt_AddHeaderTree_has_target_tag

ifdef READONLY
cmt_final_setup_AddHeaderTree = /tmp/CMT_highlandIO_AddHeaderTreesetup.make
cmt_local_AddHeaderTree_makefile = /tmp/CMT_AddHeaderTree$(cmt_lock_pid).make
else
cmt_final_setup_AddHeaderTree = $(bin)highlandIO_AddHeaderTreesetup.make
cmt_local_AddHeaderTree_makefile = $(bin)AddHeaderTree.make
endif

else

ifdef READONLY
cmt_final_setup_AddHeaderTree = /tmp/CMT_highlandIOsetup.make
cmt_local_AddHeaderTree_makefile = /tmp/CMT_AddHeaderTree$(cmt_lock_pid).make
else
cmt_final_setup_AddHeaderTree = $(bin)highlandIOsetup.make
cmt_local_AddHeaderTree_makefile = $(bin)AddHeaderTree.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandIOsetup.make
else
cmt_final_setup = $(bin)highlandIOsetup.make
endif

AddHeaderTree ::


ifdef READONLY
AddHeaderTree ::
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
	$(echo) 'AddHeaderTree'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = AddHeaderTree/
AddHeaderTree::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

AddHeaderTree :: dirs  $(bin)AddHeaderTree${application_suffix}
	$(echo) "AddHeaderTree ok"

#-- end of application_header
#-- start of application

$(bin)AddHeaderTree${application_suffix} :: $(bin)AddHeaderTree.o $(use_stamps) $(AddHeaderTreestamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)AddHeaderTree.o $(cmt_installarea_linkopts) $(AddHeaderTree_use_linkopts) $(AddHeaderTreelinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
AddHeaderTreeinstallname = AddHeaderTree${application_suffix}

AddHeaderTree :: AddHeaderTreeinstall

install :: AddHeaderTreeinstall

AddHeaderTreeinstall :: $(install_dir)/$(AddHeaderTreeinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(AddHeaderTreeinstallname) :: $(bin)$(AddHeaderTreeinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(AddHeaderTreeinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##AddHeaderTreeclean :: AddHeaderTreeuninstall

uninstall :: AddHeaderTreeuninstall

AddHeaderTreeuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(AddHeaderTreeinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (AddHeaderTree.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),AddHeaderTreeclean)

#$(bin)AddHeaderTree_dependencies.make :: dirs

ifndef QUICK
$(bin)AddHeaderTree_dependencies.make : ../app/AddHeaderTree.cxx $(use_requirements) $(cmt_final_setup_AddHeaderTree)
	$(echo) "(AddHeaderTree.make) Rebuilding $@"; \
	  $(build_dependencies) AddHeaderTree -all_sources -out=$@ ../app/AddHeaderTree.cxx
endif

#$(AddHeaderTree_dependencies)

-include $(bin)AddHeaderTree_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)AddHeaderTree_dependencies.make : $(AddHeaderTree_cxx_dependencies)

$(bin)$(binobj)AddHeaderTree.o : $(AddHeaderTree_cxx_dependencies)
	$(cpp_echo) ../app/AddHeaderTree.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(AddHeaderTree_pp_cppflags) $(app_AddHeaderTree_pp_cppflags) $(AddHeaderTree_pp_cppflags) $(use_cppflags) $(AddHeaderTree_cppflags) $(app_AddHeaderTree_cppflags) $(AddHeaderTree_cppflags) $(AddHeaderTree_cxx_cppflags) -I../app ../app/AddHeaderTree.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: AddHeaderTreeclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(AddHeaderTree.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(AddHeaderTree.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_AddHeaderTree)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(AddHeaderTree.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(AddHeaderTree.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(AddHeaderTree.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

AddHeaderTreeclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) AddHeaderTree${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f AddHeaderTree${application_suffix}

#	@echo "------> (AddHeaderTree.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)AddHeaderTree.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf AddHeaderTree_deps AddHeaderTree_dependencies.make
#-- end of cleanup_objects ------
