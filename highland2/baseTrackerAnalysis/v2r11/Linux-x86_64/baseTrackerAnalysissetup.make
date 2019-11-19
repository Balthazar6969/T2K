----------> uses
# use baseAnalysis v2r19 highland2
#   use highlandIO v2r21 highland2
#     use psycheIO v3r17 psyche
#       use psycheCore v3r21 psyche
#         use psychePolicy v3r1 psyche
#         use ROOT * 
#           use EXTERN * 
#           use MYSQL * 
#             use EXTERN * 
#           use GSL * 
#             use EXTERN * 
#       use psycheND280Utils v3r23 psyche
#         use psycheUtils v3r21 psyche
#           use psycheEventModel v3r21 psyche
#             use psycheCore v3r21 psyche
#         use psycheEventModel * psyche
#     use highlandCorrections v2r7 highland2
#       use highlandUtils v2r19 highland2
#         use highlandCore v2r21 highland2
#           use psycheCore v3r21 psyche
#         use highlandEventModel v2r19 highland2
#           use psycheEventModel v3r21 psyche
#           use oaAnalysisReader v2r9 highland2
#             use ROOT * 
#         use psycheND280Utils v3r23 psyche
#       use psycheSystematics v3r23 psyche
#         use psycheSelections v3r25 psyche
#           use psycheCore v3r21 psyche
#           use psycheEventModel v3r21 psyche
#           use psycheIO v3r17 psyche
#   use highlandCorrections v2r7 highland2
#   use psycheSelections v3r25 psyche
#   use psycheSystematics v3r23 psyche
#   use highlandTools v2r17p1 highland2
#     use highlandCore v2r21 highland2
#
# Selection :
use CMT v1r20p20081118 (/mnt/data/T2K)
use EXTERN v3r7  (/mnt/data/T2K/frozen)
use GSL v1r15p0n00  (/mnt/data/T2K/frozen)
use MYSQL v5r051an01  (/mnt/data/T2K/frozen)
use ROOT v5r34p09n04  (/mnt/data/T2K/frozen)
use oaAnalysisReader v2r9 highland2 (/mnt/data/backup/home/cfrancois/T2K2/)
use psychePolicy v3r1 psyche (/mnt/data/backup/home/cfrancois/T2K2/)
use psycheCore v3r21 psyche (/mnt/data/backup/home/cfrancois/T2K2/)
use highlandCore v2r21 highland2 (/mnt/data/backup/home/cfrancois/T2K2/)
use highlandTools v2r17p1 highland2 (/mnt/data/backup/home/cfrancois/T2K2/)
use psycheEventModel v3r21 psyche (/mnt/data/backup/home/cfrancois/T2K2/)
use highlandEventModel v2r19 highland2 (/mnt/data/backup/home/cfrancois/T2K2/)
use psycheUtils v3r21 psyche (/mnt/data/backup/home/cfrancois/T2K2/)
use psycheND280Utils v3r23 psyche (/mnt/data/backup/home/cfrancois/T2K2/)
use highlandUtils v2r19 highland2 (/mnt/data/backup/home/cfrancois/T2K2/)
use psycheIO v3r17 psyche (/mnt/data/backup/home/cfrancois/T2K2/)
use psycheSelections v3r25 psyche (/mnt/data/backup/home/cfrancois/T2K2/)
use psycheSystematics v3r23 psyche (/mnt/data/backup/home/cfrancois/T2K2/)
use highlandCorrections v2r7 highland2 (/mnt/data/backup/home/cfrancois/T2K2/)
use highlandIO v2r21 highland2 (/mnt/data/backup/home/cfrancois/T2K2/)
use baseAnalysis v2r19 highland2 (/mnt/data/backup/home/cfrancois/T2K2/)
----------> tags
CMTv1 (from CMTVERSION)
CMTr20 (from CMTVERSION)
CMTp20081118 (from CMTVERSION)
Linux (from uname) package CMT implies [Unix]
Linux-x86_64 (from CMTCONFIG)
cfrancois_config (from PROJECT) excludes [cfrancois_no_config]
cfrancois_root (from PROJECT) excludes [cfrancois_no_root]
cfrancois_cleanup (from PROJECT) excludes [cfrancois_no_cleanup]
cfrancois_prototypes (from PROJECT) excludes [cfrancois_no_prototypes]
cfrancois_without_installarea (from PROJECT) excludes [cfrancois_with_installarea]
cfrancois_with_version_directory (from PROJECT) excludes [cfrancois_without_version_directory]
cfrancois (from PROJECT)
T2K_config (from PROJECT) excludes [T2K_no_config]
T2K_root (from PROJECT) excludes [T2K_no_root]
T2K_cleanup (from PROJECT) excludes [T2K_no_cleanup]
T2K_prototypes (from PROJECT) excludes [T2K_no_prototypes]
T2K_without_installarea (from PROJECT) excludes [T2K_with_installarea]
T2K_with_version_directory (from PROJECT) excludes [T2K_without_version_directory]
x86_64 (from package CMT)
slc610 (from package CMT)
gcc447 (from package CMT)
Unix (from package CMT) excludes [WIN32 Win32]
----------> CMTPATH
# Add path /mnt/data/backup/home/cfrancois/T2K2 from initialization
# Add path /mnt/data/T2K/frozen from initialization
