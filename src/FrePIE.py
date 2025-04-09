import numpy as np
import ctypes
from numpy.ctypeslib import ndpointer


# simple 1d fft example checks out
"""
lib = ctypes.cdll.LoadLibrary("ePIElibc/ePIE.so")
fft = lib.compute_fft
fft.restype = None
complex128_ptr = ndpointer(dtype=np.complex128, flags="C_CONTIGUOUS")
fft.argtypes = [ctypes.c_size_t,
                complex128_ptr,
                complex128_ptr]

def call_fft(indata):
    outdata = np.empty(indata.size, dtype=indata.dtype)
    fft(indata.size,
        np.ascontiguousarray(indata),
        np.ascontiguousarray(outdata))
    return outdata

template = np.linspace(0, 8*np.pi, 32)
indata = np.cos(template) + 0j
outdata = call_fft(indata)
"""



def FrePIE(init_obj, init_prb, dps, scan_pos,
           obj_step = 1, prb_step = 1, n_iters = 100):
    """wrapper to call C ePIE implementation
    inputs:
        obj: initialized object (big_N, big_N)
        prb: initialized probe (lil_N, lil_N)
        dps: diffraction data (k, lil_N, lil_N)
        scan_pos: (k, 2)
    outputs:
        rec_object, rec_probe, error_array
    """
    pass


