#ifndef UFTRACE_FSTACK_H
#define UFTRACE_FSTACK_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "uftrace.h"
#include "utils/filter.h"

struct sym;

enum fstack_flag {
	FSTACK_FL_FILTERED	= (1U << 0),
	FSTACK_FL_NOTRACE	= (1U << 1),
	FSTACK_FL_NORECORD	= (1U << 2),
	FSTACK_FL_EXEC		= (1U << 3),
	FSTACK_FL_LONGJMP	= (1U << 4),
};

enum context {
	FSTACK_CTX_UNKNOWN	= 0,
	FSTACK_CTX_USER		= 1,
	FSTACK_CTX_KERNEL	= 2,
};

struct time_filter_stack {
	struct time_filter_stack *next;
	uint64_t threshold;
	int depth;
	enum context context;
};

struct uftrace_task_reader {
	int tid;
	bool valid;
	bool done;
	bool lost_seen;
	bool sched_out_seen;
	bool fork_handled;
	bool fstack_set;
	bool display_depth_set;
	FILE *fp;
	struct sym *func;
	struct uftrace_task *t;
	struct uftrace_data *h;
	struct uftrace_record ustack;
	struct uftrace_record kstack;
	struct uftrace_record estack;
	struct uftrace_record xstack;
	struct uftrace_record *rstack;
	struct uftrace_rstack_list rstack_list;
	struct uftrace_rstack_list event_list;
	int stack_count;
	int lost_count;
	int user_stack_count;
	int display_depth;
	int user_display_depth;
	int fork_display_depth;
	int column_index;
	int event_color;
	int sched_cpu;
	enum context ctx;
	uint64_t timestamp;
	uint64_t timestamp_last;
	struct filter {
		int	in_count;
		int	out_count;
		int	depth;
		struct time_filter_stack *time;
	} filter;
	struct fstack {
		uint64_t addr;
		bool valid;
		int orig_depth;
		unsigned long flags;
		uint64_t total_time;
		uint64_t child_time;
	} *func_stack;
	struct fstack_arguments args;
};

enum argspec_string_bits {
	/* bit index */
	NEEDS_PAREN_BIT,
	NEEDS_SEMI_COLON_BIT,
	HAS_MORE_BIT,
	IS_RETVAL_BIT,
	NEEDS_ASSIGNMENT_BIT,
	NEEDS_JSON_BIT,

	/* bit mask */
	NEEDS_PAREN		= (1U << NEEDS_PAREN_BIT),
	NEEDS_SEMI_COLON	= (1U << NEEDS_SEMI_COLON_BIT),
	HAS_MORE		= (1U << HAS_MORE_BIT),
	IS_RETVAL		= (1U << IS_RETVAL_BIT),
	NEEDS_ASSIGNMENT	= (1U << NEEDS_ASSIGNMENT_BIT),
	NEEDS_JSON		= (1U << NEEDS_JSON_BIT),
};

extern bool fstack_enabled;
extern bool live_disabled;

struct uftrace_task_reader *get_task_handle(struct uftrace_data *handle,
					   int tid);
void reset_task_handle(struct uftrace_data *handle);

void fstack_setup_task(char *tid_filter, struct uftrace_data *handle);

int read_rstack(struct uftrace_data *handle,
		struct uftrace_task_reader **task);
int peek_rstack(struct uftrace_data *handle,
		struct uftrace_task_reader **task);
void fstack_consume(struct uftrace_data *handle,
		    struct uftrace_task_reader *task);

int read_task_ustack(struct uftrace_data *handle,
		     struct uftrace_task_reader *task);
int read_task_args(struct uftrace_task_reader *task,
		   struct uftrace_record *rstack,
		   bool is_retval);

static inline bool is_user_record(struct uftrace_task_reader *task,
				  struct uftrace_record *rec)
{
	return rec == &task->ustack;
}

static inline bool is_kernel_record(struct uftrace_task_reader *task,
				    struct uftrace_record *rec)
{
	return rec == &task->kstack;
}

static inline bool is_event_record(struct uftrace_task_reader *task,
				  struct uftrace_record *rec)
{
	return rec == &task->estack;
}

static inline bool is_extern_record(struct uftrace_task_reader *task,
				    struct uftrace_record *rec)
{
	return rec == &task->xstack;
}

void setup_fstack_args(char *argspec, char *retspec,
		       struct uftrace_data *handle,
		       struct uftrace_filter_setting *setting);
int fstack_setup_filters(struct opts *opts, struct uftrace_data *handle);

int fstack_entry(struct uftrace_task_reader *task,
		 struct uftrace_record *rstack,
		 struct uftrace_trigger *tr);
void fstack_exit(struct uftrace_task_reader *task);
int fstack_update(int type, struct uftrace_task_reader *task,
		  struct fstack *fstack);
struct uftrace_task_reader *fstack_skip(struct uftrace_data *handle,
				       struct uftrace_task_reader *task,
				       int curr_depth, struct opts *opts);
bool fstack_check_filter(struct uftrace_task_reader *task);
bool fstack_check_opts(struct uftrace_task_reader *task, struct opts *opts);

void get_argspec_string(struct uftrace_task_reader *task,
		        char *args, size_t len,
		        enum argspec_string_bits str_mode);

#define EXTERN_DATA_MAX  1024

struct uftrace_extern_reader {
	FILE			*fp;
	bool			valid;
	uint64_t		time;
	char			msg[EXTERN_DATA_MAX];
	struct uftrace_record	rec;
};

int setup_extern_data(struct uftrace_data *handle, struct opts *opts);
int read_extern_data(struct uftrace_extern_reader *extn);
struct uftrace_record * get_extern_record(struct uftrace_extern_reader *extn,
					  struct uftrace_record *rec);
int finish_extern_data(struct uftrace_data *handle);

static inline bool has_extern_data(struct uftrace_data *handle)
{
	return handle->extn != NULL;
}

#endif /* UFTRACE_FSTACK_H */
