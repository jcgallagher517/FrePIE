# FrePIE

A basic implementation of ePIE for simulated EUV or soft x-ray ptychography data using C++ linked to Python. Done for the purpose of learning how to write and call more performant code within Python when needed. Originally, I planned on implementing the backend with Fortran, hence "FrePIE", but I gave up using that arcane language and kept the name. I then tried C with Python's native Ctypes, but wrapping complex types and flattening all the matrices for computation proved to be a headache. Finally, C++ with PyBind11 proved to be very seamless and practical.

FrePIE requires the [FFTW](https://fftw.org/fftw3_doc/Installation-on-Unix.html) library. 
To get the shared libraries for linking with Python, you should build fftw from source, 
with the ```./configure --enable-shared``` configuration flag.

# References

* Andrew M. Maiden, John M. Rodenburg, "An improved ptychographical phase retrieval algorithm for diffractive imaging", Ultramicroscopy, Volume 109, Issue 10, Pages 1256-1262, ISSN 0304-3991 (2009)

* Zeping Qin, Zijian Xu, Ruoru Li, Haigang Liu, Shilei Liu, Qingcao Wen, Xing Chen, Xiangzhi Zhang, and Renzhong Tai, "Initial probe function construction in ptychography based on zone-plate optics," Appl. Opt. 62, 3542-3550 (2023) 
