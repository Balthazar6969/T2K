
#-- start of constituents_header ------

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

tags      = $(tag),$(CMTEXTRATAGS)

numuBkgInAntiNuModeAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile = $(numuBkgInAntiNuModeAnalysis_tag).make
cmt_local_tagfile = $(bin)$(numuBkgInAntiNuModeAnalysis_tag).make
endif

#-include $(cmt_local_tagfile)
include $(cmt_local_tagfile)

ifdef READONLY
cmt_local_setup = /tmp/CMT_numuBkgInAntiNuModeAnalysissetup$(cmt_lock_pid).make
cmt_final_setup = /tmp/CMT_numuBkgInAntiNuModeAnalysissetup.make
else
#cmt_local_setup = $(bin)numuBkgInAntiNuModeAnalysissetup$(cmt_lock_pid).make
cmt_local_setup = $(bin)$(package)setup$$$$.make
#cmt_final_setup = $(bin)numuBkgInAntiNuModeAnalysissetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag)_doxygen.make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_numuBkgInAntiNuModeAnalysis_doxygensetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(numuBkgInAntiNuModeAnalysis_tag)_doxygen.make
cmt_local_tagfile_doxygen = $(bin)$(numuBkgInAntiNuModeAnalysis_tag)_doxygen.make
cmt_final_setup_doxygen = $(bin)numuBkgInAntiNuModeAnalysis_doxygensetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_numuBkgInAntiNuModeAnalysissetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(numuBkgInAntiNuModeAnalysis_tag).make
cmt_local_tagfile_doxygen = $(bin)$(numuBkgInAntiNuModeAnalysis_tag).make
cmt_final_setup_doxygen = $(bin)numuBkgInAntiNuModeAnalysissetup.make
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

cmt_numuBkgInAntiNuModeAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_numuBkgInAntiNuModeAnalysis_has_target_tag

ifdef READONLY
cmt_local_tagfile_numuBkgInAntiNuModeAnalysis = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag)_numuBkgInAntiNuModeAnalysis.make$(cmt_lock_pid)
cmt_final_setup_numuBkgInAntiNuModeAnalysis = /tmp/CMT_numuBkgInAntiNuModeAnalysis_numuBkgInAntiNuModeAnalysissetup.make
cmt_local_numuBkgInAntiNuModeAnalysis_makefile = /tmp/CMT_numuBkgInAntiNuModeAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_numuBkgInAntiNuModeAnalysis = $(numuBkgInAntiNuModeAnalysis_tag)_numuBkgInAntiNuModeAnalysis.make
cmt_local_tagfile_numuBkgInAntiNuModeAnalysis = $(bin)$(numuBkgInAntiNuModeAnalysis_tag)_numuBkgInAntiNuModeAnalysis.make
cmt_final_setup_numuBkgInAntiNuModeAnalysis = $(bin)numuBkgInAntiNuModeAnalysis_numuBkgInAntiNuModeAnalysissetup.make
cmt_local_numuBkgInAntiNuModeAnalysis_makefile = $(bin)numuBkgInAntiNuModeAnalysis.make
endif

numuBkgInAntiNuModeAnalysis_extratags = -tag_add=target_numuBkgInAntiNuModeAnalysis

#$(cmt_local_tagfile_numuBkgInAntiNuModeAnalysis) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_numuBkgInAntiNuModeAnalysis) ::
else
$(cmt_local_tagfile_numuBkgInAntiNuModeAnalysis) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_numuBkgInAntiNuModeAnalysis)"
	@if test -f $(cmt_local_tagfile_numuBkgInAntiNuModeAnalysis); then /bin/rm -f $(cmt_local_tagfile_numuBkgInAntiNuModeAnalysis); fi ; \
	  $(cmtexe) -tag=$(tags) $(numuBkgInAntiNuModeAnalysis_extratags) build tag_makefile >>$(cmt_local_tagfile_numuBkgInAntiNuModeAnalysis); \
	  if test -f $(cmt_final_setup_numuBkgInAntiNuModeAnalysis); then /bin/rm -f $(cmt_final_setup_numuBkgInAntiNuModeAnalysis); fi; \
	  $(cmtexe) -tag=$(tags) $(numuBkgInAntiNuModeAnalysis_extratags) show setup >>$(cmt_final_setup_numuBkgInAntiNuModeAnalysis)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_numuBkgInAntiNuModeAnalysis = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_numuBkgInAntiNuModeAnalysis = /tmp/CMT_numuBkgInAntiNuModeAnalysissetup.make
