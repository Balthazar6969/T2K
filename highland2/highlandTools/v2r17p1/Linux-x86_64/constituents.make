
#-- start of constituents_header ------

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

tags      = $(tag),$(CMTEXTRATAGS)

highlandTools_tag = $(tag)

ifdef READONLY
cmt_local_tagfile = /tmp/CMT_$(highlandTools_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile = $(highlandTools_tag).make
cmt_local_tagfile = $(bin)$(highlandTools_tag).make
endif

#-include $(cmt_local_tagfile)
include $(cmt_local_tagfile)

ifdef READONLY
cmt_local_setup = /tmp/CMT_highlandToolssetup$(cmt_lock_pid).make
cmt_final_setup = /tmp/CMT_highlandToolssetup.make
else
#cmt_local_setup = $(bin)highlandToolssetup$(cmt_lock_pid).make
cmt_local_setup = $(bin)$(package)setup$$$$.make
#cmt_final_setup = $(bin)highlandToolssetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(highlandTools_tag)_doxygen.make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_highlandTools_doxygensetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(highlandTools_tag)_doxygen.make
cmt_local_tagfile_doxygen = $(bin)$(highlandTools_tag)_doxygen.make
cmt_final_setup_doxygen = $(bin)highlandTools_doxygensetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(highlandTools_tag).make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_highlandToolssetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(highlandTools_tag).make
cmt_local_tagfile_doxygen = $(bin)$(highlandTools_tag).make
cmt_final_setup_doxygen = $(bin)highlandToolssetup.make
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
#-- start of constituent_lock ------

cmt_dictionary_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_dictionary_has_target_tag

ifdef READONLY
cmt_local_tagfile_dictionary = /tmp/CMT_$(highlandTools_tag)_dictionary.make$(cmt_lock_pid)
cmt_final_setup_dictionary = /tmp/CMT_highlandTools_dictionarysetup.make
cmt_local_dictionary_makefile = /tmp/CMT_dictionary$(cmt_lock_pid).make
else
#cmt_local_tagfile_dictionary = $(highlandTools_tag)_dictionary.make
cmt_local_tagfile_dictionary = $(bin)$(highlandTools_tag)_dictionary.make
cmt_final_setup_dictionary = $(bin)highlandTools_dictionarysetup.make
cmt_local_dictionary_makefile = $(bin)dictionary.make
endif

dictionary_extratags = -tag_add=target_dictionary

#$(cmt_local_tagfile_dictionary) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_dictionary) ::
else
$(cmt_local_tagfile_dictionary) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_dictionary)"
	@if test -f $(cmt_local_tagfile_dictionary); then /bin/rm -f $(cmt_local_tagfile_dictionary); fi ; \
	  $(cmtexe) -tag=$(tags) $(dictionary_extratags) build tag_makefile >>$(cmt_local_tagfile_dictionary); \
	  if test -f $(cmt_final_setup_dictionary); then /bin/rm -f $(cmt_final_setup_dictionary); fi; \
	  $(cmtexe) -tag=$(tags) $(dictionary_extratags) show setup >>$(cmt_final_setup_dictionary)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_dictionary = /tmp/CMT_$(highlandTools_tag).make$(cmt_lock_pid)
cmt_final_setup_dictionary = /tmp/CMT_highlandToolssetup.make
cmt_local_dictionary_makefile = /tmp/CMT_dictionary$(cmt_lock_pid).make
else
#cmt_local_tagfile_dictionary = $(highlandTools_tag).make
cmt_local_tagfile_dictionary = $(bin)$(highlandTools_tag).make
cmt_final_setup_dictionary = $(bin)highlandToolssetup.make
cmt_local_dictionary_makefile = $(bin)dictionary.make
endif

endif

ifndef QUICK
$(cmt_local_dictionary_makefile) :: $(dictionary_dependencies) $(cmt_local_tagfile_dictionary) build_library_links dirs
else
$(cmt_local_dictionary_makefile) :: $(cmt_local_tagfile_dictionary)
endif
	$(echo) "(constituents.make) Building dictionary.make"; \
	  $(cmtexe) -tag=$(tags) $(dictionary_extratags) build constituent_makefile -out=$(cmt_local_dictionary_makefile) dictionary

dictionary :: $(dictionary_dependencies) $(cmt_local_dictionary_makefile)
	$(echo) "(constituents.make) Creating dictionary${lock_suffix} and Starting dictionary"
	@${lock_command} dictionary${lock_suffix} || exit $$?; \
	  retval=$$?; \
	  trap '${unlock_command} dictionary${lock_suffix}; exit $${retval}' 1 2 15; \
	  $(MAKE) -f $(cmt_local_dictionary_makefile) cmt_lock_pid=$${cmt_lock_pid} dictionary; \
	  retval=$$?; ${unlock_command} dictionary${lock_suffix}; exit $${retval}
	$(echo) "(constituents.make) dictionary done"

clean :: dictionaryclean

dictionaryclean :: $(dictionaryclean_dependencies) ##$(cmt_local_dictionary_makefile)
	$(echo) "(constituents.make) Starting dictionaryclean"
	@-if test -f $(cmt_local_dictionary_makefile); then \
	  $(MAKE) -f $(cmt_local_dictionary_makefile) cmt_lock_pid=$${cmt_lock_pid} dictionaryclean; \
	fi

##	  /bin/rm -f $(cmt_local_dictionary_makefile) $(bin)dictionary_dependencies.make

install :: dictionaryinstall

