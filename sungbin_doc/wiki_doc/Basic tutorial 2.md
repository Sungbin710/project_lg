## Basic tutorial 2



### 요소 생성



```c
/* create the elements */
GstElement *source, *sink;

source = gst_element_factory_make ("videotestsrc", "source");
sink = gst_element_factory_make ("autovideosink", "sink");
```

- `gst_element_factory_make`를 사용해서 새 요소를 만든다.
- 첫 번째 매개변수: 생성할 요소의 유형
- 두 번째 매개변수: 이 특정 인스턴스에 지정하려는 이름
  - 요소의 이름을 지정하면 포인터를 유지하지 않은 경우 나중에 검색하는데 유용
  - NULL을 전달하면 GStreamer가 고유한 이름을 제공





###  파이프라인 생성

```c
/* Create the empty pipeline */
GstElement *pipeline;

pipeline = gst_pipeline_new("test-pipeline");
```

- GStreamer의 모든 요소는 일반적으로 일부 클럭 및 메시징 기능을 처리하기 때문에 사용하기 전에 파이프라인 내부에 포함되어야 함
- `gst_pipeline_new`를 사용하여 새로운 파이프라인을 생성





```c
/* Build the pipeline */
gst_bin_add_many (GST_BIN (pipeline), source, sink, NULL);
if (gst_element_link (source, sink) != TRUE ){
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (pipeline);
    return -1;
}
```

- 파이프라인은 다른 요소를 포함하는데 사용되는 요소인 빈의 특정 유형이다. 따라서 빈에 적용되는 모든 방법은 파이프라인에도 적용됨
- 파이프라인에 요소를 추가하기 위해 `gst_bin_add_many()`를 호출함, NULL로 끝나는 목록을 허용함. 개별요소는 `gst_bin_add()`를 사용하여 추가할 수 있음
- 이러한 요소는 아직 서로 연결되어 있지 않기때문에 `gst_element_link()`를 사용해서 연결시킴
- `gst_element_link()`
  - 첫 번째 매개변수: 소스
  - 두 번째 매개변수: 대상
- 데이터 흐름(소스 -> 싱크)에 따라 링크가 설정되어야 하므로 순서가 중요
- 동일한 저장소에 있는 요소만 함께 연결될 수 있으므로 연결을 시도하기전에 파이프라인에 추가해야 함



#### 속성

- GStreamer 요소는 모두 속성기능을 제공하는 `GObject`의 특정 종류
- 요소의 동작을 변경하기 위해 수정할 수 있는 속성 또는 요소의 내부 상태를 찾기 위해 조회할 수 있는 속성이 있음
- `g_object_get()`으로 읽고, `g_object_set()`으로 씀
- `g_object_set()`은 속성-이름, 속성-값 쌍의 NULL로 끝나는 목록을 허용하므로 여러 속성을 한번에 변경할 수 있음

```c
/* Modify the source's properties */
g_object_set(source, "pattern", 0, NULL);
```

- 테스트 비디오 유형을 제어하는 `videotestsrc`의 `pattern`속성을 변경함



#### 오류 확인

```c
/* Start playing */
ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
if(ret == GST_STATE_CHANGE_FAILURE){
	g_printerr ("Unable to set the pipeline to the playing state. \n");
    gst_object_unref (pipeline);
    return -1;
}
```

- `gst_element_set_state()`를 호출하고 반환값에 오류가 있는지 확인함



```c
  /* Wait until error or EOS */
  GstMessage *msg; 

  bus = gst_element_get_bus (pipeline);
  msg =
      gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
      GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  /* Parse message */
  if (msg != NULL) {
    GError *err;
    gchar *debug_info;

    switch (GST_MESSAGE_TYPE (msg)) {
      case GST_MESSAGE_ERROR:
        gst_message_parse_error (msg, &err, &debug_info);
        g_printerr ("Error received from element %s: %s\n",
            GST_OBJECT_NAME (msg->src), err->message);
        g_printerr ("Debugging information: %s\n",
            debug_info ? debug_info : "none");
        g_clear_error (&err);
        g_free (debug_info);
        break;
      case GST_MESSAGE_EOS:
        g_print ("End-Of-Stream reached.\n");
        break;
      default:
        /* We should not reach here because we only asked for ERRORs and EOS */
        g_printerr ("Unexpected message received.\n");
        break;
    }
    gst_message_unref (msg);
  }

```

- `get_bus_timed_pop_filtered()`는 실행이 끝날 때까지 기다렸다가 이전에 무시했던 GstMessage를 반환함
- Gstreamer가 오류 조건이나 EOS를 만났을 때 반환하도록 `gst_bus_timed_pop_filterer()`를 요청했으므로 어떤 일이 발생했는지 확인하고 화면에 메시지를 인쇄해야 함
- `GstMessage`는 거의 모든 종류의 정보를 전달할 수 있는 다양한 구조
- `GST_MESSAGE_TYPE()` 매크로를 사용하여 오류가 포함되어 있음을 확인할 수 있음
- `gst_message_parse_error()`를 사용하여 GError 오류구조와 디버깅에 유용한 문자열을 반환할 수 있음



#### GStreamer 버스

- 요소에의해 생성된 `GstMessage`를 애플리케이션 스레드에 순서대로 전달하는 역할을 하는 객체
- 미디어의 실제 스트리밍은 어플리케이션이 아닌 다른 스레드에서 수행됨
- 메시지는 `gst_bus_timed_pop_filtered()` 및 그 형제를 사용하여 동기적으로 또는 신호를 사용하여 비동기적으로 버스에서 추출할 수 있음.
- 애플리케이션은 오류 및 기타 재생 관련 문제에 대한 알림을 받기위해 항상 버스를 주시해야 함

