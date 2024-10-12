! Created by vlasovdl on 10.10.24.

subroutine ib01bd_space(meth, job, jobck, nobr, n, m, l, lda, ldc, ldb, ldd, &
  ldq, ldry, lds, ldk, liwork, ldwork, lbwork) &
  bind (c, name = 'ib01bd_space_')
  use iso_c_binding, only : c_double, c_int, c_char
  implicit none
! -- Arguments
  character (c_char), intent(in), value :: meth, job, jobck
  integer (c_int),    intent(in), value :: nobr, n, m, l
  integer (c_int),    intent(out)       :: lda, ldc, ldb, ldd, ldq, ldry, lds, &
                                           ldk, liwork, ldwork, lbwork
! -- Locals
  integer :: i, ia, iaw, ic, id, ierr, ifact, ig, ik, io,iq, ir, is, it, iv, &
             iwarnl, iwi, iwr, ix,jwork, ldunn, ll, lmmnol, lmnobr, lnobr, &
             maxwrk, minwrk, mnobr, mnobrn, n2, nl, nn, npl, nr
  logical :: combin, moesp, n4sid, withal, withb, withc,withco, withd, withk

! -- Externals
  logical  lsame
  external lsame

  moesp  = lsame (  meth, 'M')
  n4sid  = lsame (  meth, 'N')
  combin = lsame (  meth, 'C')
  withal = lsame (   job, 'A')
  withc  = lsame (   job, 'C') .or. withal
  withd  = lsame (   job, 'D') .or. withal
  withb  = lsame (   job, 'B') .or. withd
  withk  = lsame ( jobck, 'K')
  withco = lsame ( jobck, 'C') .or. withk

  mnobr  = m*nobr
  lnobr  = l*nobr
  lmnobr = lnobr + mnobr
  mnobrn = mnobr + n
  ldunn  = ( lnobr - l )*n
  lmmnol = lnobr + 2*mnobr + l
  nr     = lmnobr + lmnobr
  npl    = n + l
  n2     = n + n
  nn     = n*n
  nl     = n*l
  ll     = l*l
  ldwork = 1

! -- Compute LDA, LDC
  if ( withc .or. ( withb .and. .not.moesp ) ) then
    lda = n; ldc = l
  else
    lda = 1; ldc = 1
  end if

! -- Compute LDB
  if ( m > 0 .and. withb ) then
    ldb = n
  else
    ldb = 1
  end if

! -- Compute LDD
  if (m > 0 .and. withd) then
    ldd = l
  else
    ldd = 1
  end if

! -- Compute LDQ, LDRY, LDS, LDK
  if (withco) then
    ldq = n; ldry = l; lds = n
  else
    ldq = 1; ldry = 1; lds = 1
  end if

! -- Compute LDK
  if (withk) then
    ldk = n
  else
    ldk = 1;
  end if

! -- Compute ldwork
  iaw    = 0
  minwrk = ldunn + 4*n
  if( .not.n4sid ) then
    id = 0
    if( withc ) then
      minwrk = max( minwrk, 2*ldunn + n2, ldunn + nn + 7*n )
    end if
  else
    id = n
  end if
  !
  if( ( m>0 .and. withb ) .or. .not.moesp ) then
    minwrk = max( minwrk, 2*ldunn + nn + id + 7*n )
    if ( moesp )minwrk = max( minwrk, ldunn + n + 6*mnobr, ldunn + n +max( l + mnobr, lnobr +max( 3*lnobr + 1, m ) ) )
  else
    if( .not.n4sid )iaw = n + nn
  end if
  !
  if( .not.moesp .or. withco ) then
    minwrk = max( minwrk, ldunn + iaw + n2 + max( 5*n, lmmnol ),id + 4*mnobrn + 1, id + mnobrn + npl )
    if( .not.moesp .and. m>0 .and. withb )minwrk = max( minwrk, mnobr*npl*( m*npl + 1 ) +max( npl**2, 4*m*npl + &
      1 ) )
    minwrk = lnobr*n + minwrk
  end if
  !
  if( withk ) then
    minwrk = max( minwrk, 4*nn + 2*nl + ll + max( 3*l, nl ),14*nn + 12*n + 5 )
  end if
  ldwork = minwrk;

! -- Compute LIWORK
  if (.not.n4sid .and. m == 0 .or. lsame(job,'C') .and. lsame(jobck,'N')) then
    liwork = n
  elseif (.not.n4sid .and. withc .and. withk) then
    liwork = m*nobr + n
  elseif (moesp .and. withb .and. .not.withco ) then
    liwork = max( l*nobr, m*nobr )
  elseif (moesp .and. withb .and. withco ) then
    liwork = max( l*nobr, m*nobr + n)
  elseif (n4sid .or. combin .and. withb) then
    liwork = max(m*nobr+n, m*(n+l))
  end if
  if (withk) liwork = max (liwork, n*n)

! -- Compute lbwork
  if (withk) then
    lbwork = 2 * n
  else
    lbwork = 0
  end if

end subroutine ib01bd_space