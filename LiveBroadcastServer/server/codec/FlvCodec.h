#ifndef UTILS_CODEC_FLVCODEC_H
#define UTILS_CODEC_FLVCODEC_H

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>

constexpr int FLV_HEADER_LENGTH = 9;
constexpr int FLV_TAG_HEADER_LENGTH = 15;

class FlvHeader
{
public:
	
	FlvHeader() :
		flv_(),
		version_(0),
		type_flag_(0),
		header_length_(0)
	{
	}

	void SetInfo(const std::string& flv, uint8_t version, uint8_t type_flag,
		uint32_t header_length)
	{
		flv_ = flv;
		version_ = version;
		type_flag_ = type_flag;
		header_length_ = header_length;
	}

	std::string DebugInfo()
	{
		std::string result = "flv: " + flv_ + 
			",version: " + std::to_string(version_) + 
			",type_flag: " + std::to_string(type_flag_) + 
			",header_length: " + std::to_string(header_length_);

		return result;
	}

private:
	std::string flv_; // FLV
	uint8_t version_; // 1
	uint8_t type_flag_; // 5
	uint32_t header_length_; // 9
};

class FlvTag
{
public:

	FlvTag();
	~FlvTag();

	/**
	 * @brief ��ȡ�����ݰ���data_����
	 * @return 
	*/
	uint32_t GetDataSize() const;

	/**
	 * @brief ��ȡ��һ�����ݰ��ĳ��� ���в�����previous_tag_size���ĸ��ֽ� ����DataSize
	 * @return 
	*/
	uint32_t GetPreviousTagSize() const;

	/**
	 * @brief ��������GetPreviousTagSize�ĵ�ǰ���ݰ��汾 ���ڽ���У��
	 * @return 
	*/
	uint32_t GetTagSize() const;
	uint32_t GetCurrentDataSize() const;

	void SetData(const char* data, size_t length);
	void AppendData(const char* data, size_t length);

	ssize_t DecodeTagHander(const char* data, size_t length);

	size_t EncodeTag(const char* buffer, size_t length);

	void Init();

private:
	uint32_t previous_tag_size_; // ����previous_tag_size  sizeof ��һ��Tag - 4
	uint8_t tag_type_; // ��Ƶ 8 ��Ƶ 9 scripts 18
	uint8_t data_size_[3]; // AudioTag VideoTag �����ݳ��� ��stream_id��ʼ����
	uint8_t timestamp_[3];
	uint8_t timestamp_extend_;
	uint8_t stream_id_[3]; // 0

	std::string data_;
};

class FlvCodec
{
public:
	FlvCodec() = default;
	~FlvCodec() = default;

	ssize_t DecodeFileHeader(const char* data, size_t length, FlvHeader* tag);
	ssize_t DecodeTagHander(const char* data, size_t length, FlvTag* tag);

private:
};

#endif // !UTILS_CODEC_FLVCODEC_H
