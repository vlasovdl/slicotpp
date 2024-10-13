! Created by vlasovdl on 13.10.24.
! Calculation size of output matrices for IB01BD subroutine
subroutine ib01cd_sizes(jobx0, comuse, job, n, m, l, nsmp, ldx0, ldv) &
  bind(c, name = 'ib01cd_sizes_')
  use iso_c_binding, only : c_double, c_int, c_char
  implicit none
! -- Arguments
  character (c_char), intent(in), value :: jobx0, comuse, job
  integer (c_int),    intent(in), value :: n, m, l, nsmp
  integer (c_int),    intent(out)       :: ldx0, ldv
! -- Externals
  logical  lsame
  external lsame

  ldx0 = n

  if ( lsame( jobx0, 'X' ) .or. lsame( comuse, 'C' ) ) then
    ldv = max ( 1, n )
  else
    ldv = 1
  end if

end subroutine ib01cd_sizes