# FrePIE

A basic implementation of ePIE for simulated EUV or soft x-ray ptychography data using C++ linked to Python. 
Done for the purpose of learning how to write and call more performant code within Python when needed. 
Originally, I planned on implementing the backend with Fortran, hence "FrePIE", but I gave up using that arcane language and kept the name. 
Next, I tried C with Python's native ctypes, but complex numbers and pointer arithmatic created unnecessary headaches.
Finally, I landed on C++ with PyBind11, which allows almost seamless integration.

The requirements are as follows:
* PyBind11 (installed with pip)
* Eigen3, FFTW3 (installed with vcpkg)

## Takeaways
* I should never take NumPy for granted. 
Sure, for-loops are slow in Python, but vectorized array operations compiled from Fortran are fast.
Especially so when written by developers more talanted than I.
* Simply switching to a lower-level language does not guarantee substantial performance gains.
It's surprisingly easy to write very bad C++, especially when 
My first pass at the core ePIE routine was about 15x slower than Python/NumPy.
Successive optimizations closed the gap


# References

* Andrew M. Maiden, John M. Rodenburg, "An improved ptychographical phase retrieval algorithm for diffractive imaging", Ultramicroscopy, Volume 109, Issue 10, Pages 1256-1262, ISSN 0304-3991 (2009)

* Zeping Qin, Zijian Xu, Ruoru Li, Haigang Liu, Shilei Liu, Qingcao Wen, Xing Chen, Xiangzhi Zhang, and Renzhong Tai, "Initial probe function construction in ptychography based on zone-plate optics," Appl. Opt. 62, 3542-3550 (2023) 

* Wenzel Jakob, Jason Rhinelander, & Dean Moldovan. (2017). pybind11 – Seamless operability between C++11 and Python. 

* Frigo, M., & Johnson, S. (2005). The Design and Implementation of FFTW3. Proceedings of the IEEE, 93(2), 216–231.

* Gaël Guennebaud, Benoît Jacob, & others. (2010). Eigen v3.
