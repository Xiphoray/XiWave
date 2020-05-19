#include "XiWave.h"

bool XiWave::readwav(char* filename) {

    fstream wavfile;
    wav_struct WAV;
    wavfile.open(filename, ios::binary | ios::in);


    unsigned short bit = 0x00;


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

    bit = 0x22;

    char tt[4];

    while (1) {
        bit += 0x2;
        wavfile.seekg(bit);
        wavfile.read((char*)&tt, sizeof(tt));

        if (tt[0] == 'd' && tt[1] == 'a' && tt[2] == 't' && tt[3] == 'a') {
            break;
        }
    }
    bit += 4;

    wavfile.seekg(bit);
    wavfile.read((char*)&WAV.data_size, sizeof(WAV.data_size));

    bit += 4;
    //begin to read data.
    WAV.data = new char[WAV.data_size];
    WAV.head = new char[bit];

    wavfile.seekg(0x00);
    wavfile.read((char*)WAV.head, sizeof(char) * bit);

   
    wavfile.seekg(bit);
    wavfile.read((char*)WAV.data, sizeof(char) * WAV.data_size);

    cout << "音频通道数  ：" << WAV.channel << endl;
    cout << "采样频率    ：" << WAV.frequency << endl;
    cout << "样本位数    ：" << WAV.sample_num_bit << endl;
    cout << "音频数据大小：" << WAV.data_size << endl;

    for (unsigned long i = 0; i < bit; i++) {
        char head_sound = WAV.head[i];
        soundhead.push_back(head_sound);
    }
    soundheadsize = bit;
    buf_head_c = &soundhead[0];

    for (unsigned long i = 0; i < WAV.data_size; i++) {
        char data_sound = WAV.data[i];
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
    char head_sound;
    char data_sound;

    wavfile.open(filename, ios::binary | ios::out | ios::trunc);

    for (int i = 0; i < soundheadsize; i++) {
        head_sound = soundhead[i];
        wavfile.write((char*)&head_sound, sizeof(char));
    }

    for (long int j = 0; j < sounddatasize; j++) {
        data_sound = sounddata[j];
        wavfile.write((char*)&data_sound, sizeof(char));
    }

    wavfile.close();
}


void XiWave::OneC8bit() {
    int value;
    char data_sound;
    for (long int i = 0; i < sounddatasize; i++) {
        data_sound = sounddata[i];
        value = (int)data_sound;
        //TODO: 后续操作

    }

}

void XiWave::TwoC8bit() {
    int valuel,valuer;
    char data_soundl, data_soundr;
    for (long int i = 0; i < sounddatasize; i++) {
        data_soundl = sounddata[i];
        data_soundr = sounddata[++i];
        valuel = (int)data_soundl;
        valuer = (int)data_soundr;
        //TODO: 后续操作

    }
}

void XiWave::OneC16bit() {
    int value;
    char data_soundhigh, data_soundlow;
    for (long int i = 0; i < sounddatasize; i++) {
        data_soundlow = sounddata[i];
        data_soundhigh = sounddata[++i];
        value = (int)(data_soundlow | data_soundhigh << 8);
        //TODO: 后续操作

    }
}

void XiWave::TwoC16bit() {
    int valuel, valuer;
    char data_lhigh, data_llow, data_rhigh, data_rlow;
    for (long int i = 0; i < sounddatasize; i++) {
        data_llow = sounddata[i];
        data_lhigh = sounddata[++i];
        data_rlow = sounddata[++i];
        data_rhigh = sounddata[++i];
        valuel = (int)(data_llow | data_lhigh << 8);
        valuer = (int)(data_rlow | data_rhigh << 8);
        //TODO: 后续操作

    }
}