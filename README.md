# ForkFS
It's a fork from FatFS. The original project is from ELM-Chan.

* 2017-03-31 - * 2017-03-31 - First version of the test project - 000.

* 2016-10-13 - Added FatFS from ELM-ChaN version R0.12B
  * This initial code have no change from ELN-ChaN version.
  
* 2016-09-04 - [ELM-ChaN](http://elm-chan.org/) R0.12b
  * Improved f_rename() to be able to rename objects with the same name but case.
  * Fixed an error in the case conversion teble of code page 866. (ff.c)
  * Fixed writing data is truncated at the file offset 4GiB on the exFAT volume. (appeared at R0.12)
  * Fixed creating a file in the root directory of exFAT volume can fail. (appeared at R0.12)
  * Fixed f_mkfs() creating exFAT volume with too small cluster size can collapse unallocated memory. (appeared at R0.12)
  * Fixed wrong object name can be returned when read directory at Unicode cfg. (appeared at R0.12)
  * Fixed large file allocation/removing on the exFAT volume collapses allocation bitmap. (appeared at R0.12)
  * Fixed some internal errors in f_expand() and f_lseek(). (appeared at R0.12)

* 2016-07-10 - [ELM-ChaN](http://elm-chan.org/) R0.12a
  * Added support for creating exFAT volume with some changes of f_mkfs().
  * Added a file open method FA_OPEN_APPEND. An f_lseek() following f_open() is no longer needed.
  * f_forward() is available regardless of _FS_TINY.
  * Fixed f_mkfs() creates wrong volume. (appeared at R0.12)
  * Fixed wrong memory read in create_name(). (appeared at R0.12)
  * Fixed compilation fails at some configurations, _USE_FASTSEEK and _USE_FORWARD.

* 2016-04-12 - [ELM-ChaN](http://elm-chan.org/) R0.12
  * Added support for exFAT file system. (_FS_EXFAT)
  * Added f_expand(). (_USE_EXPAND)
  * Changed some members in FINFO structure and behavior of f_readdir().
  * Added an option _USE_CHMOD and removed an option _WORD_ACCESS.
  * Fixed errors in the case conversion teble of Unicode (cc*.c).

* 2015-09-05 - [ELM-ChaN](http://elm-chan.org/) R0.11a
  * Fixed wrong media change can lead a deadlock at thread-safe configuration.
  * Added code page 771, 860, 861, 863, 864, 865 and 869. (_CODE_PAGE)
  * Removed some code pages actually not exist on the standard systems. (_CODE_PAGE)
  * Fixed errors in the case conversion teble of code page 437 and 850 (ff.c).
  * Fixed errors in the case conversion teble of Unicode (cc*.c).

* 2015-02-09 - [ELM-ChaN](http://elm-chan.org/) R0.11
  * Added f_findfirst() and f_findnext(). (_USE_FIND)
  * Fixed f_unlink() does not remove cluster chain of the file. (appeared at R0.10c)
  * Fixed _FS_NORTC option does not work properly. (appeared at R0.10c)

* 2014-09-09 - [ELM-ChaN](http://elm-chan.org/) R0.10c
  * Added a configuration option for the platforms without RTC. (_FS_NORTC)
  * Fixed volume label created by Mac OS X cannot be retrieved with f_getlabel(). (appeared at R0.09b)
  * Fixed a potential problem of FAT access that can appear on disk error.
  * Fixed null pointer dereference on attempting to delete the root direcotry. (appeared at R0.08)

* 2014-05-19 - [ELM-ChaN](http://elm-chan.org/) R0.10b
  * Fixed a hard error in the disk I/O layer can collapse the directory entry.
  * Fixed LFN entry is not deleted on delete/rename an object with its lossy converted SFN. (appeared at R0.07)

* 2014-01-15 - [ELM-ChaN](http://elm-chan.org/) R0.10a
  * Added arbitrary strings as drive number in the path name. (_STR_VOLUME_ID)
  * Added an option for minimum sector size. (_MIN_SS)
  * 2nd argument of f_rename() can have a drive number and it will be ignored.
  * Fixed f_mount() with forced mount fails when drive number is larger than 0. (appeared at R0.10)
  * Fixed f_close() invalidates the file object without volume lock.
  * Fixed volume lock is left acquired after return from f_closedir(). (appeared at R0.10)
  * Fixed creation of a directory entry with LFN fails on too many SFN collisions. (appeared at R0.07)

* 2013-10-02 - [ELM-ChaN](http://elm-chan.org/) R0.10
  * Added an option for character encoding on the file. (_STRF_ENCODE)
  * Added f_closedir().
  * Added forced full FAT scan option for f_getfree(). (_FS_NOFSINFO)
  * Added forced mount option with changes of f_mount().
  * Improved behavior of volume auto detection.
  * Improved write throughput of f_puts() and f_printf().
  * Changed argument of f_chdrive(), f_mkfs(), disk_read() and disk_write().
  * Fixed f_write() can be truncated when the file size is close to 4GB.
  * Fixed f_open(), f_mkdir() and f_setlabel() can return incorrect result code on error.

* 2013-01-24 - [ELM-ChaN](http://elm-chan.org/) R0.09b
  * Added f_getlabel() and f_setlabel(). (_USE_LABEL = 1)

* 2012-08-27 - [ELM-ChaN](http://elm-chan.org/) R0.09a
  * Fixed assertion failure due to OS/2 EA on FAT12/16 volume.
  * Changed file functions reject null object pointer to avoid crash.
  * Changed option name _FS_SHARE to _FS_LOCK.

* 2011-09-06 - [ELM-ChaN](http://elm-chan.org/) R0.09
  * f_mkfs() supports multiple partition on a physical drive.
  * Added f_fdisk(). (_MULTI_PARTITION = 2)

* 2011-01-15 - [ELM-ChaN](http://elm-chan.org/) R0.08b
  * Fast seek function is also applied to f_read() and f_write().
  * f_lseek() reports required table size on creating CLMP.
  * Extended format syntax of f_printf().
  * Ignores duplicated directory separators in given path names.

* 2010-08-16 - [ELM-ChaN](http://elm-chan.org/) R0.08a
  * Added f_getcwd(). (_FS_RPATH = 2)
  * Added sector erase function. (_USE_ERASE)
  * Moved file lock semaphore table from fs object to the bss.
  * Fixed a wrong directory entry is created on non-LFN cfg when the given name contains ';'.
  * Fixed f_mkfs() creates wrong FAT32 volume.

* 2010-05-15 - [ELM-ChaN](http://elm-chan.org/) R0.08
  * Added a memory configuration option. (_USE_LFN)
  * Added support of file lock. (_FS_SHARE)
  * Added fast seek function. (_USE_FASTSEEK)
  * Changed some types on the API, XCHAR->TCHAR.
  * Changed fname member in the FILINFO structure on Unicode cfg.
  * String functions support UTF-8 encoding files on Unicode cfg.

* 2009-11-03 - [ELM-ChaN](http://elm-chan.org/) R0.07e
  * Separated out configuration options from ff.h to ffconf.h.
  * Added a configuration option, _LFN_UNICODE.
  * Fixed f_unlink() fails to remove a sub-dir on _FS_RPATH.
  * Fixed name matching error on the 13 char boundary.
  * Changed f_readdir() to return the SFN with always upper case on non-LFN cfg.

* 2009-01-21 - [ELM-ChaN](http://elm-chan.org/) R0.07c
  * Fixed f_unlink() may return FR_OK on error.
  * Fixed wrong cache control in f_lseek().
  * Added support of relative path.
  * Added f_chdir().
  * Added f_chdrive().
  * Added proper case conversion to extended characters.

* 2009-04-14 - [ELM-ChaN](http://elm-chan.org/) R0.07a
  * Separated out OS dependent code on re-entrant configuration.
  * Added multiple sector size support.

* 2009-04-01 - [ELM-ChaN](http://elm-chan.org/) R0.07
  * Merged Tiny-FatFs into FatFs as a buffer configuration option.
  * Added long file name support.
  * Added multiple code page support.
  * Added re-entrancy for multitask operation.
  * Added auto cluster size selection to f_mkfs().
  * Added rewind option to f_readdir().
  * Changed result code of critical errors.
  * Renamed string functions to avoid name collision.

* 2008-04-01 - [ELM-ChaN](http://elm-chan.org/) R0.06
  * Added f_forward(). (Tiny-FatFs)
  * Added string functions: fgets(), fputc(), fputs() and fprintf().
  * Improved performance of f_lseek() on moving to the same or following cluster.

* 2008-02-03 - [ELM-ChaN](http://elm-chan.org/) R0.05a
  * Added f_truncate().
  * Added f_utime().
  * Fixed off by one error at FAT sub-type determination.
  * Fixed btr in f_read can be mistruncated.
  * Fixed cached sector is left not flushed when create and close without write.

* 2007-08-26 - [ELM-ChaN](http://elm-chan.org/) R0.05
  * Changed arguments of f_read(), f_write().
  * Changed arguments of f_mkfs(). (FatFs)
  * Fixed f_mkfs() on FAT32 creates incorrect FSInfo. (FatFs)
  * Fixed f_mkdir() on FAT32 creates incorrect directory. (FatFs)

* 2007-05-05 - [ELM-ChaN](http://elm-chan.org/) R0.04b
  * Added _USE_NTFLAG option.
  * Added FSInfo support.
  * Fixed some problems corresponds to FAT32. (Tiny-FatFs)
  * Fixed DBCS name can result FR_INVALID_NAME.
  * Fixed short seek (<= csize) collapses the file object.

* 2007-04-01 - [ELM-ChaN](http://elm-chan.org/) R0.04a
  * Supported multiple partitions on a plysical drive. (FatFs)
  * Added minimization level 3.
  * Added a capability of extending file size to f_lseek().
  * Fixed an endian sensitive code in f_mkfs(). (FatFs)
  * Fixed a problem corresponds to FAT32 support. (Tiny-FatFs)

* 2007-02-04 - [ELM-ChaN](http://elm-chan.org/) R0.04
  * Supported multiple drive system. (FatFs)
  * Changed some APIs for multiple drive system.
  * Added f_mkfs(). (FatFs)
  * Added _USE_FAT32 option. (Tiny-FatFs)

* 2006-12-11 - [ELM-ChaN](http://elm-chan.org/) R0.03a
  * Improved cluster scan algolithm to write files fast.
  * Fixed f_mkdir() creates incorrect directory on FAT32.

* 2006-09-22 - [ELM-ChaN](http://elm-chan.org/) R0.03
  * Added f_rename().
  * Changed option _FS_MINIMUM to _FS_MINIMIZE.

* 2006-06-10 - [ELM-ChaN](http://elm-chan.org/) R0.02a
  * Added a configuration option _FS_MINIMUM.

* 2006-06-01 - [ELM-ChaN](http://elm-chan.org/) R0.02
  * Added FAT12.
  * Removed unbuffered mode.
  * Fixed a problem on small (<32M) patition.

* 2006-04-29 - [ELM-ChaN](http://elm-chan.org/) R0.01
  * First release

* 2006-02-26 - [ELM-ChaN](http://elm-chan.org/) R0.00
  * Prototype (not released)