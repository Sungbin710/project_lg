#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#include "playlist.h"

static int print_playlist(char *playlist[], int *list_len)
{

	DIR *dp;
	struct dirent *dent;
	char file_path[256];

	/* current path */
	getcwd(file_path, sizeof(file_path));
	/* change path to videos directory in current path */
	strcat(file_path, MEDIA_FOLDER);

	/* open directory */
	if ((dp = opendir(file_path)) == NULL) {
		perror("opendir: ");
		return 1;
	}

	/* list_len init */
	int len = 0;
	int media_name_len = 0;
	printf("========Media list========\n");
	while ((dent = readdir(dp))) {

		if (strcmp(dent->d_name, ".") == 0
		    || strcmp(dent->d_name, "..") == 0) {
			continue;
		}

		printf("%d ", len + 1);
		printf("%s\n", dent->d_name);

		media_name_len = strlen(dent->d_name) + 1;
		if (!
		    (playlist[len] =
		     (char *)malloc(sizeof(char) * media_name_len))) {
			perror("alloc: ");
			return 1;
		}
		strcpy(playlist[len], dent->d_name);

		len += 1;

	}
	printf("===================\n");

	*list_len = len;

	closedir(dp);
	return 0;

}

int
playback_media_choice(char *file_path, int path_size, char *file_name,
		      int name_size)
{

	char *playlist[PLAYLIST_SIZE];
	int list_len = 0;
	int media_num = 0;

	getcwd(file_path, path_size);
	strcat(file_path, MEDIA_FOLDER);

	if (print_playlist(playlist, &list_len) != 0) {	// SUCCESS
		printf("error occured\n");
		return 1;
	}

	//파일 이름 입력 받기
	printf("choice media num: ");
	scanf("%d", &media_num);

	if (!(media_num > 0 && media_num <= list_len)) {
		printf("incorrect number\n");
		return 1;
	}

	strcat(file_path, playlist[media_num - 1]);
	strcpy(file_name, playlist[media_num - 1]);

	for (int i = 0; i < list_len; ++i)
		free(playlist[i]);

	// printf("file_path : %s \n", file_path);
	// printf("file_name : %s \n", file_name);

	return 0;
}

int print_media_info_choice(char *file_path, int path_size)
{

	char *playlist[PLAYLIST_SIZE];
	int list_len = 0;
	int media_num = 0;

	getcwd(file_path, FILE_NAME_LEN);
	strcat(file_path, MEDIA_FOLDER);

	if (print_playlist(playlist, &list_len) != 0) {	// SUCCESS
		printf("error occured\n");
		return 1;
	}
	//파일 이름 입력 받기
	printf("choice media num: ");
	scanf("%d", &media_num);

	if (!(media_num > 0 && media_num <= list_len)) {
		printf("incorrect number\n");
		return 1;
	}

	strcat(file_path, playlist[media_num - 1]);

	for (int i = 0; i < list_len; ++i)
		free(playlist[i]);

	printf("file_path : %s \n", file_path);

	return 0;
}
