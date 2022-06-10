#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_playlist(){
	
	DIR *dp;
	struct dirent *dent;

    char path[1024];
	
	/* find realpath */
	if(realpath(".", path) == NULL){
	  perror("realpath error");
	  return -1;
	}
	strcat(path, "/videos/");

	if((dp=opendir(path)) == NULL){
		perror("opendir: ");
		exit(1);
	}		
	

	int list = 0;
	while((dent=readdir(dp))){
		// printf("%d ", (int)dent->d_ino);
		printf("%d ", ++list);
		printf("%s\n", dent->d_name);
		

	}
	if(list == 0){
		printf("재생할 수 있는 목록이 없습니다.\n");
	}	
	
	closedir(dp);
	return 0;

}

int main(){
	
	print_playlist();	

	return 0;
}

