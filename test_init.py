import ctypes
import sys
import time
from datetime import datetime
import subprocess



result = subprocess.Popen(['/usr/bin/python', 'read_voltage.py', '0', '0', '3','0'])
print(result)
