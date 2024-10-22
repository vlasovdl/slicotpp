add_library(SLICOTPP STATIC
  XERBLA.f
  ftruefalse.f
  ib01ad_space.f90
  ib01ad_sizes.f90
  ib01bd_space.f90
  ib01bd_sizes.f90
  ib01cd_space.f90
  ib01cd_sizes.f90
)
target_link_libraries(SLICOTPP INTERFACE
  SLICOT_REF
  LAPACK::LAPACK
)
target_include_directories(SLICOTPP PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)