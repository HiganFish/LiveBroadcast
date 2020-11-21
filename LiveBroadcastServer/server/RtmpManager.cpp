#include "RtmpManager.h"

RtmpManager::RtmpManager():
	parsed_status_(RtmpManager::PARSE_RTMP_FIRST_HEADER),
	parsed_length_(0),
	rtmp_pack_(),
	rtmp_codec_(),
	flv_manager_(),
	read_chunk_size_(0),
	chunk_over_(true)
{
}

RtmpManager::~RtmpManager()
{
}

ssize_t RtmpManager::ParseData(Buffer* buffer)
{
	if (!buffer)
	{
		return -1;
	}

	ssize_t current_loop_parsed = 0;
	ssize_t parsed = 0;

	while (true)
	{
		if (parsed_status_ == RtmpManager::PARSE_RTMP_BODY)
		{
			parsed = ParseBody(buffer);

			if (parsed > 0)
			{
				PushBackCurrentFlvTag();
			}
			else if (parsed < 0)
			{
				printf("ParseBody error\n");
			}
		}
		else
		{
			if (parsed_status_ == RtmpManager::PARSE_RTMP_HEADER)
			{
				parsed = ParseHeader(buffer);
				if (parsed < 0)
				{
					printf("ParseHeader error\n");
				}

			}
			else if (parsed_status_ == RtmpManager::PARSE_RTMP_FIRST_HEADER)
			{
				parsed = ParseFirstHeader(buffer);
				if (parsed < 0)
				{
					printf("ParseFirstHeader error\n");
				}

			}
		}

		if (parsed < 0)
		{
			parsed_length_ += current_loop_parsed;
			return -1;
		}
		else if (parsed == 0)
		{
			break;
		}
		current_loop_parsed += parsed;
		
	}

	parsed_length_ += current_loop_parsed;
	return current_loop_parsed;
}

FlvManager* RtmpManager::GetFlvManager()
{
	return &flv_manager_;
}

ssize_t RtmpManager::ParseFirstHeader(Buffer* buffer)
{
	if (buffer->ReadableLength() < RTMP_START_PARSE_LENGTH)
	{
		return 0;
	}

	ssize_t parsed_script = ParseScriptPack(buffer);
	if (parsed_script < 0)
	{
		return -1;
	}
	
	ssize_t parsed_video_audio = ParseVideoAudio(buffer);
	if (parsed_video_audio < 0)
	{
		return -1;
	}

	parsed_status_ = PARSE_RTMP_HEADER;
	return parsed_script + parsed_video_audio;
}

ssize_t RtmpManager::ParseScriptPack(Buffer* buffer)
{
	ssize_t result = 0;
	for (;;)
	{
		ssize_t parsed = rtmp_codec_.DecodeHeader(buffer->ReadBegin(), buffer->ReadableLength(), &rtmp_pack_);
		buffer->AddReadIndex(parsed);
		result += parsed;

		if (parsed <= 0)
		{
			return -1;
		}

		if (rtmp_pack_.GetRtmpPackType() != RtmpPack::RTMP_SCRIPT)
		{
			buffer->AddReadIndex(rtmp_pack_.GetDataSize());
			result += rtmp_pack_.GetDataSize();
		}
		else
		{
			break;
		}
	}

	FlvTag* script_tag = flv_manager_.GetScriptTag();

	/*
	��rtmp_pack��header���ֱ��뵽FlvTag�� Ȼ���buffer����data��FlvTag�м��ٿ�������
	*/
	rtmp_pack_.EncodeHeaderToFlvTag(script_tag);
	script_tag->AppendData(buffer->ReadBegin(), script_tag->GetDataSize());
	buffer->AddReadIndex(script_tag->GetDataSize());
	result += script_tag->GetDataSize();

	

	return result;
}

