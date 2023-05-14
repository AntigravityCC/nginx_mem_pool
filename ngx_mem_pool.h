
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define ngx_align(d, a)             (((d) + (a - 1)) & ~(a - 1))
#define ngx_align_ptr(p, a)         (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#define ngx_pagesize                4096
#define NGX_MAX_ALLOC_FROM_POOL     (ngx_pagesize - 1)

#define NGX_DEFAULT_POOL_SIZE       (16 * 1024)

#define NGX_POOL_ALIGNMENT          16
#define NGX_MIN_POOL_SIZE           ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),NGX_POOL_ALIGNMENT)
#define NGX_ALIGNMENT               sizeof(unsigned long)    /* platform word */

#define ngx_alloc                   malloc
#define ngx_free                    free
#define ngx_memalign(alignment, size)  ngx_alloc(size)

#define ngx_memzero(buf, n)         (void) memset(buf, 0, n)
#define ngx_memset(buf, c, n)       (void) memset(buf, c, n)
#define ngx_inline                  inline

#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6

typedef int32_t                     ngx_int_t;
typedef uint32_t                    ngx_uint_t;
typedef struct ngx_pool_s           ngx_pool_t;
typedef struct ngx_pool_data_s      ngx_pool_data_t;
typedef void (*ngx_pool_cleanup_pt) (void *data);
typedef struct ngx_pool_cleanup_s   ngx_pool_cleanup_t;
typedef struct ngx_pool_large_s     ngx_pool_large_t;

struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    void                 *data;
    ngx_pool_cleanup_t   *next;
};

struct ngx_pool_large_s {
    ngx_pool_large_t     *next;
    void                 *alloc;
};

struct ngx_pool_data_s{
    u_char               *last;
    u_char               *end;
    ngx_pool_t           *next;
    ngx_uint_t            failed;
};

struct ngx_pool_s{
    ngx_pool_data_t       d;
    size_t                max;
    ngx_pool_t           *current;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
};

ngx_pool_t *ngx_create_pool(size_t size);
void ngx_destroy_pool(ngx_pool_t *pool);
void ngx_reset_pool(ngx_pool_t *pool);

void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);

ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);

#endif /* _NGX_PALLOC_H_INCLUDED_ */
