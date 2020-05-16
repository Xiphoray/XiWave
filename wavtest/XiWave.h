#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>

using namespace std;

struct wav_struct
{
	unsigned short encodinformat; //�����ʽ
	unsigned short channel;             //ͨ����
	unsigned long frequency;            //����Ƶ��
	unsigned short sample_num_bit;      //һ��������λ��
	unsigned long data_size;            //���ݳ���
	unsigned char* data;                         //��Ƶ���� 
	unsigned char* head;
};

class XiWave
{
public:

	bool readwav(char* filename);
	void writewav(char* filename);

	void OneC8bit();
	void TwoC8bit();
	void OneC16bit();
	void TwoC16bit();

	char* get_data_buf() {
		return buf_data_c;
	}
	int get_data_size() {
		return sounddatasize;
	}
	char* get_head_buf() {
		return buf_head_c;
	}
	int get_head_size() {
		return soundheadsize;
	}

private:

	int sounddatasize;
	char* buf_data_c;
	vector<char> sounddata;

	int soundheadsize;
	char* buf_head_c;
	vector<char> soundhead;

	int cutsize = 0;
	vector<long int> cutstart;
	vector<long int> cutstop;

	wav_struct WAVdata;
};

