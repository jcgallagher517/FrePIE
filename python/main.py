import numpy as np
import matplotlib.pyplot as plt
from skimage.data import camera
from skimage.transform import resize

# stuff from my own modules
from simulation import simulate_data, focused_probe
from simulation import angular_spectrum_propagation, wavlen, px_sz
from FrePIE import FrePIE
from PyePIE import PyePIE
from FRC import FRC

# ground-truth image
img = camera()/255
ground_truth = img*np.exp(2j*img*np.pi)

# simulated diffraction patterns with 30um defocus
defocus = 30e3 # 30 micron defocus for simulation
simul_probe = angular_spectrum_propagation(focused_probe, defocus, wavlen, px_sz)
# simul_probe = simul_probe + np.random.poisson(size=simul_probe.shape)
scan_pos, dps = simulate_data(ground_truth, simul_probe)

# initialize probe with incorrect amount of defocus
# one that is closer to focal plane, another that's farther away
overfocused_probe = angular_spectrum_propagation(focused_probe, defocus - 10e3, wavlen, px_sz)
underfocused_probe = angular_spectrum_propagation(focused_probe, defocus + 10e3, wavlen, px_sz)

# initialize reconstruction with constant modulus and random phase
init_obj = np.exp(1j*np.random.random(ground_truth.shape))

rec_params = {
    "obj_step":1,
    "prb_step":0.1,
    "n_iters":50,
    "prb_delay":2,
}

# reconstructions and plots 
if __name__ == '__main__':

    # select implementation
    # fn, name = PyePIE, "Python"
    fn, name = FrePIE, "C++"

    over_rec = fn(init_obj, overfocused_probe, dps, scan_pos, **rec_params)
    over_rec_obj = over_rec["recon"]
    over_rec_prb = over_rec["probe"]
    over_rec_err = over_rec["error"]

    under_rec = fn(init_obj, underfocused_probe, dps, scan_pos, **rec_params)
    under_rec_obj = under_rec["recon"]
    under_rec_prb = under_rec["probe"]
    under_rec_err = under_rec["error"]

    fig, ax = plt.subplots(2, 5, figsize = (12, 7))
    fig.suptitle(f"{name} Reconstruction")

    ax[0][0].imshow(np.abs(over_rec_obj))
    ax[0][0].set_title("Overfocused Rec Object Modulus")

    ax[0][1].imshow(np.angle(over_rec_obj))
    ax[0][1].set_title("Overfocused Rec Object Phase")

    ax[1][0].imshow(np.abs(over_rec_prb))
    ax[1][0].set_title("Overfocused Rec Probe Modulus")

    ax[1][1].imshow(np.angle(over_rec_prb))
    ax[1][1].set_title("Overfocused Rec Probe Phase")

    ax[0][2].imshow(np.abs(under_rec_obj))
    ax[0][2].set_title("Underfocused Rec Object Modulus")

    ax[0][3].imshow(np.angle(under_rec_obj))
    ax[0][3].set_title("Underfocused Rec Object Phase")

    ax[1][2].imshow(np.abs(under_rec_prb))
    ax[1][2].set_title("Underfocused Rec Probe Modulus")

    ax[1][3].imshow(np.angle(under_rec_prb))
    ax[1][3].set_title("Underfocused Rec Probe Phase")

    ax[0][4].plot(over_rec_err[1:], label = "Overfocused")
    ax[0][4].plot(under_rec_err[1:], label = "Underfocused")
    ax[0][4].set_title("Error")
    ax[0][4].set_xlabel("Iteration")
    ax[0][4].legend()

    ax[1][4].plot(FRC(over_rec_obj, ground_truth), label = "Overfocused")
    ax[1][4].plot(FRC(under_rec_obj, ground_truth), label = "Underfocused")
    ax[1][4].set_title("FRC")
    ax[1][4].set_xlabel("k-space pixels")
    ax[1][4].legend()
    
    plt.savefig(f"../images/{name}_rec.png")
