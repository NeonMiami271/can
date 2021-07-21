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

canmsg_t msg;
char data[12];

_u8 mapping[32] = {
	10,6,8,5,2,3,0,1,
	4,12,7,9,14,11,13,15,
	16,17,18,20,19,21,25,22,
	26,29,23,24,31,30,28,27
};
_u8 rmapping[32] = {
	6, //0
	7, //1
	4, //2
	5, //3
	8, //4
	3, //5
	1, //6
	10, //7
	2, //8
	11, //9
	0, //10
	13, //11
	9, //12
	14, //13
	12, //14
	15, //15
	16, //16
	17, //17
	18, //18
	20, //19
	19, //20
	21, //21
	23, //22
	26, //23
	27, //24
	22, //25
	24, //26
	31, //27
	30, //28
	25, //29
	29, //30
	28 //31
};



//Иницализация и открытие канала CAN
void init_bus(int bus_ch){
	_u8 bus_channel = (_u8) (bus_ch);
	int ret;

  printf("Trying to initialize chai library from green/main\n");
	if ((ret = CiInit()) < 0) {
		printf("Initialization failed. Error: %d\n", ret);
		exit(1);
	}

  printf("Trying to open CAN bus_channel %d\n", bus_channel);
	ret = CiOpen(bus_channel, CIO_CAN11);
	if (ret < 0)
		printf("Open bus_channel failed. bus_channel: %d Errcode: %d\n", bus_channel, ret);
	else
		printf("Open bus_channel OK. bus_channel: %d\n", bus_channel);

  printf("Trying to set baud speed %d\n", bus_channel);
  ret = CiSetBaud(bus_channel, BCI_1M);
  if (ret < 0) {
    printf("Setting baud speed failed. bus_channel: %d Error: %d\n", bus_channel, ret);
  } else {
    printf("Setting baud speed OK. bus_channel: %d\n", bus_channel);
  }

  printf("Starting CAN bus_channel %d\n", bus_channel);
  ret = CiStart(bus_channel);
  if (ret < 0) {
    printf("Starting bus_channel failed. bus_channel: %d Error: %d\n", bus_channel, ret);
  } else {
    printf("Starting bus_channel OK. bus_channel: %d\n", bus_channel);
  }
	usleep(1000000);
}

//Закрытие канала CAN
void cleanup_bus(int bus_ch){
	_u8 bus_channel = (_u8) (bus_ch);
	int ret;
	ret = CiStop(bus_channel);
	if (ret < 0) {
    printf("Error stop %d\n", bus_channel);
  } else {
    printf("Stop OK. bus_channel: %d\n", bus_channel);
  }

	ret = CiClose(bus_channel);
	if (ret < 0) {
    printf("Error close %d\n", bus_channel);
  } else {
    printf("Stop close. bus_channel: %d\n", bus_channel);
  }
}

//Установка напряжения на отдельно взятом канале.
void set_channel(int bus_ch, int board, int mez, int mez_channel, int voltage){
	_u8 bus_channel = (_u8) (bus_ch);
	int ret;
	canmsg_t frame;
  msg_zero(&frame);

	if (board == 0){
		frame.id = 0x05;
	}
	else if (board == 1){
		frame.id = 0x07;
	}
	//frame.id = 0x07;
  frame.flags = 0;
  frame.len = 7;
  frame.data[0] = 0x10;
  frame.data[1] = mez & 0xff; //номер мезонина
  frame.data[2] = mapping[mez_channel]; //номер канала
  frame.data[3] = voltage & 0xff; //младший разряд напряжения
  frame.data[4] = (voltage >> 8) & 0xff; //старший разряд напряжения
  frame.data[5] = 0;
  frame.data[6] = 0;

  ret = CiWrite(bus_channel, &frame, 1);

  if (ret < 0) {
    printf("Error while sending frame via channel %d\n", bus_channel);
  } else {
    printf("Sending frame OK. Channel: %d\n", bus_channel);
  }

}

void 	disable_flud_func(int bus_ch, int board){
	_u8 bus_channel = (_u8) (bus_ch);
	int ret;
	canmsg_t frame;
  msg_zero(&frame);

	if (board == 0){
		frame.id = 0x05;
	}
	else if (board == 1){
		frame.id = 0x07;
	}
	//frame.id = 0x07;
  frame.flags = 0;
  frame.len = 7;
  frame.data[0] = 0x03;
  frame.data[1] = 0x00; //номер мезонина
  frame.data[2] = 0x00; //номер канала
  frame.data[3] = 0x00; //младший разряд напряжения
  frame.data[4] = 0x00; //старший разряд напряжения
  frame.data[5] = 0x00;
  frame.data[6] = 0x00;

  ret = CiWrite(bus_channel, &frame, 1);

  if (ret < 0) {
    printf("MODE NOT OK via channel %d\n", bus_channel);
  } else {
    printf("MODE OK. Channel: %d\n", bus_channel);
  }

}

