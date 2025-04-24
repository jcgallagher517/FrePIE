# FrePIE

A basic implementation of ePIE for simulated EUV or soft x-ray ptychography data using C++ linked to Python. 
Done for the purpose of learning how to write and call more performant code within Python when needed. 
Originally, I planned on implementing the backend with Fortran, hence "FrePIE", but I gave up using that arcane language and kept the name. 
Next, I tried C with Python's native ctypes, but complex numbers and pointer arithmatic created unnecessary headaches.
Finally, I landed on C++ with PyBind11, which allows (not quite) seamless integration.

The requirements are as follows:
* PyBind11 (installed with pip)
* Eigen3, FFTW3 (installed with vcpkg)

## Takeaways
* I should never take NumPy for granted. 
Sure, for-loops are slow in Python, but vectorized array operations compiled from Fortran are fast.
* Simply switching to a lower-level language does not guarantee substantial performance gains.
It's surprisingly easy to write C++ that is poorly performant.
My first pass at the core ePIE routine was about 15x slower than Python/NumPy.
Successive optimizations eventually overcame the gap. 
However, this is somewhat volatile, and has appeared to depend substantially on external factors such as what version of FFTW I had installed (i.e. whether from vcpkg or from the Fedora repositories).
The right compiler flags also makes an immense difference.
* The process of linking types, especially complicated ones like complex-valued tensors, between Python and C++ is not abundantly elegant. 
Though I've learned a lot, I don't believe this is an approach I am likely to repeat.
Unless I really need to hand-roll some complicated linear algebra routine, I am probably better off investigating something like Julia.


# References

* Andrew M. Maiden, John M. Rodenburg, "An improved ptychographical phase retrieval algorithm for diffractive imaging", Ultramicroscopy, Volume 109, Issue 10, Pages 1256-1262, ISSN 0304-3991 (2009)

* Zeping Qin, Zijian Xu, Ruoru Li, Haigang Liu, Shilei Liu, Qingcao Wen, Xing Chen, Xiangzhi Zhang, and Renzhong Tai, "Initial probe function construction in ptychography based on zone-plate optics," Appl. Opt. 62, 3542-3550 (2023) 

* Wenzel Jakob, Jason Rhinelander, & Dean Moldovan. (2017). pybind11 – Seamless operability between C++11 and Python. 

* Frigo, M., & Johnson, S. (2005). The Design and Implementation of FFTW3. Proceedings of the IEEE, 93(2), 216–231.

* Gaël Guennebaud, Benoît Jacob, & others. (2010). Eigen v3.
