/*
 *  MemcacheDB - A distributed key-value storage system designed for persistent:
 *
 *      http://memcachedb.googlecode.com
 *
 *  The source code of Memcachedb is most based on Memcached:
 *
 *      http://danga.com/memcached/
 *
 *  Copyright 2008 Steve Chu.  All rights reserved.
 *
 *  Use and distribution licensed under the BSD license.  See
 *  the LICENSE file for full text.
 *
 *  Authors:
 *      Steve Chu <stvchu@gmail.com>
 *
 */
 
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <event.h>
#include <netdb.h>
#include <db.h>

#define DATA_BUFFER_SIZE 2048
#define UDP_READ_BUFFER_SIZE 65536
#define UDP_MAX_PAYLOAD_SIZE 1400
#define UDP_HEADER_SIZE 8
#define MAX_SENDBUF_SIZE (256 * 1024 * 1024)
/* I'm told the max legnth of a 64-bit num converted to string is 20 bytes.
 * Plus a few for spaces, \r\n, \0 */
#define SUFFIX_SIZE 24

/** Initial size of list of items being returned by "get". */
#define ITEM_LIST_INITIAL 200

/** Initial size of the sendmsg() scatter/gather array. */
#define IOV_LIST_INITIAL 400

/** Initial number of sendmsg() argument structures to allocate. */
#define MSG_LIST_INITIAL 10

/** High water marks for buffer shrinking */
#define READ_BUFFER_HIGHWAT 8192
#define ITEM_LIST_HIGHWAT 400
#define IOV_LIST_HIGHWAT 600
#define MSG_LIST_HIGHWAT 100

#define MAX_REP_PRIORITY 1000000
#define MAX_REP_ACK_POLICY 6
#define MAX_REP_NSITES 1000

/* Default path for a database, and its env home */
#define DBFILE "data.db"
#define DBHOME "/data1/memcachedb"

#define BDB_EID_SELF -3

#define RGET_MAX_ITEMS 100

/* Get a consistent bool type */
#if HAVE_STDBOOL_H
# include <stdbool.h>
#else
  typedef enum {false = 0, true = 1} bool;
#endif

#if HAVE_STDINT_H
# include <stdint.h>
#else
 typedef unsigned char             uint8_t;
#endif

/* unistd.h is here */
#if HAVE_UNISTD_H
# include <unistd.h>
#endif

/* 64-bit Portable printf */
/* printf macros for size_t, in the style of inttypes.h */
#ifdef _LP64
#define __PRIS_PREFIX "z"
#else
#define __PRIS_PREFIX
#endif

/* Use these macros after a % in a printf format string
   to get correct 32/64 bit behavior, like this:
   size_t size = records.size();
   printf("%"PRIuS"\n", size); */

#define PRIdS __PRIS_PREFIX "d"
#define PRIxS __PRIS_PREFIX "x"
#define PRIuS __PRIS_PREFIX "u"
#define PRIXS __PRIS_PREFIX "X"
#define PRIoS __PRIS_PREFIX "o"

struct stats {
    uint32_t      curr_conns;
    uint32_t      total_conns;
    uint32_t      conn_structs;
    uint64_t      get_cmds;
    uint64_t      set_cmds;
    uint64_t      get_hits;
    uint64_t      get_misses;
    time_t        started;          /* when the process was started */
    uint64_t      bytes_read;
    uint64_t      bytes_written;
};

#define MAX_VERBOSITY_LEVEL 2

struct settings {
    size_t item_buf_size;
    int maxconns;
    int port;
    int udpport;
    char *inter;
    int verbose;
    char *socketpath;   /* path to unix socket if using local socket */
    int access;  /* access mask (a la chmod) for unix domain socket */
    int num_threads;        /* number of libevent threads to run */
};

extern struct stats stats;
extern struct settings settings;

struct bdb_version {
    int majver;
    int minver;
    int patch;
};

enum mdb_rep_role { MDB_MASTER, MDB_CLIENT, MDB_UNKNOWN };

struct bdb_settings {
    char *db_file;    /* db filename, where dbfile located. */
    char *env_home;    /* db env home dir path */
    char *log_home;    /* db log home dir path*/
    u_int64_t cache_size; /* cache size */
    u_int32_t txn_lg_bsize; /* transaction log buffer size */
    u_int32_t page_size;    /* underlying database pagesize*/
    DBTYPE db_type;
    int txn_nosync;    /* DB_TXN_NOSYNC flag, if 1 will lose transaction's durability for performance */
    int log_auto_remove;    /* DB_LOG_AUTO_REMOVE flag, if 1 will make catastrophic recovery impossible. */
    int dldetect_val; /* do deadlock detect every *db_lock_detect_val* millisecond, 0 for disable */
    int chkpoint_val;  /* do checkpoint every *db_chkpoint_val* second, 0 for disable */
    int memp_trickle_val;  /* do memp_trickle every *memp_trickle_val* second, 0 for disable */
    int memp_trickle_percent; /* percent of the pages in the cache that should be clean.*/
    u_int32_t db_flags; /* database open flags */
    u_int32_t env_flags; /* env open flags */

