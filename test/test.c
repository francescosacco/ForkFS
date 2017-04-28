#include <stdio.h>  // To use printf().
#include <stdlib.h> // To use malloc() and free().

#include "ff.h" // ForkFS.

#define VERSION_NUMBER                           ( 6 )

#define FRESULT_POSITION                         ( 57 )

void print_FRESULT( const char * str , FRESULT in ) ;

/* Tests:
f_open() .............. OK
f_close() ............. OK
f_read() .............. OK
f_write() ............. OK
f_lseek()
f_truncate() .......... OK
f_sync() .............. OK
f_opendir() ........... OK
f_closedir() .......... OK
f_readdir() ........... OK
f_findfirst()
f_findnext()
f_mkdir() ............. OK
f_unlink() ............ OK
f_rename() ............ OK
f_stat() .............. OK
f_chmod()
f_utime()
f_chdir() ............. OK
f_chdrive()
f_getcwd() ............ OK
f_getfree() ........... OK
f_getlabel() .......... OK
f_setlabel() .......... OK
f_forward()
f_expand()
f_mount() ............. OK
f_mkfs() .............. OK
f_fdisk() ............. OK
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
    unsigned char workBuffer[ _MAX_SS ] ;
    unsigned char buffer[ 1024 ] ;
    FRESULT ffRet ;
    FATFS fatFs ;
    FIL file ;
    DIR dir ;
    FILINFO fileInfo ;
    UINT bw ;
    int i ;
    
    FATFS * fatFsPointer ;
    DWORD freeClust ;
    
    printf( "ForkFS - Sanity Test Software - Version: %03d\n" , VERSION_NUMBER ) ;
  
    DWORD plist[] = { 50 , 50 , 0 , 0 } ;  // Divide the drive into two partitions.
    
    ffRet = f_fdisk( 0 , plist , workBuffer ) ;
    print_FRESULT( "f_fdisk(0,plist,workBuffer)" , ffRet ) ;
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

    ffRet = f_mount( &fatFs , "0:" , 0 ) ;
    print_FRESULT( "f_mount( &fatFs , \"0:\" , 0 )" , ffRet ) ;
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

    if( strcmp( buffer , "DISKTEST" ) != 0 )
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

    memset( buffer , '\0' , sizeof( buffer ) ) ;
    ffRet = f_getcwd( ( TCHAR * ) buffer , sizeof( buffer ) ) ;
    print_FRESULT( "f_getcwd((TCHAR*)buffer,sizeof(buffer))" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    if( strcmp( buffer , "0:/testdir" ) != 0 )
    {
        printf( "\t\tWrong Directory \"%s\"\n" , buffer ) ;
        return( -1 ) ;
    }

    
    ffRet = f_open( &file, "file.dat" , FA_WRITE | FA_CREATE_ALWAYS ) ;
    print_FRESULT( "f_open(&file,\"file.dat\",FA_WRITE|FA_CREATE_ALWAYS)" , ffRet ) ;
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

    ffRet = f_open( &file , "/testdir/file.dat" , FA_READ ) ;
    print_FRESULT( "f_open(&file,\"/testdir/file.dat\",FA_READ)" , ffRet ) ;
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
        
        if( strcmp( "file.dat" , fileInfo.fname ) )
        {
            printf( "\t\tf_readdir() -> \"%s\" != \"%s\"\n" , fileInfo.fname , "file.dat" ) ;
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
    
    ffRet = f_stat( "/testdir/file.dat" , &fileInfo ) ;
    print_FRESULT( "f_stat(\"/testdir/file.dat\",&fileInfo)" , ffRet ) ;
    if( ffRet != FR_OK )
    {
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
        printf( "\t\tf_getfree() -> %u != %u\n" , ( unsigned long int ) ( freeClust * fatFsPointer->csize ) , 15996 ) ;
        return( -1 ) ;
    }
    
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
