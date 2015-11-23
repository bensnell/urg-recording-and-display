//
//  razor.h
//  Razor_AHRS_test4
//
//  Created by Ben Snell on 11/14/15.
//
// Based on sketch found here by cvl-robot:
// http://cvl-robot.hateblo.jp/entry/2013/12/25/183827
//
// Assumes Razor is already configured to automaticaly output YPR values on setup in the form "#YPR=<yaw>,<pitch>,<roll>" over serial at about 50Hz
// If it's not, follow the tutorial here to upload proper code and calibrate:
// https://github.com/ptrbrtz/razor-9dof-ahrs
//
//

#ifndef __Razor_AHRS_test4__razor__
#define __Razor_AHRS_test4__razor__

#include "ofMain.h"

class razor {
    
public:

    ofSerial serial;
    void connect(string port_name, int baud_rate);
    void reconnect();
    
    string portName;
    int baudRate;

    int nTimesRead = 0;
    float readTime = 0; // time last read

    bool readYPR();
    
    float yaw = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;
    
    string buf;     // stores the serial data read
    
    int timeout_count = 100000;
    int timeout_wdt = 200;   // resets after about 3 sec with 60Hz frame rate
    int watchDogTimer = 0;
    
    // stores the last YPR values in an array so other functions can easily access these as they please (same as individual yaw, pitch, and roll)
    float lastYPR [3] = {0.0, 0.0, 0.0};
    
};

#endif /* defined(__Razor_AHRS_test4__razor__) */
