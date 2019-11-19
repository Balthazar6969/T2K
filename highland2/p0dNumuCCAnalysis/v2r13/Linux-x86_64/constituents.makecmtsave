
#-- start of constituents_header ------

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

tags      = $(tag),$(CMTEXTRATAGS)

p0dNumuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile = /tmp/CMT_$(p0dNumuCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile = $(p0dNumuCCAnalysis_tag).make
cmt_local_tagfile = $(bin)$(p0dNumuCCAnalysis_tag).make
endif

#-include $(cmt_local_tagfile)
include $(cmt_local_tagfile)

ifdef READONLY
cmt_local_setup = /tmp/CMT_p0dNumuCCAnalysissetup$(cmt_lock_pid).make
cmt_final_setup = /tmp/CMT_p0dNumuCCAnalysissetup.make
else
#cmt_local_setup = $(bin)p0dNumuCCAnalysissetup$(cmt_lock_pid).make
cmt_local_setup = $(bin)$(package)setup$$$$.make
#cmt_final_setup = $(bin)p0dNumuCCAnalysissetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(p0dNumuCCAnalysis_tag)_doxygen.make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_p0dNumuCCAnalysis_doxygensetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(p0dNumuCCAnalysis_tag)_doxygen.make
cmt_local_tagfile_doxygen = $(bin)$(p0dNumuCCAnalysis_tag)_doxygen.make
cmt_final_setup_doxygen = $(bin)p0dNumuCCAnalysis_doxygensetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(p0dNumuCCAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_p0dNumuCCAnalysissetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(p0dNumuCCAnalysis_tag).make
cmt_local_tagfile_doxygen = $(bin)$(p0dNumuCCAnalysis_tag).make
cmt_final_setup_doxygen = $(bin)p0dNumuCCAnalysissetup.make
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

cmt_p0dNumuCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_p0dNumuCCAnalysis_has_target_tag

ifdef READONLY
cmt_local_tagfile_p0dNumuCCAnalysis = /tmp/CMT_$(p0dNumuCCAnalysis_tag)_p0dNumuCCAnalysis.make$(cmt_lock_pid)
cmt_final_setup_p0dNumuCCAnalysis = /tmp/CMT_p0dNumuCCAnalysis_p0dNumuCCAnalysissetup.make
cmt_local_p0dNumuCCAnalysis_makefile = /tmp/CMT_p0dNumuCCAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_p0dNumuCCAnalysis = $(p0dNumuCCAnalysis_tag)_p0dNumuCCAnalysis.make
cmt_local_tagfile_p0dNumuCCAnalysis = $(bin)$(p0dNumuCCAnalysis_tag)_p0dNumuCCAnalysis.make
cmt_final_setup_p0dNumuCCAnalysis = $(bin)p0dNumuCCAnalysis_p0dNumuCCAnalysissetup.make
cmt_local_p0dNumuCCAnalysis_makefile = $(bin)p0dNumuCCAnalysis.make
endif

p0dNumuCCAnalysis_extratags = -tag_add=target_p0dNumuCCAnalysis

#$(cmt_local_tagfile_p0dNumuCCAnalysis) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_p0dNumuCCAnalysis) ::
else
$(cmt_local_tagfile_p0dNumuCCAnalysis) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_p0dNumuCCAnalysis)"
	@if test -f $(cmt_local_tagfile_p0dNumuCCAnalysis); then /bin/rm -f $(cmt_local_tagfile_p0dNumuCCAnalysis); fi ; \
	  $(cmtexe) -tag=$(tags) $(p0dNumuCCAnalysis_extratags) build tag_makefile >>$(cmt_local_tagfile_p0dNumuCCAnalysis); \
	  if test -f $(cmt_final_setup_p0dNumuCCAnalysis); then /bin/rm -f $(cmt_final_setup_p0dNumuCCAnalysis); fi; \
	  $(cmtexe) -tag=$(tags) $(p0dNumuCCAnalysis_extratags) show setup >>$(cmt_final_setup_p0dNumuCCAnalysis)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_p0dNumuCCAnalysis = /tmp/CMT_$(p0dNumuCCAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_p0dNumuCCAnalysis = /tmp/CMT_p0dNumuCCAnalysissetup.make
cmt_local_p0dNumuCCAnalysis_makefile = /tmp/CMT_p0dNumuCCAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_p0dNumuCCAnalysis = $(p0dNumuCCAnalysis_tag).make
cmt_local_tagfile_p0dNumuCCAnalysis = $(bin)$(p0dNumuCCAnalysis_tag).make
cmt_final_setup_p0dNumuCCAnalysis = $(bin)p0dNumuCCAnalysissetup.make
cmt_local_p0dNumuCCAnalysis_makefile = $(bin)p0dNumuCCAnalysis.make
endif

endif

ifndef QUICK
$(cmt_local_p0dNumuCCAnalysis_makefile) :: $(p0dNumuCCAnalysis_dependencies) $(cmt_local_tagfile_p0dNumuCCAnalysis) build_library_links dirs
else
$(cmt_local_p0dNumuCCAnalysis_makefile) :: $(cmt_local_tagfile_p0dNumuCCAnalysis)
endif
	$(echo) "(constituents.make) Building p0dNumuCCAnalysis.make"; \
	  $(cmtexe) -tag=$(tags) $(p0dNumuCCAnalysis_extratags) build constituent_makefile -out=$(cmt_local_p0dNumuCCAnalysis_makefile) p0dNumuCCAnalysis

p0dNumuCCAnalysis :: $(p0dNumuCCAnalysis_dependencies) $(cmt_local_p0dNumuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting p0dNumuCCAnalysis"
	@$(MAKE) -f $(cmt_local_p0dNumuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} p0dNumuCCAnalysis
	$(echo) "(constituents.make) p0dNumuCCAnalysis done"

clean :: p0dNumuCCAnalysisclean

p0dNumuCCAnalysisclean :: $(p0dNumuCCAnalysisclean_dependencies) ##$(cmt_local_p0dNumuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting p0dNumuCCAnalysisclean"
	@-if test -f $(cmt_local_p0dNumuCCAnalysis_makefile); then \
	  $(MAKE) -f $(cmt_local_p0dNumuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} p0dNumuCCAnalysisclean; \
	fi

##	  /bin/rm -f $(cmt_local_p0dNumuCCAnalysis_makefile) $(bin)p0dNumuCCAnalysis_dependencies.make

install :: p0dNumuCCAnalysisinstall

p0dNumuCCAnalysisinstall :: $(p0dNumuCCAnalysis_dependencies) $(cmt_local_p0dNumuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting install p0dNumuCCAnalysis"
	@-$(MAKE) -f $(cmt_local_p0dNumuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install p0dNumuCCAnalysis done"

uninstall :: p0dNumuCCAnalysisuninstall

p0dNumuCCAnalysisuninstall :: $(cmt_local_p0dNumuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting uninstall p0dNumuCCAnalysis"
	@-$(MAKE) -f $(cmt_local_p0dNumuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall p0dNumuCCAnalysis done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ p0dNumuCCAnalysis"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ p0dNumuCCAnalysis done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_RunP0DNumuCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunP0DNumuCCAnalysis_has_target_tag

ifdef READONLY
cmt_local_tagfile_RunP0DNumuCCAnalysis = /tmp/CMT_$(p0dNumuCCAnalysis_tag)_RunP0DNumuCCAnalysis.make$(cmt_lock_pid)
cmt_final_setup_RunP0DNumuCCAnalysis = /tmp/CMT_p0dNumuCCAnalysis_RunP0DNumuCCAnalysissetup.make
cmt_local_RunP0DNumuCCAnalysis_makefile = /tmp/CMT_RunP0DNumuCCAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunP0DNumuCCAnalysis = $(p0dNumuCCAnalysis_tag)_RunP0DNumuCCAnalysis.make
cmt_local_tagfile_RunP0DNumuCCAnalysis = $(bin)$(p0dNumuCCAnalysis_tag)_RunP0DNumuCCAnalysis.make
cmt_final_setup_RunP0DNumuCCAnalysis = $(bin)p0dNumuCCAnalysis_RunP0DNumuCCAnalysissetup.make
cmt_local_RunP0DNumuCCAnalysis_makefile = $(bin)RunP0DNumuCCAnalysis.make
endif

RunP0DNumuCCAnalysis_extratags = -tag_add=target_RunP0DNumuCCAnalysis

#$(cmt_local_tagfile_RunP0DNumuCCAnalysis) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_RunP0DNumuCCAnalysis) ::
else
$(cmt_local_tagfile_RunP0DNumuCCAnalysis) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_RunP0DNumuCCAnalysis)"
	@if test -f $(cmt_local_tagfile_RunP0DNumuCCAnalysis); then /bin/rm -f $(cmt_local_tagfile_RunP0DNumuCCAnalysis); fi ; \
	  $(cmtexe) -tag=$(tags) $(RunP0DNumuCCAnalysis_extratags) build tag_makefile >>$(cmt_local_tagfile_RunP0DNumuCCAnalysis); \
	  if test -f $(cmt_final_setup_RunP0DNumuCCAnalysis); then /bin/rm -f $(cmt_final_setup_RunP0DNumuCCAnalysis); fi; \
	  $(cmtexe) -tag=$(tags) $(RunP0DNumuCCAnalysis_extratags) show setup >>$(cmt_final_setup_RunP0DNumuCCAnalysis)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_RunP0DNumuCCAnalysis = /tmp/CMT_$(p0dNumuCCAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_RunP0DNumuCCAnalysis = /tmp/CMT_p0dNumuCCAnalysissetup.make
cmt_local_RunP0DNumuCCAnalysis_makefile = /tmp/CMT_RunP0DNumuCCAnalysis$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunP0DNumuCCAnalysis = $(p0dNumuCCAnalysis_tag).make
cmt_local_tagfile_RunP0DNumuCCAnalysis = $(bin)$(p0dNumuCCAnalysis_tag).make
cmt_final_setup_RunP0DNumuCCAnalysis = $(bin)p0dNumuCCAnalysissetup.make
cmt_local_RunP0DNumuCCAnalysis_makefile = $(bin)RunP0DNumuCCAnalysis.make
endif

endif

ifndef QUICK
$(cmt_local_RunP0DNumuCCAnalysis_makefile) :: $(RunP0DNumuCCAnalysis_dependencies) $(cmt_local_tagfile_RunP0DNumuCCAnalysis) build_library_links dirs
else
$(cmt_local_RunP0DNumuCCAnalysis_makefile) :: $(cmt_local_tagfile_RunP0DNumuCCAnalysis)
endif
	$(echo) "(constituents.make) Building RunP0DNumuCCAnalysis.make"; \
	  $(cmtexe) -tag=$(tags) $(RunP0DNumuCCAnalysis_extratags) build constituent_makefile -out=$(cmt_local_RunP0DNumuCCAnalysis_makefile) RunP0DNumuCCAnalysis

RunP0DNumuCCAnalysis :: $(RunP0DNumuCCAnalysis_dependencies) $(cmt_local_RunP0DNumuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting RunP0DNumuCCAnalysis"
	@$(MAKE) -f $(cmt_local_RunP0DNumuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} RunP0DNumuCCAnalysis
	$(echo) "(constituents.make) RunP0DNumuCCAnalysis done"

clean :: RunP0DNumuCCAnalysisclean

RunP0DNumuCCAnalysisclean :: $(RunP0DNumuCCAnalysisclean_dependencies) ##$(cmt_local_RunP0DNumuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting RunP0DNumuCCAnalysisclean"
	@-if test -f $(cmt_local_RunP0DNumuCCAnalysis_makefile); then \
	  $(MAKE) -f $(cmt_local_RunP0DNumuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} RunP0DNumuCCAnalysisclean; \
	fi

##	  /bin/rm -f $(cmt_local_RunP0DNumuCCAnalysis_makefile) $(bin)RunP0DNumuCCAnalysis_dependencies.make

install :: RunP0DNumuCCAnalysisinstall

RunP0DNumuCCAnalysisinstall :: $(RunP0DNumuCCAnalysis_dependencies) $(cmt_local_RunP0DNumuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting install RunP0DNumuCCAnalysis"
	@-$(MAKE) -f $(cmt_local_RunP0DNumuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install RunP0DNumuCCAnalysis done"

uninstall :: RunP0DNumuCCAnalysisuninstall

RunP0DNumuCCAnalysisuninstall :: $(cmt_local_RunP0DNumuCCAnalysis_makefile)
	$(echo) "(constituents.make) Starting uninstall RunP0DNumuCCAnalysis"
	@-$(MAKE) -f $(cmt_local_RunP0DNumuCCAnalysis_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall RunP0DNumuCCAnalysis done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ RunP0DNumuCCAnalysis"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ RunP0DNumuCCAnalysis done"
endif


#-- end of constituent ------
#-- start of constituent_lock ------

cmt_make_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_make_has_target_tag

ifdef READONLY
cmt_local_tagfile_make = /tmp/CMT_$(p0dNumuCCAnalysis_tag)_make.make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_p0dNumuCCAnalysis_makesetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(p0dNumuCCAnalysis_tag)_make.make
cmt_local_tagfile_make = $(bin)$(p0dNumuCCAnalysis_tag)_make.make
cmt_final_setup_make = $(bin)p0dNumuCCAnalysis_makesetup.make
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
cmt_local_tagfile_make = /tmp/CMT_$(p0dNumuCCAnalysis_tag).make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_p0dNumuCCAnalysissetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(p0dNumuCCAnalysis_tag).make
cmt_local_tagfile_make = $(bin)$(p0dNumuCCAnalysis_tag).make
cmt_final_setup_make = $(bin)p0dNumuCCAnalysissetup.make
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
