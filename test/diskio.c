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

#define FILEDISK0_NAME                           "filedisk0.dsk"
#define FILEDISK0_SIZE                           ( 16 * 1024 * 1024 ) // 16MBytes.
#define FILEDISK0_SECTOR_SIZE                    ( 512 ) // Sector Size = 512Bytes.
#define FILEDISK0_NUMBER_OF_SECTORS              ( FILEDISK0_SIZE / FILEDISK0_SECTOR_SIZE )

#define FILEDISK1_NAME                           "filedisk1.dsk"
#define FILEDISK1_SIZE                           ( 128 * 1024 * 1024 ) // 128MBytes.
#define FILEDISK1_SECTOR_SIZE                    ( 2048 ) // Sector Size = 2KBytes.
#define FILEDISK1_NUMBER_OF_SECTORS              ( FILEDISK1_SIZE / FILEDISK1_SECTOR_SIZE )

FILE * fileDisk0 = ( FILE * ) NULL ;
FILE * fileDisk1 = ( FILE * ) NULL ;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    uint8_t pdrv       /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
    
    // Is the correct driver?
    if( pdrv == 0 )
    {
        // Yes.
        
        // Is the file open?
        if( fileDisk0 != ( ( FILE * ) NULL ) )
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
    else if( pdrv == 1 )
    {
        // Yes.
        
        // Is the file open?
        if( fileDisk1 != ( ( FILE * ) NULL ) )
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
    uint8_t pdrv               /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat ;
    unsigned long int i ;
    
    // Is the correct driver?
    if( pdrv == 0 )
    {
        // Is the file open?
        if( fileDisk0 != ( ( FILE * ) NULL ) )
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
            fileDisk0 = fopen( FILEDISK0_NAME , "wb+" ) ;
            // Did the file open correctly?
            if( fileDisk0 != ( ( FILE * ) NULL ) )
            {
                // Yes.

                // So, fill the file with 00h.
                for( i = 0 ; i < FILEDISK0_SIZE ; i++ )
                {
                    ( void ) fputc( 0x00 , fileDisk0 ) ;
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
    else if( pdrv == 1 )
    {
        // Is the file open?
        if( fileDisk1 != ( ( FILE * ) NULL ) )
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
            fileDisk1 = fopen( FILEDISK1_NAME , "wb+" ) ;
            // Did the file open correctly?
            if( fileDisk1 != ( ( FILE * ) NULL ) )
            {
                // Yes.

                // So, fill the file with 00h.
                for( i = 0 ; i < FILEDISK1_SIZE ; i++ )
                {
                    ( void ) fputc( 0x00 , fileDisk1 ) ;
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
    uint8_t pdrv,      /* Physical drive nmuber to identify the drive */
    uint8_t *buff,     /* Data buffer to store read data */
    uint32_t sector,   /* Start sector in LBA */
    unsigned int count      /* Number of sectors to read */
)
{
    DRESULT res ;
    long position ;

    // Is the correct driver?
    if( pdrv == 0 )
    {
        // Is the file open?
        if( fileDisk0 != ( ( FILE * ) NULL ) )
        {
            // Yes.
            
            // So, set the position.
            position  = ( long ) ( sector               ) ;
            position *= ( long ) ( FILEDISK0_SECTOR_SIZE ) ;
            ( void ) fseek( fileDisk0 , position , SEEK_SET ) ;
            
            // Read 
            ( void ) fread( buff , sizeof( uint8_t ) , count * FILEDISK0_SECTOR_SIZE , fileDisk0 ) ;
            
            res = RES_OK ;
        }
        else
        {
            // No.
            
            //So, the disk is not ready.
            res = RES_NOTRDY ;
        }
    }
    else if( pdrv == 1 )
    {
        // Is the file open?
        if( fileDisk1 != ( ( FILE * ) NULL ) )
        {
            // Yes.
            
            // So, set the position.
            position  = ( long ) ( sector               ) ;
            position *= ( long ) ( FILEDISK1_SECTOR_SIZE ) ;
            ( void ) fseek( fileDisk1 , position , SEEK_SET ) ;
            
            // Read 
            ( void ) fread( buff , sizeof( uint8_t ) , count * FILEDISK1_SECTOR_SIZE , fileDisk1 ) ;
            
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
    uint8_t pdrv,          /* Physical drive nmuber to identify the drive */
    const uint8_t *buff,   /* Data to be written */
    uint32_t sector,       /* Start sector in LBA */
    unsigned int count          /* Number of sectors to write */
)
{
    DRESULT res ;
    long position ;

    // Is the correct driver?
    if( pdrv == 0 )
    {
        // Is the file open?
        if( fileDisk0 != ( ( FILE * ) NULL ) )
        {
            // Yes.
            
            // So, set the position.
            position  = ( long ) ( sector               ) ;
            position *= ( long ) ( FILEDISK0_SECTOR_SIZE ) ;
            ( void ) fseek( fileDisk0 , position , SEEK_SET ) ;
            
            // Read 
            ( void ) fwrite( buff , sizeof( uint8_t ) , count * FILEDISK0_SECTOR_SIZE , fileDisk0 ) ;
            
            res = RES_OK ;
        }
        else
        {
            // No.
            
            //So, the disk is not ready.
            res = RES_NOTRDY ;
        }
    }
    else if( pdrv == 1 )
    {
        // Is the file open?
        if( fileDisk1 != ( ( FILE * ) NULL ) )
        {
            // Yes.
            
            // So, set the position.
            position  = ( long ) ( sector               ) ;
            position *= ( long ) ( FILEDISK1_SECTOR_SIZE ) ;
            ( void ) fseek( fileDisk1 , position , SEEK_SET ) ;
            
            // Read 
            ( void ) fwrite( buff , sizeof( uint8_t ) , count * FILEDISK1_SECTOR_SIZE , fileDisk1 ) ;
            
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
    uint8_t pdrv,      /* Physical drive nmuber (0..) */
    uint8_t cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res ;
    DSTATUS diskStatus_ret ;
    int stdRet ;

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
                if( ( pdrv == 0 ) && ( fileDisk0 != ( ( FILE * ) NULL ) ) )
                {
                    stdRet = fflush( fileDisk0 ) ;
                    res = ( stdRet == 0 ) ? RES_OK : RES_ERROR ;
                }
                else if( ( pdrv == 1 ) && ( fileDisk1 != ( ( FILE * ) NULL ) ) )
                {
                    stdRet = fflush( fileDisk1 ) ;
                    res = ( stdRet == 0 ) ? RES_OK : RES_ERROR ;
                }
                else
                {
                    res = RES_ERROR ;
                }
                break ;

            // Return the number of sectors of the disk.
            case GET_SECTOR_COUNT :
                if( pdrv == 0 )
                {
                    *( ( unsigned long int * ) buff ) = FILEDISK0_NUMBER_OF_SECTORS ;
                    res = RES_OK ;
                }
                else if( pdrv == 1 )
                {
                    *( ( unsigned long int * ) buff ) = FILEDISK1_NUMBER_OF_SECTORS ;
                    res = RES_OK ;
                }
                break ;

            // Return the size of the sector.
            case GET_SECTOR_SIZE :
                if( pdrv == 0 )
                {
                    *( ( unsigned long int * ) buff ) = FILEDISK0_SECTOR_SIZE ;
                    res = RES_OK ;
                }
                else if( pdrv == 1 )
                {
                    *( ( unsigned long int * ) buff ) = FILEDISK1_SECTOR_SIZE ;
                    res = RES_OK ;
                }
                break ;

            // Return the size of the erasable block.
            case GET_BLOCK_SIZE :
                if( pdrv == 0 )
                {
                    *( ( unsigned long int * ) buff ) = 1 ;
                    res = RES_OK ;
                }
                else if( pdrv == 1 )
                {
                    *( ( unsigned long int * ) buff ) = 1 ;
                    res = RES_OK ;
                }
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
