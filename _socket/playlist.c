#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

int print_playlist(char *playlist[], int *list_len){
	
	DIR *dp;
	struct dirent *dent;
	char path[1024];
	
	/* current path */
	getcwd(path,sizeof(path));
	/* change path to videos directory in current path */
	strcat(path, "/videos/");

	if((dp=opendir(path)) == NULL){
		perror("opendir: ");
		return 1;
		// exit(1);
	}		
	
	/* list_len init */
	*list_len = 0;
	int media_name_len;
	printf("========Media list========\n");
	while((dent=readdir(dp))){
		
		if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0){
			continue;
		}

		printf("%d ", ++(*list_len));
		printf("%s\n", dent->d_name);

		media_name_len = strlen(dent->d_name) + 1;
		if((playlist[*list_len] = (char *)malloc(sizeof(char) * media_name_len)) == NULL){
			perror("alloc: ");
			return 1;
		}

		strcpy(playlist[*list_len],dent->d_name);
	}
	printf("===================\n");
/*
	if(list == 0){
		printf("재생할 수 있는 목록이 없습니다.\n");
		closedir(dp);
		return 1;
	}

  printf("재생할 미디어 번호를 입력하세요.\n");
  scanf("%d", play_num);

  if(*play_num <= 0 || *play_num > list){
    printf("잘못된 번호를 입력하셨습니다.\n");
    return 1;
  }
*/	
	closedir(dp);
	return 0;

}