ssize_t RtmpManager::ParseVideoAudio(Buffer* buffer)
{
	/**
	* ��Obs������ץ��������� ����Ƶ��һ��Tag�����ŵ� ����򻯴��� ����������򷵻ش���
	*/
	ssize_t result = 0;
	ssize_t parsed = 0;

	/**
	 * tagΪһ������Ԫ�ص�����ָ�� ����Ԫ�طֱ�Ϊ��һ����Ƶ����ƵTag
	*/
	FlvTag* tag = flv_manager_.GetVideoAudioTags();

	for (int i = 0; i < 2; ++i)
	{
		parsed = rtmp_codec_.DecodeHeader(buffer->ReadBegin(), buffer->ReadableLength(), &rtmp_pack_);
		buffer->AddReadIndex(parsed);
		result += parsed;
		if (parsed <= 0)
		{
			return -1;
		}

		if (rtmp_pack_.GetRtmpPackType() == RtmpPack::RTMP_AUDIO || 
			rtmp_pack_.GetRtmpPackType() == RtmpPack::RTMP_VIDEO)
		{
			rtmp_pack_.EncodeHeaderToFlvTag(&tag[i]);
			tag[i].AppendData(buffer->ReadBegin(), tag[i].GetDataSize());
			buffer->AddReadIndex(rtmp_pack_.GetDataSize());
			result += rtmp_pack_.GetDataSize();
		}
		else
		{
			// �򻯴��� ����������򷵻ش���
			return -1;
		}
	}

	return result;
}

ssize_t RtmpManager::ParseHeader(Buffer* buffer)
{
	ssize_t parsed = rtmp_codec_.DecodeHeader(buffer->ReadBegin(), buffer->ReadableLength(), &rtmp_pack_);
	if (parsed < 0)
	{
		printf("ParseHeader-DecodeHeader error\n");
		return -1;
	}
	else if (parsed == 0)
	{
		return 0;
	}
	buffer->AddReadIndex(parsed);

	/**
	 * ��ǰchunkδ������ʱ�� ������������� append����ǰ��tag���� ��Ӧ��newһ���µ�
	*/
	if (chunk_over_)
	{
		current_tag_ = new FlvTag;
		rtmp_pack_.EncodeHeaderToFlvTag(current_tag_);
	}
	

	parsed_status_ = RtmpManager::PARSE_RTMP_BODY;

	return parsed;
}

ssize_t RtmpManager::ParseBody(Buffer* buffer)
{
	
	// TODO ���˵�����Ƶ���ݰ������� ���ڹ������趪��body���� ������ʱ��

	uint8_t csid = rtmp_pack_.GetCsid();
	if (csid != MOVIE_CSID)
	{
		if (csid == 3 && rtmp_pack_.GetDataSize() == 31)
		{
			printf("parse maybe success!\n");
			return -1;
		}
		else
		{
			printf("rtmp_pack_.GetCsid: %u != MOVIE_CSID\n", rtmp_pack_.GetCsid());
			return -1;
		}
	}

	size_t readable = buffer->ReadableLength();
	size_t remain = current_tag_->GetRemainDataSize();

	// ֻ���ڶ���һ��chunk�ֿ�4096�ֽں� ���ؽ���һ���µ�header��ʱ��
	// ��remainС�ڵ���RTMP_CHUNK_SIZE��ʱ��˵�� ��chunk�ֿ������
	if (remain <= RTMP_CHUNK_SIZE && read_chunk_size_ == 0)
	{
		chunk_over_ = true;
	}
	else
	{
		chunk_over_ = false;
	}

	if (chunk_over_)
	{
		// ��ǰchunkû�зֿ� �������һ��chunk�ֿ鱻����

		if (readable < remain)
		{
			current_tag_->AppendData(buffer->ReadBegin(), readable);
			buffer->Reset();
			return 0;
		}
		else
		{
			current_tag_->AppendData(buffer->ReadBegin(), remain);
			buffer->AddReadIndex(remain);

			parsed_status_ = RtmpManager::PARSE_RTMP_HEADER;
			return current_tag_->GetDataSize();
		}
	}
	else
	{
		// ��ǰchunk�ֿ�û��ȫ������
		size_t current_chunk_remain = RTMP_CHUNK_SIZE - read_chunk_size_;
		if (readable < current_chunk_remain)
		{
			current_tag_->AppendData(buffer->ReadBegin(), readable);
			buffer->AddReadIndex(readable);
			read_chunk_size_ += readable;
		}
		else
		{
			current_tag_->AppendData(buffer->ReadBegin(), current_chunk_remain);
			buffer->AddReadIndex(current_chunk_remain);

			/* chunk ���� �����ǰchunk�Ѷ��ֽ���*/
			read_chunk_size_ = 0;

			parsed_status_ = RtmpManager::PARSE_RTMP_HEADER;
		}
		// ��ǰchunk�ֿ���� ����chunkδ���� ��Ҫ��������Header �жϽ������Ƕ����ֽ�ҪAppend��Data
		return 0;
	}
}

void RtmpManager::PushBackCurrentFlvTag()
{
	flv_manager_.PushBackFlvTagAndSetPreviousSize(current_tag_);
}
