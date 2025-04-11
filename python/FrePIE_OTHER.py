import sys
from glob import glob
import numpy as np

sys.path.append(glob("../build/lib*/")[0])
from FrePIE import ePIE

def FrePIE(init_obj, init_prb, dps, scan_pos,
           obj_step = 1, prb_step = 1, n_iters = 100):
    """wrapper function to C++ FrePIE module
    to ensure c-contiguousness and (im)mutable of inputs
    """

    obj = np.ascontiguousarray(np.copy(init_obj))
    prb = np.ascontiguousarray(np.copy(init_prb))
    error = ePIE(obj, prb, dps, scan_pos,
                 obj_step, prb_step, n_iters)

    return {"recon":obj, "probe":prb, "error":error}


