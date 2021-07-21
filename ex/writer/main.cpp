#include <iostream>
#include <unistd.h>
#include <chai.h>

using namespace std;

int main()
{
	int chan1 = 0;
	int chan2 = 1;

	// инициализация chai
	if (CiInit() < 0) {
		printf("CHAI library initialization failed\n");
		return 1;
	}

	// данные для отправки
	const int FRAMES_COUNT = 16;
	canmsg_t frames[FRAMES_COUNT];
	for (int i = 0; i < FRAMES_COUNT; ++i) {
		msg_zero(&frames[i]);
		frames[i].id = i;
		frames[i].len = 1;
		frames[i].data[0] = i;
	}

	// открытие устройств и запуск chai
	if (CiOpen(chan1, CIO_CAN11 | CIO_CAN29) < 0) {
		printf("Failed to open channel %d\n", chan1);
		return 1;
	}
	if (CiOpen(chan2, CIO_CAN11 | CIO_CAN29) < 0) {
		printf("Failed to open channel %d\n", chan2);
		return 1;
	}
	if (CiStart(chan1) < 0) {
		printf("Failed to start chai on channel %d\n", chan1);
		return 1;
	}
	if (CiStart(chan2) < 0) {
		printf("Failed to start chai on channel %d\n", chan2);
		return 1;
	}

	// отправка
	int16_t ret = 0;
	for (int i = 0; i < FRAMES_COUNT; ++i) {
		ret = CiWrite(chan1, &frames[i], FRAMES_COUNT);
		if (ret <= 0)
			printf("Sending frame %d failed\n", i);
	}
	usleep(40000);


	// чтение
	canmsg_t frames_in[FRAMES_COUNT];
	for (int i = 0; i < FRAMES_COUNT; ++i)
		msg_zero(&frames_in[i]);

	ret = CiRead(chan2, &frames_in[0], FRAMES_COUNT);
	printf("Got %d frames\n", ret);

	for (int i = 0; i < FRAMES_COUNT; ++i)
		printf("Got frame: id = %d; data = %d\n", frames_in[i].id, frames_in[i].data[0]);


	// остановка chai
	CiStop(chan1);
	CiClose(chan1);
	CiStop(chan2);
	CiClose(chan2);

	return 0;
}
