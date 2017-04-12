/*****************************************************************************
*******************Ibrahim,CBICR,Tsinghua University**************************
******************************************************************************
File name:    dvs128_class.cpp
Description:  define the functions in dvs128_class.h
Author:       Ibrahim Yang
Version:      V1.0
Date:         2017-4-12
History:      inilabs->libcaer VLOGroup->dvs-reconstruction libusb-1.0
*****************************************************************************/
#include "dvs128_class.h"

/*************************************************
Function:     void DVSCameraWorker::run()
Description:  the is the run function to get dvs image
Calls:        DVSCameraWorker::init() DVSCameraWorker::deinit() DVSCameraWorker::stop(void)
Called By:    int main()
Input:        none
Output:       event_buf
Return:       none
Others:       none
*************************************************/
void DVSCameraWorker::run()
{
   int32_t count=0;
   if(init()) {
       while(running_)
       {
           // get event and update timestamps
           caerEventPacketContainer packetContainer = caerDeviceDataGet(dvs128_handle);
           if (packetContainer == NULL) {
               continue; // Skip if nothing there.
           }
           /*this line is important*/
           //events_buffer.clear();
           int32_t packetNum = caerEventPacketContainerGetEventPacketsNumber(packetContainer);
           for (int32_t i = 0; i < packetNum; i++) {
               caerEventPacketHeader packetHeader = caerEventPacketContainerGetEventPacket(packetContainer, i);
               if (packetHeader == NULL) {
                   continue; // Skip if nothing there.
               }
               // Packet 0 is always the special events packet for DVS128, while packet is the polarity events packet.
               if (i == POLARITY_EVENT) {

                   caerPolarityEventPacket polarity = (caerPolarityEventPacket) packetHeader;
                   for (int32_t caerPolarityIteratorCounter = 0; caerPolarityIteratorCounter < caerEventPacketHeaderGetEventNumber(&(polarity)->packetHeader);caerPolarityIteratorCounter++) {
                       caerPolarityEvent caerPolarityIteratorElement = caerPolarityEventPacketGetEvent(polarity, caerPolarityIteratorCounter);
                       if (!caerPolarityEventIsValid(caerPolarityIteratorElement)) { continue; }
                       Event event;
                       event.t = caerPolarityEventGetTimestamp(caerPolarityIteratorElement)*1e-6;
                       event.x = caerPolarityEventGetX(caerPolarityIteratorElement); // don't know why it is other way round?
                       event.y = caerPolarityEventGetY(caerPolarityIteratorElement);
                       event.polarity = caerPolarityEventGetPolarity(caerPolarityIteratorElement);//?1.0f:-1.0f;
                       //if(undistortPoint(event,params.K_cam,params.radial))
                       //printf("event - t: %f, x: %d, y: %d.\n", event.t, event.x, event.y);
                       events_buffer.push_back(event);
                       count++;

                       if (event.t > 0.5)
                       {
                         stop();
                       }
                   }
               }
           }
           caerEventPacketContainerFree(packetContainer);
       }
       deinit();
       auto begin_=events_buffer.begin(),
            end_  =events_buffer.end();
       while(begin_ != end_) 
       {
            ++begin_;
            printf("event - t: %f, x: %d, y: %d.\n", begin_->t, begin_->x, begin_->y);
       }
       printf("the size of events_buf size=%d\n", (int32_t)(events_buffer.size()));
       printf("the size of events_buf capacity=%d\n", (int32_t)(events_buffer.capacity()));
       events_buffer.clear();
   }
}

/*************************************************
Function:     void DVSCameraWorker::run()
Description:  the is the run function to get dvs image
Calls:        DVSCameraWorker::init() DVSCameraWorker::deinit() DVSCameraWorker::stop(void)
Called By:    int main()
Input:        none
Output:       event_buf
Return:       none
Others:       none
*************************************************/
// void DVSCameraWorker::run_simple()
// {
//    if(init()) {
//            caerEventPacketContainer packetContainer = caerDeviceDataGet(dvs128_handle);
//            if (packetContainer == NULL) {
//                continue; // Skip if nothing there.
//            }
//            events_buffer.clear();
//            int32_t packetNum = caerEventPacketContainerGetEventPacketsNumber(packetContainer);
//            for (int32_t i = 0; i < packetNum; i++) {
//                caerEventPacketHeader packetHeader = caerEventPacketContainerGetEventPacket(packetContainer, i);
//                if (packetHeader == NULL) {
//                    continue; // Skip if nothing there.
//                }
//                // Packet 0 is always the special events packet for DVS128, while packet is the polarity events packet.
//                if (i == POLARITY_EVENT) {