//Установка напряжения на всех каналах
void set_channel_all(int bus_ch, int board, int voltage){
	_u8 bus_channel = (_u8) (bus_ch);
	int ret;
	canmsg_t frame;
  msg_zero(&frame);

	if (board == 0){
		frame.id = 0x05;
	}
	else if (board == 1){
		frame.id = 0x07;
	}
	//frame.id = 0x07;
  frame.flags = 0;
  frame.len = 7;
  frame.data[0] = 0x11;
  frame.data[1] = 0; //номер мезонина
  frame.data[2] = 0; //номер канала
  frame.data[3] = voltage & 0xff; //младший разряд напряжения
  frame.data[4] = (voltage >> 8) & 0xff; //старший разряд напряжения
  frame.data[5] = 0;
  frame.data[6] = 0;

  ret = CiWrite(bus_channel, &frame, 1);

  if (ret < 0) {
    printf("Error while sending frame via channel %d\n", bus_channel);
  } else {
    printf("Sending frame OK. Channel: %d\n", bus_channel);
  }

}

//Сброс всех каналов в ноль
void reset(int bus_ch, int board){
	_u8 bus_channel = (_u8) (bus_ch);
	int ret;
	canmsg_t frame;
  msg_zero(&frame);

	if (board == 0){
		frame.id = 0x05;
	}
	else if (board == 1){
		frame.id = 0x07;
	}
	//frame.id = 0x07;
  frame.flags = 0;
  frame.len = 7;
  frame.data[0] = 0x01;
  frame.data[1] = 0; //номер мезонина
  frame.data[2] = 0; //номер канала
  frame.data[3] = 0; //младший разряд напряжения
  frame.data[4] = 0; //старший разряд напряжения
  frame.data[5] = 0;
  frame.data[6] = 0;

  ret = CiWrite(bus_channel, &frame, 1);

  if (ret < 0) {
    printf("Error while sending frame via channel %d\n", bus_channel);
  } else {
    printf("Sending frame OK. Channel: %d\n", bus_channel);
  }

}

//Отправка сообщения для чтения определенного канала. Не работет.
void* read_channel(int bus_ch, int board, int mez, int mez_channel){
	_u8 bus_channel = (_u8) (bus_ch);
	int ret;
	canmsg_t frame;
	msg_zero(&frame);

	if (board == 0){
		frame.id = 0x06;
	}
	else if (board == 1){
		frame.id = 0x08;
	}

  frame.flags = 0;
  frame.len = 7;
  frame.data[0] = 0x04;
  frame.data[1] = mez & 0xff; //номер мезонина
  frame.data[2] = mapping[mez_channel]; //номер канала

	ret = CiWrite(bus_channel, &frame, 1);
  if (ret < 0) {
    printf("Error while sending frame via channel %d\n", bus_channel);
  } else {
    printf("Sending frame OK. Channel: %d\n", bus_channel);
  }

	canmsg_t mbuf[1];
	ret = CiRead(bus_channel, mbuf, 1);
  if (ret < 0) {
    printf("Error while reading frame via channel %d\n", bus_channel);
  } else {
    printf("Read frame from channel: %d. OK.\n", bus_channel);
  }

	msg = mbuf[0];
	for (int i = 0; i<8;i++){
		data[i] = msg.data[i];
	}

	data[8] = (msg.id >> 24) & 0xff;
	data[9] = (msg.id >> 16) & 0xff;
	data[10] = (msg.id >> 8) & 0xff;
	data[11] = msg.id & 0xff;
	return data;
}

//Чтение напряжения
void* get_voltage(int bus_ch) {
	_u8 bus_channel = (_u8) (bus_ch);
	int ret;
	canmsg_t mbuf[1];

	// mbuf = (canmsg_t*)malloc(10 * sizeof(canmsg_t));

	ret = CiRead(bus_channel, mbuf, 1);
  // if (ret < 0) {
  //   printf("Error reading frame from channel %d\n", bus_channel);
  // }
  //printf("READ! channel %d:\n", bus_channel);

	// for (int i = 0; i < 10; i++){
	// 	canmsg_t msg;
	// 	msg_zero(&msg);
	// 	msg = mbuf[i];
	// 	//if (msg.data[1] == 3 && msg.data[2] == mapping[0]){
	// 		printf("========================\n");
	// 		printf("id = %d\n", msg.id);
	// 		printf("data[0] = %x\n", msg.data[0]);
	// 		printf("data[1] = %x\n", msg.data[1]);
	// 		printf("data[2] = %x\n", msg.data[2]);
	//   	printf("data[3] = %x\n", msg.data[3]);
	// 		printf("data[4] = %x\n", msg.data[4]);
	// 		printf("data[5] = %x\n", msg.data[5]);
	// 		printf("data[6] = %x\n", msg.data[6]);
	// 	//}
	// }
		//canmsg_t msg;
		msg_zero(&msg);
		msg = mbuf[0];
		//if (msg.data[1] == 3 && msg.data[2] == mapping[0]){
			// printf("========================\n");
			// printf("id = %d\n", msg.id);
			// printf("data[0] = %x\n", msg.data[0]);
			// printf("data[1] = %x\n", msg.data[1]);
			// printf("data[2] = %x\n", msg.data[2]);
	  	// printf("data[3] = %x\n", msg.data[3]);
			// printf("data[4] = %x\n", msg.data[4]);
			// printf("data[5] = %x\n", msg.data[5]);
			// printf("data[6] = %x\n", msg.data[6]);
		//}
		// struct frame{
		// 	char *msg.data[0];
		// 	char *msg.data[1];
		// 	}
		for (int i = 0; i<8;i++){
			data[i] = msg.data[i];
		}

data[8] = (msg.id >> 24) & 0xff;
data[9] = (msg.id >> 16) & 0xff;
data[10] = (msg.id >> 8) & 0xff;
data[11] = msg.id & 0xff;
return data;

  //printf("id = %ld\n", frame.id);
  //printf("flags = %d\n", frame.flags);
  //printf("len = %d\n", frame.len);
  // for (int j = 0; j < frame.len; ++j)
  //   printf("data[%d] = %#04x\n", j, frame.data[j]);

}

