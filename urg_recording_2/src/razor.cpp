//
//  razor.cpp
//  Razor_AHRS_test4
//
//  Created by Ben Snell on 11/14/15.
//
//

#include "razor.h"

// setup serial connection to razor
void razor::connect(string port_name, int baud_rate){
    
    portName = port_name;
    baudRate = baud_rate;
    
    serial.setup(portName, baudRate);
    
    nTimesRead = 0;
    
    while(!serial.available() ){
        nTimesRead++;

        if(nTimesRead>timeout_count) {
            cout << "Could not connect to razor" << endl;
            return;
        }
    }
}

// ---------------------------------------------------------------------

// reconnect to serial
void razor::reconnect() {
    serial.setup(portName, baudRate);
    
    nTimesRead = 0;
    
    while(!serial.available() ){
        nTimesRead++;
        
        if(nTimesRead>timeout_count) {
            cout << "Could not connect to razor" << endl;
            return;
        }
    }
}

// ---------------------------------------------------------------------

// read incoming data from serial (Yaw, Pitch, Roll)
bool razor::readYPR() {
    
    // if the watchdog is greater than the timeout_wdt
    if (watchDogTimer > timeout_wdt) {
        
        cout << "watch dog triggered" << endl;
        
        // reconnect to device
        serial.setup(portName, baudRate);
        
        nTimesRead = 0;
        
        while(!serial.available() ){
            nTimesRead++;
            
            if(nTimesRead>timeout_count) {
                cout << "Could not connect to razor" << endl;
                return;
            }
        }
        
        // reset timer
        watchDogTimer = 0;
        
        return false;
    }
    
    // if the serial isnt working, don't update the values but do return false
    if (!serial.isInitialized() || !serial.available()) {
        watchDogTimer++; // increment watch dog
        return false;
    }
    
    int nRead = 0; // temp variable keeps count per read
    serial.drain();
    buf.clear();
    
    // wait until the header symbol (#) is read to align the message
    nTimesRead = 0;
    while( (nRead = serial.readByte()) != '#'){
        nTimesRead++;
        
        if(nTimesRead>timeout_count) {
            cout << "Razor is not sending YPR" << endl;
            return;
        }
    }
    
    // read the line until the carriage return
    nTimesRead = 0;
    while( (nRead = serial.readByte()) != '\n'){
        
        nTimesRead++;
        if(nRead > 0){
            buf.push_back(nRead);   // add last int read to the buf
        }
        
        if(nTimesRead>timeout_count) {
            cout << "Could not read YPR" << endl;
            return;
        }
    }

    // if the data matches the expected format, store it in the yaw, pitch and roll variables
    if (sscanf(buf.data(), "YPR=%f,%f,%f", &yaw, &pitch, &roll) == 3) {
        
        // update the time the last message was read
        readTime = ofGetElapsedTimef();
        
        // update array too
        lastYPR[0] = yaw;
        lastYPR[1] = pitch;
        lastYPR[2] = roll;
        
        // what if not all yaw pitch and roll were assigned? They were still put into the variables... Do we restart the serial connection at that point?
    }
    
    // reset watch dog timer because data has been successfully retrieved
    watchDogTimer = 0;
    
    return true;
}
