import numpy as np
import matplotlib.pyplot as plt
from numpy.fft import fft2, fftshift
from skimage.data import camera

from probe import defocused_probe

img = camera()/255
ground_truth = img + 1j*img


# get diffraction patterns from Fourier transform (Fraunhofer diffraction)
def simulate_data(gt, probe):

    num_pos = 32 # scan positions per axis
    x_rng, y_rng = gt.shape[0] - probe.shape[0], gt.shape[1] - probe.shape[1]
    x_scan_idxs = np.arange(0, x_rng, x_rng//num_pos)
    y_scan_idxs = np.arange(0, y_rng, y_rng//num_pos)

    # initialize diffraction patterns
    dps = np.zeros((*probe.shape, len(x_scan_idxs)*len(y_scan_idxs)),
                   dtype=np.complex128)

    # scan across x for each y position
    dp_i = 0
    for y in y_scan_idxs:
        for x in x_scan_idxs:
            sub_obj = ground_truth[x:x+probe.shape[0], y:y+probe.shape[1]]
            dps[:, :, dp_i] = fftshift(fft2(sub_obj))
            dp_i += 1
    return dps

dps = simulate_data(ground_truth, defocused_probe)


# try python reconstruction to see if I did it right
def ePIE(dps, init_obj, init_probe):
    pass