//Отправка сообщения для установки режима чтения кодов с АЦП
void set_channel_bin(int bus_ch, int board){
	_u8 bus_channel = (_u8) (bus_ch);
	int ret;
	canmsg_t frame;
  msg_zero(&frame);

	if (board == 0){
		frame.id = 0x05;
	}
	else if (board == 1){
		frame.id = 0x07;
	}
  frame.flags = 0;
  frame.len = 11;
  frame.data[0] = 0x20;
  // frame.data[1] = 0x00; //номер мезонина
  // frame.data[2] = 0x00; //номер канала
  frame.data[3] = 0x00; //младший разряд напряжения
  frame.data[4] = 0x00; //старший разряд напряжения
  frame.data[5] = 0x00;
  frame.data[6] = 0x00;
	frame.data[7] = 0x00;

  ret = CiWrite(bus_channel, &frame, 1);

  if (ret < 0) {
    printf("Don't install CODE mode from ADC via channel %d\n", bus_channel);
  } else {
    printf("Install CODE mode from ADC. Channel: %d\n", bus_channel);
  }

}

void* get_temperature(int bus_ch, int board, int mez) {
	_u8 bus_channel = (_u8) (bus_ch);
	int ret;
	canmsg_t mbuf[1];

	// mbuf = (canmsg_t*)malloc(10 * sizeof(canmsg_t));

	ret = CiRead(bus_channel, mbuf, 1);
  // if (ret < 0) {
  //   printf("Error reading frame from channel %d\n", bus_channel);
  // }
  //printf("READ! channel %d:\n", bus_channel);

	// for (int i = 0; i < 10; i++){
	// 	canmsg_t msg;
	// 	msg_zero(&msg);
	// 	msg = mbuf[i];
	// 	//if (msg.data[1] == 3 && msg.data[2] == mapping[0]){
	// 		printf("========================\n");
	// 		printf("id = %d\n", msg.id);
	// 		printf("data[0] = %x\n", msg.data[0]);
	// 		printf("data[1] = %x\n", msg.data[1]);
	// 		printf("data[2] = %x\n", msg.data[2]);
	//   	printf("data[3] = %x\n", msg.data[3]);
	// 		printf("data[4] = %x\n", msg.data[4]);
	// 		printf("data[5] = %x\n", msg.data[5]);
	// 		printf("data[6] = %x\n", msg.data[6]);
	// 	//}
	// }
		//canmsg_t msg;
		msg_zero(&msg);
		msg = mbuf[0];
		//if (msg.data[1] == 3 && msg.data[2] == mapping[0]){
			// printf("========================\n");
			// printf("id = %d\n", msg.id);
			// printf("data[0] = %x\n", msg.data[0]);
			// printf("data[1] = %x\n", msg.data[1]);
			// printf("data[2] = %x\n", msg.data[2]);
	  	// printf("data[3] = %x\n", msg.data[3]);
			// printf("data[4] = %x\n", msg.data[4]);
			// printf("data[5] = %x\n", msg.data[5]);
			// printf("data[6] = %x\n", msg.data[6]);
		//}
		// struct frame{
		// 	char *msg.data[0];
		// 	char *msg.data[1];
		// 	}
		for (int i = 0; i<8;i++){
			data[i] = msg.data[i];
		}

data[8] = (msg.id >> 24) & 0xff;
data[9] = (msg.id >> 16) & 0xff;
data[10] = (msg.id >> 8) & 0xff;
data[11] = msg.id & 0xff;
return data;

  //printf("id = %ld\n", frame.id);
  //printf("flags = %d\n", frame.flags);
  //printf("len = %d\n", frame.len);
  // for (int j = 0; j < frame.len; ++j)
  //   printf("data[%d] = %#04x\n", j, frame.data[j]);

}
