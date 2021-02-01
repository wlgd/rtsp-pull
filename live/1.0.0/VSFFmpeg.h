#pragma once
#include <mutex>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
}

#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 500000

class VSFFmpeg {
public:
    VSFFmpeg();

    ~VSFFmpeg();

    bool InitH264DecodeEnv();

    void SetPlayState(bool pause);

    void DecodeFrame(unsigned char* sPropBytes, int sPropLength, uint8_t* frameBuffer, int frameLength, long second,
                     long microSecond);

    void GetDecodedFrameData(unsigned char* data, int& length);

    void GetDecodedFrameInfo(int& width, int& heigth);

public:
    int VideoWidth;
    int VideoHeight;

private:
    AVCodec*         m_codec;
    AVCodecContext*  m_codecContext;
    AVFrame*         m_frame;
    AVFrame*         m_frameBGR;
    uint8_t*         out_buffer;
    AVFormatContext* m_outFmtCtx;
    SwsContext*      m_swsContext;
    AVPicture*       m_pAVPicture;
    std::mutex       m_playMutex;
    unsigned char*   out_bufferRGB;
    int              m_videondex;
    int              m_isRecord;
};
