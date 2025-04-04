import numpy as np
import matplotlib.pyplot as plt

from ePIE import epie

obj = np.ones((512, 512), dtype='complex', order='F') 
prb = np.ones((128, 128), dtype='complex', order='F')
dps = np.ones((128, 128, 1024))

epie(obj, prb, dps, 1, 1, 100)
