
#-- start of constituents_header ------

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

tags      = $(tag),$(CMTEXTRATAGS)

highlandIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile = $(highlandIO_tag).make
cmt_local_tagfile = $(bin)$(highlandIO_tag).make
endif

#-include $(cmt_local_tagfile)
include $(cmt_local_tagfile)

ifdef READONLY
cmt_local_setup = /tmp/CMT_highlandIOsetup$(cmt_lock_pid).make
cmt_final_setup = /tmp/CMT_highlandIOsetup.make
else
#cmt_local_setup = $(bin)highlandIOsetup$(cmt_lock_pid).make
cmt_local_setup = $(bin)$(package)setup$$$$.make
#cmt_final_setup = $(bin)highlandIOsetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(highlandIO_tag)_doxygen.make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_highlandIO_doxygensetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(highlandIO_tag)_doxygen.make
cmt_local_tagfile_doxygen = $(bin)$(highlandIO_tag)_doxygen.make
cmt_final_setup_doxygen = $(bin)highlandIO_doxygensetup.make
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
cmt_local_tagfile_doxygen = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_highlandIOsetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(highlandIO_tag).make
cmt_local_tagfile_doxygen = $(bin)$(highlandIO_tag).make
cmt_final_setup_doxygen = $(bin)highlandIOsetup.make
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
cmt_local_tagfile_dictionary = /tmp/CMT_$(highlandIO_tag)_dictionary.make$(cmt_lock_pid)
cmt_final_setup_dictionary = /tmp/CMT_highlandIO_dictionarysetup.make
cmt_local_dictionary_makefile = /tmp/CMT_dictionary$(cmt_lock_pid).make
else
#cmt_local_tagfile_dictionary = $(highlandIO_tag)_dictionary.make
cmt_local_tagfile_dictionary = $(bin)$(highlandIO_tag)_dictionary.make
cmt_final_setup_dictionary = $(bin)highlandIO_dictionarysetup.make
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
cmt_local_tagfile_dictionary = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
cmt_final_setup_dictionary = /tmp/CMT_highlandIOsetup.make
cmt_local_dictionary_makefile = /tmp/CMT_dictionary$(cmt_lock_pid).make
else
#cmt_local_tagfile_dictionary = $(highlandIO_tag).make
cmt_local_tagfile_dictionary = $(bin)$(highlandIO_tag).make
cmt_final_setup_dictionary = $(bin)highlandIOsetup.make
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

cmt_highlandIO_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_highlandIO_has_target_tag

ifdef READONLY
cmt_local_tagfile_highlandIO = /tmp/CMT_$(highlandIO_tag)_highlandIO.make$(cmt_lock_pid)
cmt_final_setup_highlandIO = /tmp/CMT_highlandIO_highlandIOsetup.make
cmt_local_highlandIO_makefile = /tmp/CMT_highlandIO$(cmt_lock_pid).make
else
#cmt_local_tagfile_highlandIO = $(highlandIO_tag)_highlandIO.make
cmt_local_tagfile_highlandIO = $(bin)$(highlandIO_tag)_highlandIO.make
cmt_final_setup_highlandIO = $(bin)highlandIO_highlandIOsetup.make
cmt_local_highlandIO_makefile = $(bin)highlandIO.make
endif

highlandIO_extratags = -tag_add=target_highlandIO

#$(cmt_local_tagfile_highlandIO) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_highlandIO) ::
else
$(cmt_local_tagfile_highlandIO) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_highlandIO)"
	@if test -f $(cmt_local_tagfile_highlandIO); then /bin/rm -f $(cmt_local_tagfile_highlandIO); fi ; \
	  $(cmtexe) -tag=$(tags) $(highlandIO_extratags) build tag_makefile >>$(cmt_local_tagfile_highlandIO); \
	  if test -f $(cmt_final_setup_highlandIO); then /bin/rm -f $(cmt_final_setup_highlandIO); fi; \
	  $(cmtexe) -tag=$(tags) $(highlandIO_extratags) show setup >>$(cmt_final_setup_highlandIO)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_highlandIO = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
