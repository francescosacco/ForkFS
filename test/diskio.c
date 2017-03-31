/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <stdio.h>
#include "diskio.h"     /* FatFs lower layer API */

#define FILEDISK_NAME                            "filedisk.dsk"
#define FILEDISK_SIZE                            ( 16 * 1024 * 1024 ) // 16MBytes.
#define FILEDISK_SECTOR_SIZE                     ( 512 ) // Sector Size = 512Bytes.
#define FILEDISK_NUMBER_OF_SECTORS               ( FILEDISK_SIZE / FILEDISK_SECTOR_SIZE )


FILE * fileDisk = ( FILE * ) NULL ;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;

    // Is the correct driver?
    if( pdrv == 0 )
    {
        // Yes.
        
        // Is the file open?
        if( fileDisk != ( ( FILE * ) NULL ) )
        {
            // Yes.
            // So, it's ok.
            stat = 0 ;          
        }
        else
        {
            // No.
            // So return "Not Initialized".
            stat = STA_NOINIT ;
        }
    }
    else
    {
        // No.
        // It's the wrong disk.
        // Return "No Disk".
        stat = STA_NODISK ;
    }
    
    return( stat ) ;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv               /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat ;
    unsigned long int i ;
    
    // Is the correct driver?
    if( pdrv == 0 )
    {
        // Is the file open?
        if( fileDisk != ( ( FILE * ) NULL ) )
        {
            // Yes.
            // So, return Ok.
            stat = 0 ;
        }
        else
        {
            // No.
            // So, criate the file.
            
            // Open the file to Read and Write.
            fileDisk = fopen( FILEDISK_NAME , "wb+" ) ;
            // Did the file open correctly?
            if( fileDisk != ( ( FILE * ) NULL ) )
            {
                // Yes.

                // So, fill the file with 00h.
                for( i = 0 ; i < FILEDISK_SIZE ; i++ )
                {
                    ( void ) fputc( 0x00 , fileDisk ) ;
                }
                stat = 0 ;
            }
            else
            {
                // No, there is a problem.
                // Return "No Disk".
                stat = STA_NODISK ;
            }
            
        }
    }
    else
    {
        // No.
        // It's the wrong disk.
        // Return "No Disk".
        stat = STA_NODISK ;
    }
    
    return( stat ) ;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    DRESULT res ;
    fpos_t position ;

    // Is the correct driver?
    if( pdrv == 0 )
    {
        // Is the file open?
        if( fileDisk != ( ( FILE * ) NULL ) )
        {
            // Yes.
            
            // So, set the position.
            position = ( fpos_t ) ( sector * FILEDISK_SECTOR_SIZE ) ;
            ( void ) fsetpos( fileDisk , &position ) ;
            
            // Read 
            ( void ) fread( buff , sizeof( BYTE ) , count * FILEDISK_SECTOR_SIZE , fileDisk ) ;
            
            res = RES_OK ;
        }
        else
        {
            // No.
            
            //So, the disk is not ready.
            res = RES_NOTRDY ;
        }
    }
    else
    {
        // No.
        // It's the wrong disk.
        // Return Error.
        res = RES_ERROR ;
    }
    
    return( res ) ;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    DRESULT res ;
    fpos_t position ;

    // Is the correct driver?
    if( pdrv == 0 )
    {
        // Is the file open?
        if( fileDisk != ( ( FILE * ) NULL ) )
        {
            // Yes.
            
            // So, set the position.
            position = ( fpos_t ) ( sector * FILEDISK_SECTOR_SIZE ) ;
            ( void ) fsetpos( fileDisk , &position ) ;
            
            // Read 
            ( void ) fwrite( buff , sizeof( BYTE ) , count * FILEDISK_SECTOR_SIZE , fileDisk ) ;
            
            res = RES_OK ;
        }
        else
        {
            // No.
            
            //So, the disk is not ready.
            res = RES_NOTRDY ;
        }
    }
    else
    {
        // No.
        // It's the wrong disk.
        // Return Error.
        res = RES_ERROR ;
    }
    
    return( res ) ;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res ;
    DSTATUS diskStatus_ret ;

    // Check the disk status.
    // If 'pdrv' is wrong, it'll return error.
    diskStatus_ret = disk_status( pdrv ) ;
    // Is the disk Ok?
    if( diskStatus_ret == 0 )
    {
        // Yes.

        // Processes received command.
        switch( cmd )
        {
            // Sync buffer.
            case CTRL_SYNC :
                res = RES_OK ;
                break ;

            // Return the number of sectors of the disk.
            case GET_SECTOR_COUNT :
                *( ( unsigned long int * ) buff ) = FILEDISK_NUMBER_OF_SECTORS ;
                res = RES_OK ;
                break ;

            // Return the size of the sector.
            case GET_SECTOR_SIZE :
                *( ( unsigned long int * ) buff ) = FILEDISK_SECTOR_SIZE ;
                res = RES_OK ;
                break ;

            // Return the size of the erasable block.
            case GET_BLOCK_SIZE :
                *( ( unsigned long int * ) buff ) = 1 ;
                res = RES_OK ;
                break ;

            // Unknown command.
            default :
                res = RES_PARERR ;
                break ;
        }
    }
    else
    {
        // No.
        
        // So, return error.
        res = RES_ERROR ;
    }
    
    return( res ) ;
}
