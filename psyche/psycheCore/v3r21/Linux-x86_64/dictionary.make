#-- start of make_header -----------------

#====================================
#  Document dictionary
#
#   Generated Mon Nov  4 15:26:36 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_dictionary_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_dictionary_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_dictionary

psycheCore_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_dictionary = /tmp/CMT_$(psycheCore_tag)_dictionary.make$(cmt_lock_pid)
else
#cmt_local_tagfile_dictionary = $(psycheCore_tag)_dictionary.make
cmt_local_tagfile_dictionary = $(bin)$(psycheCore_tag)_dictionary.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheCore_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_dictionary = /tmp/CMT_$(psycheCore_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_dictionary = $(psycheCore_tag).make
cmt_local_tagfile_dictionary = $(bin)$(psycheCore_tag).make
endif

endif

-include $(cmt_local_tagfile_dictionary)

ifdef cmt_dictionary_has_target_tag

ifdef READONLY
cmt_final_setup_dictionary = /tmp/CMT_psycheCore_dictionarysetup.make
cmt_local_dictionary_makefile = /tmp/CMT_dictionary$(cmt_lock_pid).make
else
cmt_final_setup_dictionary = $(bin)psycheCore_dictionarysetup.make
cmt_local_dictionary_makefile = $(bin)dictionary.make
endif

else

ifdef READONLY
cmt_final_setup_dictionary = /tmp/CMT_psycheCoresetup.make
cmt_local_dictionary_makefile = /tmp/CMT_dictionary$(cmt_lock_pid).make
else
cmt_final_setup_dictionary = $(bin)psycheCoresetup.make
cmt_local_dictionary_makefile = $(bin)dictionary.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheCoresetup.make
else
cmt_final_setup = $(bin)psycheCoresetup.make
endif

dictionary ::


ifdef READONLY
dictionary ::
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
	$(echo) 'dictionary'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = dictionary/
dictionary::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

dictionary_output = $(dict)

dictionary :: $(filter %_LinkDef.rootcint,$(dictionary_output)TreeManager_LinkDef.rootcint $(dictionary_output)CoreDataClasses_LinkDef.rootcint $(dictionary_output)DataSample_LinkDef.rootcint $(dictionary_output)SystematicBase_LinkDef.rootcint $(dictionary_output)Experiment_LinkDef.rootcint $(dictionary_output)HLClonesArray_LinkDef.rootcint $(dictionary_output)Header_LinkDef.rootcint $(dictionary_output)SelectionBase_LinkDef.rootcint $(dictionary_output)StepBase_LinkDef.rootcint)
	@echo "------> dictionary ok"

TreeManager_LinkDef_h_dependencies = ../src/TreeManager_LinkDef.h
CoreDataClasses_LinkDef_h_dependencies = ../src/CoreDataClasses_LinkDef.h
DataSample_LinkDef_h_dependencies = ../src/DataSample_LinkDef.h
SystematicBase_LinkDef_h_dependencies = ../src/SystematicBase_LinkDef.h
Experiment_LinkDef_h_dependencies = ../src/Experiment_LinkDef.h
HLClonesArray_LinkDef_h_dependencies = ../src/HLClonesArray_LinkDef.h
Header_LinkDef_h_dependencies = ../src/Header_LinkDef.h
SelectionBase_LinkDef_h_dependencies = ../src/SelectionBase_LinkDef.h
StepBase_LinkDef_h_dependencies = ../src/StepBase_LinkDef.h
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)TreeManager_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,TreeManager_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,TreeManager_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)TreeManager_LinkDef.h) \
    $(src)TreeManager_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)CoreDataClasses_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,CoreDataClasses_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,CoreDataClasses_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)CoreDataClasses_LinkDef.h) \
    $(src)CoreDataClasses_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)DataSample_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,DataSample_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,DataSample_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)DataSample_LinkDef.h) \
    $(src)DataSample_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)SystematicBase_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,SystematicBase_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,SystematicBase_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)SystematicBase_LinkDef.h) \
    $(src)SystematicBase_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)Experiment_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,Experiment_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,Experiment_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)Experiment_LinkDef.h) \
    $(src)Experiment_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)HLClonesArray_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,HLClonesArray_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,HLClonesArray_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)HLClonesArray_LinkDef.h) \
    $(src)HLClonesArray_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)Header_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,Header_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,Header_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)Header_LinkDef.h) \
    $(src)Header_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)SelectionBase_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,SelectionBase_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,SelectionBase_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)SelectionBase_LinkDef.h) \
    $(src)SelectionBase_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)StepBase_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,StepBase_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,StepBase_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)StepBase_LinkDef.h) \
    $(src)StepBase_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

clean :: dictionaryclean
	@cd .

dictionaryclean ::
	@echo "Removing root dictionaries: $(dict)"
	/bin/rm -rf $(dict)
#-- start of cleanup_header --------------

clean :: dictionaryclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(dictionary.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(dictionary.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_dictionary)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(dictionary.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(dictionary.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(dictionary.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

dictionaryclean ::
#-- end of cleanup_header ---------------
