#ifndef CORE_FLVMANAGER_H
#define CORE_FLVMANAGER_H

#include <vector>

#include "server/codec/FlvCodec.h"
#include "utils/File.h"
#include "utils/Buffer.h"

/**
 * ���ڹ���FlvHeader��FlvTag����
*/

/**
 * @brief �ļ���ȡ����������
*/
constexpr size_t BUFFER_SIZE = 30000;
class FlvManager
{
public:
	enum ParseStatus
	{
		TAG_HEADER,
		TAG_DATA
	};

	FlvManager();
	FlvManager(const std::string& file);
	~FlvManager();

	bool SetFilePath(const std::string& file);

	/**
	 * @brief �����ļ�
	 * @param parse_length ����������
	 * @return �ѽ������� -1���� ���򷵻ؽ�������
	*/
	ssize_t ParseFile(size_t parse_length);
	
	/**
	 * @brief ��ȡScriptTag��ָ��
	 * @return 
	*/
	FlvTag* GetScriptTag();

	/**
	 * @brief video_audio_tags ָ��
	 * @return 
	*/
	FlvTag* GetVideoAudioTags();

	void PushBackFlvTagAndSetPreviousSize(FlvTag* flv_tag);
private:

	FlvCodec codec_;

	File file_;

	/* ÿ��Flv�ļ����ҽ���һ��*/
	FlvHeader flv_header_;

	FlvTag script_tag_;

	/* ��һ����Ƶ����Ƶtag�洢�ű�����Ϣ ����ʹ����Ҫ�����洢*/
	FlvTag video_audio_tags[2];

	/* ��ǰ���ڴ����tag*/
	FlvTag* current_tag_;

	/* ��һ���������tag*/
	FlvTag* last_tag_;

	/* tag����*/
	std::vector<FlvTag*> flv_tags_;


	ParseStatus parse_status_;

	/* �Ѿ������ĳ���*/
	size_t parsed_length_;

	Buffer buffer_;

	size_t ReadDataFromFile();

	ssize_t ParseHeader();
	ssize_t ParseScripTag();
	ssize_t ParseVideoAudio();
	ssize_t ParseTagHeader(FlvTag* tag);
	ssize_t ParseTagData(FlvTag* tag);

	bool CheckTag();
};

#endif // !CORE_FLVMANAGER_H
