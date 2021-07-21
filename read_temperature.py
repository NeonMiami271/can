from ctypes import *
import sys
import time
from datetime import datetime

def read_one_mez_temperature(bus_ch, board, mez):
    power = CDLL("./bin/green/libpower.so")
    power.init_bus(bus_ch)
    while True:
        power.get_temperature.restype = POINTER(c_ubyte*12)
        msg = power.get_temperature(bus_ch, board, mez).contents

        temperature = 0
        temperature = temperature|msg[3]
        temperature = (msg[4]<<8)|temperature
        temperature = (msg[5]<<16)|temperature
        temperature = (msg[6]<<24)|temperature

        id = 0
        id = id|msg[11]
        id = (msg[10]<<24)|id
        id = (msg[9]<<16)|id
        id = (msg[8]<<8)|id

        s_id = "0x%02x" % id
        s_temperature = "0x%08x" % temperature
        s_type = "0x%02x" % msg[0]
        s_mez = "0x%02x" % msg[1]
        s_mez_ch = "0x%02x" % msg[2]

        if board == 0:
            board_id = "0x%02x"%6
        elif board == 1:
            board_id = "0x%02x"%8

        if (board_id == s_id) and (("0x%02x"%5) == s_type) and (("0x%02x"%mez) == s_mez):
            current_time = datetime.now().time()
            time = str(current_time.hour) + ":" + str(current_time.minute) + ":" + str(current_time.second)
            result_temperature = float(int(str(s_temperature), 16))/1000
            print(str(time) + "  " + str(s_temperature) + "  " + str(result_temperature) + " C")
            #return(time, s_temperature, result_temperature)
    power.cleanup_bus(bus_ch)

if __name__ == "__main__":
    bus_ch = int(sys.argv[1])
    board = int(sys.argv[2])
    mez = int(sys.argv[3])
    read_one_mez_temperature(bus_ch, board, mez)
