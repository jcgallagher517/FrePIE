import sys
from glob import glob
import numpy as np

sys.path.append(glob("../build/lib*/")[0])
from libFrePIE import ePIE as _ePIE_cpp

def FrePIE(init_obj, init_prb, dps, scan_pos,
           obj_step = 1, prb_step = 1, n_iters = 100):
    """wrapper function to C++ FrePIE module
    to ensure c-contiguousness and (im)mutability of inputs
    """
    obj = np.ascontiguousarray(np.copy(init_obj), dtype=np.complex128)
    prb = np.ascontiguousarray(np.copy(init_prb), dtype=np.complex128)
    dps = np.ascontiguousarray(dps, dtype=np.float64)
    scan_pos = np.ascontiguousarray(scan_pos, dtype=np.int32)
    error = _ePIE_cpp(obj, prb, dps, scan_pos,
                      obj_step, prb_step, n_iters)
    return {"recon":obj, "probe":prb, "error":error}
