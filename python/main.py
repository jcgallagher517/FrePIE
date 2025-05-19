import numpy as np
import matplotlib.pyplot as plt
from skimage.data import camera
from skimage.transform import resize

from simulation import simulate_data, focused_probe, defocused_probe, init_probe
from FrePIE import FrePIE
from PyePIE import PyePIE
from FRC import FRC

# ground-truth image
img = camera()/255
ground_truth = img*np.exp(2j*img*np.pi)

# simulated diffraction patterns
scan_pos, dps = simulate_data(ground_truth, defocused_probe)

# initialize reconstruction with constant modulus and random phase
init_obj = np.exp(1j*np.random.random(ground_truth.shape))

rec_params = {
    "obj_step":1,
    "prb_step":1,
    "n_iters":30,
    "prb_delay":0,
}

# reconstructions and plots 
if __name__ == '__main__':

    rec = PyePIE(init_obj, defocused_probe, dps, scan_pos, **rec_params)
    rec_obj = rec["recon"]
    rec_prb = rec["probe"]
    rec_err = rec["error"]

    fig, ax = plt.subplots(2, 3, figsize = (12, 7))

    ax[0][0].imshow(np.abs(rec_obj))
    ax[0][0].set_title("Object Modulus")

    ax[0][1].imshow(np.angle(rec_obj))
    ax[0][1].set_title("Object Phase")

    ax[1][0].imshow(np.abs(rec_prb))
    ax[1][0].set_title("Probe Modulus")

    ax[1][1].imshow(np.angle(rec_prb))

    ax[0][2].plot(rec_err)
    ax[1][2].plot(FRC(rec_obj, ground_truth))
    
    plt.show()
