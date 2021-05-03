from ctypes import cdll
# either
balash = cdll.LoadLibrary("libc.so.6")
balash.time()