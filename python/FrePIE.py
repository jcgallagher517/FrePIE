import sys
from glob import glob
import numpy as np

sys.path.append(glob("../build/lib*/")[0])
from libFrePIE import ePIE as _ePIE_cpp

def FrePIE(init_obj, init_prb, dps, scan_pos, **kwargs):
    """wrapper function to C++ FrePIE module
    to ensure c-contiguousness and (im)mutability of inputs
    """
    # fetch keyword arguments with default values
    obj_step = kwargs.get("obj_step", 1)
    prb_step = kwargs.get("prb_step", 1)
    n_iters = kwargs.get("n_iters", 25)
    prb_delay = kwargs.get("prb_delay", 0)

    obj = np.ascontiguousarray(np.copy(init_obj), dtype=np.complex128)
    prb = np.ascontiguousarray(np.copy(init_prb), dtype=np.complex128)
    dps = np.ascontiguousarray(dps, dtype=np.float64)
    scan_pos = np.ascontiguousarray(scan_pos, dtype=np.int32)
    error = _ePIE_cpp(obj, prb, dps, scan_pos,
                      obj_step, prb_step, n_iters)
    return {"recon":obj, "probe":prb, "error":error}
