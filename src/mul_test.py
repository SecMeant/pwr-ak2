import ctypes
import numpy as np
c_i64_p = ctypes.POINTER(ctypes.c_int64)
uint64_array_c = ctypes.c_uint64 * 2

class bigint(ctypes.Structure):
  _fields_= [
    ("val", c_i64_p),
    ("size", ctypes.c_uint64)
  ]

function = ctypes.CDLL("./multiply.so")
in_data = np.array([0x10, 0x11])
print(in_data[1])
print( ctypes.c_int64(in_data.ctypes.data) )

first =  bigint( c_i64_p( ctypes.c_int64(in_data.ctypes.data)) , ctypes.c_uint64(2) )
second = bigint( c_i64_p( ctypes.c_int64(in_data.ctypes.data)) , ctypes.c_uint64(2) )

# print(first.val[0])
# print(second.val[0][0])

function.bignum_multiply(first,second)