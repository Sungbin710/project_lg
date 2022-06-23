## 인터페이스 명세

<!-- ### 시스템 연계
-   Socket: 서버는 통신을 위한 소켓을 생성하여 포트를 할당하고 클라이언트의 통신 요청 시 클라이언트와 연결하여 통신하는 네트워크 기술이다. -->


1. `IF01`: 사용자 ID 발급
-  `IF01-1`: 클라이언트가 사용자 정보를 서버로 전송
   - cmd [0x00]
   - 데이터 규격
```c
char cmd;
typedef struct UserInfo{
    int id;
    char name[16];
    unsigned int age;
    unsigned int sex;
} UserInfo;
```

-   `IF01-2`: 사용자가 사용할 ID를 서버로부터 수신   
    - cmd [0x00]
    - 데이터 규격
```c
char cmd;
int id;
```

2. `IF02`: 로그인
-   `IF02-1`: 클라이언트가 사용자 ID를 서버로 전송
    -   cmd: [0x01]
    -   데이터 규격
```c
char cmd;
int id;
```


-   `IF02-2`: 사용자 로그인 승인 여부를 서버로부터 수신
    - cmd [0x01]
    - 데이터 규격
    - is_success : 0 -> 로그인 성공
    - is_success : 1 -> 로그인 실패
```c
char cmd;
int is_success;
```


3.  `IF03`: 미디어 재생
-   `IF03-1`: 클라이언트가 사용자 ID와 미디어 정보를 서버로 전송
    -  cmd: [0x02]
    -  데이터 규격
```c
char cmd;
typedef struct MediaInfo{
    int id;
    char file_name[256];
    long file_size;
    gint64 played;
} MediaInfo;
```


-   `IF03-2`: 해당 미디어의 이전 재생 기록 정보를 서버로부터 수신
    -   cmd: [0x02]
    -   데이터 규격
```c
char cmd;
gint64 played;
```


-   `IF03-3`: 미디어 재생이 종료될 시, 마지막 재생 기록 정보를 서버로 전송
    -   cmd: [0x03]
    -   데이터 규격

```c
char cmd;
typedef struct MediaInfo{
    int id;
    char file_name[256];
    long file_size;
    gint64 played;
} MediaInfo;
```

4.  `IF04`: 사용자 정보 출력
-   `IF04-1`: 클라이언트가 사용자 ID를 서버로 전송
    -   cmd: [0x04]
    -   데이터 규격
```c
char cmd;
int id;
```


-   `IF04-2`: 사용자의 정보를 서버로부터 수신
    -   cmd [0x04]
    -   데이터 규격
```c
char cmd;
typedef struct UserInfo{
    int id;
    char name[16];
    unsigned int age;
    unsigned int sex;
} UserInfo;
```

5.  `IF05`: 최근 시청한 미디어 컨텐츠 목록 출력
-   `IF05-1`: 클라이언트가 사용자 ID 및 원하는 목록 개수를 서버로 전송
    -   cmd: [0x05]
    -   데이터 규격
```c
char cmd;
int id;
int size;
```
-   `IF05-2`: 최근 시청한 미디어 컨텐츠 목록을 서버로부터 수신
    -   cmd [0x05]
    -   데이터 규격
```c
char cmd;
typedef struct RecentList{
  int size;
  char file_list[5][256];
} RecentList;
```

1.  `IF06`: 서버에 저장된 인기 미디어 컨텐츠 목록 출력
-   `IF06-1`: 클라이언트가 원하는 성별 및 목록 개수를 서버로 전송
    -   cmd: [0x06]
    -   데이터 규격
```c
char cmd;
int sex;
int size;
```
-   `IF06-2`: 인기 미디어 컨텐츠 목록을 서버로부터 수신
    -   cmd [0x06]
    -   데이터 규격
```c
char cmd;
typedef struct RecoList{
  unsigned int sex;
  int size;
  char file_list[5][256];
} RecoList;
```



