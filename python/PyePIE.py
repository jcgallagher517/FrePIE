import numpy as np
from numpy.fft import fft2, ifft2, fftshift, ifftshift
import timeit

def PyePIE(init_obj, init_prb, dps, scan_pos, **kwargs):
    """python implementation of ePIE for prototyping and speed comparison
    """
    # fetch keyword arguments with default values
    obj_step = kwargs.get("obj_step", 1)
    prb_step = kwargs.get("prb_step", 1)
    n_iters = kwargs.get("n_iters", 25)
    prb_delay = kwargs.get("prb_delay", 0)

    obj, prb = init_obj, init_prb
    prb_sz = init_prb.shape[0]
    time_elapsed, errors = 0, []
    error_norm = np.sum(np.abs(dps)**2)
    print("Commencing reconstruction...")
    for iter in range(n_iters):
        iter_start = timeit.default_timer()
        error_per_iter = 0
        for (x, y), dp in zip(scan_pos, dps):

            lil_obj = obj[x:x+prb_sz, y:y+prb_sz]

            # exit wave and its Fourier transform
            psi = lil_obj * prb
            psi_k = fft2(psi)

            # replace modulus with (amplitude of..) diffraction pattern
            psi_k_p = dp * psi_k / np.abs(psi_k)
            psi_p = ifft2(psi_k_p)

            # update object and probe
            d_psi = psi_p - psi
            obj[x:x+prb_sz, y:y+prb_sz] = lil_obj + obj_step * d_psi * np.conj(prb)/np.max(np.abs(prb)**2)

            if iter > prb_delay:
                prb = prb + prb_step * d_psi * np.conj(lil_obj)/np.max(np.abs(lil_obj)**2)

            error_per_iter += np.mean(np.abs(psi_p - psi)**2)

        time_diff = timeit.default_timer() - iter_start
        time_elapsed += time_diff
        print(f"Iteration {iter+1}, Error: {error_per_iter}, Time: {time_diff:.5f}s")
        errors.append(np.sqrt(error_per_iter))

    print(f"Reconstruction completed. Time elapsed: {time_elapsed:.5f}s")
    return {"recon":obj, "probe":prb, "error":np.array(errors)}
