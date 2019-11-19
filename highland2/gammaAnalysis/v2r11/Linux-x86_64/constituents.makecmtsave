
#-- start of constituents_header ------

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

tags      = $(tag),$(CMTEXTRATAGS)

gammaAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile = /tmp/CMT_$(gammaAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile = $(gammaAnalysis_tag).make
cmt_local_tagfile = $(bin)$(gammaAnalysis_tag).make
endif

#-include $(cmt_local_tagfile)
include $(cmt_local_tagfile)

ifdef READONLY
cmt_local_setup = /tmp/CMT_gammaAnalysissetup$(cmt_lock_pid).make
cmt_final_setup = /tmp/CMT_gammaAnalysissetup.make
else
#cmt_local_setup = $(bin)gammaAnalysissetup$(cmt_lock_pid).make
cmt_local_setup = $(bin)$(package)setup$$$$.make
#cmt_final_setup = $(bin)gammaAnalysissetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(gammaAnalysis_tag)_doxygen.make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_gammaAnalysis_doxygensetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(gammaAnalysis_tag)_doxygen.make
cmt_local_tagfile_doxygen = $(bin)$(gammaAnalysis_tag)_doxygen.make
cmt_final_setup_doxygen = $(bin)gammaAnalysis_doxygensetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(gammaAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_gammaAnalysissetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(gammaAnalysis_tag).make
cmt_local_tagfile_doxygen = $(bin)$(gammaAnalysis_tag).make
cmt_final_setup_doxygen = $(bin)gammaAnalysissetup.make
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

cmt_gammaAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_gammaAnalysis_has_target_tag

ifdef READONLY
cmt_local_tagfile_gammaAnalysis = /tmp/CMT_$(gammaAnalysis_tag)_gammaAnalysis.make$(cmt_lock_pid)
cmt_final_setup_gammaAnalysis = /tmp/CMT_gammaAnalysis_gammaAnalysissetup.make
cmt_local_gammaAnalysis_makefile = /tmp/CMT_gammaAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_gammaAnalysis = $(gammaAnalysis_tag)_gammaAnalysis.make
cmt_local_tagfile_gammaAnalysis = $(bin)$(gammaAnalysis_tag)_gammaAnalysis.make
cmt_final_setup_gammaAnalysis = $(bin)gammaAnalysis_gammaAnalysissetup.make
cmt_local_gammaAnalysis_makefile = $(bin)gammaAnalysis.make
endif

gammaAnalysis_extratags = -tag_add=target_gammaAnalysis

#$(cmt_local_tagfile_gammaAnalysis) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_gammaAnalysis) ::
else
$(cmt_local_tagfile_gammaAnalysis) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_gammaAnalysis)"
	@if test -f $(cmt_local_tagfile_gammaAnalysis); then /bin/rm -f $(cmt_local_tagfile_gammaAnalysis); fi ; \
	  $(cmtexe) -tag=$(tags) $(gammaAnalysis_extratags) build tag_makefile >>$(cmt_local_tagfile_gammaAnalysis); \
	  if test -f $(cmt_final_setup_gammaAnalysis); then /bin/rm -f $(cmt_final_setup_gammaAnalysis); fi; \
	  $(cmtexe) -tag=$(tags) $(gammaAnalysis_extratags) show setup >>$(cmt_final_setup_gammaAnalysis)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_gammaAnalysis = /tmp/CMT_$(gammaAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_gammaAnalysis = /tmp/CMT_gammaAnalysissetup.make
cmt_local_gammaAnalysis_makefile = /tmp/CMT_gammaAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_gammaAnalysis = $(gammaAnalysis_tag).make
cmt_local_tagfile_gammaAnalysis = $(bin)$(gammaAnalysis_tag).make
cmt_final_setup_gammaAnalysis = $(bin)gammaAnalysissetup.make
cmt_local_gammaAnalysis_makefile = $(bin)gammaAnalysis.make
endif

endif

ifndef QUICK
$(cmt_local_gammaAnalysis_makefile) :: $(gammaAnalysis_dependencies) $(cmt_local_tagfile_gammaAnalysis) build_library_links dirs
else
$(cmt_local_gammaAnalysis_makefile) :: $(cmt_local_tagfile_gammaAnalysis)
endif
	$(echo) "(constituents.make) Building gammaAnalysis.make"; \
	  $(cmtexe) -tag=$(tags) $(gammaAnalysis_extratags) build constituent_makefile -out=$(cmt_local_gammaAnalysis_makefile) gammaAnalysis

gammaAnalysis :: $(gammaAnalysis_dependencies) $(cmt_local_gammaAnalysis_makefile)
	$(echo) "(constituents.make) Starting gammaAnalysis"
	@$(MAKE) -f $(cmt_local_gammaAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} gammaAnalysis
	$(echo) "(constituents.make) gammaAnalysis done"

clean :: gammaAnalysisclean

gammaAnalysisclean :: $(gammaAnalysisclean_dependencies) ##$(cmt_local_gammaAnalysis_makefile)
	$(echo) "(constituents.make) Starting gammaAnalysisclean"
	@-if test -f $(cmt_local_gammaAnalysis_makefile); then \
	  $(MAKE) -f $(cmt_local_gammaAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} gammaAnalysisclean; \
	fi

##	  /bin/rm -f $(cmt_local_gammaAnalysis_makefile) $(bin)gammaAnalysis_dependencies.make

install :: gammaAnalysisinstall

gammaAnalysisinstall :: $(gammaAnalysis_dependencies) $(cmt_local_gammaAnalysis_makefile)
	$(echo) "(constituents.make) Starting install gammaAnalysis"
	@-$(MAKE) -f $(cmt_local_gammaAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install gammaAnalysis done"

uninstall :: gammaAnalysisuninstall

gammaAnalysisuninstall :: $(cmt_local_gammaAnalysis_makefile)
	$(echo) "(constituents.make) Starting uninstall gammaAnalysis"
	@-$(MAKE) -f $(cmt_local_gammaAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall gammaAnalysis done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ gammaAnalysis"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ gammaAnalysis done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_RunGammaAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunGammaAnalysis_has_target_tag

ifdef READONLY
cmt_local_tagfile_RunGammaAnalysis = /tmp/CMT_$(gammaAnalysis_tag)_RunGammaAnalysis.make$(cmt_lock_pid)
cmt_final_setup_RunGammaAnalysis = /tmp/CMT_gammaAnalysis_RunGammaAnalysissetup.make
cmt_local_RunGammaAnalysis_makefile = /tmp/CMT_RunGammaAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunGammaAnalysis = $(gammaAnalysis_tag)_RunGammaAnalysis.make
cmt_local_tagfile_RunGammaAnalysis = $(bin)$(gammaAnalysis_tag)_RunGammaAnalysis.make
cmt_final_setup_RunGammaAnalysis = $(bin)gammaAnalysis_RunGammaAnalysissetup.make
cmt_local_RunGammaAnalysis_makefile = $(bin)RunGammaAnalysis.make
endif

RunGammaAnalysis_extratags = -tag_add=target_RunGammaAnalysis

#$(cmt_local_tagfile_RunGammaAnalysis) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_RunGammaAnalysis) ::
else
$(cmt_local_tagfile_RunGammaAnalysis) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_RunGammaAnalysis)"
	@if test -f $(cmt_local_tagfile_RunGammaAnalysis); then /bin/rm -f $(cmt_local_tagfile_RunGammaAnalysis); fi ; \
	  $(cmtexe) -tag=$(tags) $(RunGammaAnalysis_extratags) build tag_makefile >>$(cmt_local_tagfile_RunGammaAnalysis); \
	  if test -f $(cmt_final_setup_RunGammaAnalysis); then /bin/rm -f $(cmt_final_setup_RunGammaAnalysis); fi; \
	  $(cmtexe) -tag=$(tags) $(RunGammaAnalysis_extratags) show setup >>$(cmt_final_setup_RunGammaAnalysis)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_RunGammaAnalysis = /tmp/CMT_$(gammaAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_RunGammaAnalysis = /tmp/CMT_gammaAnalysissetup.make
cmt_local_RunGammaAnalysis_makefile = /tmp/CMT_RunGammaAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunGammaAnalysis = $(gammaAnalysis_tag).make
cmt_local_tagfile_RunGammaAnalysis = $(bin)$(gammaAnalysis_tag).make
cmt_final_setup_RunGammaAnalysis = $(bin)gammaAnalysissetup.make
cmt_local_RunGammaAnalysis_makefile = $(bin)RunGammaAnalysis.make
endif

endif

ifndef QUICK
$(cmt_local_RunGammaAnalysis_makefile) :: $(RunGammaAnalysis_dependencies) $(cmt_local_tagfile_RunGammaAnalysis) build_library_links dirs
else
$(cmt_local_RunGammaAnalysis_makefile) :: $(cmt_local_tagfile_RunGammaAnalysis)
endif
	$(echo) "(constituents.make) Building RunGammaAnalysis.make"; \
	  $(cmtexe) -tag=$(tags) $(RunGammaAnalysis_extratags) build constituent_makefile -out=$(cmt_local_RunGammaAnalysis_makefile) RunGammaAnalysis

RunGammaAnalysis :: $(RunGammaAnalysis_dependencies) $(cmt_local_RunGammaAnalysis_makefile)
	$(echo) "(constituents.make) Starting RunGammaAnalysis"
	@$(MAKE) -f $(cmt_local_RunGammaAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} RunGammaAnalysis
	$(echo) "(constituents.make) RunGammaAnalysis done"

clean :: RunGammaAnalysisclean

RunGammaAnalysisclean :: $(RunGammaAnalysisclean_dependencies) ##$(cmt_local_RunGammaAnalysis_makefile)
	$(echo) "(constituents.make) Starting RunGammaAnalysisclean"
	@-if test -f $(cmt_local_RunGammaAnalysis_makefile); then \
	  $(MAKE) -f $(cmt_local_RunGammaAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} RunGammaAnalysisclean; \
	fi

##	  /bin/rm -f $(cmt_local_RunGammaAnalysis_makefile) $(bin)RunGammaAnalysis_dependencies.make

install :: RunGammaAnalysisinstall

RunGammaAnalysisinstall :: $(RunGammaAnalysis_dependencies) $(cmt_local_RunGammaAnalysis_makefile)
	$(echo) "(constituents.make) Starting install RunGammaAnalysis"
	@-$(MAKE) -f $(cmt_local_RunGammaAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install RunGammaAnalysis done"

uninstall :: RunGammaAnalysisuninstall

RunGammaAnalysisuninstall :: $(cmt_local_RunGammaAnalysis_makefile)
	$(echo) "(constituents.make) Starting uninstall RunGammaAnalysis"
	@-$(MAKE) -f $(cmt_local_RunGammaAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall RunGammaAnalysis done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ RunGammaAnalysis"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ RunGammaAnalysis done"
endif


#-- end of constituent ------
#-- start of constituent_lock ------

cmt_make_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_make_has_target_tag

ifdef READONLY
cmt_local_tagfile_make = /tmp/CMT_$(gammaAnalysis_tag)_make.make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_gammaAnalysis_makesetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(gammaAnalysis_tag)_make.make
cmt_local_tagfile_make = $(bin)$(gammaAnalysis_tag)_make.make
cmt_final_setup_make = $(bin)gammaAnalysis_makesetup.make
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
cmt_local_tagfile_make = /tmp/CMT_$(gammaAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_gammaAnalysissetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(gammaAnalysis_tag).make
cmt_local_tagfile_make = $(bin)$(gammaAnalysis_tag).make
cmt_final_setup_make = $(bin)gammaAnalysissetup.make
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
