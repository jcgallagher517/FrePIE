import numpy as np
from numpy.fft import fft2, ifft2, fftshift, ifftshift

def FrePIE(init_obj, init_prb, dps, scan_pos,
           obj_step = 1, prb_step = 1, n_iters = 100):
    """wrapper to call F90 ePIE implementation
    so that args are not mutated directly
    inputs:
        obj: initialized object (big_N, big_N)
        prb: initialized probe (lil_N, lil_N)
        dps: diffraction data (lil_N, lil_N, k)
        scan_pos: (2, k)
    outputs:
        rec_object, rec_probe, error_array
    """
    f_obj = np.asfortranarray(np.copy(obj, order = 'F'))
    f_prb = np.asfortranarray(np.copy(prb, order = 'F'))
    f_dps = np.asfortranarray(np.copy(dps, order = 'F'))
    from ePIE import epie # are local imports cursed?
    errors = epie(f_obj, f_prb, f_dps, scan_pos, obj_step, prb_step, n_iters)
    return f_obj, f_prb, errors


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
        for (x, y), dp in zip(scan_pos, np.rollaxis(dps, 2, 0)):

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
