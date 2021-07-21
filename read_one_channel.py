from ctypes import *
import sys
from time import *

def read_one_channel(bus_ch, board, mez, mez_ch):
    power = CDLL("./bin/green/libpower.so")
    power.init_bus(bus_ch)
    
    power.read_channel.restype = POINTER(c_ubyte*12)
    msg = power.read_channel(bus_ch, board, mez, mez_ch).contents
    
    voltage = 0
    voltage = voltage|msg[6]
    voltage = (msg[5]<<8)|voltage
    voltage = (msg[4]<<16)|voltage
    voltage = (msg[3]<<24)|voltage

    id = 0
    id = id|msg[11]
    id = (msg[10]<<24)|id
    id = (msg[9]<<16)|id
    id = (msg[8]<<8)|id

    s_id= "0x%02x" % id
    s_voltage = "0x%08x" % voltage
    s_type = "0x%02x" % msg[0]
    s_mez = "0x%02x" % msg[1]
    s_mez_ch = "0x%02x" % msg[2]

    if board == 0:
        board_id = "0x%02x"%6
    elif board == 1:
        board_id = "0x%02x"%8

    #if (board_id == s_id) and (("0x%02x"%4) == s_type) and (("0x%02x"%mez) == s_mez) and (("0x%02x"%mez_ch) == s_mez_ch):
   
    result_voltage = float(int(str(s_voltage), 16))/1000
    print(str(s_voltage) + "  " + str(result_voltage) + "V")

    power.cleanup_bus(bus_ch)

if __name__ == "__main__":
    bus_ch = int(sys.argv[1])
    board = int(sys.argv[2])
    mez = int(sys.argv[3])
    mez_ch = int(sys.argv[4])
    read_one_channel(bus_ch, board, mez, mez_ch)