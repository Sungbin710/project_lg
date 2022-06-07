typedef struct UserInfo {
unsigned int cmd; //
unsigned int user_id;
char user_name[16];
unsigned int user_age;
unsigned int user_sex;
}UserInfo;

// typedef struct SaveUserInfo{
// unsigned int cmd;
// UserInfo user; 
// }SaveUserInfo;

typedef struct UserID {
unsigned int cmd;
unsigned int user_id;
};


struct 미디어 컨텐츠 기록 요청, 저장 요청
{
unsigned int cmd;
unsigned int user_id;
char file_name[256];
unsigned int file_size;
signed long played; // 재생 기록 요청시 null
};

미디어 컨텐츠 재생 기록 제공
signed long played;

struct 메타데이터
{
char codec[64];
};

struct 최근 재생목록 제공
char file_name[256][5];
};