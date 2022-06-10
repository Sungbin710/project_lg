#include <stdio.h>
#include <sys/stat.h>

int filesize( const char  *filename){

    struct  stat    file_info;
    int             sz_file;

    if ( 0 > stat( filename, &file_info)){
	    return -1; // file이 없거나 에러
    }
    return file_info.st_size;
}

int main(int argc, char *argv[])
{
    printf("%d", filesize(argv[1]));

    return 0;
}