//                    caerPolarityEventPacket polarity = (caerPolarityEventPacket) packetHeader;
//                    for (int32_t caerPolarityIteratorCounter = 0; caerPolarityIteratorCounter < caerEventPacketHeaderGetEventNumber(&(polarity)->packetHeader);caerPolarityIteratorCounter++) {
//                        caerPolarityEvent caerPolarityIteratorElement = caerPolarityEventPacketGetEvent(polarity, caerPolarityIteratorCounter);
//                        if (!caerPolarityEventIsValid(caerPolarityIteratorElement)) { continue; }
//                        Event event;
//                        event.t = caerPolarityEventGetTimestamp(caerPolarityIteratorElement)*1e-6;
//                        event.x = caerPolarityEventGetX(caerPolarityIteratorElement); // don't know why it is other way round?
//                        event.y = caerPolarityEventGetY(caerPolarityIteratorElement);
//                        event.polarity = caerPolarityEventGetPolarity(caerPolarityIteratorElement);//?1.0f:-1.0f;
//                        //if(undistortPoint(event,params.K_cam,params.radial))
//                        //printf("event - t: %f, x: %d, y: %d.\n", event.t, event.x, event.y);
//                        events_buffer.push_back(event);
//                        count++;

//                        if (event.t > 1)
//                        {
//                          stop();
//                        }
//                    }
//                }
//            }
//            caerEventPacketContainerFree(packetContainer);
//        deinit();
//        auto begin_=events_buffer.begin(),
//             end_  =events_buffer.end();
//        while(begin_ != end_) 
//        {
//             ++begin_;
//             printf("event - t: %f, x: %d, y: %d.\n", begin_->t, begin_->x, begin_->y);
//        }
//        printf("the size of event buf=%d\n", (int32_t)(events_buffer.size()));
//        printf("the size of event cap=%d\n", (int32_t)(events_buffer.capacity()));
//        events_buffer.clear();
//    }
// }

/*************************************************
Function:     bool DVSCameraWorker::init()
Description:  dvs128 init fuction
Calls:        none
Called By:    void DVSCameraWorker::run()
Input:        none
Output:       dvs128_handle
Return:       bool does dvs init right?
Others:       none
*************************************************/
bool DVSCameraWorker::init()
{
   // init camera
   // Open a DVS128, give it a device ID of 1, and don't care about USB bus or SN restrictions.
   dvs128_handle = caerDeviceOpen(1, CAER_DEVICE_DVS128, 0, 0, NULL);
   if (dvs128_handle == NULL) {
   	   printf("NO dvs128\n");
       return false;
   }
//    // Let's take a look at the information we have on the device.
//    struct caer_dvs128_info dvs128_info = caerDVS128InfoGet(dvs128_handle);

//    printf("%s --- ID: %d, Master: %d, DVS X: %d, DVS Y: %d, Logic: %d.\n", dvs128_info.deviceString,
//        dvs128_info.deviceID, dvs128_info.deviceIsMaster, dvs128_info.dvsSizeX, dvs128_info.dvsSizeY,
//        dvs128_info.logicVersion);
   caerDeviceSendDefaultConfig(dvs128_handle);

   // Values taken from DVS_FAST
   caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_CAS, 1992);
   caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_DIFF, 13125);
   caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_DIFFON, 209996);
   caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_DIFFOFF, 132);
   caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_FOLL, 271);
   caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_INJGND, 1108364);
   caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_PR, 217);
   caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_PUX, 8159221);
   caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_PUY, 16777215);
   caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_REFR, 969);
   caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_REQ, 309590);
   caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_REQPD, 16777215);
   // Values taken from DVS_SLOW
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_CAS, 54);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_DIFF, 30153);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_DIFFON, 482443);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_DIFFOFF, 132);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_FOLL, 51);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_INJGND, 1108364);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_PR, 3);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_PUX, 8159221);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_PUY, 16777215);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_REFR, 6);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_REQ, 159147);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_REQPD, 16777215);

   caerDeviceDataStart(dvs128_handle, NULL, NULL, NULL, NULL, NULL);
   caerDeviceConfigSet(dvs128_handle, CAER_HOST_CONFIG_DATAEXCHANGE, CAER_HOST_CONFIG_DATAEXCHANGE_BLOCKING, true);
   return true;
}

/*************************************************
Function:     void DVSCameraWorker::deinit()
Description:  dvs128 deinit fuction
Calls:        none
Called By:    void DVSCameraWorker::run()
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
void DVSCameraWorker::deinit()
{
   caerDeviceDataStop(dvs128_handle);

   caerDeviceClose(&dvs128_handle);
}

/*************************************************
Function:     bool DVSCameraWorker::init()
Description:  run stop fuction
Calls:        none
Called By:    void DVSCameraWorker::run()
Input:        none
Output:       running_
Return:       none
Others:       none
*************************************************/
void DVSCameraWorker::stop(void)
{
  running_=false;
  printf("DVS128 stop\n");
}