
#-- start of constituents_header ------

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

tags      = $(tag),$(CMTEXTRATAGS)

psycheSelections_tag = $(tag)

ifdef READONLY
cmt_local_tagfile = /tmp/CMT_$(psycheSelections_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile = $(psycheSelections_tag).make
cmt_local_tagfile = $(bin)$(psycheSelections_tag).make
endif

#-include $(cmt_local_tagfile)
include $(cmt_local_tagfile)

ifdef READONLY
cmt_local_setup = /tmp/CMT_psycheSelectionssetup$(cmt_lock_pid).make
cmt_final_setup = /tmp/CMT_psycheSelectionssetup.make
else
#cmt_local_setup = $(bin)psycheSelectionssetup$(cmt_lock_pid).make
cmt_local_setup = $(bin)$(package)setup$$$$.make
#cmt_final_setup = $(bin)psycheSelectionssetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(psycheSelections_tag)_doxygen.make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_psycheSelections_doxygensetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(psycheSelections_tag)_doxygen.make
cmt_local_tagfile_doxygen = $(bin)$(psycheSelections_tag)_doxygen.make
cmt_final_setup_doxygen = $(bin)psycheSelections_doxygensetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(psycheSelections_tag).make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_psycheSelectionssetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(psycheSelections_tag).make
cmt_local_tagfile_doxygen = $(bin)$(psycheSelections_tag).make
cmt_final_setup_doxygen = $(bin)psycheSelectionssetup.make
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

cmt_psycheSelections_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_psycheSelections_has_target_tag

ifdef READONLY
cmt_local_tagfile_psycheSelections = /tmp/CMT_$(psycheSelections_tag)_psycheSelections.make$(cmt_lock_pid)
cmt_final_setup_psycheSelections = /tmp/CMT_psycheSelections_psycheSelectionssetup.make
cmt_local_psycheSelections_makefile = /tmp/CMT_psycheSelections$(cmt_lock_pid).make
else
#cmt_local_tagfile_psycheSelections = $(psycheSelections_tag)_psycheSelections.make
cmt_local_tagfile_psycheSelections = $(bin)$(psycheSelections_tag)_psycheSelections.make
cmt_final_setup_psycheSelections = $(bin)psycheSelections_psycheSelectionssetup.make
cmt_local_psycheSelections_makefile = $(bin)psycheSelections.make
endif

psycheSelections_extratags = -tag_add=target_psycheSelections

#$(cmt_local_tagfile_psycheSelections) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_psycheSelections) ::
else
$(cmt_local_tagfile_psycheSelections) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_psycheSelections)"
	@if test -f $(cmt_local_tagfile_psycheSelections); then /bin/rm -f $(cmt_local_tagfile_psycheSelections); fi ; \
	  $(cmtexe) -tag=$(tags) $(psycheSelections_extratags) build tag_makefile >>$(cmt_local_tagfile_psycheSelections); \
	  if test -f $(cmt_final_setup_psycheSelections); then /bin/rm -f $(cmt_final_setup_psycheSelections); fi; \
	  $(cmtexe) -tag=$(tags) $(psycheSelections_extratags) show setup >>$(cmt_final_setup_psycheSelections)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_psycheSelections = /tmp/CMT_$(psycheSelections_tag).make$(cmt_lock_pid)
cmt_final_setup_psycheSelections = /tmp/CMT_psycheSelectionssetup.make
cmt_local_psycheSelections_makefile = /tmp/CMT_psycheSelections$(cmt_lock_pid).make
else
#cmt_local_tagfile_psycheSelections = $(psycheSelections_tag).make
cmt_local_tagfile_psycheSelections = $(bin)$(psycheSelections_tag).make
cmt_final_setup_psycheSelections = $(bin)psycheSelectionssetup.make
cmt_local_psycheSelections_makefile = $(bin)psycheSelections.make
endif

endif

