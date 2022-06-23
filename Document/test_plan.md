# 미디어 재생 클라이언트 테스트 플랜
## 테스트 목적
-  미디어 컨텐츠 재생에 따라 발생하는 사용자 로그 및 미디어 데이터를 파악하고, 추출하여 서버로 전송하는 클라이언트 프로그램을 테스트하여 기능/품질 요구사항을 만족할 수 있도록 하고자 한다.

## 검증 범위 선정
요구사항 명세서의 기능/품질 요구사항을 기반으로 테스트 레벨과 검증범위를 선정한다.

<!-- ### 테스트 방법

1. 단위 테스트1
-  입력 포맷 검증 
-  파일 재생 여부 확인
-  미디어 재생 지원 확인
-  미디어 메타데이터 출력 확인
2. 단위 테스트2
-  서버와의 송, 수신이 포함된 기능
3. 통합 테스트 및 시스템 테스트
-  각각의 기능 테스트 및 시나리오 테스트 -->

## 요구사항 분석

1. `FR01-1` : 사용자 요청에 따라 사용자 정보를 입력받고, ID를 발급
   -  단위 테스트: 사용자 정보에 포함되는 이름, 나이, 성별에 잘못된 타입의 데이터가 들어가는 경우에 정상적인 입력이 들어올 때까지 재입력이 요청되는지 확인한다. 이름은 15글자까지 저장되며, 넘어가는 문자들은 삭제되도록 한다.
      -  Test cases: [사용자 정보 형식 검증 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#사용자-정보-형식-검증-테스트)
   -  단위 테스트(테스트 스텁): 더미서버를 생성해 소켓통신에서의 송/수신을 확인하고, 더미서버로부터 수신한 id값이 클라이언트에게 정상적으로 출력되는지 확인한다.
      -  Test cases: [아이디 수신 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#아이디-수신-테스트)
   -  통합 테스트: 입력된 사용자 정보의 형식이 검증되는지, 서버로 ID발급을 요청했을 때, 사용가능한 ID가 발급되는지 확인한다.
      -  Test cases: 단위 테스트와 동일 [아이디 발급 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#아이디-발급-테스트)

2. `FR02-1` : 사용자 요청에 따라 ID를 입력받고, 로그인 기능 수행
   -  단위 테스트: ID값이 양의 정수로 들어오는 지 확인하고, 양의 정수가 아닌 경우에 재입력을 요청한다.
      -  Test cases: [사용자 아이디 형식 검증 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#사용자-아이디-형식-검증-테스트)
   -  단위 테스트(테스트 스텁): 더미서버를 생성해 소켓통신에서의 송/수신을 확인하고, 더미서버로부터 정해진 id 등록여부를 수신하고, 등록되어 있지 않다면 로그인 실패를 출력하고, 등록되어 있다면 로그인 성공을 출력하고 로그인 이후 상태로 변경되는지 확인한다.
      -  Test cases: [로그인 승인여부 수신 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#로그인-승인여부-수신-테스트)
   -  통합 테스트 : 입력된 ID값의 형식이 검증되는지, 서버로 ID 등록여부를 요청했을 때, 서버로부터 온 응답에 맞게 사용자의 로그인 상태가 변경되는지 확인한다.
      - Test cases: 단위 테스트와 동일 [로그인 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#로그인-테스트)

3. `FR03-1` : 미디어 선택 및 확인
   -  단위 테스트: .mp4 파일을 입력하여 재생 가능한 파일이라고 출력되는지, .txt 파일을 입력하여 재생 불가능한 파일이라고 출력되는지 확인한다.
      -  Test cases: [미디어 파일 재생 검증 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#미디어-파일-재생-검증-테스트)

4. `FR03-2`, `QA01` : 미디어 이어보기 
   -  통합 테스트: 임의의 미디어를 재생하여 특정 위치에서 중단하고, 다시 미디어 재생을 요청했을 때, 중단되었던 위치에서 미디어가 시작되는 것을 확인한다. 
      -  Test cases: [미디어 이어보기 검증 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#미디어-이어보기-검증-테스트)
   -  품질 테스트: GStreamer Basic4 tutorial을 통해 사전에 확인

5. `FR03-3`, `QA02`: 미디어 재생 옵션 지원
   -  단위 테스트: 미디어 재생창이 생성되고, 재생/일시정지가 정상적으로 동작하는지 확인한다.
      -  Test cases: [미디어 컨텐츠 재생 제어 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#미디어-컨텐츠-재생-제어-테스트)
   -  품질 테스트: GStreamer Basic5 tutorial을 통해 사전에 확인

6. `FR04-1` : 사용자 정보 출력
   -  통합 테스트: 사용자 정보 출력이 요청되었을 때, 현재 로그인된 ID에 맞는 서버에 저장된 사용자 정보가 출력되는지 확인한다.
      -  Test cases: [사용자 정보 출력 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#사용자-정보-출력-테스트)

7. `FR05-1` : 최근 재생 목록 출력
   -  통합 테스트: 최근 재생 목록 출력이 요청되었을 때, 현재 로그인 된 ID에 맞는 서버에 저장된 사용자의 최근 재생 목록이 출력되는지 확인한다.
      -  Test cases: [최근 재생 목록 출력 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#최근-재생-목록-출력-테스트)

8. `FR06-1` : 인기 재생 목록 출력
   -  통합 테스트: 인기 재생 목록 출력이 요청되었을 때, 서버에 저장된 인기 재생 목록이 출력되는지 확인한다.
      -  Test cases:  Test cases: [인기 재생 목록 출력 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#인기-재생-목록-출력-테스트)

9.  `FR07-1` : 미디어 정보 출력
    -  단위 테스트: .mp4 파일을 입력하여 미디어 정보가 출력되는지, .txt파일을 입력하여 미디어 파일이 아니라고 출력되는지 확인한다.
       -  Test cases: [미디어 정보 출력 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#미디어-정보-출력-테스트)


10. `FR08-1` : 로그아웃
   -  통합테스트: 로그인 상태에서 로그아웃을 요청하면 로그인 이전 대기 상태로 변경되는지 확인한다.
      -  Test cases: [로그아웃 테스트](http://mod.lge.com/hub/tv-software-engineer-internship-program/2022-05-sungbin1.lee/tree/master/Document/test_case.md#로그아웃-테스트)

<!-- ### 단위 테스트
-  `FR01`, `FR02`, `FR04` 는 각각 하나의 모듈로 가정하고 모듈테스트를 진행한다. 
-  `FR01`, `FR02`, `FR04` 는 모두 서버와 동일한 통신과정(송/수신)을 거친다. 즉, 하나의 더미 서버로 각각의 기능에 대해 모듈 테스트를 진행할 수 있다.
-  모듈 테스트 환경은 아래와 같다.

<img src="./images/test_diagram.png" width="700" height="350"/>
  
-  Module : 테스트하고자 하는 대상
-  Socket : 서버와의 인터페이스
-  Dummy server : 클라이언트와의 통신을 확인하기 위한 서버
-  Input : 테스트를 위한 입력
-  Output : 기대값과 일치하는지 확인 
 -->