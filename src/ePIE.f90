function ePIE (obj, prb, dps, scan_pos, obj_step, prb_step, n_iters) result (errors)
  implicit none

  ! external fft implementations courtesy of fftpack5.f90
  external ZFFT2I, ZFFT2F, ZFFT2B

  ! RMS error array, return value of function
  real :: errors(n_iters)

  ! dps and scan_pos should share first axis dimension
  real, intent(in) :: dps(:, :, :)
  real, intent(in) :: scan_pos(size(dps, 1), 2)

  ! hyperparameters
  integer(4), intent(in) :: n_iters
  real(8), intent(in) :: obj_step, prb_step

  ! initialized object and probe, MUTATED by ePIE
  complex(16), intent(inout) :: obj(:, :)
  complex(16), intent(inout) :: prb(:, :) 

  ! initialize state for fft calls
  integer(4) :: iw
  complex(8), dimension(size(dps, 2)*size(dps, 3)) :: work

  ! other useful quantities
  integer(4) :: iter, scan_loc_iter, n_dps, dim_x, dim_y
  real :: error_per_iter
  n_dps = size(dps, 1)
  dim_x = size(dps, 2)
  dim_y = size(dps, 3)

  ! ePIE algorithm begins here
  call ZFFT2I(dim_x, dim_y, iw, work)





end function ePIE
