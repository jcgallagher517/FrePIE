import numpy as np
import matplotlib.pyplot as plt
from skimage.data import camera

from simulation import simulate_data, defocused_probe, init_probe
from FrePIE import FrePIE
from PyePIE import PyePIE

# ground-truth image
img = camera()/255
ground_truth = img*np.exp(1j*(img - 0.5)*2*np.pi)

# simulated diffraction patterns
scan_pos, dps = simulate_data(ground_truth, defocused_probe)

# initialize reconstruction with constant modulus and random phase
init_obj = np.exp(1j*np.random.random(ground_truth.shape))

results_dict = FrePIE(ground_truth, init_probe, dps, scan_pos,
                      obj_step = 1, prb_step = 1, n_iters = 100)


recon = results_dict["recon"]
fft_of_gt = np.fftshift(np.fft.fft2(ground_truth))

# THERE IS A MINOR BUG IN SIMULATION CODE
# last scan_pos on either axis is 372
# 372 + 128 = 500 != 512
# the scan positions are in steps of 12
# so I am missing the last 12 pixels along either axis
# which I noticed earlier from PyePIE and found strange
