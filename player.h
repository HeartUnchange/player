#pragma once
#include "demuxer.h"
#include "display.h"
#include "format_converter.h"
#include "queue.h"
#include "timer.h"
#include "video_decoder.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
extern "C" {
	#include <libavcodec/avcodec.h>
}

class Player {
public:
	Player(const std::string &file_name);
	~Player();
	void operator()();
	friend void sdl_audio_callback(void *opaque, Uint8 *stream, int len);
private:
	void demultiplex();
	void decode_video();
	void decode_audio();
	void video();
	void audio();
	void cleanup();
private:
	std::unique_ptr<Demuxer> demuxer_;
	std::unique_ptr<VideoDecoder> video_decoder_;
	std::unique_ptr<FormatConverter> format_converter_;
	std::unique_ptr<Display> display_;
	std::unique_ptr<Timer> timer_;
	std::unique_ptr<PacketQueue> video_packet_queue_;
	std::unique_ptr<PacketQueue> audio_packet_queue_;
	std::unique_ptr<FrameQueue> video_frame_queue_;
	std::unique_ptr<FrameQueue> audio_frame_queue_;
	std::vector<std::thread> stages_;
	static const size_t video_queue_size_;
	static const size_t audio_queue_size_;
	std::exception_ptr exception_{};
};
