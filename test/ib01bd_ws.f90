! Created by vlasovdl on 10.10.24.

subroutine ib01bd_ws(METH, JOB, JOBCK, NOBR, N, M, L LDWORK) &
bind (c, name = 'ib01bd_ws')
  use iso_c_binding, only : c_double, c_int
  implicit none
  logical :: METH, JOB, JOBCK
  integer(c_int), intent(in)  :: NOBR, N, M, L
  integer(c_int), intent(out) :: LDWORK
  ! -- Local variables
  integer :: IAW, LDUNN


  IAW = 0
  LDUNN = ( L*NOBR - L )*N
  MINWORK = LDUNN + 4*N
  if (not(METH == 'N')) then
    ID = 0
    if (WITHC) then
      MINWRK = max( MINWRK, 2*LDUNN + N2, LDUNN + NN + 7*N )
    end if
  end if



end subroutine ib01bd_ws