#-- start of make_header -----------------

#====================================
#  Library psycheEventModel
#
#   Generated Mon Nov  4 15:26:41 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_psycheEventModel_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_psycheEventModel_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_psycheEventModel

psycheEventModel_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheEventModel = /tmp/CMT_$(psycheEventModel_tag)_psycheEventModel.make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheEventModel = $(psycheEventModel_tag)_psycheEventModel.make
cmt_local_tagfile_psycheEventModel = $(bin)$(psycheEventModel_tag)_psycheEventModel.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheEventModel_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheEventModel = /tmp/CMT_$(psycheEventModel_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheEventModel = $(psycheEventModel_tag).make
cmt_local_tagfile_psycheEventModel = $(bin)$(psycheEventModel_tag).make
endif

endif

-include $(cmt_local_tagfile_psycheEventModel)

ifdef cmt_psycheEventModel_has_target_tag

ifdef READONLY
cmt_final_setup_psycheEventModel = /tmp/CMT_psycheEventModel_psycheEventModelsetup.make
cmt_local_psycheEventModel_makefile = /tmp/CMT_psycheEventModel$(cmt_lock_pid).make
else
cmt_final_setup_psycheEventModel = $(bin)psycheEventModel_psycheEventModelsetup.make
cmt_local_psycheEventModel_makefile = $(bin)psycheEventModel.make
endif

else

ifdef READONLY
cmt_final_setup_psycheEventModel = /tmp/CMT_psycheEventModelsetup.make
cmt_local_psycheEventModel_makefile = /tmp/CMT_psycheEventModel$(cmt_lock_pid).make
else
cmt_final_setup_psycheEventModel = $(bin)psycheEventModelsetup.make
cmt_local_psycheEventModel_makefile = $(bin)psycheEventModel.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheEventModelsetup.make
else
cmt_final_setup = $(bin)psycheEventModelsetup.make
endif

psycheEventModel ::


ifdef READONLY
psycheEventModel ::
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
	$(echo) 'psycheEventModel'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = psycheEventModel/
psycheEventModel::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

psycheEventModellibname   = $(bin)$(library_prefix)psycheEventModel$(library_suffix)
psycheEventModellib       = $(psycheEventModellibname).a
psycheEventModelstamp     = $(bin)psycheEventModel.stamp
psycheEventModelshstamp   = $(bin)psycheEventModel.shstamp

psycheEventModel :: dirs  psycheEventModelLIB
	$(echo) "psycheEventModel ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

psycheEventModelLIB :: $(psycheEventModellib) $(psycheEventModelshstamp)
	$(echo) "psycheEventModel : library ok"

$(psycheEventModellib) :: $(bin)BaseDataClasses.o $(bin)BasicUtils.o $(bin)SampleId.o $(bin)SubDetId.o $(bin)BaseDataClasses_Dict.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(psycheEventModellib) $?
	$(lib_silent) $(ranlib) $(psycheEventModellib)
	$(lib_silent) cat /dev/null >$(psycheEventModelstamp)

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

$(psycheEventModellibname).$(shlibsuffix) :: $(psycheEventModellib) requirements $(use_requirements) $(psycheEventModelstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" psycheEventModel $(psycheEventModel_shlibflags)

$(psycheEventModelshstamp) :: $(psycheEventModellibname).$(shlibsuffix)
	$(lib_silent) if test -f $(psycheEventModellibname).$(shlibsuffix) ; then cat /dev/null >$(psycheEventModelshstamp) ; fi

psycheEventModelclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)BaseDataClasses.o $(bin)BasicUtils.o $(bin)SampleId.o $(bin)SubDetId.o $(bin)BaseDataClasses_Dict.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf psycheEventModel_deps psycheEventModel_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
psycheEventModelinstallname = $(library_prefix)psycheEventModel$(library_suffix).$(shlibsuffix)

psycheEventModel :: psycheEventModelinstall

install :: psycheEventModelinstall

psycheEventModelinstall :: $(install_dir)/$(psycheEventModelinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(psycheEventModelinstallname) :: $(bin)$(psycheEventModelinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheEventModelinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##psycheEventModelclean :: psycheEventModeluninstall

uninstall :: psycheEventModeluninstall

psycheEventModeluninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheEventModelinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),psycheEventModelclean)

#$(bin)psycheEventModel_dependencies.make :: dirs

ifndef QUICK
$(bin)psycheEventModel_dependencies.make : ../src/BaseDataClasses.cxx ../src/BasicUtils.cxx ../src/SampleId.cxx ../src/SubDetId.cxx ../dict/BaseDataClasses_Dict.cxx $(use_requirements) $(cmt_final_setup_psycheEventModel)
	$(echo) "(psycheEventModel.make) Rebuilding $@"; \
	  $(build_dependencies) psycheEventModel -all_sources -out=$@ ../src/BaseDataClasses.cxx ../src/BasicUtils.cxx ../src/SampleId.cxx ../src/SubDetId.cxx ../dict/BaseDataClasses_Dict.cxx
endif

#$(psycheEventModel_dependencies)

-include $(bin)psycheEventModel_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)psycheEventModel_dependencies.make : $(BaseDataClasses_cxx_dependencies)