ifndef QUICK
$(cmt_local_psycheSelections_makefile) :: $(psycheSelections_dependencies) $(cmt_local_tagfile_psycheSelections) build_library_links dirs
else
$(cmt_local_psycheSelections_makefile) :: $(cmt_local_tagfile_psycheSelections)
endif
	$(echo) "(constituents.make) Building psycheSelections.make"; \
	  $(cmtexe) -tag=$(tags) $(psycheSelections_extratags) build constituent_makefile -out=$(cmt_local_psycheSelections_makefile) psycheSelections

psycheSelections :: $(psycheSelections_dependencies) $(cmt_local_psycheSelections_makefile)
	$(echo) "(constituents.make) Starting psycheSelections"
	@$(MAKE) -f $(cmt_local_psycheSelections_makefile) cmt_lock_pid=$${cmt_lock_pid} psycheSelections
	$(echo) "(constituents.make) psycheSelections done"

clean :: psycheSelectionsclean

psycheSelectionsclean :: $(psycheSelectionsclean_dependencies) ##$(cmt_local_psycheSelections_makefile)
	$(echo) "(constituents.make) Starting psycheSelectionsclean"
	@-if test -f $(cmt_local_psycheSelections_makefile); then \
	  $(MAKE) -f $(cmt_local_psycheSelections_makefile) cmt_lock_pid=$${cmt_lock_pid} psycheSelectionsclean; \
	fi

##	  /bin/rm -f $(cmt_local_psycheSelections_makefile) $(bin)psycheSelections_dependencies.make

install :: psycheSelectionsinstall

