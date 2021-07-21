#include <stdio.h>
#include <stdarg.h>
#include <time.h>


#ifdef LINUX
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif                          // LINUX

#ifdef WIN32
#include <windows.h>
#endif                          // WIN32

#include <chai.h>



int main(int argc, char **argv)
{

	//1 - номер мезонина, 2 - номер канала, 3 - напряжение
	int ret;
	int channel;

  channel = (_u8) 1;

  printf("Trying to initialize chai library from green/main\n");
	if ((ret = CiInit()) < 0) {
		printf("Initialization failed. Error: %d\n", ret);
		exit(1);
	}

  printf("Trying to open CAN channel %d\n", channel);
	ret = CiOpen(channel, CIO_CAN11);
	if (ret < 0)
		printf("Open channel failed. Channel: %d Errcode: %d\n", channel, ret);
	else
		printf("Open channel OK. Channel: %d\n", channel);

  printf("Trying to set baud speed %d\n", channel);
  ret = CiSetBaud(channel, BCI_1M);
  if (ret < 0) {
    printf("Setting baud speed failed. Channel: %d Error: %d\n", channel, ret);
  } else {
    printf("Setting baud speed OK. Channel: %d\n", channel);
  }

  printf("Starting CAN channel %d\n", channel);
  ret = CiStart(channel);
  if (ret < 0) {
    printf("Starting channel failed. Channel: %d Error: %d\n", channel, ret);
  } else {
    printf("Starting channel OK. Channel: %d\n", channel);
  }


  canmsg_t frame;
  msg_zero(&frame);
  frame.id = 0x07;
  frame.flags = 0;
  frame.len = 7;
  frame.data[0] = 0x10;
  frame.data[1] = 3; //номер мезонина
  frame.data[2] = 0x0a; //номер канала
  frame.data[3] = 0xff; //
  frame.data[4] = 0x3f; //
  frame.data[5] = 0;
  frame.data[6] = 0;

  ret = CiWrite(channel, &frame, 1);
  if (ret < 0) {
    printf("Error while sending frame via channel %d\n", channel);
  } else {
    printf("Sending frame OK. Channel: %d\n", channel);
  }

	ret = CiStop(channel);
	if (ret < 0) {
    printf("Error stop %d\n", channel);
  } else {
    printf("Stop OK. Channel: %d\n", channel);
  }

	ret = CiClose(channel);
	if (ret < 0) {
    printf("Error close %d\n", channel);
  } else {
    printf("Stop close. Channel: %d\n", channel);
  }



  // canmsg_t inFrame;
  // memset(&inFrame, 0, sizeof(canmsg_t));
  // ret = CiRead(channel, &inFrame);
  // if (ret < 0) {
  //   printf("Error reading frame from channel %d\n", channel);
  // }
  // printf("Got frame on channel %d:\n", channel);
  // printf("id = %ld\n", inFrame.id);
  // printf("flags = %d\n", inFrame.flags);
  // printf("len = %d\n", inFrame.len);
  // for (int j = 0; j < inFrame.len; ++j)
  //   printf("data[%d] = %#04x\n", j, inFrame.data[j]);
  // }

  printf("End of main\n");
	exit(0);
}
