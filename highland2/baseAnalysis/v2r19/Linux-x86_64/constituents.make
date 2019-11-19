
#-- start of constituents_header ------

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

tags      = $(tag),$(CMTEXTRATAGS)

baseAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile = /tmp/CMT_$(baseAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile = $(baseAnalysis_tag).make
cmt_local_tagfile = $(bin)$(baseAnalysis_tag).make
endif

#-include $(cmt_local_tagfile)
include $(cmt_local_tagfile)

ifdef READONLY
cmt_local_setup = /tmp/CMT_baseAnalysissetup$(cmt_lock_pid).make
cmt_final_setup = /tmp/CMT_baseAnalysissetup.make
else
#cmt_local_setup = $(bin)baseAnalysissetup$(cmt_lock_pid).make
cmt_local_setup = $(bin)$(package)setup$$$$.make
#cmt_final_setup = $(bin)baseAnalysissetup.make
cmt_final_setup = $(bin)$(package)setup.make
endif

#--------------------------------------------------------

#cmt_lock_setup = /tmp/lock$(cmt_lock_pid).make
#cmt_temp_tag = /tmp/tag$(cmt_lock_pid).make

#first :: $(cmt_local_tagfile)
#	@echo $(cmt_local_tagfile) ok
ifndef QUICK
first :: $(cmt_final_setup) ;
else
first :: ;
endif

##	@bin=`$(cmtexe) show macro_value bin`

#$(cmt_local_tagfile) : $(cmt_lock_setup)
#	@echo "#CMT> Error: $@: No such file" >&2; exit 1
$(cmt_local_tagfile) :
	@echo "#CMT> Warning: $@: No such file" >&2; exit
#	@echo "#CMT> Info: $@: No need to rebuild file" >&2; exit

$(cmt_final_setup) : $(cmt_local_tagfile) 
	$(echo) "(constituents.make) Rebuilding $@"
	@if test ! -d $(@D); then $(mkdir) -p $(@D); fi; \
	  if test -f $(cmt_local_setup); then /bin/rm -f $(cmt_local_setup); fi; \
	  trap '/bin/rm -f $(cmt_local_setup)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) show setup >>$(cmt_local_setup); \
	  if test ! -f $@; then \
	    mv $(cmt_local_setup) $@; \
	  else \
	    if /usr/bin/diff $(cmt_local_setup) $@ >/dev/null ; then \
	      : ; \
	    else \
	      mv $(cmt_local_setup) $@; \
	    fi; \
	  fi

#	@/bin/echo $@ ok   

config :: checkuses
	@exit 0
checkuses : ;

env.make ::
	printenv >env.make.tmp; $(cmtexe) check files env.make.tmp env.make

ifndef QUICK
all :: build_library_links
	$(echo) "(constituents.make) all done"
endif

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

build_library_links : dirs requirements
	$(echo) "(constituents.make) Rebuilding library links"; \
	if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi; \
	$(build_library_links)

.DEFAULT ::
	$(echo) "(constituents.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: Using default commands" >&2; exit

#	@if test "$@" = "$(cmt_lock_setup)"; then \
	#  /bin/rm -f $(cmt_lock_setup); \
	 # touch $(cmt_lock_setup); \
	#fi

#-- end of constituents_header ------
#-- start of group ------

all_groups :: all

all :: $(all_dependencies)  $(all_pre_constituents) $(all_constituents)  $(all_post_constituents)
	$(echo) "all ok."

#	@/bin/echo " all ok."

clean :: allclean

allclean ::  $(all_constituentsclean)
	$(echo) $(all_constituentsclean)
	$(echo) "allclean ok."

#	@echo $(all_constituentsclean)
#	@/bin/echo " allclean ok."

allclean :: makefilesclean

#-- end of group ------
#-- start of group ------

all_groups :: cmt_actions

cmt_actions :: $(cmt_actions_dependencies)  $(cmt_actions_pre_constituents) $(cmt_actions_constituents)  $(cmt_actions_post_constituents)
	$(echo) "cmt_actions ok."

#	@/bin/echo " cmt_actions ok."

clean :: allclean

cmt_actionsclean ::  $(cmt_actions_constituentsclean)
	$(echo) $(cmt_actions_constituentsclean)
	$(echo) "cmt_actionsclean ok."

#	@echo $(cmt_actions_constituentsclean)
#	@/bin/echo " cmt_actionsclean ok."

cmt_actionsclean :: makefilesclean

#-- end of group ------
#-- start of group ------

all_groups :: documentation

documentation :: $(documentation_dependencies)  $(documentation_pre_constituents) $(documentation_constituents)  $(documentation_post_constituents)
	$(echo) "documentation ok."

#	@/bin/echo " documentation ok."

clean :: allclean

documentationclean ::  $(documentation_constituentsclean)
	$(echo) $(documentation_constituentsclean)
	$(echo) "documentationclean ok."

#	@echo $(documentation_constituentsclean)
#	@/bin/echo " documentationclean ok."

documentationclean :: makefilesclean

#-- end of group ------
#-- start of constituent_lock ------

cmt_doxygen_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_doxygen_has_target_tag

ifdef READONLY
cmt_local_tagfile_doxygen = /tmp/CMT_$(baseAnalysis_tag)_doxygen.make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_baseAnalysis_doxygensetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(baseAnalysis_tag)_doxygen.make
cmt_local_tagfile_doxygen = $(bin)$(baseAnalysis_tag)_doxygen.make
cmt_final_setup_doxygen = $(bin)baseAnalysis_doxygensetup.make
cmt_local_doxygen_makefile = $(bin)doxygen.make
endif

doxygen_extratags = -tag_add=target_doxygen

#$(cmt_local_tagfile_doxygen) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_doxygen) ::
else
$(cmt_local_tagfile_doxygen) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_doxygen)"
	@if test -f $(cmt_local_tagfile_doxygen); then /bin/rm -f $(cmt_local_tagfile_doxygen); fi ; \
	  $(cmtexe) -tag=$(tags) $(doxygen_extratags) build tag_makefile >>$(cmt_local_tagfile_doxygen); \
	  if test -f $(cmt_final_setup_doxygen); then /bin/rm -f $(cmt_final_setup_doxygen); fi; \
	  $(cmtexe) -tag=$(tags) $(doxygen_extratags) show setup >>$(cmt_final_setup_doxygen)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_doxygen = /tmp/CMT_$(baseAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_baseAnalysissetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(baseAnalysis_tag).make
cmt_local_tagfile_doxygen = $(bin)$(baseAnalysis_tag).make
cmt_final_setup_doxygen = $(bin)baseAnalysissetup.make
cmt_local_doxygen_makefile = $(bin)doxygen.make
endif

endif

ifndef QUICK
$(cmt_local_doxygen_makefile) :: $(doxygen_dependencies) $(cmt_local_tagfile_doxygen) build_library_links dirs
else
$(cmt_local_doxygen_makefile) :: $(cmt_local_tagfile_doxygen)
endif
	$(echo) "(constituents.make) Building doxygen.make"; \
	  $(cmtexe) -tag=$(tags) $(doxygen_extratags) build constituent_makefile -out=$(cmt_local_doxygen_makefile) doxygen

doxygen :: $(doxygen_dependencies) $(cmt_local_doxygen_makefile)
	$(echo) "(constituents.make) Creating doxygen${lock_suffix} and Starting doxygen"
	@${lock_command} doxygen${lock_suffix} || exit $$?; \
	  retval=$$?; \
	  trap '${unlock_command} doxygen${lock_suffix}; exit $${retval}' 1 2 15; \
	  $(MAKE) -f $(cmt_local_doxygen_makefile) cmt_lock_pid=$${cmt_lock_pid} doxygen; \
	  retval=$$?; ${unlock_command} doxygen${lock_suffix}; exit $${retval}
	$(echo) "(constituents.make) doxygen done"

clean :: doxygenclean

doxygenclean :: $(doxygenclean_dependencies) ##$(cmt_local_doxygen_makefile)
	$(echo) "(constituents.make) Starting doxygenclean"
	@-if test -f $(cmt_local_doxygen_makefile); then \
	  $(MAKE) -f $(cmt_local_doxygen_makefile) cmt_lock_pid=$${cmt_lock_pid} doxygenclean; \
	fi

##	  /bin/rm -f $(cmt_local_doxygen_makefile) $(bin)doxygen_dependencies.make

install :: doxygeninstall

doxygeninstall :: $(doxygen_dependencies) $(cmt_local_doxygen_makefile)
	$(echo) "(constituents.make) Starting install doxygen"
	@-$(MAKE) -f $(cmt_local_doxygen_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install doxygen done"

uninstall :: doxygenuninstall

doxygenuninstall :: $(cmt_local_doxygen_makefile)
	$(echo) "(constituents.make) Starting uninstall doxygen"
	@-$(MAKE) -f $(cmt_local_doxygen_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall doxygen done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ doxygen"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ doxygen done"
endif


#-- end of constituent_lock ------
#-- start of constituent ------

cmt_baseAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_baseAnalysis_has_target_tag

ifdef READONLY
cmt_local_tagfile_baseAnalysis = /tmp/CMT_$(baseAnalysis_tag)_baseAnalysis.make$(cmt_lock_pid)
cmt_final_setup_baseAnalysis = /tmp/CMT_baseAnalysis_baseAnalysissetup.make
cmt_local_baseAnalysis_makefile = /tmp/CMT_baseAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_baseAnalysis = $(baseAnalysis_tag)_baseAnalysis.make
cmt_local_tagfile_baseAnalysis = $(bin)$(baseAnalysis_tag)_baseAnalysis.make
cmt_final_setup_baseAnalysis = $(bin)baseAnalysis_baseAnalysissetup.make
cmt_local_baseAnalysis_makefile = $(bin)baseAnalysis.make
endif

baseAnalysis_extratags = -tag_add=target_baseAnalysis

#$(cmt_local_tagfile_baseAnalysis) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_baseAnalysis) ::
else
$(cmt_local_tagfile_baseAnalysis) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_baseAnalysis)"
	@if test -f $(cmt_local_tagfile_baseAnalysis); then /bin/rm -f $(cmt_local_tagfile_baseAnalysis); fi ; \
	  $(cmtexe) -tag=$(tags) $(baseAnalysis_extratags) build tag_makefile >>$(cmt_local_tagfile_baseAnalysis); \
	  if test -f $(cmt_final_setup_baseAnalysis); then /bin/rm -f $(cmt_final_setup_baseAnalysis); fi; \
	  $(cmtexe) -tag=$(tags) $(baseAnalysis_extratags) show setup >>$(cmt_final_setup_baseAnalysis)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_baseAnalysis = /tmp/CMT_$(baseAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_baseAnalysis = /tmp/CMT_baseAnalysissetup.make
cmt_local_baseAnalysis_makefile = /tmp/CMT_baseAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_baseAnalysis = $(baseAnalysis_tag).make
cmt_local_tagfile_baseAnalysis = $(bin)$(baseAnalysis_tag).make
cmt_final_setup_baseAnalysis = $(bin)baseAnalysissetup.make
cmt_local_baseAnalysis_makefile = $(bin)baseAnalysis.make
endif

endif

ifndef QUICK
$(cmt_local_baseAnalysis_makefile) :: $(baseAnalysis_dependencies) $(cmt_local_tagfile_baseAnalysis) build_library_links dirs
else
$(cmt_local_baseAnalysis_makefile) :: $(cmt_local_tagfile_baseAnalysis)
endif
	$(echo) "(constituents.make) Building baseAnalysis.make"; \
	  $(cmtexe) -tag=$(tags) $(baseAnalysis_extratags) build constituent_makefile -out=$(cmt_local_baseAnalysis_makefile) baseAnalysis

baseAnalysis :: $(baseAnalysis_dependencies) $(cmt_local_baseAnalysis_makefile)
	$(echo) "(constituents.make) Starting baseAnalysis"
	@$(MAKE) -f $(cmt_local_baseAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} baseAnalysis
	$(echo) "(constituents.make) baseAnalysis done"

clean :: baseAnalysisclean

baseAnalysisclean :: $(baseAnalysisclean_dependencies) ##$(cmt_local_baseAnalysis_makefile)
	$(echo) "(constituents.make) Starting baseAnalysisclean"
	@-if test -f $(cmt_local_baseAnalysis_makefile); then \
	  $(MAKE) -f $(cmt_local_baseAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} baseAnalysisclean; \
	fi

##	  /bin/rm -f $(cmt_local_baseAnalysis_makefile) $(bin)baseAnalysis_dependencies.make

install :: baseAnalysisinstall

baseAnalysisinstall :: $(baseAnalysis_dependencies) $(cmt_local_baseAnalysis_makefile)
	$(echo) "(constituents.make) Starting install baseAnalysis"
	@-$(MAKE) -f $(cmt_local_baseAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install baseAnalysis done"

uninstall :: baseAnalysisuninstall

baseAnalysisuninstall :: $(cmt_local_baseAnalysis_makefile)
	$(echo) "(constituents.make) Starting uninstall baseAnalysis"
	@-$(MAKE) -f $(cmt_local_baseAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall baseAnalysis done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ baseAnalysis"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ baseAnalysis done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_RunRooTrackerVtxExample_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunRooTrackerVtxExample_has_target_tag

ifdef READONLY
cmt_local_tagfile_RunRooTrackerVtxExample = /tmp/CMT_$(baseAnalysis_tag)_RunRooTrackerVtxExample.make$(cmt_lock_pid)
cmt_final_setup_RunRooTrackerVtxExample = /tmp/CMT_baseAnalysis_RunRooTrackerVtxExamplesetup.make
cmt_local_RunRooTrackerVtxExample_makefile = /tmp/CMT_RunRooTrackerVtxExample$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunRooTrackerVtxExample = $(baseAnalysis_tag)_RunRooTrackerVtxExample.make
cmt_local_tagfile_RunRooTrackerVtxExample = $(bin)$(baseAnalysis_tag)_RunRooTrackerVtxExample.make
cmt_final_setup_RunRooTrackerVtxExample = $(bin)baseAnalysis_RunRooTrackerVtxExamplesetup.make
cmt_local_RunRooTrackerVtxExample_makefile = $(bin)RunRooTrackerVtxExample.make
endif

RunRooTrackerVtxExample_extratags = -tag_add=target_RunRooTrackerVtxExample

#$(cmt_local_tagfile_RunRooTrackerVtxExample) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_RunRooTrackerVtxExample) ::
else
$(cmt_local_tagfile_RunRooTrackerVtxExample) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_RunRooTrackerVtxExample)"
	@if test -f $(cmt_local_tagfile_RunRooTrackerVtxExample); then /bin/rm -f $(cmt_local_tagfile_RunRooTrackerVtxExample); fi ; \
	  $(cmtexe) -tag=$(tags) $(RunRooTrackerVtxExample_extratags) build tag_makefile >>$(cmt_local_tagfile_RunRooTrackerVtxExample); \
	  if test -f $(cmt_final_setup_RunRooTrackerVtxExample); then /bin/rm -f $(cmt_final_setup_RunRooTrackerVtxExample); fi; \
	  $(cmtexe) -tag=$(tags) $(RunRooTrackerVtxExample_extratags) show setup >>$(cmt_final_setup_RunRooTrackerVtxExample)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_RunRooTrackerVtxExample = /tmp/CMT_$(baseAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_RunRooTrackerVtxExample = /tmp/CMT_baseAnalysissetup.make
cmt_local_RunRooTrackerVtxExample_makefile = /tmp/CMT_RunRooTrackerVtxExample$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunRooTrackerVtxExample = $(baseAnalysis_tag).make
cmt_local_tagfile_RunRooTrackerVtxExample = $(bin)$(baseAnalysis_tag).make
cmt_final_setup_RunRooTrackerVtxExample = $(bin)baseAnalysissetup.make
cmt_local_RunRooTrackerVtxExample_makefile = $(bin)RunRooTrackerVtxExample.make
endif

endif

ifndef QUICK
$(cmt_local_RunRooTrackerVtxExample_makefile) :: $(RunRooTrackerVtxExample_dependencies) $(cmt_local_tagfile_RunRooTrackerVtxExample) build_library_links dirs
else
$(cmt_local_RunRooTrackerVtxExample_makefile) :: $(cmt_local_tagfile_RunRooTrackerVtxExample)
endif
	$(echo) "(constituents.make) Building RunRooTrackerVtxExample.make"; \
	  $(cmtexe) -tag=$(tags) $(RunRooTrackerVtxExample_extratags) build constituent_makefile -out=$(cmt_local_RunRooTrackerVtxExample_makefile) RunRooTrackerVtxExample

RunRooTrackerVtxExample :: $(RunRooTrackerVtxExample_dependencies) $(cmt_local_RunRooTrackerVtxExample_makefile)
	$(echo) "(constituents.make) Starting RunRooTrackerVtxExample"
	@$(MAKE) -f $(cmt_local_RunRooTrackerVtxExample_makefile) cmt_lock_pid=$${cmt_lock_pid} RunRooTrackerVtxExample
	$(echo) "(constituents.make) RunRooTrackerVtxExample done"

clean :: RunRooTrackerVtxExampleclean

RunRooTrackerVtxExampleclean :: $(RunRooTrackerVtxExampleclean_dependencies) ##$(cmt_local_RunRooTrackerVtxExample_makefile)
	$(echo) "(constituents.make) Starting RunRooTrackerVtxExampleclean"
	@-if test -f $(cmt_local_RunRooTrackerVtxExample_makefile); then \
	  $(MAKE) -f $(cmt_local_RunRooTrackerVtxExample_makefile) cmt_lock_pid=$${cmt_lock_pid} RunRooTrackerVtxExampleclean; \
	fi

##	  /bin/rm -f $(cmt_local_RunRooTrackerVtxExample_makefile) $(bin)RunRooTrackerVtxExample_dependencies.make

install :: RunRooTrackerVtxExampleinstall

RunRooTrackerVtxExampleinstall :: $(RunRooTrackerVtxExample_dependencies) $(cmt_local_RunRooTrackerVtxExample_makefile)
	$(echo) "(constituents.make) Starting install RunRooTrackerVtxExample"
	@-$(MAKE) -f $(cmt_local_RunRooTrackerVtxExample_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install RunRooTrackerVtxExample done"

uninstall :: RunRooTrackerVtxExampleuninstall

RunRooTrackerVtxExampleuninstall :: $(cmt_local_RunRooTrackerVtxExample_makefile)
	$(echo) "(constituents.make) Starting uninstall RunRooTrackerVtxExample"
	@-$(MAKE) -f $(cmt_local_RunRooTrackerVtxExample_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall RunRooTrackerVtxExample done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ RunRooTrackerVtxExample"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ RunRooTrackerVtxExample done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_EventDisplayND280_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_EventDisplayND280_has_target_tag

ifdef READONLY
cmt_local_tagfile_EventDisplayND280 = /tmp/CMT_$(baseAnalysis_tag)_EventDisplayND280.make$(cmt_lock_pid)
cmt_final_setup_EventDisplayND280 = /tmp/CMT_baseAnalysis_EventDisplayND280setup.make
cmt_local_EventDisplayND280_makefile = /tmp/CMT_EventDisplayND280$(cmt_lock_pid).make
else
#cmt_local_tagfile_EventDisplayND280 = $(baseAnalysis_tag)_EventDisplayND280.make
cmt_local_tagfile_EventDisplayND280 = $(bin)$(baseAnalysis_tag)_EventDisplayND280.make
cmt_final_setup_EventDisplayND280 = $(bin)baseAnalysis_EventDisplayND280setup.make
cmt_local_EventDisplayND280_makefile = $(bin)EventDisplayND280.make
endif

EventDisplayND280_extratags = -tag_add=target_EventDisplayND280

#$(cmt_local_tagfile_EventDisplayND280) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_EventDisplayND280) ::
else
$(cmt_local_tagfile_EventDisplayND280) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_EventDisplayND280)"
	@if test -f $(cmt_local_tagfile_EventDisplayND280); then /bin/rm -f $(cmt_local_tagfile_EventDisplayND280); fi ; \
	  $(cmtexe) -tag=$(tags) $(EventDisplayND280_extratags) build tag_makefile >>$(cmt_local_tagfile_EventDisplayND280); \
	  if test -f $(cmt_final_setup_EventDisplayND280); then /bin/rm -f $(cmt_final_setup_EventDisplayND280); fi; \
	  $(cmtexe) -tag=$(tags) $(EventDisplayND280_extratags) show setup >>$(cmt_final_setup_EventDisplayND280)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_EventDisplayND280 = /tmp/CMT_$(baseAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_EventDisplayND280 = /tmp/CMT_baseAnalysissetup.make
cmt_local_EventDisplayND280_makefile = /tmp/CMT_EventDisplayND280$(cmt_lock_pid).make
else
#cmt_local_tagfile_EventDisplayND280 = $(baseAnalysis_tag).make
cmt_local_tagfile_EventDisplayND280 = $(bin)$(baseAnalysis_tag).make
cmt_final_setup_EventDisplayND280 = $(bin)baseAnalysissetup.make
cmt_local_EventDisplayND280_makefile = $(bin)EventDisplayND280.make
endif

endif

ifndef QUICK
$(cmt_local_EventDisplayND280_makefile) :: $(EventDisplayND280_dependencies) $(cmt_local_tagfile_EventDisplayND280) build_library_links dirs
else
$(cmt_local_EventDisplayND280_makefile) :: $(cmt_local_tagfile_EventDisplayND280)
endif
	$(echo) "(constituents.make) Building EventDisplayND280.make"; \
	  $(cmtexe) -tag=$(tags) $(EventDisplayND280_extratags) build constituent_makefile -out=$(cmt_local_EventDisplayND280_makefile) EventDisplayND280

EventDisplayND280 :: $(EventDisplayND280_dependencies) $(cmt_local_EventDisplayND280_makefile)
	$(echo) "(constituents.make) Starting EventDisplayND280"
	@$(MAKE) -f $(cmt_local_EventDisplayND280_makefile) cmt_lock_pid=$${cmt_lock_pid} EventDisplayND280
	$(echo) "(constituents.make) EventDisplayND280 done"

clean :: EventDisplayND280clean

EventDisplayND280clean :: $(EventDisplayND280clean_dependencies) ##$(cmt_local_EventDisplayND280_makefile)
	$(echo) "(constituents.make) Starting EventDisplayND280clean"
	@-if test -f $(cmt_local_EventDisplayND280_makefile); then \
	  $(MAKE) -f $(cmt_local_EventDisplayND280_makefile) cmt_lock_pid=$${cmt_lock_pid} EventDisplayND280clean; \
	fi

##	  /bin/rm -f $(cmt_local_EventDisplayND280_makefile) $(bin)EventDisplayND280_dependencies.make

install :: EventDisplayND280install

EventDisplayND280install :: $(EventDisplayND280_dependencies) $(cmt_local_EventDisplayND280_makefile)
	$(echo) "(constituents.make) Starting install EventDisplayND280"
	@-$(MAKE) -f $(cmt_local_EventDisplayND280_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install EventDisplayND280 done"

uninstall :: EventDisplayND280uninstall

EventDisplayND280uninstall :: $(cmt_local_EventDisplayND280_makefile)
	$(echo) "(constituents.make) Starting uninstall EventDisplayND280"
	@-$(MAKE) -f $(cmt_local_EventDisplayND280_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall EventDisplayND280 done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ EventDisplayND280"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ EventDisplayND280 done"
endif


#-- end of constituent ------
#-- start of constituent_lock ------

cmt_make_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_make_has_target_tag

ifdef READONLY
cmt_local_tagfile_make = /tmp/CMT_$(baseAnalysis_tag)_make.make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_baseAnalysis_makesetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(baseAnalysis_tag)_make.make
cmt_local_tagfile_make = $(bin)$(baseAnalysis_tag)_make.make
cmt_final_setup_make = $(bin)baseAnalysis_makesetup.make
cmt_local_make_makefile = $(bin)make.make
endif

make_extratags = -tag_add=target_make

#$(cmt_local_tagfile_make) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_make) ::
else
$(cmt_local_tagfile_make) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_make)"
	@if test -f $(cmt_local_tagfile_make); then /bin/rm -f $(cmt_local_tagfile_make); fi ; \
	  $(cmtexe) -tag=$(tags) $(make_extratags) build tag_makefile >>$(cmt_local_tagfile_make); \
	  if test -f $(cmt_final_setup_make); then /bin/rm -f $(cmt_final_setup_make); fi; \
	  $(cmtexe) -tag=$(tags) $(make_extratags) show setup >>$(cmt_final_setup_make)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_make = /tmp/CMT_$(baseAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_baseAnalysissetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(baseAnalysis_tag).make
cmt_local_tagfile_make = $(bin)$(baseAnalysis_tag).make
cmt_final_setup_make = $(bin)baseAnalysissetup.make
cmt_local_make_makefile = $(bin)make.make
endif

endif

ifndef QUICK
$(cmt_local_make_makefile) :: $(make_dependencies) $(cmt_local_tagfile_make) build_library_links dirs
else
$(cmt_local_make_makefile) :: $(cmt_local_tagfile_make)
endif
	$(echo) "(constituents.make) Building make.make"; \
	  $(cmtexe) -tag=$(tags) $(make_extratags) build constituent_makefile -out=$(cmt_local_make_makefile) make

make :: $(make_dependencies) $(cmt_local_make_makefile)
	$(echo) "(constituents.make) Creating make${lock_suffix} and Starting make"
	@${lock_command} make${lock_suffix} || exit $$?; \
	  retval=$$?; \
	  trap '${unlock_command} make${lock_suffix}; exit $${retval}' 1 2 15; \
	  $(MAKE) -f $(cmt_local_make_makefile) cmt_lock_pid=$${cmt_lock_pid} make; \
	  retval=$$?; ${unlock_command} make${lock_suffix}; exit $${retval}
	$(echo) "(constituents.make) make done"

clean :: makeclean

makeclean :: $(makeclean_dependencies) ##$(cmt_local_make_makefile)
	$(echo) "(constituents.make) Starting makeclean"
	@-if test -f $(cmt_local_make_makefile); then \
	  $(MAKE) -f $(cmt_local_make_makefile) cmt_lock_pid=$${cmt_lock_pid} makeclean; \
	fi

##	  /bin/rm -f $(cmt_local_make_makefile) $(bin)make_dependencies.make

install :: makeinstall

makeinstall :: $(make_dependencies) $(cmt_local_make_makefile)
	$(echo) "(constituents.make) Starting install make"
	@-$(MAKE) -f $(cmt_local_make_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install make done"

uninstall :: makeuninstall

makeuninstall :: $(cmt_local_make_makefile)
	$(echo) "(constituents.make) Starting uninstall make"
	@-$(MAKE) -f $(cmt_local_make_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall make done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ make"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ make done"
endif


#-- end of constituent_lock ------
#-- start of constituents_trailer ------

clean :: remove_library_links

remove_library_links ::
	$(echo) "Removing library links"; \
	  $(remove_library_links)

makefilesclean ::

###	@/bin/rm -f checkuses

###	/bin/rm -f *.make*

clean :: makefilesclean

binclean :: clean
	$(echo) "Removing binary directory $(bin)"
	@if test ! "$(bin)" = "./"; then \
	  /bin/rm -rf $(bin); \
	fi

#-- end of constituents_trailer ------