$(bin)$(binobj)BaseDataClasses.o : $(BaseDataClasses_cxx_dependencies)
	$(cpp_echo) $(src)BaseDataClasses.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheEventModel_pp_cppflags) $(lib_psycheEventModel_pp_cppflags) $(BaseDataClasses_pp_cppflags) $(use_cppflags) $(psycheEventModel_cppflags) $(lib_psycheEventModel_cppflags) $(BaseDataClasses_cppflags) $(BaseDataClasses_cxx_cppflags)  $(src)BaseDataClasses.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheEventModel_dependencies.make : $(BasicUtils_cxx_dependencies)

$(bin)$(binobj)BasicUtils.o : $(BasicUtils_cxx_dependencies)
	$(cpp_echo) $(src)BasicUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheEventModel_pp_cppflags) $(lib_psycheEventModel_pp_cppflags) $(BasicUtils_pp_cppflags) $(use_cppflags) $(psycheEventModel_cppflags) $(lib_psycheEventModel_cppflags) $(BasicUtils_cppflags) $(BasicUtils_cxx_cppflags)  $(src)BasicUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheEventModel_dependencies.make : $(SampleId_cxx_dependencies)

$(bin)$(binobj)SampleId.o : $(SampleId_cxx_dependencies)
	$(cpp_echo) $(src)SampleId.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheEventModel_pp_cppflags) $(lib_psycheEventModel_pp_cppflags) $(SampleId_pp_cppflags) $(use_cppflags) $(psycheEventModel_cppflags) $(lib_psycheEventModel_cppflags) $(SampleId_cppflags) $(SampleId_cxx_cppflags)  $(src)SampleId.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheEventModel_dependencies.make : $(SubDetId_cxx_dependencies)

$(bin)$(binobj)SubDetId.o : $(SubDetId_cxx_dependencies)
	$(cpp_echo) $(src)SubDetId.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheEventModel_pp_cppflags) $(lib_psycheEventModel_pp_cppflags) $(SubDetId_pp_cppflags) $(use_cppflags) $(psycheEventModel_cppflags) $(lib_psycheEventModel_cppflags) $(SubDetId_cppflags) $(SubDetId_cxx_cppflags)  $(src)SubDetId.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheEventModel_dependencies.make : $(BaseDataClasses_Dict_cxx_dependencies)

$(bin)$(binobj)BaseDataClasses_Dict.o : $(BaseDataClasses_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/BaseDataClasses_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheEventModel_pp_cppflags) $(lib_psycheEventModel_pp_cppflags) $(BaseDataClasses_Dict_pp_cppflags) $(use_cppflags) $(psycheEventModel_cppflags) $(lib_psycheEventModel_cppflags) $(BaseDataClasses_Dict_cppflags) $(BaseDataClasses_Dict_cxx_cppflags) -I../dict ../dict/BaseDataClasses_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: psycheEventModelclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(psycheEventModel.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(psycheEventModel.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_psycheEventModel)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheEventModel.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheEventModel.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheEventModel.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

psycheEventModelclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)psycheEventModel$(library_suffix).a $(binobj)$(library_prefix)psycheEventModel$(library_suffix).s? $(binobj)psycheEventModel.stamp $(binobj)psycheEventModel.shstamp
#-- end of cleanup_library ---------------

