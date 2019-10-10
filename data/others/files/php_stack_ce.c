#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../objects/php_stack.h"

#include "../iterators/php_stack_iterator.h"
#include "../handlers/php_stack_handlers.h"

#include "php_collection_ce.h"
#include "php_stack_ce.h"

#define METHOD(name) PHP_METHOD(Stack, name)

zend_class_entry *php_ds_stack_ce;

METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        ds_stack_push_all(THIS_DS_STACK(), values);
    }
}

METHOD(allocate)
{
    PARSE_LONG(capacity);
    ds_stack_allocate(THIS_DS_STACK(), capacity);
}

METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(DS_STACK_CAPACITY(THIS_DS_STACK()));
}

METHOD(push)
{
    PARSE_VARIADIC_ZVAL();
    ds_stack_push_va(THIS_DS_STACK(), argc, argv);
}

METHOD(pop)
{
    PARSE_NONE;
    ds_stack_pop_throw(THIS_DS_STACK(), return_value);
}

METHOD(peek)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_stack_peek_throw(THIS_DS_STACK()));
}

METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(DS_STACK_SIZE(THIS_DS_STACK()));
}

METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_stack_create_clone(THIS_DS_STACK()));
}

METHOD(clear)
{
    PARSE_NONE;
    ds_stack_clear(THIS_DS_STACK());
}

METHOD(toArray)
{
    PARSE_NONE;
    ds_stack_to_array(THIS_DS_STACK(), return_value);
}

METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DS_STACK_IS_EMPTY(THIS_DS_STACK()));
}

METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_stack_to_array(THIS_DS_STACK(), return_value);
}

void php_ds_register_stack()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        PHP_DS_ME(Stack, __construct)
        PHP_DS_ME(Stack, allocate)
        PHP_DS_ME(Stack, capacity)
        PHP_DS_ME(Stack, peek)
        PHP_DS_ME(Stack, pop)
        PHP_DS_ME(Stack, push)

        PHP_DS_COLLECTION_ME_LIST(Stack)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, PHP_DS_NS(Stack), methods);

    php_ds_stack_ce = zend_register_internal_class(&ce);
    php_ds_stack_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_ds_stack_ce->create_object  = php_ds_stack_create_object;
    php_ds_stack_ce->get_iterator   = php_ds_stack_get_iterator;
    php_ds_stack_ce->serialize      = php_ds_stack_serialize;
    php_ds_stack_ce->unserialize    = php_ds_stack_unserialize;

    zend_class_implements(php_ds_stack_ce, 1, collection_ce);
    php_register_ds_stack_handlers();
}
