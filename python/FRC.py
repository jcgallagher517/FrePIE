import numpy as np
from numpy.fft import fft2, fftshift, ifftshift

def FRC(I1, I2): 
    """returns Fourier Ring Correlation of two images, given in the form of square numpy arrays
    """

    if I1.shape != I2.shape: 
        raise ValueError("Images must be the same size")
    if len(I1.shape) != 2: 
        raise ValueError("Images must be 2D")
    if I1.shape[0] != I1.shape[1]: 
        raise ValueError("Images must be square")

    N_r = I1.shape[0] // 2 # radial pixel range in k-space
    frc = np.zeros(N_r)
    F1 = fftshift(fft2(ifftshift(I1)))
    F2 = fftshift(fft2(ifftshift(I2)))
    odd_shift = 1 if I1.shape[0] % 2 == 1 else 0 # needed because N_r set w/ floor division
    k_rng = list(range(-N_r, N_r+odd_shift))
    kx, ky = np.meshgrid(k_rng, k_rng)

    # populate frc array
    for r_i in range(N_r):

        # boolean circle of radius r_i
        # RHS of inequality acts as tolerance parameter
        circle = np.abs((kx**2) + (ky**2) - (r_i**2)) < r_i

        # numerator of frc, should be real in principal
        # numerically, some very small imaginary part can remain
        frc[r_i] = np.sum(F1[circle] * np.conjugate(F2[circle])).real

        # normalizing factor
        normlz_factor = np.sqrt(np.sum(np.abs(F1[circle])**2) * np.sum(np.abs(F2[circle])**2))
        frc[r_i] = frc[r_i] / normlz_factor if normlz_factor != 0 else np.nan

    return frc
