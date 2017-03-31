#include <stdio.h>  // To use printf().
#include <stdlib.h> // To use malloc() and free().

#include "ff.h" // ForkFS.

#define VERSION_NUMBER                           ( 0 )

void print_FRESULT( const char * str , FRESULT in ) ;

int main( int argc , char * argv[] )
{
    unsigned char workBuffer[ _MAX_SS ] ;
    unsigned char buffer[ 1024 ] ;
    FRESULT ffRet ;
    FATFS fatFs ;
    FIL file ;
    UINT bw ;
    
    printf( "ForkFS - Test Software - Version: %03d\n" , VERSION_NUMBER ) ;
  
    ffRet = f_mkfs( "" , FM_ANY , 0 , workBuffer , sizeof( workBuffer ) ) ;
    print_FRESULT( "f_mkfs()" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }

    ffRet = f_mount( &fatFs , "0:" , 0 ) ;
    print_FRESULT( "f_mount()" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
  
    ffRet = f_open( &file, "file.bin" , FA_WRITE | FA_CREATE_ALWAYS ) ;
    print_FRESULT( "f_open()" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
    
    ( void ) memset( buffer , 0x5A , sizeof( buffer ) ) ;
    ffRet = f_write( &file , ( const void * ) buffer , sizeof( buffer ) , &bw );
    print_FRESULT( "f_write()" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
 
    ffRet = f_close( &file );
    print_FRESULT( "f_close()" , ffRet ) ;
    if( ffRet != FR_OK )
    {
        return( -1 ) ; 
    }
  
    return( 0 ) ;
}

void print_FRESULT( const char * str , FRESULT in )
{
    int numPrinted ;
    
    numPrinted = printf( "\t%s" , str ) ;
    
    numPrinted = ( numPrinted < 50 ) ? ( 50 - numPrinted ) : ( 0 ) ;
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
