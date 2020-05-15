#include "XiWave.h"

bool XiWave::readwav(char* filename) {

    fstream wavfile;
    wav_struct WAV;
    wavfile.open(filename, ios::binary | ios::in);

    wavfile.seekg(0x14);
    wavfile.read((char*)&WAV.encodinformat, sizeof(WAV.encodinformat));
    WAVdata.encodinformat = WAV.encodinformat;

    if (WAVdata.encodinformat != 1) {
        cout << "非PCM编码" << endl;
        return false;
    }

    wavfile.seekg(0x16);
    wavfile.read((char*)&WAV.channel, sizeof(WAV.channel));
    WAVdata.channel = WAV.channel;

    wavfile.seekg(0x18);
    wavfile.read((char*)&WAV.frequency, sizeof(WAV.frequency));
    WAVdata.frequency = WAV.frequency;

    wavfile.seekg(0x22);
    wavfile.read((char*)&WAV.sample_num_bit, sizeof(WAV.sample_num_bit));
    WAVdata.sample_num_bit = WAV.sample_num_bit;

    wavfile.seekg(0x28);
    wavfile.read((char*)&WAV.data_size, sizeof(WAV.data_size));

    //begin to read data.
    WAV.data = new unsigned char[WAV.data_size];
    WAV.head = new unsigned char[44];

    wavfile.seekg(0x00);
    wavfile.read((char*)WAV.head, sizeof(char) * 44);

    wavfile.seekg(0x2c);
    wavfile.read((char*)WAV.data, sizeof(char) * WAV.data_size);

    cout << "音频通道数  ：" << WAV.channel << endl;
    cout << "采样频率    ：" << WAV.frequency << endl;
    cout << "样本位数    ：" << WAV.sample_num_bit << endl;
    cout << "音频数据大小：" << WAV.data_size << endl;

    for (unsigned long i = 0; i < 44; i++) {
        unsigned char head_sound = WAV.head[i];
        soundhead.push_back(head_sound);
    }
    soundheadsize = 44;
    buf_head_c = &soundhead[0];

    for (unsigned long i = 0; i < WAV.data_size; i++) {
        unsigned char data_sound = WAV.data[i];
        sounddata.push_back(data_sound);
    }

    sounddatasize = WAV.data_size;
    buf_data_c = &sounddata[0];
    wavfile.close();
    delete[] WAV.data;
    return true;
}

void XiWave::writewav(char* filename) {

    fstream wavfile;
    unsigned char head_sound;
    unsigned char data_sound;

    wavfile.open(filename, ios::binary | ios::out | ios::trunc);

    for (int i = 0; i < soundheadsize; i++) {
        head_sound = soundhead[i];
        wavfile.write((char*)&head_sound, sizeof(char));
    }


    for (int j = 0; j < cutstart[0]; j++) {
        data_sound = sounddata[j];
        wavfile.write((char*)&data_sound, sizeof(char));
    }
    for (int i = 0; i < cutsize - 1; i++) {
        for (int j = cutstop[i]; j < cutstart[i + 1]; j++) {
            data_sound = sounddata[j];
            wavfile.write((char*)&data_sound, sizeof(char));
        }
    }
    for (int j = cutstop[cutsize - 1]; j < sounddatasize; j++) {
        data_sound = sounddata[j];
        wavfile.write((char*)&data_sound, sizeof(char));
    }


    wavfile.close();

}

void XiWave::cutpro() {
    bool tag = true;
    cutsize = 0;
    for (int i = 0; i < sounddatasize; i += 40000) {
        if (tag) {
            cutstart.push_back(i);
            tag = false;
        }
        else {
            cutstop.push_back(i);
            cutsize++;
            tag = true;
        }
    }
    if (!tag) {
        cutstop.push_back(sounddatasize);
        cutsize++;
    }
}