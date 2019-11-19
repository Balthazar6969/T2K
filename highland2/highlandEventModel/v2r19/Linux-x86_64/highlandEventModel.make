#-- start of make_header -----------------

#====================================
#  Library highlandEventModel
#
#   Generated Mon Nov  4 15:26:43 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_highlandEventModel_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_highlandEventModel_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_highlandEventModel

highlandEventModel_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandEventModel = /tmp/CMT_$(highlandEventModel_tag)_highlandEventModel.make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandEventModel = $(highlandEventModel_tag)_highlandEventModel.make
cmt_local_tagfile_highlandEventModel = $(bin)$(highlandEventModel_tag)_highlandEventModel.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandEventModel_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandEventModel = /tmp/CMT_$(highlandEventModel_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandEventModel = $(highlandEventModel_tag).make
cmt_local_tagfile_highlandEventModel = $(bin)$(highlandEventModel_tag).make
endif

endif

-include $(cmt_local_tagfile_highlandEventModel)

ifdef cmt_highlandEventModel_has_target_tag

ifdef READONLY
cmt_final_setup_highlandEventModel = /tmp/CMT_highlandEventModel_highlandEventModelsetup.make
cmt_local_highlandEventModel_makefile = /tmp/CMT_highlandEventModel$(cmt_lock_pid).make
else
cmt_final_setup_highlandEventModel = $(bin)highlandEventModel_highlandEventModelsetup.make
cmt_local_highlandEventModel_makefile = $(bin)highlandEventModel.make
endif

else

ifdef READONLY
cmt_final_setup_highlandEventModel = /tmp/CMT_highlandEventModelsetup.make
cmt_local_highlandEventModel_makefile = /tmp/CMT_highlandEventModel$(cmt_lock_pid).make
else
cmt_final_setup_highlandEventModel = $(bin)highlandEventModelsetup.make
cmt_local_highlandEventModel_makefile = $(bin)highlandEventModel.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandEventModelsetup.make
else
cmt_final_setup = $(bin)highlandEventModelsetup.make
endif

highlandEventModel ::


ifdef READONLY
highlandEventModel ::
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
	$(echo) 'highlandEventModel'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = highlandEventModel/
highlandEventModel::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

highlandEventModellibname   = $(bin)$(library_prefix)highlandEventModel$(library_suffix)
highlandEventModellib       = $(highlandEventModellibname).a
highlandEventModelstamp     = $(bin)highlandEventModel.stamp
highlandEventModelshstamp   = $(bin)highlandEventModel.shstamp

highlandEventModel :: dirs  highlandEventModelLIB
	$(echo) "highlandEventModel ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

highlandEventModelLIB :: $(highlandEventModellib) $(highlandEventModelshstamp)
	$(echo) "highlandEventModel : library ok"

$(highlandEventModellib) :: $(bin)DataClasses.o $(bin)P0DDataClasses.o $(bin)DataClasses_Dict.o $(bin)P0DDataClasses_Dict.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(highlandEventModellib) $?
	$(lib_silent) $(ranlib) $(highlandEventModellib)
	$(lib_silent) cat /dev/null >$(highlandEventModelstamp)

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

$(highlandEventModellibname).$(shlibsuffix) :: $(highlandEventModellib) requirements $(use_requirements) $(highlandEventModelstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" highlandEventModel $(highlandEventModel_shlibflags)

$(highlandEventModelshstamp) :: $(highlandEventModellibname).$(shlibsuffix)
	$(lib_silent) if test -f $(highlandEventModellibname).$(shlibsuffix) ; then cat /dev/null >$(highlandEventModelshstamp) ; fi

highlandEventModelclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)DataClasses.o $(bin)P0DDataClasses.o $(bin)DataClasses_Dict.o $(bin)P0DDataClasses_Dict.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf highlandEventModel_deps highlandEventModel_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
highlandEventModelinstallname = $(library_prefix)highlandEventModel$(library_suffix).$(shlibsuffix)

highlandEventModel :: highlandEventModelinstall

install :: highlandEventModelinstall

highlandEventModelinstall :: $(install_dir)/$(highlandEventModelinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(highlandEventModelinstallname) :: $(bin)$(highlandEventModelinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandEventModelinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##highlandEventModelclean :: highlandEventModeluninstall

uninstall :: highlandEventModeluninstall

highlandEventModeluninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandEventModelinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),highlandEventModelclean)

#$(bin)highlandEventModel_dependencies.make :: dirs

ifndef QUICK
$(bin)highlandEventModel_dependencies.make : ../src/DataClasses.cxx ../src/P0DDataClasses.cxx ../dict/DataClasses_Dict.cxx ../dict/P0DDataClasses_Dict.cxx $(use_requirements) $(cmt_final_setup_highlandEventModel)
	$(echo) "(highlandEventModel.make) Rebuilding $@"; \
	  $(build_dependencies) highlandEventModel -all_sources -out=$@ ../src/DataClasses.cxx ../src/P0DDataClasses.cxx ../dict/DataClasses_Dict.cxx ../dict/P0DDataClasses_Dict.cxx
endif

#$(highlandEventModel_dependencies)

-include $(bin)highlandEventModel_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)highlandEventModel_dependencies.make : $(DataClasses_cxx_dependencies)

