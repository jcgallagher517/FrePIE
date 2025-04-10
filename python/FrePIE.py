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

lib = ctypes.cdll.LoadLibrary("ePIElibc/ePIE.so")
ePIE_c = lib.ePIE
ePIE_c.restype = None
complex128_2d_ptr = ndpointer(dtype=np.complex128, ndim = 2, flags = "C_CONTIGUOUS")
ePIE.argtypes = [complex128_2d_ptr, # object
                 complex128_2d_ptr, # probe
                 ndpointer(dtype = np.float64, ndim = 3, flags = "C_CONTIGUOUS"), # dps
                 ndpointer(dtype = np.float64, ndim = 2, flags = "C_CONTIGUOUS"), # scan_pos
                 ctypes.c_double, # obj step
                 ctypes.c_double, # prb step
                 ctypes.c_int, # n_iters
                 ctypes.c_int, # obj_dx
                 ctypes.c_int, # obj_dy
                 ctypes.c_int, # prb_dx
                 ctypes.c_int, # prb_dy
                 ctypes.c_int, # number of dps
                 ndpointer(dtype = np.float64, ndim = 1, flags = "C_CONTIGUOUS")] # error


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
    assert (init_prb.shape == dps.shape[1:]), "probe and dps must have same dim"

    obj = np.ascontiguousarray(np.copy(init_obj))
    prb = np.ascontiguousarray(np.copy(init_prb))
    c_dps = np.ascontiguousarray(np.copy(dps))
    c_scan_pos = np.ascontiguousarray(np.copy(scan_pos))
    error_arr = np.ascontiguousarray(np.empty((n_iters)))

    ePIE(obj, prb, c_dps, c_scan_pos, obj_step, prb_step, n_iters
         init_obj.shape[0], init_obj.shape[1],
         init_prb.shape[0], init_prb.shape[1],
         dps.shape[0], error_arr)

    return obj, prb, error_arr


