import numpy as np
import matplotlib.pyplot as plt
from skimage.data import camera

from simulation import simulate_data, defocused_probe, init_probe
from FrePIE import FrePIE
from PyePIE import PyePIE
from FRC import FRC

# ground-truth image
img = camera()/255
# ground_truth = img*np.exp(1j*(img - 0.5)*2*np.pi)
ground_truth = img*np.exp(2j*img*np.pi)

# simulated diffraction patterns
scan_pos, dps = simulate_data(ground_truth, defocused_probe)

# initialize reconstruction with constant modulus and random phase
init_obj = np.exp(1j*np.random.random(ground_truth.shape))

# reconstructions and plots 
if __name__ == '__main__':
    
    Fre_rec = FrePIE(init_obj, init_probe, dps, scan_pos,
                     obj_step = 1, prb_step = 1, n_iters = 100)
    Fre_rec_obj = Fre_rec["recon"]
    Fre_rec_prb = Fre_rec["probe"]
    Fre_rec_err = Fre_rec["error"]
    
    Pye_rec = PyePIE(init_obj, init_probe, dps, scan_pos,
                     obj_step = 1, prb_step = 1, n_iters = 100)
    Pye_rec_obj = Pye_rec["recon"]
    Pye_rec_prb = Pye_rec["probe"]
    Pye_rec_err = Pye_rec["error"]

    # plot reconstructed objects and diagnostics
    # switch to also show GT with rec
    # put error/FRC in a different plot
    # ideally without noise, FRC curve should be 1
    
    fig, ax = plt.subplots(2, 3, figsize = (12, 7))

    ax[0][0].imshow(np.abs(Fre_rec_obj))
    ax[0][0].set_title("C++ Modulus")

    ax[1][0].imshow(np.angle(Fre_rec_obj))
    ax[1][0].set_title("C++ Phase")

    ax[0][1].imshow(np.abs(Pye_rec_obj))
    ax[0][1].set_title("Python Modulus")

    ax[1][1].imshow(np.angle(Pye_rec_obj))
    ax[1][1].set_title("Python Phase")

    ax[0][2].plot(Fre_rec_err, label = "C++")
    ax[0][2].plot(Pye_rec_err, label = "Python")
    ax[0][2].set_title("Error per iteration")

    ax[1][2].plot(FRC(ground_truth, Fre_rec_obj), label = "C++")
    ax[1][2].plot(FRC(ground_truth, Pye_rec_obj), label = "Python")

    plt.savefig("../images/recons.png")

    # plot reconstructed probes

