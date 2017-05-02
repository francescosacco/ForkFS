# Test Software
Software to test the ForkFS.

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
