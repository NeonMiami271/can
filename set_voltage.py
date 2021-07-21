import ctypes
import sys
import time

def set_one_channel(bus_ch, board, mez, mez_ch, voltage):
    power = ctypes.CDLL("./bin/green/libpower.so")
    power.init_bus(bus_ch)
    power.set_channel(bus_ch, board, mez, mez_ch, voltage)
    power.cleanup_bus(bus_ch)
    print("ONE CHANNEL INSTALLED!")

if __name__ == "__main__":
    bus_ch = int(sys.argv[1])
    board = int(sys.argv[2])
    mez = int(sys.argv[3])
    mez_ch = int(sys.argv[4])
    voltage = int(sys.argv[5], 16)
    set_one_channel(bus_ch, board, mez, mez_ch, voltage)