cmt_final_setup_highlandIO = /tmp/CMT_highlandIOsetup.make
cmt_local_highlandIO_makefile = /tmp/CMT_highlandIO$(cmt_lock_pid).make
else
#cmt_local_tagfile_highlandIO = $(highlandIO_tag).make
cmt_local_tagfile_highlandIO = $(bin)$(highlandIO_tag).make
cmt_final_setup_highlandIO = $(bin)highlandIOsetup.make
cmt_local_highlandIO_makefile = $(bin)highlandIO.make
endif

endif

ifndef QUICK
$(cmt_local_highlandIO_makefile) :: $(highlandIO_dependencies) $(cmt_local_tagfile_highlandIO) build_library_links dirs
else
$(cmt_local_highlandIO_makefile) :: $(cmt_local_tagfile_highlandIO)
endif
	$(echo) "(constituents.make) Building highlandIO.make"; \
	  $(cmtexe) -tag=$(tags) $(highlandIO_extratags) build constituent_makefile -out=$(cmt_local_highlandIO_makefile) highlandIO

highlandIO :: $(highlandIO_dependencies) $(cmt_local_highlandIO_makefile)
	$(echo) "(constituents.make) Starting highlandIO"
	@$(MAKE) -f $(cmt_local_highlandIO_makefile) cmt_lock_pid=$${cmt_lock_pid} highlandIO
	$(echo) "(constituents.make) highlandIO done"

clean :: highlandIOclean

highlandIOclean :: $(highlandIOclean_dependencies) ##$(cmt_local_highlandIO_makefile)
	$(echo) "(constituents.make) Starting highlandIOclean"
	@-if test -f $(cmt_local_highlandIO_makefile); then \
	  $(MAKE) -f $(cmt_local_highlandIO_makefile) cmt_lock_pid=$${cmt_lock_pid} highlandIOclean; \
	fi

##	  /bin/rm -f $(cmt_local_highlandIO_makefile) $(bin)highlandIO_dependencies.make

install :: highlandIOinstall

