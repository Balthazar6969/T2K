#-- start of make_header -----------------

#====================================
#  Library psycheIO
#
#   Generated Mon Nov  4 15:26:50 2019  by cfrancois
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_psycheIO_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_psycheIO_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_psycheIO

psycheIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheIO = /tmp/CMT_$(psycheIO_tag)_psycheIO.make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheIO = $(psycheIO_tag)_psycheIO.make
cmt_local_tagfile_psycheIO = $(bin)$(psycheIO_tag)_psycheIO.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheIO = /tmp/CMT_$(psycheIO_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheIO = $(psycheIO_tag).make
cmt_local_tagfile_psycheIO = $(bin)$(psycheIO_tag).make
endif

endif

-include $(cmt_local_tagfile_psycheIO)

ifdef cmt_psycheIO_has_target_tag

ifdef READONLY
cmt_final_setup_psycheIO = /tmp/CMT_psycheIO_psycheIOsetup.make
cmt_local_psycheIO_makefile = /tmp/CMT_psycheIO$(cmt_lock_pid).make
else
cmt_final_setup_psycheIO = $(bin)psycheIO_psycheIOsetup.make
cmt_local_psycheIO_makefile = $(bin)psycheIO.make
endif

else

ifdef READONLY
cmt_final_setup_psycheIO = /tmp/CMT_psycheIOsetup.make
cmt_local_psycheIO_makefile = /tmp/CMT_psycheIO$(cmt_lock_pid).make
else
cmt_final_setup_psycheIO = $(bin)psycheIOsetup.make
cmt_local_psycheIO_makefile = $(bin)psycheIO.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheIOsetup.make
else
cmt_final_setup = $(bin)psycheIOsetup.make
endif

psycheIO ::


ifdef READONLY
psycheIO ::
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
	$(echo) 'psycheIO'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = psycheIO/
psycheIO::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

psycheIOlibname   = $(bin)$(library_prefix)psycheIO$(library_suffix)
psycheIOlib       = $(psycheIOlibname).a
psycheIOstamp     = $(bin)psycheIO.stamp
psycheIOshstamp   = $(bin)psycheIO.shstamp

psycheIO :: dirs  psycheIOLIB
	$(echo) "psycheIO ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

psycheIOLIB :: $(psycheIOlib) $(psycheIOshstamp)
	$(echo) "psycheIO : library ok"

$(psycheIOlib) :: $(bin)MiniTreeConverter.o $(bin)RedoTreeConverter.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(psycheIOlib) $?
	$(lib_silent) $(ranlib) $(psycheIOlib)
	$(lib_silent) cat /dev/null >$(psycheIOstamp)

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

$(psycheIOlibname).$(shlibsuffix) :: $(psycheIOlib) requirements $(use_requirements) $(psycheIOstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" psycheIO $(psycheIO_shlibflags)

$(psycheIOshstamp) :: $(psycheIOlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(psycheIOlibname).$(shlibsuffix) ; then cat /dev/null >$(psycheIOshstamp) ; fi

psycheIOclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)MiniTreeConverter.o $(bin)RedoTreeConverter.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf psycheIO_deps psycheIO_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
psycheIOinstallname = $(library_prefix)psycheIO$(library_suffix).$(shlibsuffix)

psycheIO :: psycheIOinstall

install :: psycheIOinstall

psycheIOinstall :: $(install_dir)/$(psycheIOinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(psycheIOinstallname) :: $(bin)$(psycheIOinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheIOinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##psycheIOclean :: psycheIOuninstall

uninstall :: psycheIOuninstall

psycheIOuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheIOinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),psycheIOclean)

#$(bin)psycheIO_dependencies.make :: dirs

ifndef QUICK
$(bin)psycheIO_dependencies.make : ../src/MiniTreeConverter.cxx ../src/RedoTreeConverter.cxx $(use_requirements) $(cmt_final_setup_psycheIO)
	$(echo) "(psycheIO.make) Rebuilding $@"; \
	  $(build_dependencies) psycheIO -all_sources -out=$@ ../src/MiniTreeConverter.cxx ../src/RedoTreeConverter.cxx
endif

#$(psycheIO_dependencies)

-include $(bin)psycheIO_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)psycheIO_dependencies.make : $(MiniTreeConverter_cxx_dependencies)

$(bin)$(binobj)MiniTreeConverter.o : $(MiniTreeConverter_cxx_dependencies)
	$(cpp_echo) $(src)MiniTreeConverter.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheIO_pp_cppflags) $(lib_psycheIO_pp_cppflags) $(MiniTreeConverter_pp_cppflags) $(use_cppflags) $(psycheIO_cppflags) $(lib_psycheIO_cppflags) $(MiniTreeConverter_cppflags) $(MiniTreeConverter_cxx_cppflags)  $(src)MiniTreeConverter.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheIO_dependencies.make : $(RedoTreeConverter_cxx_dependencies)

$(bin)$(binobj)RedoTreeConverter.o : $(RedoTreeConverter_cxx_dependencies)
	$(cpp_echo) $(src)RedoTreeConverter.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheIO_pp_cppflags) $(lib_psycheIO_pp_cppflags) $(RedoTreeConverter_pp_cppflags) $(use_cppflags) $(psycheIO_cppflags) $(lib_psycheIO_cppflags) $(RedoTreeConverter_cppflags) $(RedoTreeConverter_cxx_cppflags)  $(src)RedoTreeConverter.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: psycheIOclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(psycheIO.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(psycheIO.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_psycheIO)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheIO.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheIO.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheIO.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

psycheIOclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)psycheIO$(library_suffix).a $(binobj)$(library_prefix)psycheIO$(library_suffix).s? $(binobj)psycheIO.stamp $(binobj)psycheIO.shstamp
#-- end of cleanup_library ---------------

