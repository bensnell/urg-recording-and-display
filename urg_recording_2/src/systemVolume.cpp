//
//  systemVolume.cpp
//  urg_recording_2
//
//  Created by Ben Snell on 11/14/15.
//
//

#include "systemVolume.h"

Boolean systemVolume::readMute() {
    
    // check to see if the system is muted 10 times per second
    // note: significantly reduces speed of system.
    // question: how do we run this in a different thread?
    
    refTime = floor(ofGetElapsedTimeMillis() % 1000);
    
    // if it's been 100 ms, update the system state
    if (refTime != prevRefTime) {
        
        string argument;
        
        argument = ofToString(ofSystem("osascript -e 'output muted of (get volume settings)'"));
        
        char muteChar = ofToChar(argument);
        
        if (muteChar == 't') muteState = true;
        if (muteChar == 'f') muteState = false;
    }
    
//    cout << ofToString(prevRefTime) + "  " + ofToString(refTime) << endl;
    
    // updated muted and unmuted flags:
    
    // transition from muted to not muted indicates start of recording
    if (!muteState && prevMuteState) {
        bUnmuted = true;
    }
    
    // transition to muted signals end of recording
    else if (muteState && !prevMuteState) {
        bMuted = true;
    }
    
    // set previous mute state
    prevMuteState = muteState;
    
    // set previous time
    prevRefTime = refTime;
    
    return muteState;
}

// ----------------------------------------------------------------------

// destructive flag "caller"
Boolean systemVolume::muted() {
    if (bMuted) {
        bMuted = false;
        return true;
    } else {
        return false;
    }
}

// ----------------------------------------------------------------------

// destructive flag "caller"
Boolean systemVolume::unmuted() {
    if (bUnmuted) {
        bUnmuted = false;
        return true;
    } else {
        return false;
    }
}



