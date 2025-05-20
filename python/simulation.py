import numpy as np
import matplotlib.pyplot as plt
from numpy.fft import fft2, ifft2, fftshift, ifftshift, fftfreq
from scipy.special import j1

# simulating data with a defocused probe
"""
available functions:
    E1: Fresnel initial probe model at focal plane
    angular_spectrum_propagation: propagate waveform
    simulate_data: get diffraction patterns at normal incidence with supplied probe

available probes:
    focused_probe: 
    simul_defocused_probe: 30um defocus applied for simulation
    simul_noisy_defocused_probe: added poisson noise
    init_defocused_probe: 40um defocus applied, assuming we don't know exactly
"""


# construct Fresnel initial probe model at focal plane
def E1(r, E0, wavlen, zp_inner_rad, zp_outer_rad, zp_f):
    k, th = 2*np.pi/wavlen, r/zp_f
    wave = (-2j*E0/(wavlen*zp_f))*np.exp(1j*k*zp_f*(1 + th**2/2))
    bessel_term = lambda zp_r: zp_r**2 * j1(k*zp_r*th)/(k*zp_r*th)
    return wave*(bessel_term(zp_inner_rad) - bessel_term(zp_outer_rad))

# get defocused probe model with angular-spectrum propagation
def angular_spectrum_propagation(psi, z, wavlen, px_size):
    assert psi.shape[0] == psi.shape[1], "probe must be square"
    k_arr = fftshift(fftfreq(psi.shape[0], px_size))
    kx, ky = np.meshgrid(k_arr, k_arr)
    kz = np.sqrt((2*np.pi/wavlen)**2 - kx**2 - ky**2)
    Dz = np.exp(1j*kz*z)
    return ifft2(fft2(psi)*ifftshift(Dz))

# diffraction patterns via Fourier transform (Fraunhofer diffraction)
def simulate_data(gt, probe, step = 8):

    x_rng, y_rng = gt.shape[0] - probe.shape[0], gt.shape[1] - probe.shape[1]
    x_scan_idxs = np.arange(0, x_rng, step)
    y_scan_idxs =  np.arange(0, y_rng, step)

    # scan across x for each y position
    scan_positions, dps = [], []
    for y in y_scan_idxs:
        for x in x_scan_idxs:
            sub_obj = gt[x:x+probe.shape[0], y:y+probe.shape[1]]
            dps.append(np.abs(fft2(sub_obj*probe)))
            scan_positions.append((x, y))
    return np.array(scan_positions), np.array(dps)

# experimental parameters, all length-scales in nm
E0 = 1 # related to photon flux, not sure yet what to put here
wavlen = 13 # 13nm source
zp_inner_rad = 25e3 # beamstop radius 25um
zp_outer_rad = 125e3 # zone-plate outer radius 125um
zp_f = 60e6 # 60mm zone-plate focal length
E1_args = (E0, wavlen, zp_inner_rad, zp_outer_rad, zp_f)

# create focused probe
prb_sz, px_sz = 128, 10 # 128x128 probe with 10nm pixel size
probe_axis = np.linspace(-(prb_sz//2)*px_sz, (prb_sz//2)*px_sz, prb_sz)
xs, ys = np.meshgrid(probe_axis, probe_axis)
focused_probe = E1(xs**2 + ys**2, *E1_args)

# create initialized probe with circular support and random phase
# support = np.ones(focused_probe.shape)*(xs**2 + ys**2 < zp_outer_rad)
# init_probe = support * np.exp(1j*np.random.random(focused_probe.shape))


# display probes if run as script
if __name__ == "__main__":

    # create noisy 30um de-focused probe for simulating data
    simul_defocused_probe = angular_spectrum_propagation(focused_probe, 30e3, wavlen, px_sz)
    simul_noisy_defocused_probe = simul_defocused_probe + np.random.poisson(size=focused_probe.shape)

    # create noise-less model probe with slightly incorrect defocus for reconstruction
    init_defocused_probe = angular_spectrum_propagation(focused_probe, 40e3, wavlen, px_sz)

    fig, ax = plt.subplots(2, 2, figsize=(10, 10))

    ax[0][0].imshow(np.abs(focused_probe))
    ax[0][0].set_title("Focused probe amplitude")

    ax[1][0].imshow(np.angle(focused_probe))
    ax[1][0].set_title("Focused probe phase")

    ax[0][1].imshow(np.abs(simul_noisy_defocused_probe))
    ax[0][1].set_title("30um Noisy Defocused probe amplitude")

    ax[1][1].imshow(np.angle(simul_noisy_defocused_probe))
    ax[1][1].set_title("30um Noisy Defocused probe phase")

    plt.savefig("../images/probes.png")

