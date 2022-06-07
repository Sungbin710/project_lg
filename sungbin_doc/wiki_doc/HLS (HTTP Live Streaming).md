#### Gstreamer

- a pipeline-based multimedia framework
- library for building multimedia piplelines
- flexible, extensible
- cross-platform



gst-launch-1.0.exe videotestsrc ! videoconvert ! autovideosink



```
gst-launch-1.0 -v playbin uri=file:~/Project/tt.mp4
```

uri는 절대경로

```
gst-launch-1.0 filesrc location=music.mp3 ! mad ! audioconvert ! audioresample ! osssink
```

```
 std::string pipeline = "playbin2";
 pipeline+=" uri=file://"+m_txtVideoPath;
 std::cout << "PIPELINE: " << pipeline << std::endl; // for debugging
 data.pipeline = gst_parse_launch (pipeline.c_str(), NULL);
```





--------



### HLS (HTTP Live Streaming)

- 가장 널리 사용되는 비디오 [스트리밍](https://www.cloudflare.com/learning/performance/what-is-streaming/) 프로토콜
- 비디오 파일을 다운로드할 수 있는 HTTP 파일 조각으로 나누고 [HTTP](https://www.cloudflare.com/learning/ddos/glossary/hypertext-transfer-protocol-http/) 프로토콜을 이용하여 전송
- [클라이언트](https://www.cloudflare.com/learning/serverless/glossary/client-side-vs-server-side/) 장치는 이러한 HTTP 파일을 로드한 후 비디오로 재생
- HLS의 장점 중 하나는 모든 인터넷 연결 장치가 HTTP를 지원하기 때문에 전용 서버가 필요한 스트리밍 프로토콜보다 간단하게 실행
- HLS 스트리밍은 재생에 지장을 주지 않고 네트워크 상태에 따라 비디오 품질을 높이거나 낮출 수 있다. 이 때문에 사용자가 비디오를 보는 중에 품질이 나빠지거나 좋아질 수 있습니다



#### HLS 작동 원리

- **서버:** HLS 스트리밍은 미디어 파일이 저장된 서버(주문형 스트리밍의 경우)나 스트리밍이 제작된 서버(라이브 스트리밍의 경우)에서 시작됩니다. HLS는 HTTP에 기반하기 때문에 모든 일반 웹 서버에서 스트리밍을 시작할 수 있습니다.
  - **인코딩:** 비디오 데이터의 포맷을 다시 설정하여 모든 장치가 데이터를 인식하고 해석할 수 있게 합니다. HLS 은 [H.264](https://www.cloudflare.com/learning/video/what-is-h264-avc/)나 H.265 인코딩을 사용해야 합니다.
  - **조각화:** 비디오는 몇 초 길이의 세그먼트로 나뉩니다. 세그먼트 길이는 다양하지만 기본 길이는 6초입니다(2016년까지는 10초였습니다).
    - 비디오를 세그먼트로 나누는 것과 더불어 HLS는 비디오 세그먼트의 인덱스 파일을 만들어 세그먼트의 순서를 기록합니다.
    - HLS는 또한 480p, 720p, 1080p 등의 다양한 품질로 여러 세트의 세그먼트를 복제합니다.
  - **배포:** 인코딩된 비디오 세그먼트는 클라이언트 장치가 스트리밍을 요청하면 인터넷을 통해 클라이언트 장치로 전송됩니다. 일반적으로 [CDN(콘텐츠 전송 네트워크)](https://www.cloudflare.com/learning/cdn/what-is-a-cdn/)이 여러 지역으로 스트리밍을 배포하는 데 도움을 줍니다. CDN도 스트리밍을 캐시하여 클라이언트에 더 신속히 전송할 수 있습니다.
  - **클라이언트 장치:** 클라이언트 장치는 스트리밍을 받고 비디오를 재생하는 사용자 스마트폰이나 노트북 같은 장치입니다. 클라이언트 장치는 인덱스 파일을 참조하여 비디오를 순서대로 조합하고 필요에 따라 품질을 높이거나 낮춥니다.



#### HTTP

- HTTP는 네트워크에 연결된 장치 사이에 정보를 전송하기 위한 [애플리케이션 계층](https://www.cloudflare.com/learning/ddos/what-is-layer-7/) 프로토콜
- 일반적인 사용자가 액세스할 수 있는 모든 웹사이트와 애플리케이션은 HTTP에서 실행
-  HTTP를 통한 데이터 전송은 일반적으로 요청과 응답에 따라 이루어집니다. 거의 모든 HTTP 메시지는 요청이나 요청에 대한 응답





스트리밍에서 사용되는 프로토콜 

- HLS
- MPEG-DASH
- HDS
- RTMP
  - Adobe Flash는 비디오 스트리밍에 사용되는 대표적인 기술이었습니다. 하지만 이제는 많은 브라우저가 Flash를 지원하지 않게 됐습니다. RTMP는 여전히 사용되고는 있지만, 지원 수준은 낮아지고 있습니다.







#### HLS는 전송 프로토콜로 TCP와 UDP 중 어느 것을 사용합니까?

[TCP](https://www.cloudflare.com/learning/ddos/glossary/tcp-ip/)와 [UDP](https://www.cloudflare.com/learning/ddos/glossary/user-datagram-protocol-udp/)는 전송 프로토콜로서 인터넷을 통한 콘텐츠 전송을 담당합니다. TCP는 UDP보다 데이터 전송 신뢰성이 높지만 UDP는 전송 중 일부 데이터가 손실될 수 있지만 전송 속도가 훨씬 빠릅니다.

UDP가 빠르기 때문에 많은 스트리밍 프로토콜은 TCP 대신 UDP를 사용합니다. 하지만 HLS는 여러 가지 이유로 TCP를 사용합니다.

1. HLS는 HTTP를 사용하면 HTTP 프로토콜은 TCP를 사용하도록 제작됐습니다([일부 예외 있음](https://blog.cloudflare.com/http3-the-past-present-and-future/)).
2. 현대적인 인터넷은 스트리밍이 처음 개발됐을 때보다 신뢰성과 효율성이 높습니다. 오늘날 전 세계 많은 지역에서 모바일 연결을 중심으로 사용자 연결이 크게 개선됐습니다. 따라서 사용자는 모든 비디오 프레임 전송을 지원하는 충분한 대역을 갖고 있습니다.
3. 적응 비트 전송률 스트리밍은 TCP의 데이터 전송이 느려질 경우 보상하는 데 도움을 줍니다.
4. HLS 스트리밍은 화상 회의 연결처럼 "실시간"이지 않아도 됩니다. 몇 초 지연돼도 비디오 프레임이 손실된 것처럼 사용자 경험에 영향을 주지 않습니다.







## 서버 측



- 동영상 콘텐츠를 다양한 해상도로 인코딩해 저장해두고 데이터 단위도 동영상 콘텐츠 하나로 저장하는 게 아니라 잘게 쪼개 저장해둔다.
- 리고 사용자가 동영상을 플레이할 때 네트워크 상황에 따라서 적절한 전략으로 콘텐츠의 소스를 선택해 최적의 스트리밍 서비스를 제공
- 다양한 소스로 인코딩이 되어있으니 상황에 따라 선택할 수 있고 큰 파일 하나가 아닌 잘게 쪼개진 데이터들을 하나씩 다운로드하는 방식이라 다음 데이터를 다른 퀄리티로 쉽게 교체할 수 있게 된다.
- 예를 들면 현재 사용자의 네트워크 사정이 좋지 않다면 동영상의 480P 소스를 한 조각씩 스트리밍해주고 상황이 나아지면 다음 조각으로 그 이상의 해상도를 갖는 소스를 선택해 스트리밍





## Adaptive Streaming의 흐름

### 서버 파트

Adaptive streaming을 적용하기 위해서는 PD와는 다르게 동영상 스트리밍을 위해 미리 준비해야 할 것들이 더 있다. 하나면 되었던 동영상 파일을 지원 해상도 별로 인코딩을 해둬야 하고 한 개의 파일이 아니다 보니 각 조각난 파일들에 대한 정보를 클라이언트에 제공해줘야 한다. 전반적인 흐름은 아래와 같다.

1. 동영상을 업로드할때 파일을 작은 조각(세그먼트)으로 잘라낸다.
2. 세그먼트들은 서비스에 필요에 따라 구분한 대역폭에 대응된 해상도로 인코딩한다. 여기서 해상도 별로 세그먼트의 개수가 늘어난다.
3. 대응되는 해상도별 미디어 세그먼트의 정보등을 담은 파일(Manifest)을 클라이언트에 제공한다.

동영상 파일을 작은 세그먼트로 잘라 낼때는 코덱별로 필요한 도구를 사용하게 되고 동영상의 조각난 세그먼트에 대한 정보를 제공해주는 문서 포맷에는 Apple-HLS와 MPEG-DASH이 있다.

### 클라이언트 파트

웹 클라이언트에서 Adaptive Streaming을 가능하게 하는 표준 기술로는 [Media Source Extensions(MSE)](https://www.w3.org/TR/media-source/) 가 있고 이를 통해 스트리밍 데이터를 플레이어에 전달한다.

1. 클라이언트는 플레이할 동영상의 각 해상도별 세그먼트정보를 담은 Manifest 파일을 서버에게 요청한다.
2. Manifest 파일을 파싱해 필요한 정보들을 얻은 후 비디오에 대한 정보, 어떤 해상도의 퀄리티들을 사용할 수 있는지 그리고 어디서 해당 세그먼트들을 받을 수있는지(e.g. CDN URL)를 파악한다.
3. 클라이언트는 사용자의 네트워크 대역폭을 측정하고 Manifest의 내용에 따라 가장 최적의 비디오 퀄리티를 선택한 뒤 필요한 세그먼트를 다운로드한다.(세그먼트를 다운로드하면서 다시 대역폭 측정)
4. 다운로드한 세그먼트의 데이터를 MSE 버퍼에 제공한다.
5. MSE는 데이터를 디코딩하고 비디오객체에 제공해 플레이한다.(goto 3)



## 적응 비트 전송률 스트리밍이란 무엇입니?

[적응 비트 전송률 스트리밍](https://www.cloudflare.com/learning/video/what-is-adaptive-bitrate-streaming/)은 네트워크 조건이 변할 때 스트림의 중간에서 비디오 품질을 조정하는 기능입니다. MPEG-DASH, HLS, HDS 등의 스트리밍 프로토콜에서는 적응 비트 전송률 스트리밍이 가능합니다.

적응 비트 전송률 스트리밍이 가능한 것은 원본 서버가 다양한 품질 수준으로 비디오 세그먼트를 인코딩하기 때문입니다. 이는 인코딩 및 조각화 과정에서 일어납니다. 비디오 플레이어는 재생을 중단하지 않고 비디오의 중간에서 품질 수준을 변경할 수 있습니다. 따라서 네트워크 대역폭이 갑자기 줄어드는 경우에도 비디오 자체를 중단하지 않아도 됩니다.



## HLS와 DASH: 주요 차이점

HLS는 현재 널리 사용되고 있는 또 다른 스트리밍 프로토콜입니다. MPEG-DASH와 HLS는 여러 면에서 유사합니다. 두 프로토콜 모두 HTTP를 통해 실행되고, 전송 프로토콜로 [TCP](https://www.cloudflare.com/learning/ddos/glossary/tcp-ip/)를 사용하고 색인 파일이 따르는 세그먼트로 비디오를 구분하며 적응 비트 전송률 스트리밍을 제공합니다.

하지만 두 프로토콜 간에는 몇 가지 주요 차이점이 있습니다.

**인코딩 형식:** MPEG-DASH에서는 어떠한 인코딩 표준도 사용할 수 있지만, HLS는 [H.264](https://www.cloudflare.com/learning/video/what-is-h264-avc/) 또는 H.265를 사용해야 합니다.

**장치 지원:** HLS는 Apple 장치에서 지원하는 유일한 형식입니다. iPhone, MacBook 등 Apple 제품은 MPEG-DASH로 전송되는 비디오를 재생하지 못합니다.

**세그먼트 길이:** 2016년 이전에는 HLS의 기본 세그먼트 길이가 10초로서 두 프로토콜 간의 큰 차이 요인이었습니다. 현재는 HLS의 기본 길이가 6초이지만, 기본값에서 조정할 수 있습니다. MPEG-DASH 세그먼트는 통상적으로 길이가 2-10초이지만, 최적 길이는 2-4초입니다.

**표준화:** MPEG-DASH는 국제 표준입니다. HLS는 Apple에 의해 개발되었으며 광범위한 지원을 받고 있지만 국제 표준으로 게시되지는 않았습니다.





적응형 스트리밍 시스템

- HLS ( 애플 HTTP 라이브 스트리밍 )
- MSS ( 마이크로소프트 부드러운 스트리밍 )
- ADS (어도비 HTTP 동적 스트리밍)
- MPEG-DASH (HTTP를 통한 MPEG 동적 적응 스트리밍 )
  -  MPD(Media Presentation Description)라는 XML 설명 파일을 기반
  - 미디어 유형(오디오, 비디오 또는 자막)이 있는 집합을 설명하고 `representations`다양한 비트 전송률 또는 미디어 형식을 제공합니다.







```
4.1 Streaming Server* Streaming Server 역할 - RTP Server   RTP로 미디어 패킷들을 클라이언트에게 전송하는 역할을 한다. RTCP를 통해 클   라이언트의 수신 대역폭에 따라 전송률을 조정한다.  - RTSP Server   RTSP를 사용해서 사용자와 통신하는 역할을 한다. RTP가 단방향 통신인데 비   해, RTSP는 양방향 통신이다. 사용자는 RTSP를 통해 비디오 요청, 스킵, 멈춤과    같은 기능을 사용할 수 있다. - Media Packetizer   오디오나 비디오와 같은 미디어파일을 그 파일의 포맷에 따라 네트워크로 전송   할 수 있는 패킷으로 쪼개는 역할을 한다. - Packet Builder   Media Packetizer에서 쪼갠 미디어 데이터를 RTP 패킷으로 만드는 역할을 한다.     * Adobe Flash Media Server* crtmpserver* Erlyvideo* Flazr - Java 구현* FreeSWITCH RTMP media streaming* haXeVideo - 프로그래밍 언어 haXe 로 작성된 다중 스레드 FLV 스트리밍 서버* Helix Universal Server - 리얼 네트웍스가 판매하고 RealServer의 후계 기종 - 전송 매체 종류 Real Media, Windows Media, Quick Time - 지원 OS로는 Windows, Linux , FreeBSD , Solaris , HP-UX 등* Netris iStream Video Server* OneTeam Media Server* Onlinelib VCS Video Communication Server (iPhone 지원을 포함)* QuickTime Streaming Server - 애플이 개발한 스트리밍 서버로 오픈 소스 Darwin Streaming Server를 기반* Unreal Media Server* WebORB Integration Server* Windows Media Service - 마이크로소프트가 제공하는 플랫폼으로 Windows NT Server 4.0,    Windows 2000 Server 용이 Windows Media Server 4.1 그리고 Windows   Server 2003 용이 Windows Media Server 9가 사용 되고 있다.    컨텐츠 개발 인코더 등도 무료로 배포되고 있다.* Wowza Media Server - Wowza Media Systems가 개발하고 있는 동영상 스트리밍 서버 .  - RTMP를 사용하여 Adobe Flash Player와 통신할 수 있지만 클라이언트 서버 간   의 원격 프로시저 호출도 지원한다.
```



```
* GStreamer* Media Player Classic* MPEG4IP* MPlayer* QuickTime* RealPlayer* Skype* VLC media player - VLC 미디어 플레이어 (VLC media player, Video LAN Client)은 크로스 플랫   폼 에서 동작하는 미디어 플레이어 이다. 많은 미디어 파일에 대한 코덱이 내장   되어있어 동영상 파일이나 음성 파일 등 많은 미디어 파일을 재생 표시할 수 있   다. GPL 하에 있는 무료 소프트웨어 이다* Winamp* Windows Media Player* Xine* MythTV via Freebox
```

