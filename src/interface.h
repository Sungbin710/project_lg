// #ifndef _INTER_H
// #define _INTER_H

#define BUFFER_SIZE  2048

enum CMD{
    REGISTER = 0, /* 아이디 발급 요청 */
    LOGIN,        /* 로그인 승인 요청 */
    PLAY_MEDIA,   /* 미디어 기록 요청 */
    END_MEDIA,    /* 미디어 기록 갱신 */
    USER_INFO,    /* 사용자 정보 요청 */
    RECENT_LIST,  /* 최근 시청한 목록 */
    POPULAR_LIST  /* 성별 인기 순위 목록*/
};

typedef struct _Playback_Callbacks {
  
  gboolean (*get_start_pos) (gint64 *user_data, gint64 *receive_pos);
} Playback_Callbacks;


typedef struct _Account_Callbacks {

  gboolean (*get_popular_list) (char file[][256], int *list_size);
  gboolean (*get_userinfo) (char *name, unsigned int *age, unsigned int *sex);  
  gboolean (*allow_login) (int *is_success, int *user_data);
  gboolean (*get_unique_id) (int *id);
  gboolean (*get_recent_list) (char file[][256], int *list_size);
} Account_Callbacks;


typedef struct UserInfo{
    int id;
    char name[16];
    unsigned int age;
    unsigned int sex;
} UserInfo;

typedef struct MediaInfo{
    int id;
    char file_name[256];
    long file_size;
    gint64 played;
} MediaInfo;

typedef struct Filelist{
  int size;
  char file_list[5][256];
} Filelist;

typedef struct Recommed{
  unsigned int sex; //0 남자 1 여자 2 전체
  int size;
  char file_list[5][256];
} Recommed;

/* 콜백함수 등록 */
void account_install_callbacks (const Account_Callbacks * callbacks, void *user_data);
void playback_install_callbacks (const Playback_Callbacks * callbacks, void *user_data);

/* 서버로 전송 */

/* REGISTER */
int register_user_to_server(int sockfd, char name[16], unsigned int age, unsigned int sex);
/* LOGIN */
int login_to_server(int sockfd, int id);
/* PLAY_MEDIA */
int request_last_playback_to_server(int sockfd, int id, char *file_name, long file_size);
/* END_MEDIA */
int update_last_playback_to_server(int sockfd, int id, char *file_name, long file_size, signed long last_pos);
/* USER_INFO */
int request_userinfo_to_server(int sockfd, int id);
/* RECENT_LIST */
int request_recent_list_to_server(int sockfd, int id, int size);
/* POPULAR_LIST */
int request_popular_list_to_server(int sockfd, int sex, int size);

/* 서버로부터 수신 */

void *receive_from_server(void *arg);


// #endif /* _INTER_H */
