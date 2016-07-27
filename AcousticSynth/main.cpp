//
//  main.cpp
//  AcousticSynth
//
//  Created by 池田央 on 2016/07/27.
//  Copyright © 2016年 池田央. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#import "sndfile.h"


#define SAMPLE_RATE 44100 //sampling rate
#define LENGTH 5 //seconds
#define AMPLITUDE 1.0// 16bit
#define CHANNEL_NUM 1//channel
#define OUT_FILE "OutWave.wav"

int main(void)

{
    int F0 = 440;
    int mode = 1;
    int n = 20;
    SNDFILE *fp;
    SF_INFO sfinfo;
    
    float *buffer = (float*)malloc(CHANNEL_NUM * SAMPLE_RATE * LENGTH * sizeof(float));
    
    memset(&sfinfo, 0, sizeof(sfinfo));
    
    sfinfo.samplerate = SAMPLE_RATE;
    sfinfo.frames = SAMPLE_RATE*LENGTH;
    sfinfo.channels = CHANNEL_NUM;
    sfinfo.format = (SF_FORMAT_WAV | SF_FORMAT_PCM_16);
    
    if(!(fp = sf_open(OUT_FILE, SFM_WRITE, &sfinfo))){
        printf("Error : Could not open output file\n");
        return 1;
    }
    
    for(int t = 0; t < SAMPLE_RATE*LENGTH; t++){ // STEREO
        for(int c = 0; c < CHANNEL_NUM; c++){
            if(mode == 0){
                buffer[CHANNEL_NUM*t + c] = AMPLITUDE * sin(2.0 * M_PI * ((float)F0 / SAMPLE_RATE) * t);
            }else if (mode == 1){
                
                float normal = 0;
                
                for (int k = 1; k <= n; k++) {
                    normal += (-4.0*sin(M_PI * k) + 8.0*sin(M_PI*k/2.0))/(pow(k*M_PI, 2)) * sin(2*M_PI*k*((float)F0 / SAMPLE_RATE)*t);
                }
                
                buffer[CHANNEL_NUM*t + c] = AMPLITUDE * normal;
                
            }
        }
    }
    
    if(sf_write_float(fp, buffer, sfinfo.channels * SAMPLE_RATE*LENGTH) != sfinfo.channels * sfinfo.frames){
        puts(sf_strerror (fp));
    }
    
    sf_close(fp);
    
    return 0;
}