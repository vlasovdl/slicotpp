! Created by vlasovdl on 11.10.24.
! Calculation the number of rows of matrices  U  and  Y  (number of samples)
! for IB01CD subroutine.
subroutine ib01cd_minsmp(jobx0, comuse, job, n, m, l, minsmp)&
  bind(c, name = 'ib01cd_minsmp_')
  use iso_c_binding, only : c_double, c_int, c_char
  implicit none
! -- Arguments
  character (c_char), intent(in), value :: jobx0, comuse, job
  integer (c_int),    intent(in), value :: n, m, l
  integer (C_int),    intent(out)       :: minsmp

! -- Locals
  integer :: ncol
  logical :: compbd, withd, withx0

! -- Externals
  logical  lsame
  external lsame

  withx0 = lsame( jobx0,  'x' )
  compbd = lsame( comuse, 'c' )
  withd  = lsame( job,    'd' )

  if( compbd ) then
    ncol = n*m
    if( withx0 )ncol = ncol + n
    minsmp = ncol
    if( withd ) then
      minsmp = minsmp + m
    else if ( .not.withx0 ) then
      minsmp = minsmp + 1
    end if
  else
    ncol = n
    if( withx0 ) then
      minsmp = n
    else
      minsmp = 0
    end if
  end if
end subroutine ib01cd_minsmp