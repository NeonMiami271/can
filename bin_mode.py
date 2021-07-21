from ctypes import *
import sys
import time
power = CDLL("./bin/green/libpower.so")

can_ch = int(sys.argv[1])
board = int(sys.argv[2])


power.init_bus(can_ch)
power.set_channel_bin(can_ch, board)
power.cleanup_bus(can_ch)