    int is_replicated; /* replication is ON?? */

    char *rep_localhost; /* local host in replication */
    int rep_localport;  /* local port in replication */
    char *rep_remotehost; /* remote host in replication */
    int rep_remoteport;  /* remote port in replication */

    int rep_whoami;  /* replication role, MDB_MASTER/MDB_CLIENT/MDB_UNKNOWN */
    int rep_master_eid; /* replication master's eid */

    u_int32_t rep_start_policy;
    u_int32_t rep_nsites;

    int rep_ack_policy;

    u_int32_t rep_ack_timeout;
    u_int32_t rep_chkpoint_delay;
    u_int32_t rep_conn_retry;
    u_int32_t rep_elect_timeout;
    u_int32_t rep_elect_retry;
    u_int32_t rep_heartbeat_monitor; /* only available on a client */
    u_int32_t rep_heartbeat_send;    /* only available on a master */
    u_int32_t rep_lease_timeout;

    int rep_bulk;
	  int rep_lease;  /* if master lease is enabled? */

    u_int32_t rep_priority;

    u_int32_t rep_req_max;
    u_int32_t rep_req_min;

    u_int32_t rep_fast_clock;
    u_int32_t rep_slow_clock;

    u_int32_t rep_limit_gbytes; 
    u_int32_t rep_limit_bytes; 
};

extern struct bdb_settings bdb_settings;
extern struct bdb_version bdb_version;

typedef struct _stritem {
    int             nbytes;     /* size of data */
    uint8_t         nsuffix;    /* length of flags-and-length string */
    uint8_t         nkey;       /* key length, w/terminating null and padding */
    void * end[];
    /* then null-terminated key */
    /* then " flags length\r\n" (no terminating null) */
    /* then data with terminating \r\n (no terminating null; it's binary!) */
} item;

#define ITEM_key(item) ((char*)&((item)->end[0]))

/* warning: don't use these macros with a function, as it evals its arg twice */
#define ITEM_suffix(item) ((char*) &((item)->end[0]) + (item)->nkey + 1)
#define ITEM_data(item) ((char*) &((item)->end[0]) + (item)->nkey + 1 + (item)->nsuffix)
#define ITEM_ntotal(item) (sizeof(struct _stritem) + (item)->nkey + 1 + (item)->nsuffix + (item)->nbytes)

enum conn_states {
    conn_listening,  /** the socket which listens for connections */
    conn_read,       /** reading in a command line */
    conn_write,      /** writing out a simple response */
    conn_nread,      /** reading in a fixed number of bytes */
    conn_swallow,    /** swallowing unnecessary bytes w/o storing */
    conn_closing,    /** closing this connection */
    conn_mwrite,     /** writing out many items sequentially */
};

#define NREAD_ADD 1
#define NREAD_SET 2
#define NREAD_REPLACE 3
#define NREAD_APPEND 4
#define NREAD_PREPEND 5

typedef struct conn conn;
struct conn {
    int    sfd;
    int    state;
    struct event event;
    short  ev_flags;
    short  which;   /** which events were just triggered */

    char   *rbuf;   /** buffer to read commands into */
    char   *rcurr;  /** but if we parsed some already, this is where we stopped */
    int    rsize;   /** total allocated size of rbuf */
    int    rbytes;  /** how much data, starting from rcur, do we have unparsed */

    char   *wbuf;
    char   *wcurr;
    int    wsize;
    int    wbytes;
    int    write_and_go; /** which state to go into after finishing current write */
    void   *write_and_free; /** free this memory after finishing writing */

    char   *ritem;  /** when we read in an item's value, it goes here */
    int    rlbytes;

    /* data for the nread state */

    /**
     * item is used to hold an item structure created after reading the command
     * line of set/add/replace commands, but before we finished reading the actual
     * data. The data is read into ITEM_data(item) to avoid extra copying.
     */

    void   *item;     /* for commands set/add/replace  */
    int    item_comm; /* which one is it: set/add/replace */

    /* data for the swallow state */
    int    sbytes;    /* how many bytes to swallow */

    /* data for the mwrite state */
    struct iovec *iov;
    int    iovsize;   /* number of elements allocated in iov[] */
    int    iovused;   /* number of elements used in iov[] */

    struct msghdr *msglist;
    int    msgsize;   /* number of elements allocated in msglist[] */
    int    msgused;   /* number of elements used in msglist[] */
    int    msgcurr;   /* element in msglist[] being transmitted now */
    int    msgbytes;  /* number of bytes in current msg */

    item   **ilist;   /* list of items to write out */
    int    isize;
    item   **icurr;
    int    ileft;

