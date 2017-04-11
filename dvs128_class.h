/*
 * dvs128_class.h
 *
 *  Created on: 2017-4-10
 *      Author: root
 */
#ifndef DVS128_CLASS_H
#define DVS128_CLASS_H

#include <libcaer/libcaer.h>
#include <libcaer/devices/dvs128.h>
#include <queue>

using namespace std;

typedef struct
{
   int x;
   int y;
   float x_undist;
   float y_undist;
   float t;
   float polarity;
}Event;

class DVSCameraWorker
{
public:
   DVSCameraWorker() = default;
   void run();

protected:
   bool init(void);
   void deinit(void);
   std::vector<Event> events_buffer;
   caerDeviceHandle dvs128_handle;
};

#endif
