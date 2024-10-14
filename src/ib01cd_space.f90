! Created by vlasovdl on 11.10.24.

subroutine ib01cd_space(jobx0, comuse, job, n, m, l, nsmp, liwork, ldwork) &
bind(c, name = 'ib01cd_space_')
  use iso_c_binding, only : c_double, c_int, c_char
  implicit none
! -- Arguments
  character (c_char), intent(in)  :: jobx0, comuse, job
  integer (c_int),    intent(in)  :: n, m, l, nsmp
  integer (C_int),    intent(out) :: liwork, ldwork
! -- Locals
  integer :: ia, ic, iq, isize, itau, ldw, ldw2, ldw3, lm, ln, minsmp, minwls,&
             minwrk,  n2m, ncol, ncp1, nm, nn, nsmpl
  logical :: compbd, usebd, maxdia, maxdim, withb, withd, withx0

! -- Externals
  logical  lsame
  external lsame

  withx0 = lsame( jobx0,  'x' )
  compbd = lsame( comuse, 'c' )
  usebd  = lsame( comuse, 'u' )
  withd  = lsame( job,    'd' )
  withb  = lsame( job,    'b' )   .or. withd
  maxdim = ( withx0 .and. usebd ) .or. compbd
  maxdia = withx0 .or. compbd

  ldw   = max( 1, n )
  lm    = l*m
  ln    = l*n
  nn    = n*n
  nm    = n*m
  n2m   = n*nm
  if( compbd ) then
    ncol = nm
    if( withx0 )ncol = ncol + n
    minsmp = ncol
    if( withd ) then
      minsmp = minsmp + m
      iq     = minsmp
    else if ( .not.withx0 ) then
      iq     = minsmp
      minsmp = minsmp + 1
    else
      iq     = minsmp
    end if
  else
    ncol = n
    if( withx0 ) then
      minsmp = n
    else
      minsmp = 0
    end if
    iq = minsmp
  end if

! -- Compute LDWORK
  if ( .not.maxdia .or. max( n, m )==0 ) then
    minwrk = 2
  else
    nsmpl = nsmp*l
    iq    = iq*l
    ncp1  = ncol + 1
    isize = nsmpl*ncp1
    if ( compbd ) then
      if ( n>0 .and. withx0 ) then
        ic = 2*nn + n
      else
        ic = 0
      end if
    else
      ic = 2*nn
    end if
    minwls = ncol*ncp1
    if ( compbd ) then
      if ( withd )minwls = minwls + lm*ncp1
      if ( m>0 .and. withd ) then
        ia = m + max( 2*ncol, m )
      else
        ia = 2*ncol
      end if
      itau = n2m + max( ic, ia )
      if ( withx0 )itau = itau + ln
      ldw2 = isize  + max( n + max( ic, ia ), 6*ncol )
      ldw3 = minwls + max( iq*ncp1 + itau, 6*ncol )
      if ( m>0 .and. withd ) then
        ldw2 = max( ldw2, isize  + 2*m*m + 6*m )
        ldw3 = max( ldw3, minwls + 2*m*m + 6*m )
        ia   = 3
      else
        ia = 2
      end if
    else
      itau = ic + ln
      ldw2 = isize  + 2*n + max( ic, 4*n )
      ldw3 = minwls + 2*n + max( iq*ncp1 + itau, 4*n )
      ia   = 2
    end if
    minwrk = ia + nn + nm + ln + max( 5*n, ia, min( ldw2, ldw3 ) )
    ldwork = minwrk
  end if

! -- Compute LIWORK
  if (withx0) then
    liwork = n
  else
    liwork = 0
  end if

  if (compbd) then
    if (withd) then
      liwork = max ( nm + liwork, m)
    else
      liwork = nm + liwork
    end if
  end if



end subroutine ib01cd_space