cmt_local_numuBkgInAntiNuModeAnalysis_makefile = /tmp/CMT_numuBkgInAntiNuModeAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_numuBkgInAntiNuModeAnalysis = $(numuBkgInAntiNuModeAnalysis_tag).make
cmt_local_tagfile_numuBkgInAntiNuModeAnalysis = $(bin)$(numuBkgInAntiNuModeAnalysis_tag).make
cmt_final_setup_numuBkgInAntiNuModeAnalysis = $(bin)numuBkgInAntiNuModeAnalysissetup.make
cmt_local_numuBkgInAntiNuModeAnalysis_makefile = $(bin)numuBkgInAntiNuModeAnalysis.make
endif

endif

ifndef QUICK
$(cmt_local_numuBkgInAntiNuModeAnalysis_makefile) :: $(numuBkgInAntiNuModeAnalysis_dependencies) $(cmt_local_tagfile_numuBkgInAntiNuModeAnalysis) build_library_links dirs
else
$(cmt_local_numuBkgInAntiNuModeAnalysis_makefile) :: $(cmt_local_tagfile_numuBkgInAntiNuModeAnalysis)
endif
	$(echo) "(constituents.make) Building numuBkgInAntiNuModeAnalysis.make"; \
	  $(cmtexe) -tag=$(tags) $(numuBkgInAntiNuModeAnalysis_extratags) build constituent_makefile -out=$(cmt_local_numuBkgInAntiNuModeAnalysis_makefile) numuBkgInAntiNuModeAnalysis

numuBkgInAntiNuModeAnalysis :: $(numuBkgInAntiNuModeAnalysis_dependencies) $(cmt_local_numuBkgInAntiNuModeAnalysis_makefile)
	$(echo) "(constituents.make) Starting numuBkgInAntiNuModeAnalysis"
	@$(MAKE) -f $(cmt_local_numuBkgInAntiNuModeAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} numuBkgInAntiNuModeAnalysis
	$(echo) "(constituents.make) numuBkgInAntiNuModeAnalysis done"

clean :: numuBkgInAntiNuModeAnalysisclean

numuBkgInAntiNuModeAnalysisclean :: $(numuBkgInAntiNuModeAnalysisclean_dependencies) ##$(cmt_local_numuBkgInAntiNuModeAnalysis_makefile)
	$(echo) "(constituents.make) Starting numuBkgInAntiNuModeAnalysisclean"
	@-if test -f $(cmt_local_numuBkgInAntiNuModeAnalysis_makefile); then \
	  $(MAKE) -f $(cmt_local_numuBkgInAntiNuModeAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} numuBkgInAntiNuModeAnalysisclean; \
	fi

##	  /bin/rm -f $(cmt_local_numuBkgInAntiNuModeAnalysis_makefile) $(bin)numuBkgInAntiNuModeAnalysis_dependencies.make

install :: numuBkgInAntiNuModeAnalysisinstall

