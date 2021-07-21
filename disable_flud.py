import ctypes
import sys
import time

def disable(bus_ch, board):
    power = ctypes.CDLL("./bin/green/libpower.so")
    power.init_bus(bus_ch)
    power.disable_flud_func(bus_ch, board)
    power.cleanup_bus(bus_ch)
    print("ONE CHANNEL INSTALLED!")

if __name__ == "__main__":
    bus_ch = int(sys.argv[1])
    board = int(sys.argv[2])
    disable(bus_ch, board)