$(bin)$(binobj)DataClasses.o : $(DataClasses_cxx_dependencies)
	$(cpp_echo) $(src)DataClasses.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandEventModel_pp_cppflags) $(lib_highlandEventModel_pp_cppflags) $(DataClasses_pp_cppflags) $(use_cppflags) $(highlandEventModel_cppflags) $(lib_highlandEventModel_cppflags) $(DataClasses_cppflags) $(DataClasses_cxx_cppflags)  $(src)DataClasses.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandEventModel_dependencies.make : $(P0DDataClasses_cxx_dependencies)

$(bin)$(binobj)P0DDataClasses.o : $(P0DDataClasses_cxx_dependencies)
	$(cpp_echo) $(src)P0DDataClasses.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandEventModel_pp_cppflags) $(lib_highlandEventModel_pp_cppflags) $(P0DDataClasses_pp_cppflags) $(use_cppflags) $(highlandEventModel_cppflags) $(lib_highlandEventModel_cppflags) $(P0DDataClasses_cppflags) $(P0DDataClasses_cxx_cppflags)  $(src)P0DDataClasses.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandEventModel_dependencies.make : $(DataClasses_Dict_cxx_dependencies)

$(bin)$(binobj)DataClasses_Dict.o : $(DataClasses_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/DataClasses_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandEventModel_pp_cppflags) $(lib_highlandEventModel_pp_cppflags) $(DataClasses_Dict_pp_cppflags) $(use_cppflags) $(highlandEventModel_cppflags) $(lib_highlandEventModel_cppflags) $(DataClasses_Dict_cppflags) $(DataClasses_Dict_cxx_cppflags) -I../dict ../dict/DataClasses_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandEventModel_dependencies.make : $(P0DDataClasses_Dict_cxx_dependencies)

$(bin)$(binobj)P0DDataClasses_Dict.o : $(P0DDataClasses_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/P0DDataClasses_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandEventModel_pp_cppflags) $(lib_highlandEventModel_pp_cppflags) $(P0DDataClasses_Dict_pp_cppflags) $(use_cppflags) $(highlandEventModel_cppflags) $(lib_highlandEventModel_cppflags) $(P0DDataClasses_Dict_cppflags) $(P0DDataClasses_Dict_cxx_cppflags) -I../dict ../dict/P0DDataClasses_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: highlandEventModelclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(highlandEventModel.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(highlandEventModel.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_highlandEventModel)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandEventModel.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandEventModel.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandEventModel.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

highlandEventModelclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)highlandEventModel$(library_suffix).a $(binobj)$(library_prefix)highlandEventModel$(library_suffix).s? $(binobj)highlandEventModel.stamp $(binobj)highlandEventModel.shstamp
#-- end of cleanup_library ---------------

