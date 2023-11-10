# ===================================================================
# CMAKE のおまじない
# ===================================================================
cmake_minimum_required (VERSION 3.14)


# C++17 の指定
set (CMAKE_CXX_STANDARD 17)

# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================
set ( YM_VERSION_MAJOR 1 )
set ( YM_VERSION_MINOR 0 )
set ( YM_VERSION ${YM_VERSION_MAJOR}.${YM_VERSION_MINOR} )
project ( pysat VERSION "${YM_VERSION}" )

if ( NOT CMAKE_INSTALL_PREFIX )
  set ( CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/stage" )
endif ()


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================
list ( APPEND CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/ym-common/cmake"
  )


# 自前のパッケージをインクルード
include ( YmUtils )


# よく使ういくつかのパッケージの検査を行う．
# このオプションでは bzip2, lzma, zlib, popt の検査も行う．
ym_init ( )
ym_init_gperftools ()
ym_init_ctest ()

if ( PYTHON_VERSION )
  set( INIT_PYTHON_ARGS ${PYTHON_VERSION} EXACT )
endif ()
ym_init_python ( ${INIT_PYTHON_ARGS} )


# ===================================================================
# google-test を有効にする．
# ===================================================================
ym_use_gtest ()


# ===================================================================
# ヘッダファイルの生成
# ===================================================================

# ===================================================================
# サブモジュールのリスト
# ===================================================================

ym_init_submodules (
  ym-common
  ym-json
  ym-sat
  )


# ===================================================================
# インクルードパスの設定
# ===================================================================

include_directories(
  ${PROJECT_BINARY_DIR}/include
  ${YM_SUBMODULE_INCLUDE_LIST}
  ${Python3_INCLUDE_DIRS}
)


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

foreach ( dir ${YM_SUBMODULE_LIST} )
  add_subdirectory ( ${dir} )
endforeach ()


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  executable ターゲットの設定
# ===================================================================

add_executable( pysat
  c++-srcs/pysat.cc
  $<TARGET_OBJECTS:py_ymsat_obj>
  $<TARGET_OBJECTS:ym_sat_obj>
  $<TARGET_OBJECTS:ym_json_obj>
  )

target_link_libraries ( pysat
  ${YM_LIBDEPENDS}
  ${Python3_LIBRARIES}
  )


# ===================================================================
#  library ターゲットの設定
# ===================================================================

add_library( py_ymsatlib
  SHARED
  $<TARGET_OBJECTS:py_ymsat_obj>
  $<TARGET_OBJECTS:ym_sat_obj>
  $<TARGET_OBJECTS:ym_json_obj>
  )

set_target_properties( py_ymsatlib
  PROPERTIES
  PREFIX ""
  OUTPUT_NAME "ymsat"
  )

target_link_libraries( py_ymsatlib
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )


# ===================================================================
# インストールターゲット
# ===================================================================

install( TARGETS pysat
  DESTINATION bin
  )

install( TARGETS py_ymsatlib
  LIBRARY DESTINATION lib/${YM_PYTHON_STRING}/site-packages
  )
