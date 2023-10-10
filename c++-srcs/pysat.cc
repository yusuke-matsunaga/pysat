
/// @file pysat.cc
/// @brief pysat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "pym/ymsat.h"


BEGIN_NAMESPACE_YM

extern "C" PyObject* PyInit_ymsat();

static struct _inittab init_table[] = {
  {"ymsat", PyInit_ymsat},
  {nullptr, nullptr}
};

void
ymsat_init()
{
  PyImport_ExtendInittab(init_table);
}

END_NAMESPACE_YM

int
main(
  int argc,
  char** argv
)
{
  YM_NAMESPACE::ymsat_init();

  return Py_BytesMain(argc, argv);
}
