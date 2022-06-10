#ifndef _INTER_H
#define _INTER_H

#define BUFFER_SIZE  512

enum CMD{
    REGISTER = 0,
    LOGIN,
    PLAY_MEDIA,
    END_MEDIA,
    USER_INFO
};



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


#endif /* _INTER_H */