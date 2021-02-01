#pragma once

#include <iostream>
#include <string.h>
#include <vector>

#include "BasicUsageEnvironment.hh"
#include "DigestAuthentication.hh"
#include "H264VideoRTPSource.hh"
#include "liveMedia.hh"

#include "VSFFmpeg.h"

class DummySink : public MediaSink {
public:
    static DummySink* createNew(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId = NULL);

    void SetFFmpeg(VSFFmpeg* ffmpeg);

private:
    DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId);

    virtual ~DummySink();

    static void afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
                                  struct timeval presentationTime, unsigned durationInMicroseconds)
    {
        static_cast<DummySink*>(clientData)
            ->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
    }

    void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime,
                           unsigned durationInMicroseconds);

private:
    // redefined virtual functions:
    virtual Boolean continuePlaying();

private:
    u_int8_t*        fReceiveBuffer;
    MediaSubsession& fSubsession;
    char*            fStreamId;
    VSFFmpeg*        m_ffmpeg;
};

class RtspClient : public RTSPClient {
public:
    RtspClient(UsageEnvironment& env, char const* rtspURL, char const* sUser, char const* sPasswd, VSFFmpeg** ffmpeg);

    void Play();

    void ContinueAfterSetup(int resultCode, char* resultString);

    void ContinueAfterPlay(int resultCode, char* resultString);

    void ContinueAfterDescribe(int resultCode, char* resultString);

    void SendNextCommand();

    inline void SetStreamTCP(bool istcp)
    {
        m_isTcp = istcp;
    }

    virtual ~RtspClient();

protected:
    static void continueAfterSetup(RTSPClient* rtspClient, int resultCode, char* resultString);

    static void continueAfterPlay(RTSPClient* rtspClient, int resultCode, char* resultString);

    static void continueAfterDescribe(RTSPClient* rtspClient, int resultCode, char* resultString);

    // called only by createNew();

private:
    VSFFmpeg*                m_ffmpeg;
    Authenticator            m_Authenticator;
    bool                     m_isTcp = true;
    MediaSession*            m_session;
    MediaSubsession*         m_subSession;
    MediaSubsessionIterator* m_subSessionIter;
};

class RtspThread {

public:
    RtspThread(std::string pUrl, std::string pUser, std::string pPasswd)
    {
        m_url = pUrl;
        m_user = pUser;
        m_passwd = pPasswd;
    }
    ~RtspThread() {}

    void Run();

    void OpenCameraPlay();

    VSFFmpeg* ffmpegH264;

private:
    TaskScheduler*    m_scheduler;
    UsageEnvironment* m_env;

    std::string m_url;
    std::string m_user;
    std::string m_passwd;
};
