cmake_minimum_required(VERSION 3.14.0)
project( slicotpp
        VERSION         0.0.2
        DESCRIPTION     "C++ wrapper for SLICOT routines"
        LANGUAGES       C CXX Fortran
        HOMEPAGE_URL    "https://github.com/vlasovdl/slicotpp"
)
include(FetchContent)

find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)

# Конфигурация компиляции SLICOT
FetchContent_Declare( SLICOT
  URL https://github.com/SLICOT/SLICOT-Reference/archive/refs/tags/v5.9.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(SLICOT)
if (slicot_POPULATED)
  file (GLOB SLICOT_SRC CONFIGURE_DEPENDS
    ${slicot_SOURCE_DIR}/src/*.f
    ${slicot_SOURCE_DIR}/src_aux/*.f
  )
  add_library(SLICOT_REF STATIC ${SLICOT_SRC})
endif ()

# Включение основных исходных файлов
add_subdirectory(src)

# Конфигурация тестов
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(VLASOVDL_SLICOTPP_TEST "Whether or not tests should be built" ON)
  if (VLASOVDL_SLICOTPP_TEST)
    add_subdirectory(test bin/test)
  endif ()
endif ()