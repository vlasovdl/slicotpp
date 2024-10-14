! Created by vlasovdl on 12.10.24.
! Calculation size of output matrices for IB01BD subroutine
subroutine ib01bd_sizes ( meth, job, jobck, n, m, l, lda, ldc, ldb, ldd, &
  ldq, ldry, lds, ldk ) &
  bind (c, name = 'ib01bd_sizes_')
  use iso_c_binding, only : c_double, c_int, c_char
  implicit none
  ! -- Arguments
  character (c_char), intent(in)  :: meth, job, jobck
  integer (c_int),    intent(in)  :: n, m, l
  integer (c_int),    intent(out) :: lda, ldc, ldb, ldd, ldq, ldry, lds, &
    ldk

  ! -- Locals
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


end subroutine ib01bd_sizes