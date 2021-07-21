bch = (_u8) 1;

void init_bus(){
	int ret;
	bus_channel = bch;

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

}

void cleanup_bus(){
	bus_channel = bch;
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


int set_channel(int mez, int mez_channel, int voltage){
	int mapping[32] = {10,6,8,5,2,3,0,1,
  4,12,7,9,14,11,13,15,
  16,17,18,20,19,21,25,
  22,26,29,23,24,31,30,28,27};

	int mez_set = mez;
	mez_set_channel = (_u8) mapping[mez_channel];
	set_voltage = (_u8) voltage;

	canmsg_t frame;
  msg_zero(&frame);
  frame.id = 0x07;
  frame.flags = 0;
  frame.len = 7;
  frame.data[0] = 0x10;
  frame.data[1] = mez_set; //номер мезонина
  frame.data[2] = mez_set_channel; //номер канала
  frame.data[3] = set_voltage & 0xff; //младший разряд напряжения
  frame.data[4] = (set_voltage >> 8) & 0xff; //старший разряд напряжения
  frame.data[5] = 0;
  frame.data[6] = 0;

// 0x3fff
  ret = CiWrite(channel, &frame, 1);
  if (ret < 0) {
    printf("Error while sending frame via channel %d\n", channel);
  } else {
    printf("Sending frame OK. Channel: %d\n", channel);
  }

}