psycheSelectionsinstall :: $(psycheSelections_dependencies) $(cmt_local_psycheSelections_makefile)
	$(echo) "(constituents.make) Starting install psycheSelections"
	@-$(MAKE) -f $(cmt_local_psycheSelections_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install psycheSelections done"

uninstall :: psycheSelectionsuninstall

psycheSelectionsuninstall :: $(cmt_local_psycheSelections_makefile)
	$(echo) "(constituents.make) Starting uninstall psycheSelections"
	@-$(MAKE) -f $(cmt_local_psycheSelections_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall psycheSelections done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ psycheSelections"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ psycheSelections done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_RunSelectionExample_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunSelectionExample_has_target_tag

ifdef READONLY
cmt_local_tagfile_RunSelectionExample = /tmp/CMT_$(psycheSelections_tag)_RunSelectionExample.make$(cmt_lock_pid)
cmt_final_setup_RunSelectionExample = /tmp/CMT_psycheSelections_RunSelectionExamplesetup.make
cmt_local_RunSelectionExample_makefile = /tmp/CMT_RunSelectionExample$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunSelectionExample = $(psycheSelections_tag)_RunSelectionExample.make
cmt_local_tagfile_RunSelectionExample = $(bin)$(psycheSelections_tag)_RunSelectionExample.make
cmt_final_setup_RunSelectionExample = $(bin)psycheSelections_RunSelectionExamplesetup.make
cmt_local_RunSelectionExample_makefile = $(bin)RunSelectionExample.make
endif

RunSelectionExample_extratags = -tag_add=target_RunSelectionExample

#$(cmt_local_tagfile_RunSelectionExample) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_RunSelectionExample) ::
else
$(cmt_local_tagfile_RunSelectionExample) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_RunSelectionExample)"
	@if test -f $(cmt_local_tagfile_RunSelectionExample); then /bin/rm -f $(cmt_local_tagfile_RunSelectionExample); fi ; \
	  $(cmtexe) -tag=$(tags) $(RunSelectionExample_extratags) build tag_makefile >>$(cmt_local_tagfile_RunSelectionExample); \
	  if test -f $(cmt_final_setup_RunSelectionExample); then /bin/rm -f $(cmt_final_setup_RunSelectionExample); fi; \
	  $(cmtexe) -tag=$(tags) $(RunSelectionExample_extratags) show setup >>$(cmt_final_setup_RunSelectionExample)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_RunSelectionExample = /tmp/CMT_$(psycheSelections_tag).make$(cmt_lock_pid)
cmt_final_setup_RunSelectionExample = /tmp/CMT_psycheSelectionssetup.make
cmt_local_RunSelectionExample_makefile = /tmp/CMT_RunSelectionExample$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunSelectionExample = $(psycheSelections_tag).make
cmt_local_tagfile_RunSelectionExample = $(bin)$(psycheSelections_tag).make
cmt_final_setup_RunSelectionExample = $(bin)psycheSelectionssetup.make
cmt_local_RunSelectionExample_makefile = $(bin)RunSelectionExample.make
endif

endif

ifndef QUICK
$(cmt_local_RunSelectionExample_makefile) :: $(RunSelectionExample_dependencies) $(cmt_local_tagfile_RunSelectionExample) build_library_links dirs
else
$(cmt_local_RunSelectionExample_makefile) :: $(cmt_local_tagfile_RunSelectionExample)
endif
	$(echo) "(constituents.make) Building RunSelectionExample.make"; \
	  $(cmtexe) -tag=$(tags) $(RunSelectionExample_extratags) build constituent_makefile -out=$(cmt_local_RunSelectionExample_makefile) RunSelectionExample

RunSelectionExample :: $(RunSelectionExample_dependencies) $(cmt_local_RunSelectionExample_makefile)
	$(echo) "(constituents.make) Starting RunSelectionExample"
	@$(MAKE) -f $(cmt_local_RunSelectionExample_makefile) cmt_lock_pid=$${cmt_lock_pid} RunSelectionExample
	$(echo) "(constituents.make) RunSelectionExample done"

clean :: RunSelectionExampleclean

RunSelectionExampleclean :: $(RunSelectionExampleclean_dependencies) ##$(cmt_local_RunSelectionExample_makefile)
	$(echo) "(constituents.make) Starting RunSelectionExampleclean"
	@-if test -f $(cmt_local_RunSelectionExample_makefile); then \
	  $(MAKE) -f $(cmt_local_RunSelectionExample_makefile) cmt_lock_pid=$${cmt_lock_pid} RunSelectionExampleclean; \
	fi

##	  /bin/rm -f $(cmt_local_RunSelectionExample_makefile) $(bin)RunSelectionExample_dependencies.make

install :: RunSelectionExampleinstall

RunSelectionExampleinstall :: $(RunSelectionExample_dependencies) $(cmt_local_RunSelectionExample_makefile)
	$(echo) "(constituents.make) Starting install RunSelectionExample"
	@-$(MAKE) -f $(cmt_local_RunSelectionExample_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install RunSelectionExample done"

uninstall :: RunSelectionExampleuninstall

RunSelectionExampleuninstall :: $(cmt_local_RunSelectionExample_makefile)
	$(echo) "(constituents.make) Starting uninstall RunSelectionExample"
	@-$(MAKE) -f $(cmt_local_RunSelectionExample_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall RunSelectionExample done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ RunSelectionExample"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ RunSelectionExample done"
endif


#-- end of constituent ------
#-- start of constituent_lock ------

cmt_make_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_make_has_target_tag

ifdef READONLY
cmt_local_tagfile_make = /tmp/CMT_$(psycheSelections_tag)_make.make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_psycheSelections_makesetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(psycheSelections_tag)_make.make
cmt_local_tagfile_make = $(bin)$(psycheSelections_tag)_make.make
cmt_final_setup_make = $(bin)psycheSelections_makesetup.make
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
cmt_local_tagfile_make = /tmp/CMT_$(psycheSelections_tag).make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_psycheSelectionssetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(psycheSelections_tag).make
cmt_local_tagfile_make = $(bin)$(psycheSelections_tag).make
cmt_final_setup_make = $(bin)psycheSelectionssetup.make
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
