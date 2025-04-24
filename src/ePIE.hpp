#ifndef EPIE_HPP
#define EPIE_HPP

#include <vector>
#include <Eigen/Dense>

namespace eig = Eigen;

using ArrayXcdRM = eig::Array<std::complex<double>, eig::Dynamic, eig::Dynamic, eig::RowMajor>;
std::vector<double> ePIE(ArrayXcdRM& obj,
                         ArrayXcdRM& prb, 
                         const std::vector<eig::ArrayXd>& dps,
                         const std::vector<std::vector<int>> &scan_pos,
                         double obj_step, double prb_step, int n_iters);
  /* ePIE ptychographic reconstruction routine
     @params:
         obj: initialized (guess) object to be reconstructed,
              shape: (bigX, bigY), MUTATED
         prb: initialized (guess) probe to be resconstructed,
              shape: (lilX, lilY),  MUTATED
         dps: diffraction pattern data, shape: (k, lilX, lilY)
         scan_pos: scan positions (in pixels) for each dp,
                   shape: (k, 2)
         obj_step, prb_step, n_iters: hyperparameters
     @return:
         errors: vec of RMS error for all dps per iteration,
                 shape: (n_iters)
   */

#endif // EPIE_HPP