    /* data for UDP clients */
    bool   udp;       /* is this is a UDP "connection" */
    int    request_id; /* Incoming UDP request ID, if this is a UDP "connection" */
    struct sockaddr request_addr; /* Who sent the most recent request */
    socklen_t request_addr_size;
    unsigned char *hdrbuf; /* udp packet headers */
    int    hdrsize;   /* number of headers' worth of space is allocated */
    conn   *next;     /* Used for generating a list of conn structures */
};

/*
 * Functions
 */

/* bdb management */
void bdb_settings_init(void);
void bdb_env_init(void);
void bdb_db_open(void);
void start_chkpoint_thread(void);
void start_memp_trickle_thread(void);
void start_dl_detect_thread(void);
void bdb_db_close(void);
void bdb_env_close(void);
void bdb_chkpoint(void);
int bdb_defcmp(void *a, size_t i, void *b, size_t j);

/* item management */
void item_init(void);
item *do_item_from_freelist(void);
int do_item_add_to_freelist(item *it);
item *item_alloc1(char *key, const size_t nkey, const int flags, const int nbytes);
item *item_alloc2(size_t ntotal);
int item_free(item *it);
item *item_get(char *key, size_t nkey);
int item_put(char *key, size_t nkey, item *it);
int item_delete(char *key, size_t nkey);
int item_exists(char *key, size_t nkey);
item *item_cget(DBC *cursorp, char *start, size_t nstart, u_int32_t flags);

/* bdb related stats */
void stats_bdb(char *temp);
void stats_rep(char *temp);
void stats_repmgr(char *temp);
void stats_repcfg(char *temp);
void stats_repms(char *temp);

/* conn management */
conn *do_conn_from_freelist();
bool do_conn_add_to_freelist(conn *c);
conn *conn_new(const int sfd, const int init_state, const int event_flags, const int read_buffer_size, const bool is_udp, struct event_base *base);

char *do_add_delta(const bool incr, const int64_t delta, char *buf, char *key, size_t nkey);
int do_store_item(item *item, int comm);

/*
 * In multithreaded mode, we wrap certain functions with lock management and
 * replace the logic of some other functions. All wrapped functions have
 * "mt_" and "do_" variants. In multithreaded mode, the plain version of a
 * function is #define-d to the "mt_" variant, which often just grabs a
 * lock and calls the "do_" function. In singlethreaded mode, the "do_"
 * function is called directly.
 *
 * Functions such as the libevent-related calls that need to do cross-thread
 * communication in multithreaded mode (rather than actually doing the work
 * in the current thread) are called via "dispatch_" frontends, which are
 * also #define-d to directly call the underlying code in singlethreaded mode.
 */
#ifdef USE_THREADS

void thread_init(int nthreads, struct event_base *main_base);
int  dispatch_event_add(int thread, conn *c);
void dispatch_conn_new(int sfd, int init_state, int event_flags, int read_buffer_size, int is_udp);

/* Lock wrappers for cache functions that are called from main loop. */
char *mt_add_delta(const int incr, const int64_t delta, char *buf, char *key, size_t nkey);
conn *mt_conn_from_freelist(void);
bool  mt_conn_add_to_freelist(conn *c);
int   mt_is_listen_thread(void);
item *mt_item_from_freelist(void);
int mt_item_add_to_freelist(item *it);
void  mt_stats_lock(void);
void  mt_stats_unlock(void);
int   mt_store_item(item *item, int comm);

# define add_delta(x,y,z,a,b)        mt_add_delta(x,y,z,a,b)
# define conn_from_freelist()        mt_conn_from_freelist()
# define conn_add_to_freelist(x)     mt_conn_add_to_freelist(x)
# define is_listen_thread()          mt_is_listen_thread()
# define item_from_freelist()        mt_item_from_freelist()
# define item_add_to_freelist(x)     mt_item_add_to_freelist(x)
# define store_item(x,y)             mt_store_item(x,y)

# define STATS_LOCK()                mt_stats_lock()
# define STATS_UNLOCK()              mt_stats_unlock()

#else /* !USE_THREADS */

# define add_delta(x,y,z,a,b)         do_add_delta(x,y,z,a,b)
# define conn_from_freelist()         do_conn_from_freelist()
# define conn_add_to_freelist(x)      do_conn_add_to_freelist(x)
# define dispatch_conn_new(x,y,z,a,b) conn_new(x,y,z,a,b,main_base)
# define dispatch_event_add(t,c)      event_add(&(c)->event, 0)
# define is_listen_thread()           1
# define item_from_freelist()         do_item_from_freelist()
# define item_add_to_freelist(x)      do_item_add_to_freelist(x)
# define store_item(x,y)              do_store_item(x,y)
# define thread_init(x,y)             0

# define STATS_LOCK()                /**/
# define STATS_UNLOCK()              /**/

#endif /* !USE_THREADS */


#define BDB_CLEANUP_DBT() \
    memset(&dbkey, 0, sizeof(dbkey)); \
    memset(&dbdata, 0, sizeof(dbdata))

extern DB_ENV *env;
extern DB *dbp;
extern int daemon_quit;
