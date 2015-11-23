//
//  systemVolumeThread.h
//  urg_recording_2
//
//  Created by Ben Snell on 11/14/15.
//
//

#ifndef __urg_recording_2__systemVolumeThread__
#define __urg_recording_2__systemVolumeThread__

#include "ofMain.h"
#include "systemVolume.h"

class systemVolumeThread : public ofThread {
    
//    ...
    
public:
    
    // the thread function
    void threadedFunction() {
        
        // start
        
        while(isThreadRunning()) {
            
            // lock access to the resource
//            lock();
            
            myVol.readMute();
            
            // done with the resource
//            unlock();

        }
        
        // done
    }
    
    systemVolume myVol;
    
};

#endif /* defined(__urg_recording_2__systemVolumeThread__) */
