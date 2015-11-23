//
//  systemVolume.h
//  urg_recording_2
//
//  Created by Ben Snell on 11/14/15.
//
//

#ifndef __urg_recording_2__systemVolume__
#define __urg_recording_2__systemVolume__

#include "ofMain.h"

class systemVolume {
    
public:
    
    Boolean readMute();
    
    // only check mic every 10 sec
    int refTime = 0;
    int prevRefTime = 0;
    
    // handles recording state
    Boolean prevMuteState = false;
    Boolean muteState = false;
    
    Boolean muted();    // destructive flag returner
    Boolean unmuted();
    
    
    //    int readVolume();
    
    //    int writeVolume();
    
    
    
private:
    
    Boolean bMuted = false;  // use as flag to other functions
    Boolean bUnmuted = false;

    
};


#endif /* defined(__urg_recording_2__systemVolume__) */
