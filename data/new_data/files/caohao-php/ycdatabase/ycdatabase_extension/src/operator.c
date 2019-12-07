/*
  +----------------------------------------------------------------------+
  | ycdatabase   /   ycdb                                                |
  +----------------------------------------------------------------------+
  | ycdb is a php database ORM extension for mysql written in c language |
  +----------------------------------------------------------------------+
  | LICENSE: https://github.com/caohao0730/ycdatabase/blob/master/LICENSE|
  +----------------------------------------------------------------------+
  | Author: Cao Hao  <649947921@qq.com>                                  |
  | CreateTime: 2018-11-19                                               |
  +----------------------------------------------------------------------+
*/
#include "ycdatabase.h"


int yc_compare_strict_bool(zval *op1, zend_bool op2 TSRMLS_DC) {

	int bool_result;
	if(op1 == NULL) {
		return 0 == op2;
	}
	
	switch (Z_TYPE_P(op1)) {
		case IS_LONG:
			return (Z_LVAL_P(op1) ? 1 : 0) == op2;
		case IS_DOUBLE:
			return (Z_DVAL_P(op1) ? 1 : 0) == op2;
		case IS_NULL:
			return 0 == op2;
#if PHP_MAJOR_VERSION < 7
		case IS_BOOL:
			if (Z_BVAL_P(op1)) {
				return 1 == op2;
			} else {
				return 0 == op2;
			}
#else
		case IS_TRUE:
			return 1 == op2;
		case IS_FALSE:
			return 0 == op2;
#endif
		default:
			return 0;
	}

	return 0;
}