### Basic tutorial 1



#### 초기화

**gst_init(&argc, &argv)**

```c
/* Initialize GStreamer */
gst_init(&argc, &argv)
```

- 모든 내부 구조 초기화
- 사용 가능한 플러그인 확인
- GStreamer를 위한 모든 명렬줄 옵션을 실행



#### 파이프 라인 구성

```c
GstElement *pipeline;

pipeline = 
    gst_parse_launch("playbin 		uri=https://www.freedesktop.org/software/gstreamer/sdk/data/media/sintel_trailer-480p.webm", NULL); 
```

- 미디어는 "소스" 요소에서 "싱크" 요소로 내려가 모든 종류의 작업을 수행하는 일련의 중간 요소를 통과한다.
- 상호 연결된 모든 요소의 집합을 "파이프라인"이라고 함
- GStreamer에서는 일반적으로 개별 요소를 수동으로 조합하여 파이프라인을 구축하지만, 파이프라인이 충분히 쉽고, 고급기능이 필요하지 않은 경우`gst_parse_launch`를 사용할 수 있음
- `gst_parse_launch`는 파이프라인의 텍스트 표현을 가져와서 실제 파이프라인으로 변환함
- playbin이라는 단일 요소로 구성된 파이프라인을 구축함
- playbin은 소스 및 싱크 역할을 하는 특수 요소이며 전체 파이프라인이다. 내부적으로 미디어를 재생하는데 필요한 모든 요소를 만들고 연결함
- 수동 파이프라인이 수행하는 제어 세분성은 허용하지 않지만 광범위한 응용 프로그램에 사용될 수 있음



```c
/* start playing */
gst_element_set_state(pipeline, GST_STATE_PLAYING);
```

- 모든 GStreamer 요소에는 연결된 상태가 있음



```c
/* wait unitl error or EOS */
GstBus *bus;

bus = gst_element_get_bus(pipeline);
msg = 
    gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
    GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
```

- 오류가 발생하거나 스트림의 끝이 발견될 때까지 기다림
- `gst_element_get_bus`는 파이프라인의 버스를 검색
- `gst_bus_timed_pop_filtered`는 버스를 통해 ERROR 또는 EOS를 수신할 때까지 차단됨
- 미디어가 끝(EOS)에 도달하거나 오류가 발생(비디오 창을 닫거나, 네트워크 케이블을 뽑음)하면 실행이 종료됨



```c
 /* See next tutorial for proper error message handling/parsing */
  if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR) {
    g_error ("An error occurred! Re-run with the GST_DEBUG=*:WARN environment "
        "variable set for more details.");
  }
```

- 





- gst_element_factory_make() 를 사용하여 새 요소를 만든다.
- 첫 번째 매개변수는 생성할 요소의 유형
- 두 번째 매개변수는 이 특정 인스턴스에 지정하려는 이름
  - 요소의 이름을 지정하면 포인터를 유지하지 않은 경우 나중에 검색하는데 유용, 디버그 출력을 위해
  - 이름에 NULL 을 전달하면 GStreamer가 고유한 이름을 제공

```c
 /* Create the elements */
source = gst_element_factory_make("videotestsrc", "source");
sink = gst_element_factory_make ("autovideosink", "sink");
```

