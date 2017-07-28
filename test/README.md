# Test Software
Software to test the ForkFS.

* 2017-07-25 - Version 022 - Add more tests.
  * Change Code Page to Latin 1 (850).
  * Add test with long file name.
  * Add test with Code Page.
  * Add test to create a directory with wrong name.

* 2017-07-25 - Version 021 - Add test to check wrong file name.
  * Add test to create a file with wrong name.

* 2017-07-21 - Version 020 - Add Set Codepage function.
  * Add f_setcp().

* 2017-07-07 - Version 019 - Add flush and new drive to DiskIO, improve test.
  * Add fflush() to disk_ioctl().
  * Add new drive with 128MB.
  * Improve test using new drive.

* 2017-06-27 - Version 018 - Fix libraries and types.
  * Add string.h to use memset() and strcmp().
  * Change WORD type.

* 2017-06-20 - Version 017 - Changed UINT and BYTE types.
  * Remove UINT and INT types from integer.h file.

* 2017-06-06 - Version 016 - Improved f_printf() and add f_putc() test.
  * Improved f_printf() test.
  * Added f_putc() test.

* 2017-05-30 - Version 015 - Added f_printf() test at the File System.
  * Added f_printf() test.

* 2017-05-26 - Version 014 - Added f_forward() test at the File System.
  * Added f_forward() test.

* 2017-05-23 - Version 013 - Changed for FatFS R0.13 from ELM-ChaN.
  * Fixed date check problems at f_stat().
  * Changed Makefile.

* 2017-05-19 - Version 012 - Changed File System tests and added more tests.
  * Changed File System struct to use buffer.
  * Added f_chdrive() test.

* 2017-05-16 - Version 011 - Added more tests to check the File System.
  * Changed _USE_EXPAND to use f_expand().
  * Added f_expand() test.

* 2017-05-12 - Version 010 - Fixed warnings and added more test to check the File System.
  * Fixed Warning about cast using strcmp().
  * Fixed Warning about data type using printf().
  * Added f_lseek() test.

* 2017-05-09 - Version 009 - Changed to support Linux GCC.
  * Changed diskio.c to use fseek() instead of fsetpos().

* 2017-05-05 - Version 008 - Added more tests to check the File System.
  * Added FILINFO test, from f_stat().
  * Added f_utime() test.

* 2017-05-02 - Version 007 - Added more tests to check the File System.
  * Added f_chmod() test.

* 2017-04-28 - Version 006 - Added more tests to check the File System.
  * Added f_getcwd() test.
  * Added f_setlabel() test.
  * Added f_getlabel() test.
  * Added f_getfree() test.

* 2017-04-21 - Version 005 - Added more tests to check the File System.
  * Added f_stat() test.
  * Added f_sync() test.

* 2017-04-18 - Version 004 - Added more tests to check the File System.
  * Added f_chdir() test.
  * Added f_opendir() test.
  * Added f_readdir() test.
  * Added f_closedir() test.
  * Changed the software name to "Sanity Test Software".

* 2017-04-11 - Version 003 - Added more tests to check the File System.
  * Added f_unlink() test.
  * Added f_disk() test.

* 2017-04-07 - Version 002 - Added more tests to check the File System.
  * Added f_truncate() test.

* 2017-04-04 - Version 001 - Added more tests to check the File System.
  * Added fread() and f_rename() test.
  * Printed parameters.
  * Check return of written or read bytes.
  * Check file size.

* 2017-03-31 - Version 000 - First version of the test project.
  * Criate a 16MBytes disk.
  * The test formats the disk, opens a file in the root directory, writes 1KByte, and closes the file.
