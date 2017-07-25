#include <stdio.h>  // To use printf().
#include <stdlib.h> // To use malloc() and free().
#include <string.h> // To use strcpy() and memset().

#include "ff.h" // ForkFS.

#define VERSION_NUMBER                           ( 22 )
#define FRESULT_POSITION                         ( 62 )
#define BUFFER_SIZE                              ( 1024 )

void print_FRESULT( const char * str , FRESULT in ) ;
unsigned int f_forward_Check( const uint8_t * p , unsigned int btf ) ;

/* Tests:

f_open() .............. OK
f_close() ............. OK
f_read() .............. OK
f_write() ............. OK
f_lseek() ............. OK
f_truncate() .......... OK
f_sync() .............. OK
f_opendir() ........... OK
f_closedir() .......... OK
f_readdir() ........... OK
f_findfirst() ......... OK
f_findnext() .......... OK
f_mkdir() ............. OK
f_unlink() ............ OK
f_rename() ............ OK
f_stat() .............. OK
f_chmod() ............. OK
f_utime() ............. OK
f_chdir() ............. OK
f_chdrive() ........... OK
f_getcwd() ............ OK
f_getfree() ........... OK
f_getlabel() .......... OK
f_setlabel() .......... OK
f_forward() ........... OK
f_expand() ............ OK
f_mount() ............. OK
f_mkfs() .............. OK
f_fdisk() ............. OK
f_setcp() ............. OK
f_putc() .............. OK
f_puts()
f_printf() ............ OK
f_gets()
*/

