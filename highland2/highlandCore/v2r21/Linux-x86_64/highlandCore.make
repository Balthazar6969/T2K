#-- start of make_header -----------------

#====================================
#  Library highlandCore
#
#   Generated Mon Nov  4 15:26:38 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_highlandCore_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_highlandCore_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_highlandCore

highlandCore_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandCore = /tmp/CMT_$(highlandCore_tag)_highlandCore.make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandCore = $(highlandCore_tag)_highlandCore.make
cmt_local_tagfile_highlandCore = $(bin)$(highlandCore_tag)_highlandCore.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandCore_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandCore = /tmp/CMT_$(highlandCore_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandCore = $(highlandCore_tag).make
cmt_local_tagfile_highlandCore = $(bin)$(highlandCore_tag).make
endif

endif

-include $(cmt_local_tagfile_highlandCore)

ifdef cmt_highlandCore_has_target_tag

ifdef READONLY
cmt_final_setup_highlandCore = /tmp/CMT_highlandCore_highlandCoresetup.make
cmt_local_highlandCore_makefile = /tmp/CMT_highlandCore$(cmt_lock_pid).make
else
cmt_final_setup_highlandCore = $(bin)highlandCore_highlandCoresetup.make
cmt_local_highlandCore_makefile = $(bin)highlandCore.make
endif

else

ifdef READONLY
cmt_final_setup_highlandCore = /tmp/CMT_highlandCoresetup.make
cmt_local_highlandCore_makefile = /tmp/CMT_highlandCore$(cmt_lock_pid).make
else
cmt_final_setup_highlandCore = $(bin)highlandCoresetup.make
cmt_local_highlandCore_makefile = $(bin)highlandCore.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandCoresetup.make
else
cmt_final_setup = $(bin)highlandCoresetup.make
endif

highlandCore ::


ifdef READONLY
highlandCore ::
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
	$(echo) 'highlandCore'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = highlandCore/
highlandCore::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

highlandCorelibname   = $(bin)$(library_prefix)highlandCore$(library_suffix)
highlandCorelib       = $(highlandCorelibname).a
highlandCorestamp     = $(bin)highlandCore.stamp
highlandCoreshstamp   = $(bin)highlandCore.shstamp

highlandCore :: dirs  highlandCoreLIB
	$(echo) "highlandCore ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

highlandCoreLIB :: $(highlandCorelib) $(highlandCoreshstamp)
	$(echo) "highlandCore : library ok"

$(highlandCorelib) :: $(bin)AnalysisAlgorithm.o $(bin)AnalysisLoop.o $(bin)CategoryClasses.o $(bin)CategoryManager.o $(bin)ConfigurationBase.o $(bin)ConfigurationManager.o $(bin)CorrectionBase.o $(bin)CorrectionManager.o $(bin)DocString.o $(bin)DocStringManager.o $(bin)HighlandInputManager.o $(bin)MemoryUsage.o $(bin)OutputManager.o $(bin)SimpleLoopBase.o $(bin)VersionManager.o $(bin)ZeroToyMaker.o $(bin)CategoryClasses_Dict.o $(bin)ConfigurationBase_Dict.o $(bin)CorrectionBase_Dict.o $(bin)DocString_Dict.o $(bin)OutputManager_Dict.o $(bin)VersionManager_Dict.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(highlandCorelib) $?
	$(lib_silent) $(ranlib) $(highlandCorelib)
	$(lib_silent) cat /dev/null >$(highlandCorestamp)

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

$(highlandCorelibname).$(shlibsuffix) :: $(highlandCorelib) requirements $(use_requirements) $(highlandCorestamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" highlandCore $(highlandCore_shlibflags)

$(highlandCoreshstamp) :: $(highlandCorelibname).$(shlibsuffix)
	$(lib_silent) if test -f $(highlandCorelibname).$(shlibsuffix) ; then cat /dev/null >$(highlandCoreshstamp) ; fi

highlandCoreclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)AnalysisAlgorithm.o $(bin)AnalysisLoop.o $(bin)CategoryClasses.o $(bin)CategoryManager.o $(bin)ConfigurationBase.o $(bin)ConfigurationManager.o $(bin)CorrectionBase.o $(bin)CorrectionManager.o $(bin)DocString.o $(bin)DocStringManager.o $(bin)HighlandInputManager.o $(bin)MemoryUsage.o $(bin)OutputManager.o $(bin)SimpleLoopBase.o $(bin)VersionManager.o $(bin)ZeroToyMaker.o $(bin)CategoryClasses_Dict.o $(bin)ConfigurationBase_Dict.o $(bin)CorrectionBase_Dict.o $(bin)DocString_Dict.o $(bin)OutputManager_Dict.o $(bin)VersionManager_Dict.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf highlandCore_deps highlandCore_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
highlandCoreinstallname = $(library_prefix)highlandCore$(library_suffix).$(shlibsuffix)

highlandCore :: highlandCoreinstall

install :: highlandCoreinstall

highlandCoreinstall :: $(install_dir)/$(highlandCoreinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(highlandCoreinstallname) :: $(bin)$(highlandCoreinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandCoreinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##highlandCoreclean :: highlandCoreuninstall

uninstall :: highlandCoreuninstall

highlandCoreuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandCoreinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),highlandCoreclean)

#$(bin)highlandCore_dependencies.make :: dirs

ifndef QUICK
$(bin)highlandCore_dependencies.make : ../src/AnalysisAlgorithm.cxx ../src/AnalysisLoop.cxx ../src/CategoryClasses.cxx ../src/CategoryManager.cxx ../src/ConfigurationBase.cxx ../src/ConfigurationManager.cxx ../src/CorrectionBase.cxx ../src/CorrectionManager.cxx ../src/DocString.cxx ../src/DocStringManager.cxx ../src/HighlandInputManager.cxx ../src/MemoryUsage.cxx ../src/OutputManager.cxx ../src/SimpleLoopBase.cxx ../src/VersionManager.cxx ../src/ZeroToyMaker.cxx ../dict/CategoryClasses_Dict.cxx ../dict/ConfigurationBase_Dict.cxx ../dict/CorrectionBase_Dict.cxx ../dict/DocString_Dict.cxx ../dict/OutputManager_Dict.cxx ../dict/VersionManager_Dict.cxx $(use_requirements) $(cmt_final_setup_highlandCore)
	$(echo) "(highlandCore.make) Rebuilding $@"; \
	  $(build_dependencies) highlandCore -all_sources -out=$@ ../src/AnalysisAlgorithm.cxx ../src/AnalysisLoop.cxx ../src/CategoryClasses.cxx ../src/CategoryManager.cxx ../src/ConfigurationBase.cxx ../src/ConfigurationManager.cxx ../src/CorrectionBase.cxx ../src/CorrectionManager.cxx ../src/DocString.cxx ../src/DocStringManager.cxx ../src/HighlandInputManager.cxx ../src/MemoryUsage.cxx ../src/OutputManager.cxx ../src/SimpleLoopBase.cxx ../src/VersionManager.cxx ../src/ZeroToyMaker.cxx ../dict/CategoryClasses_Dict.cxx ../dict/ConfigurationBase_Dict.cxx ../dict/CorrectionBase_Dict.cxx ../dict/DocString_Dict.cxx ../dict/OutputManager_Dict.cxx ../dict/VersionManager_Dict.cxx
endif

#$(highlandCore_dependencies)

-include $(bin)highlandCore_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(AnalysisAlgorithm_cxx_dependencies)

$(bin)$(binobj)AnalysisAlgorithm.o : $(AnalysisAlgorithm_cxx_dependencies)
	$(cpp_echo) $(src)AnalysisAlgorithm.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(AnalysisAlgorithm_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(AnalysisAlgorithm_cppflags) $(AnalysisAlgorithm_cxx_cppflags)  $(src)AnalysisAlgorithm.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(AnalysisLoop_cxx_dependencies)

$(bin)$(binobj)AnalysisLoop.o : $(AnalysisLoop_cxx_dependencies)
	$(cpp_echo) $(src)AnalysisLoop.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(AnalysisLoop_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(AnalysisLoop_cppflags) $(AnalysisLoop_cxx_cppflags)  $(src)AnalysisLoop.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(CategoryClasses_cxx_dependencies)

$(bin)$(binobj)CategoryClasses.o : $(CategoryClasses_cxx_dependencies)
	$(cpp_echo) $(src)CategoryClasses.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(CategoryClasses_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(CategoryClasses_cppflags) $(CategoryClasses_cxx_cppflags)  $(src)CategoryClasses.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(CategoryManager_cxx_dependencies)

$(bin)$(binobj)CategoryManager.o : $(CategoryManager_cxx_dependencies)
	$(cpp_echo) $(src)CategoryManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(CategoryManager_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(CategoryManager_cppflags) $(CategoryManager_cxx_cppflags)  $(src)CategoryManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(ConfigurationBase_cxx_dependencies)

$(bin)$(binobj)ConfigurationBase.o : $(ConfigurationBase_cxx_dependencies)
	$(cpp_echo) $(src)ConfigurationBase.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(ConfigurationBase_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(ConfigurationBase_cppflags) $(ConfigurationBase_cxx_cppflags)  $(src)ConfigurationBase.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(ConfigurationManager_cxx_dependencies)

$(bin)$(binobj)ConfigurationManager.o : $(ConfigurationManager_cxx_dependencies)
	$(cpp_echo) $(src)ConfigurationManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(ConfigurationManager_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(ConfigurationManager_cppflags) $(ConfigurationManager_cxx_cppflags)  $(src)ConfigurationManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(CorrectionBase_cxx_dependencies)

$(bin)$(binobj)CorrectionBase.o : $(CorrectionBase_cxx_dependencies)
	$(cpp_echo) $(src)CorrectionBase.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(CorrectionBase_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(CorrectionBase_cppflags) $(CorrectionBase_cxx_cppflags)  $(src)CorrectionBase.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(CorrectionManager_cxx_dependencies)

$(bin)$(binobj)CorrectionManager.o : $(CorrectionManager_cxx_dependencies)
	$(cpp_echo) $(src)CorrectionManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(CorrectionManager_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(CorrectionManager_cppflags) $(CorrectionManager_cxx_cppflags)  $(src)CorrectionManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(DocString_cxx_dependencies)

$(bin)$(binobj)DocString.o : $(DocString_cxx_dependencies)
	$(cpp_echo) $(src)DocString.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(DocString_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(DocString_cppflags) $(DocString_cxx_cppflags)  $(src)DocString.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(DocStringManager_cxx_dependencies)

$(bin)$(binobj)DocStringManager.o : $(DocStringManager_cxx_dependencies)
	$(cpp_echo) $(src)DocStringManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(DocStringManager_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(DocStringManager_cppflags) $(DocStringManager_cxx_cppflags)  $(src)DocStringManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(HighlandInputManager_cxx_dependencies)

$(bin)$(binobj)HighlandInputManager.o : $(HighlandInputManager_cxx_dependencies)
	$(cpp_echo) $(src)HighlandInputManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(HighlandInputManager_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(HighlandInputManager_cppflags) $(HighlandInputManager_cxx_cppflags)  $(src)HighlandInputManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(MemoryUsage_cxx_dependencies)

$(bin)$(binobj)MemoryUsage.o : $(MemoryUsage_cxx_dependencies)
	$(cpp_echo) $(src)MemoryUsage.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(MemoryUsage_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(MemoryUsage_cppflags) $(MemoryUsage_cxx_cppflags)  $(src)MemoryUsage.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(OutputManager_cxx_dependencies)

$(bin)$(binobj)OutputManager.o : $(OutputManager_cxx_dependencies)
	$(cpp_echo) $(src)OutputManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(OutputManager_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(OutputManager_cppflags) $(OutputManager_cxx_cppflags)  $(src)OutputManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(SimpleLoopBase_cxx_dependencies)

$(bin)$(binobj)SimpleLoopBase.o : $(SimpleLoopBase_cxx_dependencies)
	$(cpp_echo) $(src)SimpleLoopBase.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(SimpleLoopBase_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(SimpleLoopBase_cppflags) $(SimpleLoopBase_cxx_cppflags)  $(src)SimpleLoopBase.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(VersionManager_cxx_dependencies)

$(bin)$(binobj)VersionManager.o : $(VersionManager_cxx_dependencies)
	$(cpp_echo) $(src)VersionManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(VersionManager_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(VersionManager_cppflags) $(VersionManager_cxx_cppflags)  $(src)VersionManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(ZeroToyMaker_cxx_dependencies)

$(bin)$(binobj)ZeroToyMaker.o : $(ZeroToyMaker_cxx_dependencies)
	$(cpp_echo) $(src)ZeroToyMaker.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(ZeroToyMaker_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(ZeroToyMaker_cppflags) $(ZeroToyMaker_cxx_cppflags)  $(src)ZeroToyMaker.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(CategoryClasses_Dict_cxx_dependencies)

$(bin)$(binobj)CategoryClasses_Dict.o : $(CategoryClasses_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/CategoryClasses_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(CategoryClasses_Dict_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(CategoryClasses_Dict_cppflags) $(CategoryClasses_Dict_cxx_cppflags) -I../dict ../dict/CategoryClasses_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(ConfigurationBase_Dict_cxx_dependencies)

$(bin)$(binobj)ConfigurationBase_Dict.o : $(ConfigurationBase_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/ConfigurationBase_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(ConfigurationBase_Dict_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(ConfigurationBase_Dict_cppflags) $(ConfigurationBase_Dict_cxx_cppflags) -I../dict ../dict/ConfigurationBase_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(CorrectionBase_Dict_cxx_dependencies)

$(bin)$(binobj)CorrectionBase_Dict.o : $(CorrectionBase_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/CorrectionBase_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(CorrectionBase_Dict_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(CorrectionBase_Dict_cppflags) $(CorrectionBase_Dict_cxx_cppflags) -I../dict ../dict/CorrectionBase_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(DocString_Dict_cxx_dependencies)

$(bin)$(binobj)DocString_Dict.o : $(DocString_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/DocString_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(DocString_Dict_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(DocString_Dict_cppflags) $(DocString_Dict_cxx_cppflags) -I../dict ../dict/DocString_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(OutputManager_Dict_cxx_dependencies)

$(bin)$(binobj)OutputManager_Dict.o : $(OutputManager_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/OutputManager_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(OutputManager_Dict_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(OutputManager_Dict_cppflags) $(OutputManager_Dict_cxx_cppflags) -I../dict ../dict/OutputManager_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCore_dependencies.make : $(VersionManager_Dict_cxx_dependencies)

$(bin)$(binobj)VersionManager_Dict.o : $(VersionManager_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/VersionManager_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCore_pp_cppflags) $(lib_highlandCore_pp_cppflags) $(VersionManager_Dict_pp_cppflags) $(use_cppflags) $(highlandCore_cppflags) $(lib_highlandCore_cppflags) $(VersionManager_Dict_cppflags) $(VersionManager_Dict_cxx_cppflags) -I../dict ../dict/VersionManager_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: highlandCoreclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(highlandCore.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(highlandCore.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_highlandCore)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandCore.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandCore.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandCore.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

highlandCoreclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)highlandCore$(library_suffix).a $(binobj)$(library_prefix)highlandCore$(library_suffix).s? $(binobj)highlandCore.stamp $(binobj)highlandCore.shstamp
#-- end of cleanup_library ---------------

