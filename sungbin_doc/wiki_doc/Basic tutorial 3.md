## Basic tutorial 3



```c
/* Create the elements */
data.source = gst_element_factory_make ("uridecodebin", "source");
data.convert = gst_element_factory_make ("audioconvert", "convert");
data.resample = gst_element_factory_make ("audioresample", "resample");
data.sink = gst_element_factory_make ("autoaudiosink", "sink");
```

- `uridecodebin` 은 URI를 원시 오디오 또는 비디오 스트림으로 바꾸는데 필요한 모든 요소 (소스, 디먹서 및 디코더)를 내부적으로 인스턴스화 함. 디먹서를 포함하기 때문에, 소스 패드는 초기에 이용될 수 없음
- `audioconvert`는 다른 오디오 포맷간의 변환에 유용하다. 오디오 디코더에 의해 생성되는 포멧은 플랫폼에 따라 다르기때문에 이를 해결하기위해 사용됨
- `audioresample`은 다른 오디오 샘플 레이트간의 변환에 유용하다.
- `autoaudiosink` 는 이전 튜토리얼의`autoviedosink`와 동일하다. 오디오 스트림을 오디오 카드에 렌더링한다.





```c
if (!gst_element_link_many (data.convert, data.resample, data.sink, NULL)) {
  g_printerr ("Elements could not be linked.\n");
  gst_object_unref (data.pipeline);
  return -1;
}
```

- 요소들은 링크시킨다. 하지만 소스는 연결하지 않음, 이유는 현재 상태에서 소스패드를 포함하지 않기때문이다. 



```c
/* Set the URI to play */
g_object_set (data.source, "uri", "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", NULL);
```

- file의 URI를 셋팅한다.



#### 신호

```c
/* Connect to the pad-added signal */
g_signal_connect (data.source, "pad-added", G_CALLBACK (pad_added_handler), &data);
```

- `GSignals`는 GStreamer에서 중요한 포인트이다. 흥미로운 일이 발생했을 때 콜백을 통해 알림을 받을 수 있다. 신호는 이름으로 식별되며 각각의 `GObject`는 자신만의 신호를 가진다.
- 우리의 소스인 uridecodebin 요소의 `pad-added`신호에 연결한다. 
- `g_signal_connect()`를 사용하여, 콜백함수와 데이터 포인터를 제공한다.





#### 콜백

```c
static void pad_added_handler (GstElement *src, GstPad *new_pad, CustomData *data) {
```

- 소스 요소가 데이터 생성을 시작하기에 충분한 정보를 갖게 되면 소스패드를 생성하고 "pad-added"신호를 트리거 한다. 이 시점에서 콜백이 호출된다.
- src는 신호를 트리거하는 GstElement이다. 여기서는 uridecodebin만이 될 수 있따. 우리가 연결한 유일한 신호이기 때문이다. signal handler의 첫번째 매개변수는 항상 트리거한 object이다.
- new_pad는 src 요소에 더해진 GstPad이다. 우리가 연결하기를 원하는 패드이다.
- data는 우리가 신호에 연결할 때 제공한 포인터이다. 여기서는 CustomData를 넘기기 위함이다.



```c
GstPad *sink = gst_element_get_static_pad (data->converter, "sink");
```

- CustomData로부터 우리는 converter 요소를 추출하고, 이 요소의 싱크 패드를 `gst_element_get_static_pad()`를 사용하여 반환한다. 이 패드는 new_pad와 연결시키고자 하는 패드이다.





```c
/* If our converter is already linked, we have nothing to do here */
if (gst_pad_is_linked (sink_pad)) {
  g_print ("We are already linked. Ignoring.\n");
  goto exit;
}
```

- uridecodebin은 적합하다고 생각된 가능한 많은 패드를 생성한다. 그리고 패드가 생성될 때, 콜백이 호출된다. 이 라인을 통해 한번 링크되어 있으면 링크시도를 막을수 있음



#### GStreamer 상태

| 상태    | 설명                                                         |
| ------- | ------------------------------------------------------------ |
| NULL    | 요소의 NULL 상태 또는 초기 상태                              |
| READY   | 요소가 PAUSED로 이동할 준비가 됨                             |
| PAUSED  | 요소가 PAUSED 이고, 데이터를 수락하고 처리할 준비가 됨, 그러나 싱크 요소는 하나의 버퍼만 수락한 다음 차단함 |
| PLAYING | 요소가 재생 중이고 시계가 실행중이며 데이터가 흐름           |



```c
case GST_MESSAGE_STATE_CHANGED:
  /* We are only interested in state-changed messages from the pipeline */
  if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data.pipeline)) {
    GstState old_state, new_state, pending_state;
    gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
    g_print ("Pipeline state changed from %s to %s:\n",
        gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
  }
  break;
```

- 상태 변경에 관한 버스 메시지를 수신하고 전환을 이해하는데 도움이 되도록 이를 화면에 인쇄하는 코드