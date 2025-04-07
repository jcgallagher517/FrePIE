import numpy as np
from numpy.fft import fft2, ifft2, fftshift, ifftshift

def PyePIE(init_obj, init_prb, dps, scan_pos,
           obj_step = 1, prb_step = 1, N_iters = 100):
    """python implementation of ePIE for prototyping and speed comparison
    same arguments and returns as FrePIE
    """
    obj, prb = init_obj, init_prb
    prb_sz = init_prb.shape[0]
    errors = []
    for iter in range(N_iters):
        error_per_iter = 0
        for (x, y), dp in zip(scan_pos, dps):

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

    return obj, prb, np.array(errors)