dictionaryinstall :: $(dictionary_dependencies) $(cmt_local_dictionary_makefile)
	$(echo) "(constituents.make) Starting install dictionary"
	@-$(MAKE) -f $(cmt_local_dictionary_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install dictionary done"

uninstall :: dictionaryuninstall

dictionaryuninstall :: $(cmt_local_dictionary_makefile)
	$(echo) "(constituents.make) Starting uninstall dictionary"
	@-$(MAKE) -f $(cmt_local_dictionary_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall dictionary done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ dictionary"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ dictionary done"
endif


#-- end of constituent_lock ------
#-- start of constituent ------

cmt_highlandTools_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_highlandTools_has_target_tag

ifdef READONLY
cmt_local_tagfile_highlandTools = /tmp/CMT_$(highlandTools_tag)_highlandTools.make$(cmt_lock_pid)
cmt_final_setup_highlandTools = /tmp/CMT_highlandTools_highlandToolssetup.make
cmt_local_highlandTools_makefile = /tmp/CMT_highlandTools$(cmt_lock_pid).make
else
#cmt_local_tagfile_highlandTools = $(highlandTools_tag)_highlandTools.make
cmt_local_tagfile_highlandTools = $(bin)$(highlandTools_tag)_highlandTools.make
cmt_final_setup_highlandTools = $(bin)highlandTools_highlandToolssetup.make
cmt_local_highlandTools_makefile = $(bin)highlandTools.make
endif

highlandTools_extratags = -tag_add=target_highlandTools

#$(cmt_local_tagfile_highlandTools) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_highlandTools) ::
else
$(cmt_local_tagfile_highlandTools) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_highlandTools)"
	@if test -f $(cmt_local_tagfile_highlandTools); then /bin/rm -f $(cmt_local_tagfile_highlandTools); fi ; \
	  $(cmtexe) -tag=$(tags) $(highlandTools_extratags) build tag_makefile >>$(cmt_local_tagfile_highlandTools); \
	  if test -f $(cmt_final_setup_highlandTools); then /bin/rm -f $(cmt_final_setup_highlandTools); fi; \
	  $(cmtexe) -tag=$(tags) $(highlandTools_extratags) show setup >>$(cmt_final_setup_highlandTools)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_highlandTools = /tmp/CMT_$(highlandTools_tag).make$(cmt_lock_pid)
cmt_final_setup_highlandTools = /tmp/CMT_highlandToolssetup.make
cmt_local_highlandTools_makefile = /tmp/CMT_highlandTools$(cmt_lock_pid).make
else
#cmt_local_tagfile_highlandTools = $(highlandTools_tag).make
cmt_local_tagfile_highlandTools = $(bin)$(highlandTools_tag).make
cmt_final_setup_highlandTools = $(bin)highlandToolssetup.make
cmt_local_highlandTools_makefile = $(bin)highlandTools.make
endif

endif

ifndef QUICK
$(cmt_local_highlandTools_makefile) :: $(highlandTools_dependencies) $(cmt_local_tagfile_highlandTools) build_library_links dirs
else
$(cmt_local_highlandTools_makefile) :: $(cmt_local_tagfile_highlandTools)
endif
	$(echo) "(constituents.make) Building highlandTools.make"; \
	  $(cmtexe) -tag=$(tags) $(highlandTools_extratags) build constituent_makefile -out=$(cmt_local_highlandTools_makefile) highlandTools

highlandTools :: $(highlandTools_dependencies) $(cmt_local_highlandTools_makefile)
	$(echo) "(constituents.make) Starting highlandTools"
	@$(MAKE) -f $(cmt_local_highlandTools_makefile) cmt_lock_pid=$${cmt_lock_pid} highlandTools
	$(echo) "(constituents.make) highlandTools done"

clean :: highlandToolsclean

highlandToolsclean :: $(highlandToolsclean_dependencies) ##$(cmt_local_highlandTools_makefile)
	$(echo) "(constituents.make) Starting highlandToolsclean"
	@-if test -f $(cmt_local_highlandTools_makefile); then \
	  $(MAKE) -f $(cmt_local_highlandTools_makefile) cmt_lock_pid=$${cmt_lock_pid} highlandToolsclean; \
	fi

##	  /bin/rm -f $(cmt_local_highlandTools_makefile) $(bin)highlandTools_dependencies.make

install :: highlandToolsinstall

highlandToolsinstall :: $(highlandTools_dependencies) $(cmt_local_highlandTools_makefile)
	$(echo) "(constituents.make) Starting install highlandTools"
	@-$(MAKE) -f $(cmt_local_highlandTools_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install highlandTools done"

uninstall :: highlandToolsuninstall

highlandToolsuninstall :: $(cmt_local_highlandTools_makefile)
	$(echo) "(constituents.make) Starting uninstall highlandTools"
	@-$(MAKE) -f $(cmt_local_highlandTools_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall highlandTools done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ highlandTools"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ highlandTools done"
endif


#-- end of constituent ------
#-- start of constituent_lock ------

cmt_make_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_make_has_target_tag

ifdef READONLY
cmt_local_tagfile_make = /tmp/CMT_$(highlandTools_tag)_make.make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_highlandTools_makesetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(highlandTools_tag)_make.make
cmt_local_tagfile_make = $(bin)$(highlandTools_tag)_make.make
cmt_final_setup_make = $(bin)highlandTools_makesetup.make
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
cmt_local_tagfile_make = /tmp/CMT_$(highlandTools_tag).make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_highlandToolssetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(highlandTools_tag).make
cmt_local_tagfile_make = $(bin)$(highlandTools_tag).make
cmt_final_setup_make = $(bin)highlandToolssetup.make
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