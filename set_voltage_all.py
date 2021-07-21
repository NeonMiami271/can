import ctypes
import sys
import time

def set_all_channel(bus_ch, board, voltage):
    power = ctypes.CDLL("./bin/green/libpower.so")
    power.init_bus(bus_ch)
    power.set_channel_all(bus_ch, board, voltage)
    power.cleanup_bus(bus_ch)
    print("ALL CHANNELS INSTALLED!")

if __name__ == "__main__":
        bus_ch = int(sys.argv[1])
        board = int(sys.argv[2])
        voltage = int(sys.argv[3], 16)
        set_all_channel(bus_ch, board, voltage)
