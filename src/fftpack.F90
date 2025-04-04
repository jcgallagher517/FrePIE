module fftpack
  implicit none
  real, parameter :: pi = 3.1415926536
  real, parameter :: e = 2.7182818285 

contains

  function fft2 (img_r) result (img_k)
    complex, intent(in) :: img_r(:, :)
    complex :: img_k(size(img_r, 1), size(img_r, 2))

    integer :: n, m, Dx, Dy
    Dx = size(img_r, 1)
    Dy = size(img_r, 2)
    do n = 1, Dx
       do m = 1, Dy

       enddo
    enddo
    



  end function fft2

  function ifft2 (img_k) result (img_r)
    complex, intent(in) :: img_k(:, :)
    complex :: img_r(size(img_k, 1), size(img_k, 2))


  end function ifft2
  
end module fftpack
