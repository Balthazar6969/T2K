#-- start of make_header -----------------

#====================================
#  Library highlandTools
#
#   Generated Mon Nov  4 15:26:40 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_highlandTools_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_highlandTools_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_highlandTools

highlandTools_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandTools = /tmp/CMT_$(highlandTools_tag)_highlandTools.make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandTools = $(highlandTools_tag)_highlandTools.make
cmt_local_tagfile_highlandTools = $(bin)$(highlandTools_tag)_highlandTools.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandTools_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandTools = /tmp/CMT_$(highlandTools_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandTools = $(highlandTools_tag).make
cmt_local_tagfile_highlandTools = $(bin)$(highlandTools_tag).make
endif

endif

-include $(cmt_local_tagfile_highlandTools)

ifdef cmt_highlandTools_has_target_tag

ifdef READONLY
cmt_final_setup_highlandTools = /tmp/CMT_highlandTools_highlandToolssetup.make
cmt_local_highlandTools_makefile = /tmp/CMT_highlandTools$(cmt_lock_pid).make
else
cmt_final_setup_highlandTools = $(bin)highlandTools_highlandToolssetup.make
cmt_local_highlandTools_makefile = $(bin)highlandTools.make
endif

else

ifdef READONLY
cmt_final_setup_highlandTools = /tmp/CMT_highlandToolssetup.make
cmt_local_highlandTools_makefile = /tmp/CMT_highlandTools$(cmt_lock_pid).make
else
cmt_final_setup_highlandTools = $(bin)highlandToolssetup.make
cmt_local_highlandTools_makefile = $(bin)highlandTools.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandToolssetup.make
else
cmt_final_setup = $(bin)highlandToolssetup.make
endif

highlandTools ::


ifdef READONLY
highlandTools ::
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
	$(echo) 'highlandTools'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = highlandTools/
highlandTools::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

highlandToolslibname   = $(bin)$(library_prefix)highlandTools$(library_suffix)
highlandToolslib       = $(highlandToolslibname).a
highlandToolsstamp     = $(bin)highlandTools.stamp
highlandToolsshstamp   = $(bin)highlandTools.shstamp

highlandTools :: dirs  highlandToolsLIB
	$(echo) "highlandTools ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

highlandToolsLIB :: $(highlandToolslib) $(highlandToolsshstamp)
	$(echo) "highlandTools : library ok"

$(highlandToolslib) :: $(bin)ConfigTreeTools.o $(bin)DrawingTools.o $(bin)DrawingToolsBase.o $(bin)DrawingUtils.o $(bin)HistoStack.o $(bin)SystematicTools.o $(bin)WeightTools.o $(bin)ConfigTreeTools_Dict.o $(bin)DrawingToolsBase_Dict.o $(bin)DrawingTools_Dict.o $(bin)HistoStack_Dict.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(highlandToolslib) $?
	$(lib_silent) $(ranlib) $(highlandToolslib)
	$(lib_silent) cat /dev/null >$(highlandToolsstamp)

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

$(highlandToolslibname).$(shlibsuffix) :: $(highlandToolslib) requirements $(use_requirements) $(highlandToolsstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" highlandTools $(highlandTools_shlibflags)

$(highlandToolsshstamp) :: $(highlandToolslibname).$(shlibsuffix)
	$(lib_silent) if test -f $(highlandToolslibname).$(shlibsuffix) ; then cat /dev/null >$(highlandToolsshstamp) ; fi

highlandToolsclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)ConfigTreeTools.o $(bin)DrawingTools.o $(bin)DrawingToolsBase.o $(bin)DrawingUtils.o $(bin)HistoStack.o $(bin)SystematicTools.o $(bin)WeightTools.o $(bin)ConfigTreeTools_Dict.o $(bin)DrawingToolsBase_Dict.o $(bin)DrawingTools_Dict.o $(bin)HistoStack_Dict.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf highlandTools_deps highlandTools_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
highlandToolsinstallname = $(library_prefix)highlandTools$(library_suffix).$(shlibsuffix)

highlandTools :: highlandToolsinstall

install :: highlandToolsinstall

highlandToolsinstall :: $(install_dir)/$(highlandToolsinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(highlandToolsinstallname) :: $(bin)$(highlandToolsinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandToolsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##highlandToolsclean :: highlandToolsuninstall

uninstall :: highlandToolsuninstall

highlandToolsuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandToolsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),highlandToolsclean)

#$(bin)highlandTools_dependencies.make :: dirs

ifndef QUICK
$(bin)highlandTools_dependencies.make : ../src/ConfigTreeTools.cxx ../src/DrawingTools.cxx ../src/DrawingToolsBase.cxx ../src/DrawingUtils.cxx ../src/HistoStack.cxx ../src/SystematicTools.cxx ../src/WeightTools.cxx ../dict/ConfigTreeTools_Dict.cxx ../dict/DrawingToolsBase_Dict.cxx ../dict/DrawingTools_Dict.cxx ../dict/HistoStack_Dict.cxx $(use_requirements) $(cmt_final_setup_highlandTools)
	$(echo) "(highlandTools.make) Rebuilding $@"; \
	  $(build_dependencies) highlandTools -all_sources -out=$@ ../src/ConfigTreeTools.cxx ../src/DrawingTools.cxx ../src/DrawingToolsBase.cxx ../src/DrawingUtils.cxx ../src/HistoStack.cxx ../src/SystematicTools.cxx ../src/WeightTools.cxx ../dict/ConfigTreeTools_Dict.cxx ../dict/DrawingToolsBase_Dict.cxx ../dict/DrawingTools_Dict.cxx ../dict/HistoStack_Dict.cxx
endif

#$(highlandTools_dependencies)

-include $(bin)highlandTools_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)highlandTools_dependencies.make : $(ConfigTreeTools_cxx_dependencies)

