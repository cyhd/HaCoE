
//====================================================================================-----
//== Tracking Tools DLL Library
//== Copyright NaturalPoint, Inc.
//==
//== The Rigid Body DLL Library is designed to be a simple yet full featured interface to 
//== the Rigid Body Library.  
//==
//====================================================================================-----

#ifndef NPTRACKINGTOOLS_H
#define NPTRACKINGTOOLS_H

//== Includes ========================================================================-----

//== DLL EXPORT/IMPORT PREPROCESSOR DEFINES ==========================================-----

#ifdef NPTRACKINGTOOLS_EXPORTS
#define TTAPI __declspec(dllexport)
#else
#ifndef STATIC_TT_LINK
#define TTAPI __declspec(dllimport)
#else
#define TTAPI  
#endif
#endif

//== NPRIGIDBODY PUBLIC INTERFACE ===================================================-----

#define NPRESULT int                  //== NPRESULT Defines Call Success/Failure ====-----

//== RIGID BODY STARTUP / SHUTDOWN ==================================================-----

TTAPI   NPRESULT TT_Initialize();                      //== Initialize Library =======-----
TTAPI   NPRESULT TT_Shutdown();                        //== Shutdown Library =========-----

//== RIGID BODY INTERFACE ===========================================================-----

TTAPI   NPRESULT TT_LoadCalibration(const char *filename); //== Load Calibration =====----
TTAPI   NPRESULT TT_LoadTrackables (const char *filename); //== Load Trackables ======----
TTAPI   NPRESULT TT_AddTrackables  (const char *filename); //== Add  Trackables ======----
TTAPI   NPRESULT TT_Update();                          //== Process incoming camera data -
TTAPI   NPRESULT TT_UpdateSingleFrame();               //== Process incoming camera data -
TTAPI   NPRESULT TT_LoadProject(const char *filename); //== Load Project File ==========--
TTAPI   NPRESULT TT_SaveProject(const char *filename); //== Save Project File ==========--

//== RIGID BODY STREAMING ===========================================================-----

TTAPI   NPRESULT TT_StreamTrackd(bool enabled);        //== Start/stop Trackd Stream =----
TTAPI   NPRESULT TT_StreamVRPN(bool enabled, int port);//== Start/stop VRPN Stream ===----
TTAPI   NPRESULT TT_StreamNP    (bool enabled);        //== Start/stop NaturalPoint Stream 

//== FRAME ==========================================================================-----

TTAPI   int      TT_FrameMarkerCount();               //== Returns Frame Markers Count ---
TTAPI   float    TT_FrameMarkerX(int index);          //== Returns X Coord of Marker -----
TTAPI   float    TT_FrameMarkerY(int index);          //== Returns Y Coord of Marker -----
TTAPI   float    TT_FrameMarkerZ(int index);          //== Returns Z Coord of Marker -----
TTAPI   double   TT_FrameTimeStamp();                 //== Time Stamp of Frame (seconds) -

//== TRACKABLES CONTROL =============================================================-----

TTAPI   BOOL     TT_IsTrackableTracked(int index); //== Is trackable currently tracked ---
TTAPI   void     TT_TrackableLocation(int RigidIndex,       //== Trackable Index ======---
                        float *x, float *y, float *z,                  //== Position ==---
                        float *qx, float *qy, float *qz, float *qw,    //== Orientation -- 
                        float *yaw, float *pitch, float *roll);        //== Orientation --

TTAPI   void     TT_ClearTrackableList();             //== Clear all trackables   =====---
TTAPI   NPRESULT TT_RemoveTrackable(int Index);       //== Remove single trackable ====---
TTAPI   int      TT_TrackableCount();                 //== Returns number of trackables  -

TTAPI   int      TT_TrackableID(int index);           //== Get Trackables ID ==========---
TTAPI   void     TT_SetTrackableID(int index,int ID); //== Set Trackables ID ==========---
TTAPI   const char* TT_TrackableName(int index);      //== Returns Trackable Name =====---

TTAPI   void     TT_SetTrackableEnabled(int index, bool enabled);    //== Set Tracking ---
TTAPI   BOOL     TT_TrackableEnabled(int index);                     //== Get Tracking ---

