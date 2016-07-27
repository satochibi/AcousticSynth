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
#define AMPLITUDE 0.7// 16bit 0~1
#define CHANNEL_NUM 1//channel
#define OUT_FILE "OutWave.wav"

int main(void)

{
    int F0 = 440;
    int mode = 2;
    int n = 200;
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
    
    //sample loop
    for(int t = 0; t < SAMPLE_RATE*LENGTH; t++){
        //channel loop
        for(int c = 0; c < CHANNEL_NUM; c++){
            if(mode == 0){
                
                buffer[CHANNEL_NUM*t + c] = sin(2.0 * M_PI * ((float)F0 / SAMPLE_RATE) * t);
                
            }else if (mode == 1){
                
                float summation = 0;
                
                for (int k = 1; k <= n; k++) {
                    summation += (1.0/pow(k, 2) * sin(M_PI * k / 2.0) * sin(2.0 * M_PI * k * ((float)F0 / SAMPLE_RATE) * t));
                }
                
                buffer[CHANNEL_NUM*t + c] = 8.0/pow(M_PI, 2) * summation;
                
            }else if(mode == 2){
                
                float summation = 0;
                
                for (int k = 1; k <= n; k++) {
                    summation += sin(2.0 * M_PI * (2.0*k - 1.0) * ((float)F0 / SAMPLE_RATE) * t) / (2.0*k - 1.0);
                }
                
                buffer[CHANNEL_NUM*t + c] = (4.0/M_PI) * summation;
                
            }else if (mode == 3){
                
                float summation = 0;
                
                for (int k = 1; k <= n; k++) {
                    summation += (1.0/k * pow(-1, k+1) * sin(2.0 * M_PI * k * ((float)F0 / SAMPLE_RATE) * t));
                }
                
                buffer[CHANNEL_NUM*t + c] = 2.0/M_PI * summation;

            }
            
            
            
            buffer[CHANNEL_NUM*t + c] *= AMPLITUDE;
        }
        
        
        
    }
    
    if(sf_write_float(fp, buffer, sfinfo.channels * SAMPLE_RATE*LENGTH) != sfinfo.channels * sfinfo.frames){
        puts(sf_strerror (fp));
    }
    
    sf_close(fp);
    
    return 0;
}