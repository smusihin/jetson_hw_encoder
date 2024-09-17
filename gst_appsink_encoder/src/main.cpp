#include "video_encode.h"

#include <gst/app/gstappsink.h>
#include <gst/gst.h>
#include <iostream>


// https://docs.nvidia.com/jetson/l4t-multimedia/l4t_mm_01_video_encode.html

static void
set_defaults(context_t * ctx)
{
    memset(ctx, 0, sizeof(context_t));

    ctx->raw_pixfmt = V4L2_PIX_FMT_YUV420M;
    ctx->bitrate = 4 * 1024 * 1024;
    ctx->peak_bitrate = 0;
    ctx->profile = V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE;
    ctx->ratecontrol = V4L2_MPEG_VIDEO_BITRATE_MODE_CBR;
    ctx->iframe_interval = 30;
    ctx->externalRPS = false;
    ctx->enableGDR = false;
    ctx->enableROI = false;
    ctx->bnoIframe = false;
    ctx->bGapsInFrameNumAllowed = false;
    ctx->bReconCrc = false;
    ctx->enableLossless = true;
    ctx->nH264FrameNumBits = 0;
    ctx->nH265PocLsbBits = 0;
    ctx->idr_interval = 256;
    ctx->level = -1;
    ctx->fps_n = 30;
    ctx->fps_d = 1;
    ctx->gdr_start_frame_number = 0xffffffff;
    ctx->gdr_num_frames = 0xffffffff;
    ctx->gdr_out_frame_number = 0xffffffff;
    ctx->num_b_frames = (uint32_t) -1;
    ctx->nMinQpI = (uint32_t)QP_RETAIN_VAL;
    ctx->nMaxQpI = (uint32_t)QP_RETAIN_VAL;
    ctx->nMinQpP = (uint32_t)QP_RETAIN_VAL;
    ctx->nMaxQpP = (uint32_t)QP_RETAIN_VAL;
    ctx->nMinQpB = (uint32_t)QP_RETAIN_VAL;
    ctx->nMaxQpB = (uint32_t)QP_RETAIN_VAL;
    ctx->use_gold_crc = false;
    ctx->pBitStreamCrc = NULL;
    ctx->externalRCHints = false;
    ctx->input_metadata = false;
    ctx->sMaxQp = 51;
    ctx->stats = false;
    ctx->stress_test = 1;
    ctx->output_memory_type = V4L2_MEMORY_DMABUF;
    ctx->cs = V4L2_COLORSPACE_SMPTE170M;
    ctx->copy_timestamp = false;
    ctx->sar_width = 0;
    ctx->sar_height = 0;
    ctx->start_ts = 0;
    ctx->max_perf = 0;
    ctx->blocking_mode = 1;
    ctx->startf = 0;
    ctx->endf = 0;
    ctx->num_output_buffers = 6;
    ctx->num_frames_to_encode = -1;
    ctx->poc_type = 0;
    ctx->chroma_format_idc = -1;
    ctx->bit_depth = 8;
    ctx->is_semiplanar = false;
}


static GstFlowReturn processSample(GstElement* element, context_t* context)
{
    /* get the sample from appsink */
    if (auto* sinkSample = gst_app_sink_pull_sample(GST_APP_SINK(element)))
    {
        if (auto* frameBuffer = gst_sample_get_buffer(sinkSample))
        {
            //procNextFrame(frameBuffer, *context, false);
        }
    }

    return GST_FLOW_OK;
}

int main (int argc, char *argv[])
{
  /* create encoder context. */
  context_t ctx;
  set_defaults(&ctx);

  ctx.enc = NvVideoEncoder::createVideoEncoder("enc0");
  
  GstElement *pipeline;
  GstElement *appsink;
  GstBus *bus;
  GstMessage *msg;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Build the pipeline */
  pipeline =
      gst_parse_launch
      ("videotestsrc pattern=ball ! video/x-raw,format=Y444,width=1920,height=1080 ! appsink name=rawsink emit-signals=true",
      NULL);

  appsink =  gst_bin_get_by_name(GST_BIN(pipeline), "rawsink");
  
  g_signal_connect(
        appsink,
        "new-sample",
        G_CALLBACK(processSample),
        &ctx);

  /* Start playing */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  /* Wait until error or EOS */
  bus = gst_element_get_bus (pipeline);
  msg = 
      gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
      GstMessageType(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

  /* See next tutorial for proper error message handling/parsing */
  if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR) {
    g_printerr ("An error occurred! Re-run with the GST_DEBUG=*:WARN "
        "environment variable set for more details.\n");
  }

  /* Free resources */
  gst_message_unref (msg);
  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);

  gst_object_unref (appsink);
  gst_object_unref (pipeline);
  return 0;
}