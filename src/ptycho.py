import numpy as np
import matplotlib.pyplot as plt
from numpy.fft import fft2, ifft2, fftshift, ifftshift
from skimage.data import camera

# stuff from my own modules
from probe import simulate_data
from probe import defocused_probe

# ground-truth image
img = camera()/255
ground_truth = img*np.exp(1j*(img - 0.5)*2*np.pi)

# simulated diffraction patterns
scan_positions, dps = simulate_data(ground_truth, defocused_probe)

# quick ePIE in Python so that I know my simulated data works
# and for a model when I implement in Fortran

# initialize with constant modulus and random phase
init_obj = np.exp(1j*np.random.random(ground_truth.shape))

# initialize probe with circular support
prb_sz, px_sz = 128, 10 # 128x128 probe with 10nm pixel size
probe_axis = np.linspace(-(prb_sz//2)*px_sz, (prb_sz//2)*px_sz, prb_sz)
xs, ys = np.meshgrid(probe_axis, probe_axis)
support = np.ones(defocused_probe.shape)*(xs**2 + ys**2 < 130e3)
init_prb = support * np.exp(1j*np.random.random(defocused_probe.shape))


def ePIE(init_obj, init_prb, obj_step = 1, prb_step = 1, N_iters = 50):

    obj, prb = init_obj, init_prb
    prb_sz = init_prb.shape[0]
    errors = []
    for iter in range(N_iters):
        error_per_iter = 0
        for (x, y), dp in zip(scan_positions, np.rollaxis(dps, 2, 0)):

            lil_obj = obj[x:x+prb_sz, y:y+prb_sz]

            # exit wave and its Fourier transform
            psi = lil_obj*prb
            psi_k = fft2(psi)

            # replace modulus with (amplitude of..) diffraction pattern
            psi_k_p = dp*psi_k/np.abs(psi_k)
            psi_p = ifft2(psi_k_p)

            # update object and probe
            obj[x:x+prb_sz, y:y+prb_sz] = lil_obj + obj_step*np.conj(prb)*(psi_p - psi)/np.max(np.abs(prb)**2)
            prb = prb + prb_step*np.conj(lil_obj)*(psi_p - psi)/np.max(np.abs(lil_obj)**2)

            error_per_iter += np.mean(np.abs(psi_p - psi))**2

        errors.append(np.sqrt(error_per_iter))

    return obj, np.array(errors)

rec, errors = ePIE(init_obj, init_prb)
