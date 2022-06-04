typedef struct UserInfo{
    unsigned int id;
    char name[16];
    unsigned int age;
    unsigned int sex;
} UserInfo;


typedef struct MediaInfo{
    char file_name[256];
    unsigned int file_size;
    signed long played;
} MediaInfo;