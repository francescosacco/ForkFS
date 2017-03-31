ForkFs Module Source Files V0.0 - Based on FatFS, from ELM-Chan.


FILES

  00readme.txt This file.
  ffconf.h     Configuration file for FatFs module.
  ff.h         Common include file for FatFs and application module.
  ff.c         FatFs module.
  diskio.h     Common include file for FatFs and disk I/O module.
  integer.h    Integer type definitions for FatFs.
  option       Optional external functions.


  Low level disk I/O module is not included in this archive because the FatFs
  module is only a generic file system layer and not depend on any specific
  storage device. You have to provide a low level disk I/O module that written
  to control the target storage device.