TTAPI   int      TT_TrackableMarkerCount(int index);             //== Get marker count ---
TTAPI   void     TT_TrackableMarker(int RigidIndex,              //== Get Trackable mrkr -
                        int MarkerIndex, float *x, float *y, float *z);

                 //== TT_CreateTrackable.  This creates a trackable based on the marker
                 //== count and marker list provided.  The MarkerList is a expected to
                 //== contain of list of marker coordinates in the order: x1,y1,z1,x2,
                 //== y2,z2,etc...xN,yN,zN.

TTAPI   NPRESULT TT_CreateTrackable(const char* Name, int ID, int MarkerCount,
                                    float *MarkerList);

//== POINT CLOUD INTERFACE ==========================================================-----

TTAPI   int      TT_CameraCount();                    //== Returns Camera Count =====-----
TTAPI   float    TT_CameraXLocation(int index);       //== Returns Camera's X Coord =-----
TTAPI   float    TT_CameraYLocation(int index);       //== Returns Camera's Y Coord =-----
TTAPI   float    TT_CameraZLocation(int index);       //== Returns Camera's Z Coord =-----
TTAPI   float    TT_CameraOrientationMatrix(int camera, int index); //== Orientation -----

TTAPI   const char* TT_CameraName(int index);         //== Returns Camera Name ======-----

TTAPI   int      TT_CameraMarkerCount(int CameraIndex); //== Camera's 2D Marker Count =---

                 //== CameraMarker fetches the 2D centroid location of the marker as seen
                 //== by the camera.

TTAPI   bool     TT_CameraMarker(int CameraIndex, int MarkerIndex, float &x, float &y);

                 //== Fetch predistorted marker location.  This is basically where the
                 //== camera would see the marker if there was no lens distortion.
                 //== For most of our cameras/lenses, this location is only a few pixels
                 //== from the distorted (TT_CameraMarker) position.

TTAPI   bool     TT_CameraMarkerPredistorted(int CameraIndex, int MarkerIndex, float &x, float &y);

                 //== Set camera settings.  This function allows you to set the camera's
                 //== video mode, exposure, threshold, and illumination settings.
                 
                 //== VideoType:  
                 //==     0 = Segment Mode   
                 //==     1 = Grayscale Mode 
                 //==     2 = Object Mode    
                 //==     4 = Precision Mode
                 //==     6 = MJPEG Mode     (V100R2 only)

                 //== Exposure: Valid values are:  1-480
                 //== Threshold: Valid values are: 0-255
                 //== Intensity: Valid values are: 0-15  (This should be set to 15 for all most all
                 //==                                     situations)
                  

TTAPI   bool     TT_SetCameraSettings(int CameraIndex, int VideoType, int Exposure, int Threshold, int Intensity);

                 //== Toggle camera extended options

TTAPI   bool     TT_SetCameraFilterSwitch(int CameraIndex, bool EnableIRFilter);
TTAPI   bool     TT_SetCameraAGC(int CameraIndex, bool EnableAutomaticGainControl);
TTAPI   bool     TT_SetCameraAEC(int CameraIndex, bool EnableAutomaticExposureControl);
TTAPI   bool     TT_SetCameraHighPower(int CameraIndex, bool EnableHighPowerMode);
TTAPI   bool     TT_SetCameraMJPEGHighQuality(int CameraIndex, int MJPEGQuality);


                 //== Fetch the cameras frame buffer.  This function fills the provided
                 //== buffer with an image of what is in the camera view.  The resulting
                 //== Image depends on what video mode the camera is in.  If the camera
                 //== is in grayscale mode, for example, a grayscale image is returned
                 //== from this call.

TTAPI   bool     TT_CameraFrameBuffer(int CameraIndex, int BufferPixelWidth, int BufferPixelHeight,
                                      int BufferByteSpan, int BufferPixelBitDepth, unsigned char *Buffer);

//== RESULT PROCESSING ==============================================================-----

TTAPI   const char *TT_GetResultString(NPRESULT result); //== Return Plain Text Message --

#define NPRESULT_SUCCESS 0                            //== Successful Result ======-------


#endif