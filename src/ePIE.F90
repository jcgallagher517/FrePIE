function ePIE (obj, prb, dps, scan_pos, obj_step, prb_step, n_iters) result (errors)
  implicit none
  ! RMS error array, return value of function
  real :: errors(n_iters)

  ! dps and scan_pos should share first axis dimension
  real, intent(in) :: dps(:, :, :)
  real, intent(in) :: scan_pos(2, size(dps, 3))

  ! hyperparameters
  integer, intent(in) :: n_iters
  real, intent(in) :: obj_step, prb_step

  ! initialized object and probe, MUTATED by ePIE
  complex*16, intent(inout) :: obj(:, :)
  complex*16, intent(inout) :: prb(:, :) 

  ! other useful quantities
  integer :: n_dps, iter
  real :: error_per_iter
  n_dps = size(dps, 3)
  
  ! change the errors in a non-trivial way just to test
  do iter = 1,20
     print *, iter
     errors(iter) = iter * 3.0
  enddo

  print *, "Passed"
  obj = 5*obj
  prb = 2*prb

end function ePIE
