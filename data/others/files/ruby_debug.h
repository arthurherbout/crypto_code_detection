#include <ruby.h>
#include "ruby/debug.h"

/* Context info */
enum ctx_stop_reason {
    CTX_STOP_NONE,
    CTX_STOP_STEP,
    CTX_STOP_BREAKPOINT,
    CTX_STOP_CATCHPOINT
};

/* Context flags */
#define CTX_FL_SUSPEND      (1<<1) /* suspend because another thread stopping */
#define CTX_FL_TRACING      (1<<2) /* call at_tracing method */
#define CTX_FL_SKIPPED      (1<<3) /* skip all debugger events */
#define CTX_FL_IGNORE       (1<<4) /* this context belongs to ignored thread */
#define CTX_FL_DEAD         (1<<5) /* this context belonged to died thraed */
#define CTX_FL_WAS_RUNNING  (1<<6) /* previous thread state */
#define CTX_FL_ENABLE_BKPT  (1<<7) /* can check breakpoint */
#define CTX_FL_STEPPED      (1<<8)
#define CTX_FL_FORCE_MOVE   (1<<9)
#define CTX_FL_CATCHING     (1<<10)

#if 0
static inline const char *
flag_name(int flag)
{
    switch (flag) {
#define C(x) case x: return #x
	C(CTX_FL_SUSPEND);
	C(CTX_FL_TRACING);
	C(CTX_FL_SKIPPED);
	C(CTX_FL_IGNORE);
	C(CTX_FL_DEAD);
	C(CTX_FL_WAS_RUNNING);
	C(CTX_FL_ENABLE_BKPT);
	C(CTX_FL_STEPPED);
	C(CTX_FL_FORCE_MOVE);
	C(CTX_FL_CATCHING);
      default:
	return "unknown";
    }
}
#define CTX_DUMP(set, f) fprintf(stderr, "[CTX %s:%d] %s %s\n", __FILE__, __LINE__, (set) ? "set" : "unset", flag_name(f))
#else
#define CTX_DUMP(set, f)
#endif

#define CTX_FL_TEST(c,f)  ((c)->flags & (f))
#define CTX_FL_SET(c,f)   do { CTX_DUMP(1, f); (c)->flags |= (f); } while (0)
#define CTX_FL_UNSET(c,f) do { CTX_DUMP(0, f);  (c)->flags &= ~(f); } while (0)

typedef struct {
    VALUE thread_id;
    int thnum;
    int flags;
    int calced_stack_size;
    enum ctx_stop_reason stop_reason;

    /* control flow */
    int stop_next;
    int dest_frame;
    int stop_line;
    int stop_frame;

    int thread_pause;

    VALUE last_file;
    VALUE last_line;

    VALUE breakpoint;

    /* [[loc, self, klass, binding, iseq], ...] */
    VALUE inspected_frame;
} debug_context_t;

enum inspected_frame_type {
    INSPECTED_FRAME_LOCATION,
    INSPECTED_FRAME_SELF,
    INSPECTED_FRAME_CLASS,
    INSPECTED_FRAME_BIDING,
    INSPECTED_FRAME_ISEQ,
};

/* variables in ruby_debug.c */
extern VALUE mDebugger;
extern VALUE rdebug_breakpoints;
extern VALUE rdebug_catchpoints;
extern VALUE rdebug_threads_tbl;

#define IS_STARTED  (rdebug_threads_tbl != Qnil)
static inline void
debug_check_started()
{
    if (!IS_STARTED) {
	rb_raise(rb_eRuntimeError, "Debugger.start is not called yet.");
    }
}

static inline int
classname_cmp(VALUE name, VALUE klass)
{
    VALUE mod_name;
    VALUE class_name = (Qnil == name) ? rb_str_new2("main") : name;
    if (klass == Qnil) return(0);
    mod_name = rb_mod_name(klass);
    return (mod_name != Qnil 
	    && rb_str_cmp(class_name, mod_name) == 0);
}

/* Breakpoint information */
enum bp_type {
    BP_POS_TYPE,
    BP_METHOD_TYPE
};

enum hit_condition {
    HIT_COND_NONE,
    HIT_COND_GE,
    HIT_COND_EQ,
    HIT_COND_MOD
};

typedef struct {
    int   id;
    enum bp_type type;
    VALUE source;
    union {
	int line;
	ID  mid;
    } pos;
    VALUE expr;
    VALUE enabled;
    int hit_count;
    int hit_value;
    enum hit_condition hit_condition;
} debug_breakpoint_t;

/* routines in breakpoint.c */
extern int   check_breakpoint_expression(VALUE breakpoint, VALUE binding);
extern int   check_breakpoint_hit_condition(VALUE breakpoint);
extern VALUE check_breakpoints_by_method(debug_context_t *debug_context, VALUE klass, ID mid, VALUE self);
extern VALUE check_breakpoints_by_pos(debug_context_t *debug_context, VALUE file, VALUE line);
extern VALUE create_breakpoint_from_args(int argc, VALUE *argv, int id);
extern VALUE context_breakpoint(VALUE self);
extern VALUE context_set_breakpoint(int argc, VALUE *argv, VALUE self);
extern VALUE rdebug_add_catchpoint(VALUE self, VALUE value);
extern VALUE debug_catchpoints(VALUE self);
extern VALUE rdebug_remove_breakpoint(VALUE self, VALUE id_value);

extern void Init_breakpoint();
