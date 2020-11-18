#ifndef SERVER_RTMPMANAGER_H
#define SERVER_RTMPMANAGER_H

#include "server/codec/RtmpCodec.h"
#include "server/FlvManager.h"

constexpr int RTMP_START_PARSE_LENGTH = 1000;
constexpr int RTMP_CHUNK_SIZE = 4096;


class RtmpManager
{
public:

	enum ParseStatus
	{
		PARSE_RTMP_FIRST_HEADER,
		PARSE_RTMP_HEADER,
		PARSE_RTMP_BODY
	};

	RtmpManager();
	~RtmpManager();

	ssize_t ParseData(Buffer* buffer);

	FlvManager* GetFlvManager();
private:

	ParseStatus parsed_status_;

	size_t parsed_length_;

	RtmpPack rtmp_pack_;
	RtmpCodec rtmp_codec_;

	FlvManager flv_manager_;

	FlvTag* current_tag_;

	/* ����chunk�ķֿ���� ���� ��body����4096�ֽ�ʱ, ÿ��ȡ4096���ֽ� ��Ҫ���½���һ��header���ڴ˼�¼*/
	uint32_t read_chunk_size_;
	/* ���ڱ�ʶ��ǰchunk�Ƿ������� δ�����겻�ܴ����µ�tag*/
	bool chunk_over_;

	ssize_t ParseFirstHeader(Buffer* buffer);
	ssize_t ParseScriptPack(Buffer* buffer);
	ssize_t ParseVideoAudio(Buffer* buffer);

	ssize_t ParseHeader(Buffer* buffer);

	ssize_t ParseBody(Buffer* buffer);

	void PushBackCurrentFlvTag();
};

#endif