highlandIOinstall :: $(highlandIO_dependencies) $(cmt_local_highlandIO_makefile)
	$(echo) "(constituents.make) Starting install highlandIO"
	@-$(MAKE) -f $(cmt_local_highlandIO_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install highlandIO done"

uninstall :: highlandIOuninstall

highlandIOuninstall :: $(cmt_local_highlandIO_makefile)
	$(echo) "(constituents.make) Starting uninstall highlandIO"
	@-$(MAKE) -f $(cmt_local_highlandIO_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall highlandIO done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ highlandIO"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ highlandIO done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_RunCreateFlatTree_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunCreateFlatTree_has_target_tag

ifdef READONLY
cmt_local_tagfile_RunCreateFlatTree = /tmp/CMT_$(highlandIO_tag)_RunCreateFlatTree.make$(cmt_lock_pid)
cmt_final_setup_RunCreateFlatTree = /tmp/CMT_highlandIO_RunCreateFlatTreesetup.make
cmt_local_RunCreateFlatTree_makefile = /tmp/CMT_RunCreateFlatTree$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunCreateFlatTree = $(highlandIO_tag)_RunCreateFlatTree.make
cmt_local_tagfile_RunCreateFlatTree = $(bin)$(highlandIO_tag)_RunCreateFlatTree.make
cmt_final_setup_RunCreateFlatTree = $(bin)highlandIO_RunCreateFlatTreesetup.make
cmt_local_RunCreateFlatTree_makefile = $(bin)RunCreateFlatTree.make
endif

RunCreateFlatTree_extratags = -tag_add=target_RunCreateFlatTree

#$(cmt_local_tagfile_RunCreateFlatTree) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_RunCreateFlatTree) ::
else
$(cmt_local_tagfile_RunCreateFlatTree) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_RunCreateFlatTree)"
	@if test -f $(cmt_local_tagfile_RunCreateFlatTree); then /bin/rm -f $(cmt_local_tagfile_RunCreateFlatTree); fi ; \
	  $(cmtexe) -tag=$(tags) $(RunCreateFlatTree_extratags) build tag_makefile >>$(cmt_local_tagfile_RunCreateFlatTree); \
	  if test -f $(cmt_final_setup_RunCreateFlatTree); then /bin/rm -f $(cmt_final_setup_RunCreateFlatTree); fi; \
	  $(cmtexe) -tag=$(tags) $(RunCreateFlatTree_extratags) show setup >>$(cmt_final_setup_RunCreateFlatTree)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_RunCreateFlatTree = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
cmt_final_setup_RunCreateFlatTree = /tmp/CMT_highlandIOsetup.make
cmt_local_RunCreateFlatTree_makefile = /tmp/CMT_RunCreateFlatTree$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunCreateFlatTree = $(highlandIO_tag).make
cmt_local_tagfile_RunCreateFlatTree = $(bin)$(highlandIO_tag).make
cmt_final_setup_RunCreateFlatTree = $(bin)highlandIOsetup.make
cmt_local_RunCreateFlatTree_makefile = $(bin)RunCreateFlatTree.make
endif

endif

ifndef QUICK
$(cmt_local_RunCreateFlatTree_makefile) :: $(RunCreateFlatTree_dependencies) $(cmt_local_tagfile_RunCreateFlatTree) build_library_links dirs
else
$(cmt_local_RunCreateFlatTree_makefile) :: $(cmt_local_tagfile_RunCreateFlatTree)
endif
	$(echo) "(constituents.make) Building RunCreateFlatTree.make"; \
	  $(cmtexe) -tag=$(tags) $(RunCreateFlatTree_extratags) build constituent_makefile -out=$(cmt_local_RunCreateFlatTree_makefile) RunCreateFlatTree

RunCreateFlatTree :: $(RunCreateFlatTree_dependencies) $(cmt_local_RunCreateFlatTree_makefile)
	$(echo) "(constituents.make) Starting RunCreateFlatTree"
	@$(MAKE) -f $(cmt_local_RunCreateFlatTree_makefile) cmt_lock_pid=$${cmt_lock_pid} RunCreateFlatTree
	$(echo) "(constituents.make) RunCreateFlatTree done"

clean :: RunCreateFlatTreeclean

RunCreateFlatTreeclean :: $(RunCreateFlatTreeclean_dependencies) ##$(cmt_local_RunCreateFlatTree_makefile)
	$(echo) "(constituents.make) Starting RunCreateFlatTreeclean"
	@-if test -f $(cmt_local_RunCreateFlatTree_makefile); then \
	  $(MAKE) -f $(cmt_local_RunCreateFlatTree_makefile) cmt_lock_pid=$${cmt_lock_pid} RunCreateFlatTreeclean; \
	fi

##	  /bin/rm -f $(cmt_local_RunCreateFlatTree_makefile) $(bin)RunCreateFlatTree_dependencies.make

install :: RunCreateFlatTreeinstall

RunCreateFlatTreeinstall :: $(RunCreateFlatTree_dependencies) $(cmt_local_RunCreateFlatTree_makefile)
	$(echo) "(constituents.make) Starting install RunCreateFlatTree"
	@-$(MAKE) -f $(cmt_local_RunCreateFlatTree_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install RunCreateFlatTree done"

uninstall :: RunCreateFlatTreeuninstall

RunCreateFlatTreeuninstall :: $(cmt_local_RunCreateFlatTree_makefile)
	$(echo) "(constituents.make) Starting uninstall RunCreateFlatTree"
	@-$(MAKE) -f $(cmt_local_RunCreateFlatTree_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall RunCreateFlatTree done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ RunCreateFlatTree"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ RunCreateFlatTree done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_RunCreateMiniTree_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunCreateMiniTree_has_target_tag

ifdef READONLY
cmt_local_tagfile_RunCreateMiniTree = /tmp/CMT_$(highlandIO_tag)_RunCreateMiniTree.make$(cmt_lock_pid)
cmt_final_setup_RunCreateMiniTree = /tmp/CMT_highlandIO_RunCreateMiniTreesetup.make
cmt_local_RunCreateMiniTree_makefile = /tmp/CMT_RunCreateMiniTree$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunCreateMiniTree = $(highlandIO_tag)_RunCreateMiniTree.make
cmt_local_tagfile_RunCreateMiniTree = $(bin)$(highlandIO_tag)_RunCreateMiniTree.make
cmt_final_setup_RunCreateMiniTree = $(bin)highlandIO_RunCreateMiniTreesetup.make
cmt_local_RunCreateMiniTree_makefile = $(bin)RunCreateMiniTree.make
endif

RunCreateMiniTree_extratags = -tag_add=target_RunCreateMiniTree

#$(cmt_local_tagfile_RunCreateMiniTree) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_RunCreateMiniTree) ::
else
$(cmt_local_tagfile_RunCreateMiniTree) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_RunCreateMiniTree)"
	@if test -f $(cmt_local_tagfile_RunCreateMiniTree); then /bin/rm -f $(cmt_local_tagfile_RunCreateMiniTree); fi ; \
	  $(cmtexe) -tag=$(tags) $(RunCreateMiniTree_extratags) build tag_makefile >>$(cmt_local_tagfile_RunCreateMiniTree); \
	  if test -f $(cmt_final_setup_RunCreateMiniTree); then /bin/rm -f $(cmt_final_setup_RunCreateMiniTree); fi; \
	  $(cmtexe) -tag=$(tags) $(RunCreateMiniTree_extratags) show setup >>$(cmt_final_setup_RunCreateMiniTree)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_RunCreateMiniTree = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
cmt_final_setup_RunCreateMiniTree = /tmp/CMT_highlandIOsetup.make
cmt_local_RunCreateMiniTree_makefile = /tmp/CMT_RunCreateMiniTree$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunCreateMiniTree = $(highlandIO_tag).make
cmt_local_tagfile_RunCreateMiniTree = $(bin)$(highlandIO_tag).make
cmt_final_setup_RunCreateMiniTree = $(bin)highlandIOsetup.make
cmt_local_RunCreateMiniTree_makefile = $(bin)RunCreateMiniTree.make
endif

endif

ifndef QUICK
$(cmt_local_RunCreateMiniTree_makefile) :: $(RunCreateMiniTree_dependencies) $(cmt_local_tagfile_RunCreateMiniTree) build_library_links dirs
else
$(cmt_local_RunCreateMiniTree_makefile) :: $(cmt_local_tagfile_RunCreateMiniTree)
endif
	$(echo) "(constituents.make) Building RunCreateMiniTree.make"; \
	  $(cmtexe) -tag=$(tags) $(RunCreateMiniTree_extratags) build constituent_makefile -out=$(cmt_local_RunCreateMiniTree_makefile) RunCreateMiniTree

RunCreateMiniTree :: $(RunCreateMiniTree_dependencies) $(cmt_local_RunCreateMiniTree_makefile)
	$(echo) "(constituents.make) Starting RunCreateMiniTree"
	@$(MAKE) -f $(cmt_local_RunCreateMiniTree_makefile) cmt_lock_pid=$${cmt_lock_pid} RunCreateMiniTree
	$(echo) "(constituents.make) RunCreateMiniTree done"

clean :: RunCreateMiniTreeclean

RunCreateMiniTreeclean :: $(RunCreateMiniTreeclean_dependencies) ##$(cmt_local_RunCreateMiniTree_makefile)
	$(echo) "(constituents.make) Starting RunCreateMiniTreeclean"
	@-if test -f $(cmt_local_RunCreateMiniTree_makefile); then \
	  $(MAKE) -f $(cmt_local_RunCreateMiniTree_makefile) cmt_lock_pid=$${cmt_lock_pid} RunCreateMiniTreeclean; \
	fi

##	  /bin/rm -f $(cmt_local_RunCreateMiniTree_makefile) $(bin)RunCreateMiniTree_dependencies.make

install :: RunCreateMiniTreeinstall

RunCreateMiniTreeinstall :: $(RunCreateMiniTree_dependencies) $(cmt_local_RunCreateMiniTree_makefile)
	$(echo) "(constituents.make) Starting install RunCreateMiniTree"
	@-$(MAKE) -f $(cmt_local_RunCreateMiniTree_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install RunCreateMiniTree done"

uninstall :: RunCreateMiniTreeuninstall

RunCreateMiniTreeuninstall :: $(cmt_local_RunCreateMiniTree_makefile)
	$(echo) "(constituents.make) Starting uninstall RunCreateMiniTree"
	@-$(MAKE) -f $(cmt_local_RunCreateMiniTree_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall RunCreateMiniTree done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ RunCreateMiniTree"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ RunCreateMiniTree done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_FixMergedRooVtxLink_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_FixMergedRooVtxLink_has_target_tag

ifdef READONLY
cmt_local_tagfile_FixMergedRooVtxLink = /tmp/CMT_$(highlandIO_tag)_FixMergedRooVtxLink.make$(cmt_lock_pid)
cmt_final_setup_FixMergedRooVtxLink = /tmp/CMT_highlandIO_FixMergedRooVtxLinksetup.make
cmt_local_FixMergedRooVtxLink_makefile = /tmp/CMT_FixMergedRooVtxLink$(cmt_lock_pid).make
else
#cmt_local_tagfile_FixMergedRooVtxLink = $(highlandIO_tag)_FixMergedRooVtxLink.make
cmt_local_tagfile_FixMergedRooVtxLink = $(bin)$(highlandIO_tag)_FixMergedRooVtxLink.make
cmt_final_setup_FixMergedRooVtxLink = $(bin)highlandIO_FixMergedRooVtxLinksetup.make
cmt_local_FixMergedRooVtxLink_makefile = $(bin)FixMergedRooVtxLink.make
endif

FixMergedRooVtxLink_extratags = -tag_add=target_FixMergedRooVtxLink

#$(cmt_local_tagfile_FixMergedRooVtxLink) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_FixMergedRooVtxLink) ::
else
$(cmt_local_tagfile_FixMergedRooVtxLink) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_FixMergedRooVtxLink)"
	@if test -f $(cmt_local_tagfile_FixMergedRooVtxLink); then /bin/rm -f $(cmt_local_tagfile_FixMergedRooVtxLink); fi ; \
	  $(cmtexe) -tag=$(tags) $(FixMergedRooVtxLink_extratags) build tag_makefile >>$(cmt_local_tagfile_FixMergedRooVtxLink); \
	  if test -f $(cmt_final_setup_FixMergedRooVtxLink); then /bin/rm -f $(cmt_final_setup_FixMergedRooVtxLink); fi; \
	  $(cmtexe) -tag=$(tags) $(FixMergedRooVtxLink_extratags) show setup >>$(cmt_final_setup_FixMergedRooVtxLink)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_FixMergedRooVtxLink = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
cmt_final_setup_FixMergedRooVtxLink = /tmp/CMT_highlandIOsetup.make
cmt_local_FixMergedRooVtxLink_makefile = /tmp/CMT_FixMergedRooVtxLink$(cmt_lock_pid).make
else
#cmt_local_tagfile_FixMergedRooVtxLink = $(highlandIO_tag).make
cmt_local_tagfile_FixMergedRooVtxLink = $(bin)$(highlandIO_tag).make
cmt_final_setup_FixMergedRooVtxLink = $(bin)highlandIOsetup.make
cmt_local_FixMergedRooVtxLink_makefile = $(bin)FixMergedRooVtxLink.make
endif

endif

ifndef QUICK
$(cmt_local_FixMergedRooVtxLink_makefile) :: $(FixMergedRooVtxLink_dependencies) $(cmt_local_tagfile_FixMergedRooVtxLink) build_library_links dirs
else
$(cmt_local_FixMergedRooVtxLink_makefile) :: $(cmt_local_tagfile_FixMergedRooVtxLink)
endif
	$(echo) "(constituents.make) Building FixMergedRooVtxLink.make"; \
	  $(cmtexe) -tag=$(tags) $(FixMergedRooVtxLink_extratags) build constituent_makefile -out=$(cmt_local_FixMergedRooVtxLink_makefile) FixMergedRooVtxLink

FixMergedRooVtxLink :: $(FixMergedRooVtxLink_dependencies) $(cmt_local_FixMergedRooVtxLink_makefile)
	$(echo) "(constituents.make) Starting FixMergedRooVtxLink"
	@$(MAKE) -f $(cmt_local_FixMergedRooVtxLink_makefile) cmt_lock_pid=$${cmt_lock_pid} FixMergedRooVtxLink
	$(echo) "(constituents.make) FixMergedRooVtxLink done"

clean :: FixMergedRooVtxLinkclean

FixMergedRooVtxLinkclean :: $(FixMergedRooVtxLinkclean_dependencies) ##$(cmt_local_FixMergedRooVtxLink_makefile)
	$(echo) "(constituents.make) Starting FixMergedRooVtxLinkclean"
	@-if test -f $(cmt_local_FixMergedRooVtxLink_makefile); then \
	  $(MAKE) -f $(cmt_local_FixMergedRooVtxLink_makefile) cmt_lock_pid=$${cmt_lock_pid} FixMergedRooVtxLinkclean; \
	fi

##	  /bin/rm -f $(cmt_local_FixMergedRooVtxLink_makefile) $(bin)FixMergedRooVtxLink_dependencies.make

install :: FixMergedRooVtxLinkinstall

FixMergedRooVtxLinkinstall :: $(FixMergedRooVtxLink_dependencies) $(cmt_local_FixMergedRooVtxLink_makefile)
	$(echo) "(constituents.make) Starting install FixMergedRooVtxLink"
	@-$(MAKE) -f $(cmt_local_FixMergedRooVtxLink_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install FixMergedRooVtxLink done"

uninstall :: FixMergedRooVtxLinkuninstall

FixMergedRooVtxLinkuninstall :: $(cmt_local_FixMergedRooVtxLink_makefile)
	$(echo) "(constituents.make) Starting uninstall FixMergedRooVtxLink"
	@-$(MAKE) -f $(cmt_local_FixMergedRooVtxLink_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall FixMergedRooVtxLink done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ FixMergedRooVtxLink"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ FixMergedRooVtxLink done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_AddHeaderTree_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_AddHeaderTree_has_target_tag

ifdef READONLY
cmt_local_tagfile_AddHeaderTree = /tmp/CMT_$(highlandIO_tag)_AddHeaderTree.make$(cmt_lock_pid)
cmt_final_setup_AddHeaderTree = /tmp/CMT_highlandIO_AddHeaderTreesetup.make
cmt_local_AddHeaderTree_makefile = /tmp/CMT_AddHeaderTree$(cmt_lock_pid).make
else
#cmt_local_tagfile_AddHeaderTree = $(highlandIO_tag)_AddHeaderTree.make
cmt_local_tagfile_AddHeaderTree = $(bin)$(highlandIO_tag)_AddHeaderTree.make
cmt_final_setup_AddHeaderTree = $(bin)highlandIO_AddHeaderTreesetup.make
cmt_local_AddHeaderTree_makefile = $(bin)AddHeaderTree.make
endif

AddHeaderTree_extratags = -tag_add=target_AddHeaderTree

#$(cmt_local_tagfile_AddHeaderTree) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_AddHeaderTree) ::
else
$(cmt_local_tagfile_AddHeaderTree) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_AddHeaderTree)"
	@if test -f $(cmt_local_tagfile_AddHeaderTree); then /bin/rm -f $(cmt_local_tagfile_AddHeaderTree); fi ; \
	  $(cmtexe) -tag=$(tags) $(AddHeaderTree_extratags) build tag_makefile >>$(cmt_local_tagfile_AddHeaderTree); \
	  if test -f $(cmt_final_setup_AddHeaderTree); then /bin/rm -f $(cmt_final_setup_AddHeaderTree); fi; \
	  $(cmtexe) -tag=$(tags) $(AddHeaderTree_extratags) show setup >>$(cmt_final_setup_AddHeaderTree)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_AddHeaderTree = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
cmt_final_setup_AddHeaderTree = /tmp/CMT_highlandIOsetup.make
cmt_local_AddHeaderTree_makefile = /tmp/CMT_AddHeaderTree$(cmt_lock_pid).make
else
#cmt_local_tagfile_AddHeaderTree = $(highlandIO_tag).make
cmt_local_tagfile_AddHeaderTree = $(bin)$(highlandIO_tag).make
cmt_final_setup_AddHeaderTree = $(bin)highlandIOsetup.make
cmt_local_AddHeaderTree_makefile = $(bin)AddHeaderTree.make
endif

endif

ifndef QUICK
$(cmt_local_AddHeaderTree_makefile) :: $(AddHeaderTree_dependencies) $(cmt_local_tagfile_AddHeaderTree) build_library_links dirs
else
$(cmt_local_AddHeaderTree_makefile) :: $(cmt_local_tagfile_AddHeaderTree)
endif
	$(echo) "(constituents.make) Building AddHeaderTree.make"; \
	  $(cmtexe) -tag=$(tags) $(AddHeaderTree_extratags) build constituent_makefile -out=$(cmt_local_AddHeaderTree_makefile) AddHeaderTree

AddHeaderTree :: $(AddHeaderTree_dependencies) $(cmt_local_AddHeaderTree_makefile)
	$(echo) "(constituents.make) Starting AddHeaderTree"
	@$(MAKE) -f $(cmt_local_AddHeaderTree_makefile) cmt_lock_pid=$${cmt_lock_pid} AddHeaderTree
	$(echo) "(constituents.make) AddHeaderTree done"

clean :: AddHeaderTreeclean

AddHeaderTreeclean :: $(AddHeaderTreeclean_dependencies) ##$(cmt_local_AddHeaderTree_makefile)
	$(echo) "(constituents.make) Starting AddHeaderTreeclean"
	@-if test -f $(cmt_local_AddHeaderTree_makefile); then \
	  $(MAKE) -f $(cmt_local_AddHeaderTree_makefile) cmt_lock_pid=$${cmt_lock_pid} AddHeaderTreeclean; \
	fi

##	  /bin/rm -f $(cmt_local_AddHeaderTree_makefile) $(bin)AddHeaderTree_dependencies.make

install :: AddHeaderTreeinstall

AddHeaderTreeinstall :: $(AddHeaderTree_dependencies) $(cmt_local_AddHeaderTree_makefile)
	$(echo) "(constituents.make) Starting install AddHeaderTree"
	@-$(MAKE) -f $(cmt_local_AddHeaderTree_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install AddHeaderTree done"

uninstall :: AddHeaderTreeuninstall

AddHeaderTreeuninstall :: $(cmt_local_AddHeaderTree_makefile)
	$(echo) "(constituents.make) Starting uninstall AddHeaderTree"
	@-$(MAKE) -f $(cmt_local_AddHeaderTree_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall AddHeaderTree done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ AddHeaderTree"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ AddHeaderTree done"
endif


#-- end of constituent ------
#-- start of constituent_lock ------

cmt_make_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_make_has_target_tag

ifdef READONLY
cmt_local_tagfile_make = /tmp/CMT_$(highlandIO_tag)_make.make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_highlandIO_makesetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(highlandIO_tag)_make.make
cmt_local_tagfile_make = $(bin)$(highlandIO_tag)_make.make
cmt_final_setup_make = $(bin)highlandIO_makesetup.make
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
cmt_local_tagfile_make = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_highlandIOsetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(highlandIO_tag).make
cmt_local_tagfile_make = $(bin)$(highlandIO_tag).make
cmt_final_setup_make = $(bin)highlandIOsetup.make
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
