//
//  urgDisplay.h
//  urg_capture_display
//
//  Created by Ben Snell on 11/16/15.
//
//

#ifndef __urg_capture_display__urgDisplay__
#define __urg_capture_display__urgDisplay__

#include "ofMain.h"
#include "ofxCsv.h"
//#include "ofEvents.h"

struct person {
    
public:
    int nDatums = 0; // number of points
    float startScan; // beginning scan frame
    float runningAvgY = 0; // to use to calculate if person has ended
    Boolean updatedFlag = false;
    
};

class urgDisplay {
    
public:
    
    // ---------------------------
    // ---------- SETUP ----------
    // ---------------------------
    
    ofMesh pointMesh;
    void setupPointMesh();
    
    ofMesh triMesh;
    void setupTriangleMesh();
    
    // set params of the scan
    void setMeshParameters(int zScale_, int minIndex_, int maxIndex_, double minSqDist2Cam_, double maxSqTriLength_ = 100000.);
    
    int zScale;             // SPEED: distance in mm traveled per second
    int minIndex = 0;       // low bound index of point along one scan, inclusive
    int maxIndex = 682;     // high bound index of point along one scan, exclusive (there are 726 total indices, but the last chunk contains nothing, so use 682)
    double minSqDist2Cam = 70000.;    // discard all points closer than this squared distance to camera
    double maxSqTriLength = 100000.;  // for triangle mesh: only create triangles with sides less than this length
    
    void loadData(string folderName_, int fileNumber_);
    wng::ofxCsv csv;
    int nScans;
    
    string getFilePath(string folderName_, int fileNumber_);
    string fileName;
    string filePath;
    
    // ---------------------------
    // -------- FILL MESH --------
    // ---------------------------
    
    int countZero = 0;

    // fill point mesh using csv data in the format (where z is the row counter):
    // x0, y0, z0, x1, y1, z1, ...
    void fillPointMeshXYZ();
    
    // fill point mesh using csv data in the format (where z is the row counter):
    // x0, y0, z0, x1, y1, z1, ...
    void fillTriangleMeshXYZ();
    
    // fill point mesh using csv data in the format:
    // time, yaw, pitch, roll, x0, y0, x1, y2, ...
    void fillPointMeshTWPRXY(float speed_, float alignYaw_, float alignPitch_, float alignRoll_, bool bCorrectGait);
    float speed;    // speed of observer recorder (mm / second)
    float alignYaw, alignPitch, alignRoll; // of the virtual position of the lidar scanner (facing toward positive x in xy plane) to the actual physical position of the lidar scanner with respect to the URG
    
    // fill point mesh using csv data in the format:
    // time, x0, y0, x1, y1, ...
    void fillPointMeshTXY(int startScan, int endScan, bool bTimeDependent);
    
    // fill point mesh with slightly transparent points outside of given bounds
    void fillPointMeshTXYGhosted(int xMin, int xMax, int yMin, int yMax, float transparency, Boolean timeDependent);
    // two meshes:
    void fillPointMeshTXYGhostedDouble(int xMin, int xMax, int yMin, int yMax, float transparency, Boolean timeDependent);
    ofMesh pointMeshOpaque;
    ofMesh pointMeshTransparent;
    
    // array stores the best points to mirror across and resample the bad side
    // initialize all elements to zero
    ofVec3f mirrorPoints[682] = {ofVec3f(0., 0., 0.)};
    
    // find people in a scene
    void findPeople(int xMin, int xMax, int yMin, int yMax, int minPointCount, int maxDistContinuity);
    // min point count = number of points within bounds that can be considered part of a person
    // max distance of continuity = maximum distance that two average y values can be away from each other to be considered part of the same person
    // vector to hold people:
    vector<person> people;
    
    // fill a spherical mesh with timestamped data
    // speed is angle in degrees per second
    // the min and max periods are 0. and 1. for only showing the first pass at a capture
    void fillPointMeshTXYSpherical(float speed, float period, bool bClockwise, float startingPeriod, float numPeriods, float alignmentAngle);
    
    
    // ---------------------------
    // ------ DISPLAY MESH -------
    // ---------------------------

    void drawPointMeshLinear(float scale, float slide, float zRotation);
    
    void drawTriangleMeshXYZ(float scale, float slide);
    
    void drawPointMeshTWPRXY(float scale, float slide);
    
    // only uses meshes (vector) not mesh
    void drawOrthoGhostedTimeline(float zRotation, float scale, float slide);
    
    void drawGhostedPointMeshes(float scale, float slide, float xRotation, float yRotation, float zRotation, bool bOrtho, bool bRotateControl, bool bSlideControl);
    float currentLocation = 0;
    float destination = 0;
    int pplCount = 0;
    
    void drawPointMeshSpherical(float scale, float slide);
    
    ofCamera camera;
    
    ofEasyCam easyCam;
    
    void setKeyPressed(int key);
    int thisKey;
    int getKeyPressed();
    
//    void update();
};

#endif /* defined(__urg_capture_display__urgDisplay__) */