/* Volume management table defined by user (required when _MULTI_PARTITION == 1) */
PARTITION VolToPart[] =
{
    { 0 , 1 } ,   /* "0:" ==> Physical drive 0, 1st partition */
    { 0 , 2 } ,   /* "1:" ==> Physical drive 0, 2nd partition */
    { 1 , 0 }     /* "2:" ==> Physical drive 1, Autodetect */
};
int main( int argc , char * argv[] )
{
    unsigned char workBuffer[ FF_MAX_SS ] ;
    unsigned char buffer[ BUFFER_SIZE ] ;
    FRESULT ffRet ;
    FATFS fatFs[ FF_VOLUMES ] ;
    FIL file ;
    DIR dir ;
    FILINFO fileInfo ;
    unsigned int bw , bwt ;
    int i , stdRet ;

    FATFS * fatFsPointer ;
    uint32_t freeClust ;
    
    printf( "ForkFS - Sanity Test Software - Version: %03d\n" , VERSION_NUMBER ) ;

    uint32_t plist0[] = { 50  , 50 , 0 , 0 } ;  // Divide the drive into two partitions.
    uint32_t plist1[] = { 100 ,  0 , 0 , 0 } ;  // Divide the drive into two partitions.

    ffRet = f_setcp( 850 ) ;
    print_FRESULT( "f_setcp(850)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    
    ffRet = f_fdisk( 0 , plist0 , workBuffer ) ;
    print_FRESULT( "f_fdisk(0,plist0,workBuffer)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_fdisk( 1 , plist1 , workBuffer ) ;
    print_FRESULT( "f_fdisk(1,plist1,workBuffer)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_mkfs( "0:" , FM_ANY , 0 , workBuffer , sizeof( workBuffer ) ) ;
    print_FRESULT( "f_mkfs(\"0:\",FM_ANY,0,workBuffer,sizeof(workBuffer))" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_mkfs( "1:" , FM_ANY , 0 , workBuffer , sizeof( workBuffer ) ) ;
    print_FRESULT( "f_mkfs(\"1:\",FM_ANY,0,workBuffer,sizeof(workBuffer))" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_mkfs( "2:" , FM_ANY , 0 , workBuffer , sizeof( workBuffer ) ) ;
    print_FRESULT( "f_mkfs(\"2:\",FM_ANY,0,workBuffer,sizeof(workBuffer))" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_mount( &fatFs[ 0 ] , "0:" , 0 ) ;
    print_FRESULT( "f_mount(&fatFs[0],\"0:\",0)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_mount( &fatFs[ 1 ] , "1:" , 0 ) ;
    print_FRESULT( "f_mount(&fatFs[1],\"1:\",0)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_mount( &fatFs[ 2 ] , "2:" , 0 ) ;
    print_FRESULT( "f_mount(&fatFs[2],\"2:\",0)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_setlabel( "0:DiskTest" ) ;
    print_FRESULT( "f_setlabel(\"0:DiskTest\")" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_getlabel( "0:" , ( TCHAR * ) buffer , 0 ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    if( strcmp( ( const char * ) buffer , "DISKTEST" ) != 0 )
    {
        printf( "\t\tWrong Label \"%s\"\n" , buffer ) ;
        return( -1 ) ;
    }

    ffRet = f_open( &file, "file.bin" , FA_WRITE | FA_CREATE_ALWAYS ) ;
    print_FRESULT( "f_open(&file,\"file.bin\",FA_WRITE|FA_CREATE_ALWAYS)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ( void ) memset( buffer , 0x5A , sizeof( buffer ) ) ;
    ffRet = f_write( &file , ( const void * ) buffer , sizeof( buffer ) , &bw );
    print_FRESULT( "f_write(&file,(const void *)buffer,sizeof(buffer),&bw)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( bw != sizeof( buffer ) )
    {
        printf( "\t\tbw -> %d != %d\n" , bw , sizeof( buffer ) ) ;
        return( -1 ) ;
    }

    ffRet = f_lseek( &file , ( FSIZE_t ) ( sizeof( buffer ) / 2 ) ) ;
    print_FRESULT( "f_lseek(&file,(FSIZE_t)(sizeof(buffer)/2))" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    ( void ) memset( buffer , 0x3C , ( sizeof( buffer ) / 2 ) ) ;
    ffRet = f_write( &file , ( const void * ) buffer , ( sizeof( buffer ) / 2 ) , &bw );
    print_FRESULT( "f_write(&file,(const void *)buffer,sizeof(buffer),&bw)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( bw != ( sizeof( buffer ) / 2 ) )
    {
        printf( "\t\tbw -> %d != %d\n" , bw , sizeof( buffer ) ) ;
        return( -1 ) ;
    }

    ffRet = f_close( &file );
    print_FRESULT( "f_close(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_open( &file , "file.bin" , FA_READ ) ;
    print_FRESULT( "f_open(&file,\"file.bin\",FA_READ)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ( void ) memset( buffer , 0x00 , sizeof( buffer ) ) ;
    ffRet = f_read( &file , ( void * ) buffer , sizeof( buffer ) , &bw ) ;
    print_FRESULT( "f_read(&file,(void *)buffer,sizeof(buffer),&bw)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( bw != sizeof( buffer ) )
    {
        printf( "\t\tbw -> %d != %d\n" , bw , sizeof( buffer ) ) ;
        return( -1 ) ;
    }

    if( f_size( &file ) != sizeof( buffer ) )
    {
        printf( "\t\tf_size() -> %d != %d\n" , ( int ) f_size( &file ) , sizeof( buffer ) ) ;
        return( -1 ) ;
    }

    ffRet = f_close( &file );
    print_FRESULT( "f_close(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_mkdir( "/tes*dir" ) ;
    if( ffRet != FR_OK )
    {
        printf( "\tf_mkdir( \"/tes*dir\" )                                        OK\n" ) ;
    }
    else
    {
        printf( "\t\tffRet -> %d\n" , ffRet ) ;
        return( -1 ) ;
    }

    ffRet = f_mkdir( "/testdir" ) ;
    print_FRESULT( "f_mkdir(\"/testdir\")" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_rename( "file.bin" , "/testdir/data.mem" ) ;
    print_FRESULT( "f_rename(\"file.bin\",\"/testdir/data.mem\")" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_open( &file, "/testdir/data.mem" , FA_WRITE ) ;
    print_FRESULT( "f_open(&file,\"/testdir/data.mem\",FA_WRITE)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    if( f_size( &file ) != sizeof( buffer ) )
    {
        printf( "\t\tf_size() -> %d != %d\n" , ( int ) f_size( &file ) , sizeof( buffer ) ) ;
        return( -1 ) ;
    }

    ffRet = f_truncate( &file );
    print_FRESULT( "f_truncate(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    if( f_size( &file ) != 0 )
    {
        printf( "\t\tf_size() -> %d != %d\n" , ( int ) f_size( &file ) , 0 ) ;
        return( -1 ) ;
    }

    ffRet = f_expand( &file , 1048576 , 1 ) ;
    print_FRESULT( "f_expand(&file,1048576,1)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    if( f_size( &file ) != 1048576 )
    {
        printf( "\t\tf_size() -> %d != %d\n" , ( int ) f_size( &file ) , 1048576 ) ;
        return( -1 ) ;
    }

    ffRet = f_close( &file );
    print_FRESULT( "f_close(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_unlink( "/testdir/data.mem" ) ;
    print_FRESULT( "f_unlink(\"/testdir/data.mem\")" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_chdir( "/testdir" ) ;
    print_FRESULT( "f_chdir(\"/testdir\")" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ( void ) memset( buffer , '\0' , sizeof( buffer ) ) ;
    ffRet = f_getcwd( ( TCHAR * ) buffer , sizeof( buffer ) ) ;
    print_FRESULT( "f_getcwd((TCHAR*)buffer,sizeof(buffer))" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( strcmp( ( const char * ) buffer , "0:/testdir" ) != 0 )
    {
        printf( "\t\tWrong Directory \"%s\"\n" , buffer ) ;
        return( -1 ) ;
    }

    
    ffRet = f_open( &file, "file_แ.dat" , FA_WRITE | FA_CREATE_ALWAYS ) ;
    print_FRESULT( "f_open(&file,\"file_แ.dat\",FA_WRITE|FA_CREATE_ALWAYS)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ( void ) memset( buffer , 0x5A , sizeof( buffer ) ) ;
    ffRet = f_write( &file , ( const void * ) buffer , sizeof( buffer ) , &bw );
    print_FRESULT( "f_write(&file,(const void *)buffer,sizeof(buffer),&bw)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( bw != sizeof( buffer ) )
    {
        printf( "\t\tbw -> %d != %d\n" , bw , sizeof( buffer ) ) ;
        return( -1 ) ;
    }

    ffRet = f_close( &file );
    print_FRESULT( "f_close(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_chdir( ".." ) ;
    print_FRESULT( "f_chdir(\"..\")" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_open( &file , "/testdir/file_แ.dat" , FA_READ ) ;
    print_FRESULT( "f_open(&file,\"/testdir/file_รก.dat\",FA_READ)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ( void ) memset( buffer , 0x00 , sizeof( buffer ) ) ;
    ffRet = f_read( &file , ( void * ) buffer , sizeof( buffer ) , &bw ) ;
    print_FRESULT( "f_read(&file,(void *)buffer,sizeof(buffer),&bw)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( bw != sizeof( buffer ) )
    {
        printf( "\t\tbw -> %d != %d\n" , bw , sizeof( buffer ) ) ;
        return( -1 ) ;
    }

    ffRet = f_sync( &file ) ;
    print_FRESULT( "f_sync(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    if( f_size( &file ) != sizeof( buffer ) )
    {
        printf( "\t\tf_size() -> %d != %d\n" , ( int ) f_size( &file ) , sizeof( buffer ) ) ;
        return( -1 ) ;
    }

    ffRet = f_close( &file );
    print_FRESULT( "f_close(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_opendir( &dir , "/testdir" ) ;
    print_FRESULT( "f_opendir(&dir,\"/testdir\")" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    for( i = 0 ; ; i++ )
    {
        ffRet = f_readdir( &dir , &fileInfo ) ;
        print_FRESULT( "f_readdir(&dir,&fileInfo)" , ffRet ) ;
        if( ffRet != FR_OK )
        {
            return( -1 ) ; 
        }
        
        if(  fileInfo.fname[ 0 ] == 0 )
            break ;
        
        if( strcmp( "file_แ.dat" , fileInfo.fname ) )
        {
            printf( "\t\tf_readdir() -> \"%s\" != \"%s\"\n" , fileInfo.fname , "file_แ.dat" ) ;
            return( -1 ) ;
        }
        
        if( fileInfo.fsize != sizeof( buffer ) )
        {
            printf( "\t\tf_readdir() -> %d != %d\n" , ( int ) fileInfo.fsize , sizeof( buffer ) ) ;
            return( -1 ) ;
        }
    }

    ffRet = f_closedir( &dir );
    print_FRESULT( "f_closedir(&dir)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_stat( "/testdir/file_แ.dat" , &fileInfo ) ;
    print_FRESULT( "f_stat(\"/testdir/file_แ.dat\",&fileInfo)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( fileInfo.ftime != 0x0000 )
    {
        printf( "\t\tf_stat() ftime -> %04Xh != %04Xh\n" , ( unsigned short int ) fileInfo.ftime , 0x0000 ) ;
        return( -1 ) ;
    }
    if( fileInfo.fdate != 0x4A21 )
    {
        printf( "\t\tf_stat() fdate -> %04Xh != %04Xh\n" , ( unsigned short int ) fileInfo.fdate , 0x4A21 ) ;
        return( -1 ) ;
    }
    if( fileInfo.fattrib != 0x20 )
    {
        printf( "\t\tf_stat() fattrib -> %02Xh != %02Xh\n" , ( unsigned char ) fileInfo.fattrib , 0x20 ) ;
        return( -1 ) ;
    }

    ffRet = f_chmod( "/testdir/file_แ.dat" , AM_RDO | AM_ARC , AM_RDO | AM_ARC ) ;
    print_FRESULT( "f_chmod(\"/testdir/file_แ.dat\",AM_RDO|AM_ARC,AM_RDO|AM_ARC)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    // Set date and time to 2017-05-03 20:02:44
    fileInfo.fdate = ( uint16_t )( ( ( 2017 - 1980 ) * 512U ) | 5 * 32U | 3 ) ;
    fileInfo.ftime = ( uint16_t )( 20 * 2048U | 2 * 32U | 44 / 2U ) ;
    ffRet = f_utime( "/testdir/file_แ.dat" , &fileInfo ) ;
    print_FRESULT( "f_utime(\"/testdir/file_แ.dat\",&fileInfo)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_stat( "/testdir/file_แ.dat" , &fileInfo ) ;
    print_FRESULT( "f_stat(\"/testdir/file.dat\",&fileInfo)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( fileInfo.ftime != 0xA056 )
    {
        printf( "\t\tf_stat() ftime -> %04Xh != %04Xh\n" , ( unsigned short int ) fileInfo.ftime , 0xA056 ) ;
        return( -1 ) ;
    }
    if( fileInfo.fdate != 0x4AA3 )
    {
        printf( "\t\tf_stat() fdate -> %04Xh != %04Xh\n" , ( unsigned short int ) fileInfo.fdate , 0x4AA3 ) ;
        return( -1 ) ;
    }
    if( fileInfo.fattrib != 0x21 )
    {
        printf( "\t\tf_stat() fattrib -> %02Xh != %02Xh\n" , ( unsigned char ) fileInfo.fattrib , 0x21 ) ;
        return( -1 ) ;
    }

    ffRet = f_getfree( "0:" , &freeClust , &fatFsPointer ) ;
    print_FRESULT( "f_getfree(\"0:\",&freeClust,&fatFsPointer)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    if( ( freeClust * fatFsPointer->csize ) != 15996 )
    {
        printf( "\t\tf_getfree() -> %lu != %lu\n" , ( unsigned long int ) ( freeClust * fatFsPointer->csize ) , ( unsigned long int ) 15996 ) ;
        return( -1 ) ;
    }

    ffRet = f_open( &file, "1:/file.hex" , FA_WRITE | FA_CREATE_ALWAYS ) ;
    print_FRESULT( "f_open(&file,\"1:/file.hex\",FA_WRITE|FA_CREATE_ALWAYS)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ( void ) memset( buffer , 0x5A , sizeof( buffer ) ) ;
    ffRet = f_write( &file , ( const void * ) buffer , sizeof( buffer ) , &bw );
    print_FRESULT( "f_write(&file,(const void *)buffer,sizeof(buffer),&bw)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( bw != sizeof( buffer ) )
    {
        printf( "\t\tbw -> %d != %d\n" , bw , sizeof( buffer ) ) ;
        return( -1 ) ;
    }
 
    ffRet = f_close( &file );
    print_FRESULT( "f_close(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_chdrive( "1:" ) ;
    print_FRESULT( "f_chdrive(\"1:\")" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_open( &file , "file.hex" , FA_READ ) ;
    print_FRESULT( "f_open(&file,\"file.hex\",FA_READ)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ( void ) memset( buffer , 0x00 , sizeof( buffer ) ) ;
    ffRet = f_read( &file , ( void * ) buffer , sizeof( buffer ) , &bw ) ;
    print_FRESULT( "f_read(&file,(void *)buffer,sizeof(buffer),&bw)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( bw != sizeof( buffer ) )
    {
        printf( "\t\tbw -> %d != %d\n" , bw , sizeof( buffer ) ) ;
        return( -1 ) ;
    }

    if( f_size( &file ) != sizeof( buffer ) )
    {
        printf( "\t\tf_size() -> %d != %d\n" , ( int ) f_size( &file ) , sizeof( buffer ) ) ;
        return( -1 ) ;
    }
    
    ffRet = f_close( &file );
    print_FRESULT( "f_close(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_findfirst( &dir , &fileInfo , "0:/testdir" , "*.dat" ) ;
    print_FRESULT( "f_findfirst(&dir,&fileInfo,\"0:/testdir\",\"*.dat\")" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    if( strcmp( "file_แ.dat" , fileInfo.fname ) )
    {
        printf( "\t\tf_findfirst() -> \"%s\" != \"%s\"\n" , fileInfo.fname , "file_แ.dat" ) ;
        return( -1 ) ;
    }

    ffRet = f_findnext( &dir , &fileInfo ) ;
    print_FRESULT( "f_findnext(&dir,&fileInfo\")" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    if( fileInfo.fname[ 0 ] )
    {
        printf( "\t\tf_findnext() -> \"%s\" != NULL\n" , fileInfo.fname ) ;
        return( -1 ) ;
    }

    ffRet = f_closedir( &dir );
    print_FRESULT( "f_closedir(&dir)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_chdrive( "0:" ) ;
    print_FRESULT( "f_chdrive(\"0:\")" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_open( &file , "/testdir/file_แ.dat" , FA_READ ) ;
    print_FRESULT( "f_open(&file,\"/testdir/file_แ.dat\",FA_READ)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    bwt = 0 ;
    do
    {
        ffRet = f_forward( &file , f_forward_Check , sizeof( buffer ) , &bw ) ;
        print_FRESULT( "f_forward(&file,out_stream,sizeof(buffer),&bw)" , ffRet ) ;
        if( ffRet != FR_OK )
        {
            return( -1 ) ; 
        }

        if( bw != sizeof( buffer ) )
        {
            printf( "\t\tbw -> %d != %d\n" , bw , sizeof( buffer ) ) ;
            return( -1 ) ;
        }

        bwt += bw ;
    } while( bwt != sizeof( buffer ) ) ;

    ffRet = f_close( &file );
    print_FRESULT( "f_close(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_chmod( "/testdir/file_แ.dat" , AM_ARC , AM_RDO | AM_ARC ) ;
    print_FRESULT( "f_chmod(\"/testdir/file_แ.dat\",AM_ARC,AM_RDO|AM_ARC)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_unlink( "/testdir/file_แ.dat" ) ;
    print_FRESULT( "f_unlink(\"/testdir/file_แ.dat\")" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_open( &file , "/testdir/fi*e.dat" , FA_WRITE | FA_READ | FA_CREATE_ALWAYS ) ;
    if( ffRet != FR_OK )
    {
        printf( "\tf_open(&file,\"/testdir/fi*e.dat\",FA_WRITE|FA_CREATE_ALWAYS)  OK\n" ) ;
    }
    else
    {
        printf( "\t\tffRet -> %d\n" , ffRet ) ;
        return( -1 ) ;
    }

    ffRet = f_open( &file , "/testdir/file.dat" , FA_WRITE | FA_READ | FA_CREATE_ALWAYS ) ;
    print_FRESULT( "f_open(&file,\"/testdir/file.dat\",FA_WRITE|FA_CREATE_ALWAYS)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    
    stdRet = f_printf( &file , "%08X" , 0x12345678 ) ;
    if( stdRet == 8 )
    {
        printf( "\tf_printf(&file,\"%%08X\",0x12345678)                            OK\n" ) ;
    }
    else
    {
        printf( "\t\tstdRet -> %d != %d\n" , stdRet , 8 ) ;
        return( -1 ) ;
    }

    stdRet = f_putc( 'X' , &file ) ;
    if( stdRet == 1 )
    {
        printf( "\tf_putc('X',&file)                                            OK\n" ) ;
    }
    else
    {
        printf( "\t\tstdRet -> %d != %d\n" , stdRet , 1 ) ;
        return( -1 ) ;
    }

    
    ffRet = f_lseek( &file , ( FSIZE_t ) ( 0 ) ) ;
    print_FRESULT( "f_lseek(&file,(FSIZE_t)(0))" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    
    ( void ) memset( buffer , '\0' , sizeof( buffer ) ) ;
    ffRet = f_read( &file , ( void * ) buffer , 9 , &bw ) ;
    print_FRESULT( "f_read(&file,(void *)buffer,sizeof(buffer),&bw)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( bw != 9 )
    {
        printf( "\t\tbw -> %d != %d\n" , bw , 9 ) ;
        return( -1 ) ;
    }
    
    if( strcmp( ( const char * ) buffer , "12345678X" ) != 0 )
    {
        printf( "\t\tWrong data \"%s\"\n" , buffer ) ;
        return( -1 ) ;
    }
    
    ffRet = f_close( &file );
    print_FRESULT( "f_close(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_open( &file, "2:/file.bin" , FA_WRITE | FA_CREATE_ALWAYS ) ;
    print_FRESULT( "f_open(&file,\"2:/file.bin\",FA_WRITE|FA_CREATE_ALWAYS)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ( void ) memset( buffer , 0x5A , sizeof( buffer ) ) ;
    ffRet = f_write( &file , ( const void * ) buffer , sizeof( buffer ) , &bw );
    print_FRESULT( "f_write(&file,(const void *)buffer,sizeof(buffer),&bw)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( bw != sizeof( buffer ) )
    {
        printf( "\t\tbw -> %d != %d\n" , bw , sizeof( buffer ) ) ;
        return( -1 ) ;
    }
 
    ffRet = f_close( &file );
    print_FRESULT( "f_close(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_open( &file, "2:/file with a long name to test LFN.bin" , FA_WRITE | FA_CREATE_ALWAYS ) ;
    print_FRESULT( "f_open(&file,\"2:/file ...bin\",FA_WRITE|FA_CREATE_ALWAYS)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_close( &file );
    print_FRESULT( "f_close(&file)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    
    printf( "\n\tDone!\n" ) ;
    
    return( 0 ) ;
}

void print_FRESULT( const char * str , FRESULT in )
{
    int numPrinted ;

    numPrinted = printf( "\t%s" , str ) ;

    numPrinted = ( numPrinted < FRESULT_POSITION ) ? ( FRESULT_POSITION - numPrinted ) : ( 0 ) ;
    while( numPrinted-- )
    {
        ( void ) putchar( ' ' ) ;
    }

    switch( in )
    {
        case FR_OK :
            printf( "FR_OK\n" ) ;
            break ;

        case FR_DISK_ERR :
            printf( "FR_DISK_ERR\n" ) ;
            break ;

        case FR_INT_ERR :
            printf( "FR_INT_ERR\n" ) ;
            break ;

        case FR_NOT_READY :
            printf( "FR_NOT_READY\n" ) ;
            break ;

        case FR_NO_FILE :
            printf( "FR_NO_FILE\n" ) ;
            break ;

        case FR_NO_PATH :
            printf( "FR_NO_PATH\n" ) ;
            break ;

        case FR_INVALID_NAME :
            printf( "FR_INVALID_NAME\n" ) ;
            break ;

        case FR_DENIED :
            printf( "FR_DENIED\n" ) ;
            break ;

        case FR_EXIST :
            printf( "FR_EXIST\n" ) ;
            break ;

        case FR_INVALID_OBJECT :
            printf( "FR_INVALID_OBJECT\n" ) ;
            break ;

        case FR_WRITE_PROTECTED :
            printf( "FR_WRITE_PROTECTED\n" ) ;
            break ;

        case FR_INVALID_DRIVE :
            printf( "FR_INVALID_DRIVE\n" ) ;
            break ;

        case FR_NOT_ENABLED :
            printf( "FR_NOT_ENABLED\n" ) ;
            break ;

        case FR_NO_FILESYSTEM :
            printf( "FR_NO_FILESYSTEM\n" ) ;
            break ;

        case FR_MKFS_ABORTED :
            printf( "FR_MKFS_ABORTED\n" ) ;
            break ;

        case FR_TIMEOUT :
            printf( "FR_TIMEOUT\n" ) ;
            break ;

        case FR_LOCKED :
            printf( "FR_LOCKED\n" ) ;
            break ;

        case FR_NOT_ENOUGH_CORE :
            printf( "FR_NOT_ENOUGH_CORE\n" ) ;
            break ;

        case FR_TOO_MANY_OPEN_FILES :
            printf( "FR_TOO_MANY_OPEN_FILES\n" ) ;
            break ;

        case FR_INVALID_PARAMETER :
            printf( "FR_INVALID_PARAMETER\n" ) ;
            break ;

        default :
            printf( "Unknown\n" ) ;
            break ;
    }
}

unsigned int f_forward_Check( const uint8_t * p , unsigned int btf )
{
    unsigned int i ;

    // Check Sense Call.
    if( btf == 0 )
    {
        return( 1 ) ;
    }

    if( btf > BUFFER_SIZE )
    {
        printf( "\t\tbtf -> %d > %d\n" , btf , BUFFER_SIZE ) ;
        return( 0 ) ;
    }

    for( i = 0 ; i < btf ; i++ )
    {
        if( p[ i ] != 0x5A )
        {
            printf( "\t\tp[ %d ] -> %02Xh != %02Xh\n" , i , p[ i ] , 0x5A ) ;
            return( 0 ) ;
        }
    }

    return( btf ) ;
}
