import numpy as np
import matplotlib.pyplot as plt
from skimage.data import camera

# stuff from my own modules
from simulation import defocused_probe, init_probe, simulate_data
from ptycho import FrePIE, PyePIE

# ground-truth image
img = camera()/255
ground_truth = img*np.exp(1j*(img - 0.5)*2*np.pi)

# simulated diffraction patterns
scan_pos, dps = simulate_data(ground_truth, defocused_probe)

# initialize reconstruction with constant modulus and random phase
init_obj = np.exp(1j*np.random.random(ground_truth.shape))


# time first five iterations of Python and Fortran reconstructions
# run Fortran reconstruction for 100 iterations and display results

rec, rec_probe, errors = FrePIE(init_obj, init_probe, dps, scan_pos)
