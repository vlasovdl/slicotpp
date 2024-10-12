! Created by vlasovdl on 12.10.24.

subroutine ib01ad_sizes(meth, jobd, nobr, m, l, ldr, ldsv) &
  bind (c, name = 'ib01ad_sizes_')
  use iso_c_binding, only : c_double, c_int, c_char
  implicit none
  ! -- Arguments
  character (c_char), intent(in), value :: meth, jobd
  integer (c_int), intent(in), value    :: nobr, m, l
  integer (c_int), intent(out)          :: ldr, ldsv
  ! -- External functions
  logical  :: lsame
  external :: lsame
  ! -- Locals
  logical  ::  jobdm, moesp

  moesp  = lsame( meth,  'M' )
  jobdm  = lsame( jobd,  'M' )

  ! -- Calculate LDR
  if (moesp .and. jobdm) then
    ldr = max (3*m*nobr, 2*nobr*(l + m))
  else
    ldr = 2*nobr*(l + m)
  end if

  ! -- Calculate LSV
  ldsv = l*nobr

end subroutine ib01ad_sizes