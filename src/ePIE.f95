subroutine ePIE (obj, prb, dps, obj_step, prb_step, n_iters)
  implicit none

  ! inputs
  integer, intent(in) :: n_iters
  real, intent(in) :: obj_step, prb_step
  complex, intent(in) :: dps(:, :, :)

  ! initialized inputs -> outputs
  complex, intent(inout) :: obj(:, :)
  complex, intent(inout) :: prb(:, :)

  obj = 5*obj
  prb = 2*prb


end subroutine ePIE