$(bin)$(binobj)ConfigTreeTools.o : $(ConfigTreeTools_cxx_dependencies)
	$(cpp_echo) $(src)ConfigTreeTools.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandTools_pp_cppflags) $(lib_highlandTools_pp_cppflags) $(ConfigTreeTools_pp_cppflags) $(use_cppflags) $(highlandTools_cppflags) $(lib_highlandTools_cppflags) $(ConfigTreeTools_cppflags) $(ConfigTreeTools_cxx_cppflags)  $(src)ConfigTreeTools.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandTools_dependencies.make : $(DrawingTools_cxx_dependencies)

$(bin)$(binobj)DrawingTools.o : $(DrawingTools_cxx_dependencies)
	$(cpp_echo) $(src)DrawingTools.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandTools_pp_cppflags) $(lib_highlandTools_pp_cppflags) $(DrawingTools_pp_cppflags) $(use_cppflags) $(highlandTools_cppflags) $(lib_highlandTools_cppflags) $(DrawingTools_cppflags) $(DrawingTools_cxx_cppflags)  $(src)DrawingTools.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandTools_dependencies.make : $(DrawingToolsBase_cxx_dependencies)

$(bin)$(binobj)DrawingToolsBase.o : $(DrawingToolsBase_cxx_dependencies)
	$(cpp_echo) $(src)DrawingToolsBase.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandTools_pp_cppflags) $(lib_highlandTools_pp_cppflags) $(DrawingToolsBase_pp_cppflags) $(use_cppflags) $(highlandTools_cppflags) $(lib_highlandTools_cppflags) $(DrawingToolsBase_cppflags) $(DrawingToolsBase_cxx_cppflags)  $(src)DrawingToolsBase.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandTools_dependencies.make : $(DrawingUtils_cxx_dependencies)

$(bin)$(binobj)DrawingUtils.o : $(DrawingUtils_cxx_dependencies)
	$(cpp_echo) $(src)DrawingUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandTools_pp_cppflags) $(lib_highlandTools_pp_cppflags) $(DrawingUtils_pp_cppflags) $(use_cppflags) $(highlandTools_cppflags) $(lib_highlandTools_cppflags) $(DrawingUtils_cppflags) $(DrawingUtils_cxx_cppflags)  $(src)DrawingUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandTools_dependencies.make : $(HistoStack_cxx_dependencies)

