
#-- start of constituents_header ------

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

tags      = $(tag),$(CMTEXTRATAGS)

numuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile = /tmp/CMT_$(numuCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile = $(numuCCAnalysis_tag).make
cmt_local_tagfile = $(bin)$(numuCCAnalysis_tag).make
endif

#-include $(cmt_local_tagfile)
include $(cmt_local_tagfile)

ifdef READONLY
cmt_local_setup = /tmp/CMT_numuCCAnalysissetup$(cmt_lock_pid).make
cmt_final_setup = /tmp/CMT_numuCCAnalysissetup.make
else
#cmt_local_setup = $(bin)numuCCAnalysissetup$(cmt_lock_pid).make
cmt_local_setup = $(bin)$(package)setup$$$$.make
#cmt_final_setup = $(bin)numuCCAnalysissetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(numuCCAnalysis_tag)_doxygen.make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_numuCCAnalysis_doxygensetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(numuCCAnalysis_tag)_doxygen.make
cmt_local_tagfile_doxygen = $(bin)$(numuCCAnalysis_tag)_doxygen.make
cmt_final_setup_doxygen = $(bin)numuCCAnalysis_doxygensetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(numuCCAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_numuCCAnalysissetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(numuCCAnalysis_tag).make
cmt_local_tagfile_doxygen = $(bin)$(numuCCAnalysis_tag).make
cmt_final_setup_doxygen = $(bin)numuCCAnalysissetup.make
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

cmt_numuCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_numuCCAnalysis_has_target_tag

ifdef READONLY
cmt_local_tagfile_numuCCAnalysis = /tmp/CMT_$(numuCCAnalysis_tag)_numuCCAnalysis.make$(cmt_lock_pid)
cmt_final_setup_numuCCAnalysis = /tmp/CMT_numuCCAnalysis_numuCCAnalysissetup.make
cmt_local_numuCCAnalysis_makefile = /tmp/CMT_numuCCAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_numuCCAnalysis = $(numuCCAnalysis_tag)_numuCCAnalysis.make
cmt_local_tagfile_numuCCAnalysis = $(bin)$(numuCCAnalysis_tag)_numuCCAnalysis.make
cmt_final_setup_numuCCAnalysis = $(bin)numuCCAnalysis_numuCCAnalysissetup.make
cmt_local_numuCCAnalysis_makefile = $(bin)numuCCAnalysis.make
endif

numuCCAnalysis_extratags = -tag_add=target_numuCCAnalysis

#$(cmt_local_tagfile_numuCCAnalysis) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_numuCCAnalysis) ::
else
$(cmt_local_tagfile_numuCCAnalysis) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_numuCCAnalysis)"
	@if test -f $(cmt_local_tagfile_numuCCAnalysis); then /bin/rm -f $(cmt_local_tagfile_numuCCAnalysis); fi ; \
	  $(cmtexe) -tag=$(tags) $(numuCCAnalysis_extratags) build tag_makefile >>$(cmt_local_tagfile_numuCCAnalysis); \
	  if test -f $(cmt_final_setup_numuCCAnalysis); then /bin/rm -f $(cmt_final_setup_numuCCAnalysis); fi; \
	  $(cmtexe) -tag=$(tags) $(numuCCAnalysis_extratags) show setup >>$(cmt_final_setup_numuCCAnalysis)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_numuCCAnalysis = /tmp/CMT_$(numuCCAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_numuCCAnalysis = /tmp/CMT_numuCCAnalysissetup.make
cmt_local_numuCCAnalysis_makefile = /tmp/CMT_numuCCAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_numuCCAnalysis = $(numuCCAnalysis_tag).make
cmt_local_tagfile_numuCCAnalysis = $(bin)$(numuCCAnalysis_tag).make
cmt_final_setup_numuCCAnalysis = $(bin)numuCCAnalysissetup.make
cmt_local_numuCCAnalysis_makefile = $(bin)numuCCAnalysis.make
endif

endif

ifndef QUICK
$(cmt_local_numuCCAnalysis_makefile) :: $(numuCCAnalysis_dependencies) $(cmt_local_tagfile_numuCCAnalysis) build_library_links dirs
else
$(cmt_local_numuCCAnalysis_makefile) :: $(cmt_local_tagfile_numuCCAnalysis)
endif
	$(echo) "(constituents.make) Building numuCCAnalysis.make"; \
	  $(cmtexe) -tag=$(tags) $(numuCCAnalysis_extratags) build constituent_makefile -out=$(cmt_local_numuCCAnalysis_makefile) numuCCAnalysis

numuCCAnalysis :: $(numuCCAnalysis_dependencies) $(cmt_local_numuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting numuCCAnalysis"
	@$(MAKE) -f $(cmt_local_numuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} numuCCAnalysis
	$(echo) "(constituents.make) numuCCAnalysis done"

clean :: numuCCAnalysisclean

numuCCAnalysisclean :: $(numuCCAnalysisclean_dependencies) ##$(cmt_local_numuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting numuCCAnalysisclean"
	@-if test -f $(cmt_local_numuCCAnalysis_makefile); then \
	  $(MAKE) -f $(cmt_local_numuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} numuCCAnalysisclean; \
	fi

##	  /bin/rm -f $(cmt_local_numuCCAnalysis_makefile) $(bin)numuCCAnalysis_dependencies.make

install :: numuCCAnalysisinstall

numuCCAnalysisinstall :: $(numuCCAnalysis_dependencies) $(cmt_local_numuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting install numuCCAnalysis"
	@-$(MAKE) -f $(cmt_local_numuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install numuCCAnalysis done"

uninstall :: numuCCAnalysisuninstall

numuCCAnalysisuninstall :: $(cmt_local_numuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting uninstall numuCCAnalysis"
	@-$(MAKE) -f $(cmt_local_numuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall numuCCAnalysis done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ numuCCAnalysis"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ numuCCAnalysis done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_RunNumuCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunNumuCCAnalysis_has_target_tag

ifdef READONLY
cmt_local_tagfile_RunNumuCCAnalysis = /tmp/CMT_$(numuCCAnalysis_tag)_RunNumuCCAnalysis.make$(cmt_lock_pid)
cmt_final_setup_RunNumuCCAnalysis = /tmp/CMT_numuCCAnalysis_RunNumuCCAnalysissetup.make
cmt_local_RunNumuCCAnalysis_makefile = /tmp/CMT_RunNumuCCAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunNumuCCAnalysis = $(numuCCAnalysis_tag)_RunNumuCCAnalysis.make
cmt_local_tagfile_RunNumuCCAnalysis = $(bin)$(numuCCAnalysis_tag)_RunNumuCCAnalysis.make
cmt_final_setup_RunNumuCCAnalysis = $(bin)numuCCAnalysis_RunNumuCCAnalysissetup.make
cmt_local_RunNumuCCAnalysis_makefile = $(bin)RunNumuCCAnalysis.make
endif

RunNumuCCAnalysis_extratags = -tag_add=target_RunNumuCCAnalysis

#$(cmt_local_tagfile_RunNumuCCAnalysis) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_RunNumuCCAnalysis) ::
else
$(cmt_local_tagfile_RunNumuCCAnalysis) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_RunNumuCCAnalysis)"
	@if test -f $(cmt_local_tagfile_RunNumuCCAnalysis); then /bin/rm -f $(cmt_local_tagfile_RunNumuCCAnalysis); fi ; \
	  $(cmtexe) -tag=$(tags) $(RunNumuCCAnalysis_extratags) build tag_makefile >>$(cmt_local_tagfile_RunNumuCCAnalysis); \
	  if test -f $(cmt_final_setup_RunNumuCCAnalysis); then /bin/rm -f $(cmt_final_setup_RunNumuCCAnalysis); fi; \
	  $(cmtexe) -tag=$(tags) $(RunNumuCCAnalysis_extratags) show setup >>$(cmt_final_setup_RunNumuCCAnalysis)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_RunNumuCCAnalysis = /tmp/CMT_$(numuCCAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_RunNumuCCAnalysis = /tmp/CMT_numuCCAnalysissetup.make
cmt_local_RunNumuCCAnalysis_makefile = /tmp/CMT_RunNumuCCAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunNumuCCAnalysis = $(numuCCAnalysis_tag).make
cmt_local_tagfile_RunNumuCCAnalysis = $(bin)$(numuCCAnalysis_tag).make
cmt_final_setup_RunNumuCCAnalysis = $(bin)numuCCAnalysissetup.make
cmt_local_RunNumuCCAnalysis_makefile = $(bin)RunNumuCCAnalysis.make
endif

endif

ifndef QUICK
$(cmt_local_RunNumuCCAnalysis_makefile) :: $(RunNumuCCAnalysis_dependencies) $(cmt_local_tagfile_RunNumuCCAnalysis) build_library_links dirs
else
$(cmt_local_RunNumuCCAnalysis_makefile) :: $(cmt_local_tagfile_RunNumuCCAnalysis)
endif
	$(echo) "(constituents.make) Building RunNumuCCAnalysis.make"; \
	  $(cmtexe) -tag=$(tags) $(RunNumuCCAnalysis_extratags) build constituent_makefile -out=$(cmt_local_RunNumuCCAnalysis_makefile) RunNumuCCAnalysis

RunNumuCCAnalysis :: $(RunNumuCCAnalysis_dependencies) $(cmt_local_RunNumuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting RunNumuCCAnalysis"
	@$(MAKE) -f $(cmt_local_RunNumuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} RunNumuCCAnalysis
	$(echo) "(constituents.make) RunNumuCCAnalysis done"

clean :: RunNumuCCAnalysisclean

RunNumuCCAnalysisclean :: $(RunNumuCCAnalysisclean_dependencies) ##$(cmt_local_RunNumuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting RunNumuCCAnalysisclean"
	@-if test -f $(cmt_local_RunNumuCCAnalysis_makefile); then \
	  $(MAKE) -f $(cmt_local_RunNumuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} RunNumuCCAnalysisclean; \
	fi

##	  /bin/rm -f $(cmt_local_RunNumuCCAnalysis_makefile) $(bin)RunNumuCCAnalysis_dependencies.make

install :: RunNumuCCAnalysisinstall

RunNumuCCAnalysisinstall :: $(RunNumuCCAnalysis_dependencies) $(cmt_local_RunNumuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting install RunNumuCCAnalysis"
	@-$(MAKE) -f $(cmt_local_RunNumuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install RunNumuCCAnalysis done"

uninstall :: RunNumuCCAnalysisuninstall

RunNumuCCAnalysisuninstall :: $(cmt_local_RunNumuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting uninstall RunNumuCCAnalysis"
	@-$(MAKE) -f $(cmt_local_RunNumuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall RunNumuCCAnalysis done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ RunNumuCCAnalysis"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ RunNumuCCAnalysis done"
endif


#-- end of constituent ------
#-- start of constituent_lock ------

cmt_make_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_make_has_target_tag

ifdef READONLY
cmt_local_tagfile_make = /tmp/CMT_$(numuCCAnalysis_tag)_make.make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_numuCCAnalysis_makesetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(numuCCAnalysis_tag)_make.make
cmt_local_tagfile_make = $(bin)$(numuCCAnalysis_tag)_make.make
cmt_final_setup_make = $(bin)numuCCAnalysis_makesetup.make
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
cmt_local_tagfile_make = /tmp/CMT_$(numuCCAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_numuCCAnalysissetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(numuCCAnalysis_tag).make
cmt_local_tagfile_make = $(bin)$(numuCCAnalysis_tag).make
cmt_final_setup_make = $(bin)numuCCAnalysissetup.make
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
