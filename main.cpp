/*****************************************************************************
*******************Ibrahim,CBICR,Tsinghua University**************************
******************************************************************************
File name:    main.cpp
Description:  the header file of dvs128_class.cpp
Author:       Ibrahim Yang
Version:      V1.0
Date:         2017-4-12
History:      inilabs->libcaer VLOGroup->dvs-reconstruction libusb-1.0
*****************************************************************************/
#include "dvs128_class.h"

/*************************************************
Function:     int main()
Description:  main function
Calls:        ...
Called By:    none
Input:        none
Output:       none
Return:       int
Others:       none
*************************************************/
int main()
{
	DVSCameraWorker myDVSCameraWorker;
	myDVSCameraWorker.run();
	
	return(0);
}
