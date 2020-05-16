
#include "XiWave.h"


int main()
{
    XiWave test;
    char filename[] = "E:/project_cpp/wavtest/Debussy_SonataViolinPianoGMinor-02_111_20080519-SMD-ss135-189.wav";
    char outputname[] = "E:/project_cpp/wavtest/test.wav";
    if (!test.readwav(filename)) {
        cout << "error" << endl;
        return 0;
    }

    test.writewav(outputname);
   
}


