#pragma once
/* Structure to contain all our information, so we can pass it around */
typedef struct _CustomData {
  GstDiscoverer *discoverer;
  GMainLoop *loop;
} CustomData;


static void print_tag_foreach (const GstTagList *tags, const gchar *tag, gpointer user_data);

static void print_stream_info (GstDiscovererStreamInfo *info, gint depth);

static void print_topology (GstDiscovererStreamInfo *info, gint depth);

static void on_discovered_cb (GstDiscoverer *discoverer, GstDiscovererInfo *info, GError *err, CustomData *data);


static void on_finished_cb (GstDiscoverer *discoverer, CustomData *data);

int print_media_info();
