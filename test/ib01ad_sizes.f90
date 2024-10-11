! Created by vlasovdl on 11.10.24.

subroutine ib01ad_sizes(meth, alg, jobd, batch, conct, nobr, m, l, nsmp, ldr,  &
  liwork, ldwork) &
  bind (c, name = 'ib01bd_sizes_')
  use iso_c_binding, only : c_double, c_int, c_char
  implicit none
! -- Arguments
  character (c_char), intent(in), value :: meth, alg, jobd, batch, conct
  integer (c_int), intent(in), value    :: nobr, m, l, nsmp
  integer (c_int), intent(out)          :: ldr, liwork, ldwork
! -- External functions
  logical  :: lsame
  external :: lsame
! -- Locals
  integer  ::  lmnobr, lnobr, minwrk, mnobr, nobr21, nr, ns, nsmpsm
  logical  ::  chalg, connec, first, fqralg, interm, jobdm, last, moesp, n4sid,&
    onebch, qralg

  moesp  = lsame( meth,  'M' )
  n4sid  = lsame( meth,  'N' )
  fqralg = lsame( alg,   'F' )
  qralg  = lsame( alg,   'Q' )
  chalg  = lsame( alg,   'C' )
  jobdm  = lsame( jobd,  'M' )
  onebch = lsame( batch, 'O' )
  first  = lsame( batch, 'F' ) .or. onebch
  interm = lsame( batch, 'I' )
  last   = lsame( batch, 'L' ) .or. onebch

  if( .not.onebch ) then
    connec = lsame( conct, 'c' )
  else
    connec = .false.
  end if

  mnobr  = m*nobr
  lnobr  = l*nobr
  lmnobr = lnobr  + mnobr
  nr     = lmnobr + lmnobr
  nobr21 = 2*nobr - 1
  nsmpsm = nsmpsm + nsmp

  ! -- Calculate LDR
  if (moesp .and. jobdm) then
    ldr = max (3*mnobr, nr)
  else
    ldr = nr
  end if

  ! -- Calculate LIWORK
  liwork = 3
  if (n4sid) then
    liwork = max ( liwork, lmnobr)
  elseif (fqralg) then
    liwork = max ( liwork, m + l )
  end if

  ! -- Calculate DWORK
  ns = nsmp - nobr21
  if ( chalg ) then
    if ( .not.last ) then
      if ( connec ) then
        minwrk = 2*( nr - m - l )
      else
        minwrk = 1
      end if
    else if ( moesp ) then
      if ( connec .and. .not.onebch ) then
        minwrk = max( 2*( nr - m - l ), 5*lnobr )
      else
        minwrk = 5*lnobr
        if ( jobdm )minwrk = max( 2*mnobr - nobr, lmnobr, minwrk )
      end if
    else
      minwrk = 5*lmnobr + 1
    end if
  else if ( fqralg ) then
    if ( .not.onebch .and. connec ) then
      minwrk = nr*( m + l + 3 )
    else if ( (first .or. interm) .and. .not. onebch ) then
      minwrk = nr*( m + l + 1 )
    else
      minwrk = 2*nr*( m + l + 1 ) + nr
    end if
  else
    minwrk = 2*nr
    if ( onebch .and. ldr>=ns ) then
      if ( moesp ) then
        minwrk = max( minwrk, 5*lnobr )
      else
        minwrk = 5*lmnobr + 1
      end if
    end if
    if ( first ) then
      if ( ldr<ns ) then
        minwrk = minwrk + nr
      end if
    else
      if ( connec ) then
        minwrk = minwrk*( nobr + 1 )
      else
        minwrk = minwrk + nr
      end if
    end if
  end if
  !
  ldwork = minwrk
end subroutine ib01ad_sizes