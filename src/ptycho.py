import numpy as np
import matplotlib.pyplot as plt
from numpy.fft import fft2, ifft2, fftshift, ifftshift
from scipy.constants import h, c, e
from scipy.special import j1
from skimage.data import camera

ground_truth = camera()

# construct Fresnel initial probe model at focal plane
def E1(r, E0, wavlen, zp_inner_rad, zp_outer_rad, zp_f):
    k = 2*np.pi/wavlen
    th = r/zp_f
    wave = (-2j*E0/(wavlen*zp_f))*np.exp(1j*k*zp_f*(1 + th**2/2))
    bessel_term = lambda zp_r: zp_r**2 * j1(k*zp_r*th)/(k*zp_r*th)
    return wave*(bessel_term(zp_inner_rad) - bessel_term(zp_outer_rad))

# get defocused probe model with angular-spectrum propagation
def angular_spectrum_propagation(psi, df_z, wavelen, n):
    q = 1 / wavelen # figure out what momentum transfer actual is
    Dz = np.exp(-2j*np.pi*df_z/(wavlen*np.sqrt(1 - (q*wavlen)**2)))
    return ifft2(fft2(psi)*Dz)

# experimental parameters, all length-scales in nm
E0 = 1 # flux, not sure yet what to put here
wavlen = h*c*e/12e3 # 12keV
zp_inner_rad = 25e3 # beamstop radius 25um
zp_outer_rad = 125e3 # zone-plate outer radius 125um
zp_f = 60e6 # 60mm zone-plate focal length
E1_args = (E0, wavlen, zp_inner_rad, zp_outer_rad, zp_f)

# create focused probe 64x64 
px_size = 10 # 10nm pixel size
probe_axis = np.linspace(-px_size*32, px_size*32, 64)
xs, ys = np.meshgrid(probe_axis, probe_axis)
focused_probe = E1(xs**2 + ys**2, *E1_args)

plt.imshow(np.angle(focused_probe))
plt.show()

# create de-focused probe
df_z = 60e3 # 60um defocus


# plan: use Fresnel Zone-Plate model probe to simulate data
# use much simpler probe for initial reconstruction
# random with circular support slightly larger than zp_outer_rad