numuBkgInAntiNuModeAnalysisinstall :: $(numuBkgInAntiNuModeAnalysis_dependencies) $(cmt_local_numuBkgInAntiNuModeAnalysis_makefile)
	$(echo) "(constituents.make) Starting install numuBkgInAntiNuModeAnalysis"
	@-$(MAKE) -f $(cmt_local_numuBkgInAntiNuModeAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install numuBkgInAntiNuModeAnalysis done"

uninstall :: numuBkgInAntiNuModeAnalysisuninstall

numuBkgInAntiNuModeAnalysisuninstall :: $(cmt_local_numuBkgInAntiNuModeAnalysis_makefile)
	$(echo) "(constituents.make) Starting uninstall numuBkgInAntiNuModeAnalysis"
	@-$(MAKE) -f $(cmt_local_numuBkgInAntiNuModeAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall numuBkgInAntiNuModeAnalysis done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ numuBkgInAntiNuModeAnalysis"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ numuBkgInAntiNuModeAnalysis done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_RunNumuBkgInAntiNuModeAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunNumuBkgInAntiNuModeAnalysis_has_target_tag

ifdef READONLY
cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag)_RunNumuBkgInAntiNuModeAnalysis.make$(cmt_lock_pid)
cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis = /tmp/CMT_numuBkgInAntiNuModeAnalysis_RunNumuBkgInAntiNuModeAnalysissetup.make
cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile = /tmp/CMT_RunNumuBkgInAntiNuModeAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis = $(numuBkgInAntiNuModeAnalysis_tag)_RunNumuBkgInAntiNuModeAnalysis.make
cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis = $(bin)$(numuBkgInAntiNuModeAnalysis_tag)_RunNumuBkgInAntiNuModeAnalysis.make
cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis = $(bin)numuBkgInAntiNuModeAnalysis_RunNumuBkgInAntiNuModeAnalysissetup.make
cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile = $(bin)RunNumuBkgInAntiNuModeAnalysis.make
endif

RunNumuBkgInAntiNuModeAnalysis_extratags = -tag_add=target_RunNumuBkgInAntiNuModeAnalysis

#$(cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis) ::
else
$(cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis)"
	@if test -f $(cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis); then /bin/rm -f $(cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis); fi ; \
	  $(cmtexe) -tag=$(tags) $(RunNumuBkgInAntiNuModeAnalysis_extratags) build tag_makefile >>$(cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis); \
	  if test -f $(cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis); then /bin/rm -f $(cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis); fi; \
	  $(cmtexe) -tag=$(tags) $(RunNumuBkgInAntiNuModeAnalysis_extratags) show setup >>$(cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis = /tmp/CMT_numuBkgInAntiNuModeAnalysissetup.make
cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile = /tmp/CMT_RunNumuBkgInAntiNuModeAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis = $(numuBkgInAntiNuModeAnalysis_tag).make
cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis = $(bin)$(numuBkgInAntiNuModeAnalysis_tag).make
cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis = $(bin)numuBkgInAntiNuModeAnalysissetup.make
cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile = $(bin)RunNumuBkgInAntiNuModeAnalysis.make
endif

endif

ifndef QUICK
$(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile) :: $(RunNumuBkgInAntiNuModeAnalysis_dependencies) $(cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis) build_library_links dirs
else
$(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile) :: $(cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis)
endif
	$(echo) "(constituents.make) Building RunNumuBkgInAntiNuModeAnalysis.make"; \
	  $(cmtexe) -tag=$(tags) $(RunNumuBkgInAntiNuModeAnalysis_extratags) build constituent_makefile -out=$(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile) RunNumuBkgInAntiNuModeAnalysis

RunNumuBkgInAntiNuModeAnalysis :: $(RunNumuBkgInAntiNuModeAnalysis_dependencies) $(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile)
	$(echo) "(constituents.make) Starting RunNumuBkgInAntiNuModeAnalysis"
	@$(MAKE) -f $(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} RunNumuBkgInAntiNuModeAnalysis
	$(echo) "(constituents.make) RunNumuBkgInAntiNuModeAnalysis done"

clean :: RunNumuBkgInAntiNuModeAnalysisclean

RunNumuBkgInAntiNuModeAnalysisclean :: $(RunNumuBkgInAntiNuModeAnalysisclean_dependencies) ##$(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile)
	$(echo) "(constituents.make) Starting RunNumuBkgInAntiNuModeAnalysisclean"
	@-if test -f $(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile); then \
	  $(MAKE) -f $(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} RunNumuBkgInAntiNuModeAnalysisclean; \
	fi

##	  /bin/rm -f $(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile) $(bin)RunNumuBkgInAntiNuModeAnalysis_dependencies.make

install :: RunNumuBkgInAntiNuModeAnalysisinstall

RunNumuBkgInAntiNuModeAnalysisinstall :: $(RunNumuBkgInAntiNuModeAnalysis_dependencies) $(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile)
	$(echo) "(constituents.make) Starting install RunNumuBkgInAntiNuModeAnalysis"
	@-$(MAKE) -f $(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install RunNumuBkgInAntiNuModeAnalysis done"

uninstall :: RunNumuBkgInAntiNuModeAnalysisuninstall

RunNumuBkgInAntiNuModeAnalysisuninstall :: $(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile)
	$(echo) "(constituents.make) Starting uninstall RunNumuBkgInAntiNuModeAnalysis"
	@-$(MAKE) -f $(cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall RunNumuBkgInAntiNuModeAnalysis done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ RunNumuBkgInAntiNuModeAnalysis"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ RunNumuBkgInAntiNuModeAnalysis done"
endif


#-- end of constituent ------
#-- start of constituent_lock ------

cmt_make_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_make_has_target_tag

ifdef READONLY
cmt_local_tagfile_make = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag)_make.make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_numuBkgInAntiNuModeAnalysis_makesetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(numuBkgInAntiNuModeAnalysis_tag)_make.make
cmt_local_tagfile_make = $(bin)$(numuBkgInAntiNuModeAnalysis_tag)_make.make
cmt_final_setup_make = $(bin)numuBkgInAntiNuModeAnalysis_makesetup.make
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
cmt_local_tagfile_make = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_numuBkgInAntiNuModeAnalysissetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(numuBkgInAntiNuModeAnalysis_tag).make
cmt_local_tagfile_make = $(bin)$(numuBkgInAntiNuModeAnalysis_tag).make
cmt_final_setup_make = $(bin)numuBkgInAntiNuModeAnalysissetup.make
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
