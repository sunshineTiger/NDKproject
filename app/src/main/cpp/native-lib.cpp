#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window_jni.h>
//#include <android/native_window.h>
#include <unistd.h>
#include "libyuv.h"

#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"jason",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"jason",FORMAT,##__VA_ARGS__);
extern "C" {
//工具库
#include "libavutil/avutil.h"
//封装格式处理库
#include "libavformat/avformat.h"
//编码解码库
#include "libavcodec/avcodec.h"
//音频采样数据格式转换库
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include <libavutil/imgutils.h>
}
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_ndkproject_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    return env->NewStringUTF(av_version_info());
}
/**
▫ FFmpeg解码函数简介
▫ av_register_all()：注册所有组件。
▫ avformat_open_input()：打开输入视频文件。
▫ avformat_find_stream_info()：获取视频文件信息。
▫ avcodec_find_decoder()：查找解码器。
▫ avcodec_open2()：打开解码器。
▫ av_read_frame()：从输入文件读取一帧压缩数据。
▫ avcodec_decode_video2()：解码一帧压缩数据。
 *
 * **/

extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndkproject_FFmpegSurface_decode(JNIEnv *env, jobject thiz, jstring path,
                                                 jstring out_path, jobject surface) {
    const char *str_ = env->GetStringUTFChars(path, NULL);
    /**
     * 第一步
     * 初始化网络组件
     * **/
    avformat_network_init();
    /**
     * 第二步
     * 打开封装格式->打开文件
     * AVFormatContext：作用保存整个视频信息（解码器，编码器）
     * 信息：码率，帧率等
     * **/
    //获取AVFormat上下文
    AVFormatContext *pAVFContext = avformat_alloc_context();
    /**
     * 指定输入的参宿
     * 设置默认参数 AVDictionary* options = NULL;
     * **/
    int ret = avformat_open_input(&pAVFContext, str_, NULL, NULL);
    if (ret < 0) {
        LOGE("不能打开指定文件");
        if (pAVFContext) {
            //释放上下文
            avformat_free_context(pAVFContext);
        }
        return;
    }
    /**
     * 第三步
     * 获取视频信息
     * **/
    ret = avformat_find_stream_info(pAVFContext, NULL);
    if (ret < 0) {
        LOGE("查找视频信息失败");
        return;
    }
    /**
     * 第四步
     * 查找视频编码器
     * **/
    /**1.查找视频流位置索引**/
    int video_stream_index = -1;
    for (int i = 0; i < pAVFContext->nb_streams; ++i) {
        if (pAVFContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }
    /**2.根据索引值获取指定解码**/
    AVCodecParameters *pavCoderPar = pAVFContext->streams[video_stream_index]->codecpar;

    /**根据解码器上下文，获得解码器ID，根据ID查找解码器**/
    AVCodec *pAVCoder = avcodec_find_decoder(pavCoderPar->codec_id);
    AVCodecContext *avCtx = avcodec_alloc_context3(pAVCoder);
    avcodec_parameters_to_context(avCtx, pavCoderPar);
    /**
     * 第五步
     * 打开解码器
     * **/
    ret = avcodec_open2(avCtx, pAVCoder, NULL);
    if (ret < 0) {
        LOGE("打开解码器失败");
        if (avCtx) {
            avcodec_free_context(&avCtx);
        }
        return;
    }
    //输出视频信息
    LOGI("视频的文件格式：%s", pAVFContext->iformat->name);
    LOGI("视频时长：%lld", (pAVFContext->duration) / (1000 * 1000));
    LOGI("视频的宽高：%d,%d", avCtx->width, avCtx->height);
    LOGI("解码器的名称：%s", pAVCoder->name);
    /**
     * 第六步
     * 读取视频数据->循环读取
     * **/
    AVPacket *packet = av_packet_alloc();
    AVFrame *avFrame_in = av_frame_alloc();
    AVFrame *rgba_frame = av_frame_alloc();
    int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, avCtx->width, avCtx->height, 1);
    uint8_t *out_buffer = (uint8_t *) av_malloc(buffer_size * sizeof(uint8_t));
    av_image_fill_arrays(rgba_frame->data, rgba_frame->linesize, out_buffer, AV_PIX_FMT_RGBA,
                         avCtx->width, avCtx->height, 1);

    SwsContext *pSwsContext = sws_getContext(avCtx->width, avCtx->height,
                                             avCtx->pix_fmt,
                                             avCtx->width, avCtx->height,
                                             AV_PIX_FMT_RGBA,
                                             SWS_BICUBIC, NULL, NULL, NULL);
    // R4 初始化 Native Window 用于播放视频
    ANativeWindow *native_window = ANativeWindow_fromSurface(env, surface);
    ret = ANativeWindow_setBuffersGeometry(native_window, avCtx->width, avCtx->height,
                                           WINDOW_FORMAT_RGBA_8888);
    if (ret < 0) {
        LOGE("Player Error : Can not set native window buffer");
        ANativeWindow_release(native_window);
        return;
    }
    // 定义绘图缓冲区
    ANativeWindow_Buffer window_buffer;
    // 开始读取帧

    while (av_read_frame(pAVFContext, packet) >= 0) {
        if (packet->stream_index == video_stream_index) {
            avcodec_send_packet(avCtx, packet);
            ret = avcodec_receive_frame(avCtx, avFrame_in);
            if (ret == AVERROR(EAGAIN)) {
                LOGE("------->>>>>>:%d", ret);
                continue;
            }

            /**解码成功**/
            /**进行类型转码,原始数据转为RGB**/
            sws_scale(pSwsContext, avFrame_in->data, avFrame_in->linesize, 0, avCtx->height,
                      rgba_frame->data, rgba_frame->linesize);
            ANativeWindow_lock(native_window, &window_buffer, NULL);
            uint8_t *bits = (uint8_t *) window_buffer.bits;
            for (int h = 0; h < avCtx->height; h++) {
                memcpy(bits + h * window_buffer.stride * 4,
                       out_buffer + h * rgba_frame->linesize[0],
                       rgba_frame->linesize[0]);
            }
            ANativeWindow_unlockAndPost(native_window);


        }
        // 释放 packet 引用
        av_packet_unref(packet);
    }
    sws_freeContext(pSwsContext);
    // 释放 R8
    av_free(out_buffer);
    // 释放 R7
    av_frame_free(&rgba_frame);
    // 释放 R6
    av_frame_free(&avFrame_in);
    // 释放 R5
    av_packet_free(&packet);
    // 释放 R4
    ANativeWindow_release(native_window);
    // 关闭 R3
    avcodec_close(avCtx);
    // 释放 R2
    avformat_close_input(&pAVFContext);
    // 释放 R1
    env->ReleaseStringUTFChars(path, str_);
}