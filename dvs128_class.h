/*****************************************************************************
*******************Ibrahim,CBICR,Tsinghua University**************************
******************************************************************************
File name:    dvs128_class.h
Description:  the header file of dvs128_class.cpp
Author:       Ibrahim Yang
Version:      V1.0
Date:         2017-4-12
History:      inilabs->libcaer VLOGroup->dvs-reconstruction libusb-1.0
*****************************************************************************/
#ifndef DVS128_CLASS_H
#define DVS128_CLASS_H

#include <libcaer/libcaer.h>
#include <libcaer/devices/dvs128.h>
#include <iostream>
#include <queue>

using namespace std;

//#define time_window ./

/*************************************************
Struct:       Event
Description:  the struct save the event data for dvs128
Calls:        none
Called By:    DVSCameraWorker::run()
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
typedef struct
{
   int x;
   int y;
   float t;
   bool polarity;
}Event;

/*************************************************
Class:        DVSCameraWorker
Description:  the class for dvs128
Calls:        none
Called By:    functions in dvs128_class.cpp
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
class DVS128_Processer
{
public:
   DVS128_Processer() = default;
   void dvs128_run();
   void dvs128_run_single();
   void dvs128_stop(void);

protected:
   bool dvs128_init(void);
   void dvs128_deinit(void);
   std::vector<Event> events_buffer;
   caerDeviceHandle dvs128_handle;
   bool running_ = true;
   double current_time = 0;
};

#endif
