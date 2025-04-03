import numpy as np
import matplotlib.pyplot as plt
from numpy.fft import fft2, ifft2, fftshift, ifftshift, fftfreq
from scipy.special import j1
from skimage.data import camera

# construct Fresnel initial probe model at focal plane
def E1(r, E0, wavlen, zp_inner_rad, zp_outer_rad, zp_f):
    k, th = 2*np.pi/wavlen, r/zp_f
    wave = (-2j*E0/(wavlen*zp_f))*np.exp(1j*k*zp_f*(1 + th**2/2))
    bessel_term = lambda zp_r: zp_r**2 * j1(k*zp_r*th)/(k*zp_r*th)
    return wave*(bessel_term(zp_inner_rad) - bessel_term(zp_outer_rad))

# get defocused probe model with angular-spectrum propagation
def angular_spectrum_propagation(psi, z, wavlen, px_size):
    assert psi.shape[0] == psi.shape[1], "probe must be square"
    k_arr = fftfreq(psi.shape[0], px_size)
    kx, ky = np.meshgrid(k_arr, k_arr)
    kz = np.sqrt((2*np.pi/wavlen)**2 - kx**2 - ky**2)
    Dz = np.exp(-1j*kz*z)
    return ifft2(fft2(psi)*ifftshift(Dz))

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

# create de-focused probe
df_z = 30 # 30um defocus
defocused_probe = angular_spectrum_propagation(focused_probe, df_z, wavlen, px_sz)

# plan: use Fresnel Zone-Plate model probe to simulate data
# use much simpler probe for initial reconstruction
# random with circular support slightly larger than zp_outer_rad

# now work on simulating data with the defocused probe
ground_truth = camera()