$(bin)$(binobj)HistoStack.o : $(HistoStack_cxx_dependencies)
	$(cpp_echo) $(src)HistoStack.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandTools_pp_cppflags) $(lib_highlandTools_pp_cppflags) $(HistoStack_pp_cppflags) $(use_cppflags) $(highlandTools_cppflags) $(lib_highlandTools_cppflags) $(HistoStack_cppflags) $(HistoStack_cxx_cppflags)  $(src)HistoStack.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandTools_dependencies.make : $(SystematicTools_cxx_dependencies)

$(bin)$(binobj)SystematicTools.o : $(SystematicTools_cxx_dependencies)
	$(cpp_echo) $(src)SystematicTools.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandTools_pp_cppflags) $(lib_highlandTools_pp_cppflags) $(SystematicTools_pp_cppflags) $(use_cppflags) $(highlandTools_cppflags) $(lib_highlandTools_cppflags) $(SystematicTools_cppflags) $(SystematicTools_cxx_cppflags)  $(src)SystematicTools.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandTools_dependencies.make : $(WeightTools_cxx_dependencies)

$(bin)$(binobj)WeightTools.o : $(WeightTools_cxx_dependencies)
	$(cpp_echo) $(src)WeightTools.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandTools_pp_cppflags) $(lib_highlandTools_pp_cppflags) $(WeightTools_pp_cppflags) $(use_cppflags) $(highlandTools_cppflags) $(lib_highlandTools_cppflags) $(WeightTools_cppflags) $(WeightTools_cxx_cppflags)  $(src)WeightTools.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandTools_dependencies.make : $(ConfigTreeTools_Dict_cxx_dependencies)

$(bin)$(binobj)ConfigTreeTools_Dict.o : $(ConfigTreeTools_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/ConfigTreeTools_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandTools_pp_cppflags) $(lib_highlandTools_pp_cppflags) $(ConfigTreeTools_Dict_pp_cppflags) $(use_cppflags) $(highlandTools_cppflags) $(lib_highlandTools_cppflags) $(ConfigTreeTools_Dict_cppflags) $(ConfigTreeTools_Dict_cxx_cppflags) -I../dict ../dict/ConfigTreeTools_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandTools_dependencies.make : $(DrawingToolsBase_Dict_cxx_dependencies)

$(bin)$(binobj)DrawingToolsBase_Dict.o : $(DrawingToolsBase_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/DrawingToolsBase_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandTools_pp_cppflags) $(lib_highlandTools_pp_cppflags) $(DrawingToolsBase_Dict_pp_cppflags) $(use_cppflags) $(highlandTools_cppflags) $(lib_highlandTools_cppflags) $(DrawingToolsBase_Dict_cppflags) $(DrawingToolsBase_Dict_cxx_cppflags) -I../dict ../dict/DrawingToolsBase_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandTools_dependencies.make : $(DrawingTools_Dict_cxx_dependencies)

$(bin)$(binobj)DrawingTools_Dict.o : $(DrawingTools_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/DrawingTools_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandTools_pp_cppflags) $(lib_highlandTools_pp_cppflags) $(DrawingTools_Dict_pp_cppflags) $(use_cppflags) $(highlandTools_cppflags) $(lib_highlandTools_cppflags) $(DrawingTools_Dict_cppflags) $(DrawingTools_Dict_cxx_cppflags) -I../dict ../dict/DrawingTools_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandTools_dependencies.make : $(HistoStack_Dict_cxx_dependencies)

$(bin)$(binobj)HistoStack_Dict.o : $(HistoStack_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/HistoStack_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandTools_pp_cppflags) $(lib_highlandTools_pp_cppflags) $(HistoStack_Dict_pp_cppflags) $(use_cppflags) $(highlandTools_cppflags) $(lib_highlandTools_cppflags) $(HistoStack_Dict_cppflags) $(HistoStack_Dict_cxx_cppflags) -I../dict ../dict/HistoStack_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: highlandToolsclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(highlandTools.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(highlandTools.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_highlandTools)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandTools.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandTools.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandTools.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

highlandToolsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)highlandTools$(library_suffix).a $(binobj)$(library_prefix)highlandTools$(library_suffix).s? $(binobj)highlandTools.stamp $(binobj)highlandTools.shstamp
#-- end of cleanup_library ---------------

