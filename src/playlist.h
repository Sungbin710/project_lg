#define PLAYLIST_SIZE   100
#define FILE_NAME_LEN   256
#define MEDIA_FOLDER    "/videos/"
#define URI_PROTO       "file://"

static int print_playlist(char **playlist, int *list_len);
int playback_media_choice(char *file_path, int path_size, char *file_name, int name_size);
int print_media_info_choice(char *file_path, int path_size);
