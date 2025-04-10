import sys
import glob
import numpy as np
import matplotlib.pyplot as plt
from skimage.data import camera

from simulation import defocused_probe, init_probe, simulate_data
sys.path.append(glob.glob("../build/lib*/")[0])
import FrePIE

# from FrePIE import FrePIE
from PyePIE import PyePIE

# ground-truth image
img = camera()/255
ground_truth = img*np.exp(1j*(img - 0.5)*2*np.pi)

# simulated diffraction patterns
scan_pos, dps = simulate_data(ground_truth, defocused_probe)

# initialize reconstruction with constant modulus and random phase
init_obj = np.exp(1j*np.random.random(ground_truth.shape))


obj = np.copy(init_obj)
prb = np.copy(init_probe)
errors = FrePIE.ePIE(obj, prb, dps, scan_pos, 1, 1, 100)
