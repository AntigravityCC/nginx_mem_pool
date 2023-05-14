#include <stdio.h>
#include <string.h>
#include "ngx_mem_pool.h"

typedef struct _data_str
{
    int *data;
	char *str;
}data_str;

void clean_func_back(void *p1)
{
	char *p = (char*)p1;
	printf("free ptr mem!:[%s]\n", p);
	free(p);
}

int main()
{
    ngx_pool_t  *pool = ngx_create_pool(512);
    if (pool == NULL) {
        return NULL;
    }

    data_str *data = ngx_palloc(pool, sizeof(data_str));
    if (data == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }

    data->data = ngx_palloc(pool, sizeof(int));
    data->data = 12345;
    printf("data->date:%d\n",data->data);

    data->str = malloc(20);
    strncpy(data->str, "hello world", 20);

    printf("data->str:%s\n",data->str);

    ngx_pool_cleanup_t *clean = ngx_pool_cleanup_add(pool, sizeof(char*));
    clean->handler = clean_func_back;
    clean->data = data->str;

    ngx_destroy_pool(pool);
    return 0;
}