import ctypes
import sys
import time

def board_reset(can_ch, board):
    power = ctypes.CDLL("./bin/green/libpower.so")
    power.init_bus(can_ch)
    power.reset(can_ch, board)
    power.cleanup_bus(can_ch)
    print("RESET DONE!")

if __name__ == "__main__":
    can_ch = int(sys.argv[1])
    board = int(sys.argv[2])
    board_reset(can_ch, board)
