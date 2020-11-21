#ifndef UTILS_CODEC_FLVCODEC_H
#define UTILS_CODEC_FLVCODEC_H

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>

/**
 * ����Flv�ļ��ı���ͽ���
 * 
 * ��Flv�ļ������������� FlvTag Vector
 * ��FlvTag�����char* ������
*/

/**
 * @brief λ��Flv�ļ�ͷ ���ҽ���һ��
*/
class FlvHeader
{
public:
	
	/**
	* @brief FlvHeader����
	*/
	const static int FLV_HEADER_LENGTH = 9;
	static char DEFAULT_HEADER[];

	FlvHeader() :
		flv_("FLV"),
		version_(1),
		type_flag_(5),
		header_length_(0x09000000) // 9�Ĵ����
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

	ssize_t EncodeToBuffer(char* data, size_t length);

private:
	std::string flv_; // FLV
	uint8_t version_; // 1
	uint8_t type_flag_; // 5
	uint32_t header_length_; // 9
};

/**
 * @brief ����ÿ��tag��data���� ���Ӵ���ᾭ�������Ŀ���
 * �����ʵ�������ڼ��ٿ��������İ�װ
 * 
 * TODO �滻����Ч�� std::string
*/
class FlvTagBody
{
public:
	FlvTagBody() = default;
	~FlvTagBody() = default;

	/*FlvTagZeroCopy(const FlvTagZeroCopy& rhs) = delete;
	FlvTagZeroCopy& operator=(const FlvTagZeroCopy& rhs) = delete;*/

	const std::string* GetBody() const;

	size_t GetBodySize() const;

	void AppendData(const char* data, size_t length);
private:

	std::string body_;
};

/**
 * @brief FlvTag Flv�ļ�Tag ��Tagͷ��Tag Data
 * ��Flv�ļ�FlvHeader�� ΪFlvTag�ļ���
*/
class FlvTag
{
public:

	/**
	 * @brief FlvTagHeader����
	*/
	const static int FLV_TAG_HEADER_LENGTH = 15;

	FlvTag();
	~FlvTag();

	/**
	 * @brief ��ȡ�����ݰ� ͷ���ݲ��ֱ�ʶ�ĵ�data_����
	 * @return 
	*/
	uint32_t GetDataSize() const;

	/**
	 * @brief ��ȡ��һ�����ݰ��ĳ��� ���в�����previous_tag_size���ĸ��ֽ� ����DataSize
	 * @return 
	*/
	uint32_t GetPreviousTagSize() const;

	/**
	 * @brief ��������GetPreviousTagSize ����Ϊ��ȡ�ĵ�ǰ���ݰ������ݰ����� ���ڽ���У��
	 * @return 
	*/
	uint32_t GetTagSize() const;

	/**
	 * @brief ��ȡ�����Ѿ�����ĵ�data_����
	 * @return 
	*/
	uint32_t GetCurrentDataSize() const;

	/**
	 * @brief ��ȡȱ�ٵ�data�ֽ���
	 * @return 
	*/
	uint32_t GetRemainDataSize() const;

	/**
	 * @brief ׷��data����
	 * @param data dataָ��
	 * @param length ׷�ӵĳ���
	*/
	void AppendData(const char* data, size_t length);

	/**
	 * @brief ���������н�����FlvTag��Header����
	 * @param data ������
	 * @param length ����������
	 * @return �ɹ����ؽ������� �������󷵻�-1 ���ݳ��Ȳ�������0
	*/
	ssize_t DecodeTagHander(const char* data, size_t length);

	const FlvTagBody* GetBody() const;
	const char* GetHeader() const;

	void SetTagType(uint8_t tag_type);

	void SetDataSize(uint8_t* data_size);

	void SetTimeStamp(uint8_t* timestamp);

	void SetSteamId(uint8_t* stream_id);

	void SetPreviousTagSize(uint32_t previous_tag_size);

private:
	//uint32_t previous_tag_size_; // ����previous_tag_size  sizeof ��һ��Tag - 4  ����򱣴�
	//uint8_t tag_type_; // ��Ƶ 8 ��Ƶ 9 scripts 18
	//uint8_t data_size_[3]; // AudioTag VideoTag �����ݳ��� ��stream_id��ʼ����
	//uint8_t timestamp_[3];
	//uint8_t timestamp_extend_;
	//uint8_t stream_id_[3]; // 0

	const static int PREVIOUS_TAG_SIZE_SUB = 0;
	const static int TAG_TYPE_SUB = 4;
	const static int DATA_SIZE_SUB = 5;
	const static int TIMESTAMP_SUB = 8;
	const static int STREAM_ID_SUB = 10;

	const static int PREVIOUS_TAG_SIZE_LENGTH = 4;
	const static int TAG_TYPE_LENGTH = 1;
	const static int DATA_SIZE_LENGTH = 3;
	const static int TIMESTAMP_LENGTH = 3;
	const static int STREAM_ID_LENGTH = 3;

	/* ����header����ж�ο��� ����Ϊ�˼��ٿ������� ֱ�ӱ������л���Ľ��*/
	char header_[15];
	FlvTagBody body_;
};

/**
 * @brief Flv������ ��FlvTag��FlvHeader�����Ͻ��а�װ �ṩ��������
*/
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
