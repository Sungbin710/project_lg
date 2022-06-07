<img src="./images/FR01.png" width="300" height="250" align="center"/>


<img src="./images/FR02.png" width="600" height="600" align="center"/>


title 인터페이스

group IF01 (사용자 등록)
Client->Server: IF01-1: CMD[1]/ID[1]/NAME[8]/AGE[1]/SEX[1]
Client<<--Server: IF01-2: success(0) or failure(1)

end


group IF02 (사용자 로그인)
Client->Server: IF02-1: ID[1]
Client<<--Server: IF02-2: success(0) or failure(1)


end

group IF03 (미디어 재생 요청)
Client->Server: IF03-1: CMD[1] / ID[1] / FILE_NAME[8] / FILE_SIZE[2]
Client<<--Server: IF03-2: LAST_PLAYBACK[2]
Client->Server: IF03-3: CMD[1] / ID[1] / FILE_NAME[8] / FILE_SIZE[2] / LAST_PLAYBACK[2]

end

group IF04 (사용자 정보 요청)
Client->Server: IF04-1: CMD[1] / ID[1]
Client<<--Server: IF04-2: NAME[8] / AGE[1] / SEX[1]


end