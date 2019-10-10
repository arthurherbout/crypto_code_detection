#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "map.h"

static int __map_node_new(struct map *map,struct map_node **node)
{
    *node=malloc(sizeof(struct map_node));
    if(NULL==(*node)) return ENOMEM;

    #ifdef DEBUG
    map->__dbg_malloc_cnt+=1;
    #endif

    bzero(*node,sizeof(struct map_node));

    return 0;
}


static int  __map_list_add(struct map *map,struct map_node *node)
{
    struct __map_list *tmplist;
    
    tmplist=malloc(sizeof(struct __map_list));

    if(NULL==tmplist) return ENOMEM;

    #ifdef DEBUG
    map->__dbg_malloc_cnt+=1;
    #endif

    bzero(tmplist,sizeof(struct __map_list));
    
    if(NULL==map->list_header){
        map->list_header=tmplist;
        map->list_end=tmplist;
    }else{
        tmplist->previous=map->list_end;
        map->list_end->next=tmplist;
        map->list_end=tmplist;
    }

    tmplist->node=node;
    node->list=tmplist;

    return 0;
}

static void __map_list_del(struct map *map,struct __map_list *list)
{
    if(NULL==list->previous) map->list_header=list->next;
    if(NULL==list->next) map->list_end=list->previous;

    if(NULL!=list->previous && NULL!=list->next){
        list->previous->next=list->next;
        list->next->previous=list->previous;
    }
    
    #ifdef DEBUG
    map->__dbg_free_cnt+=1;
    #endif

    free(list);
}

static void __map_delete(struct map *map,struct __map_list *list)
{
    struct map_node *cur_node,*pre_node;
    unsigned char key_v;

    cur_node=list->node;

    while(NULL!=cur_node){
        key_v=cur_node->key_v;
        pre_node=cur_node->previous;

        // root node
        if(NULL==pre_node){
            if(0==cur_node->slot_flags){
                free(cur_node);
                #ifdef DEBUG
                map->__dbg_free_cnt+=1;
                #endif
                map->root=NULL;
            }
            break;
        }

        pre_node->refcnt[key_v]-=1;
        
        if(0==pre_node->refcnt[key_v]){
                pre_node->next[key_v]=NULL;
                pre_node->slot_flags-=1;
        }

        if(0==cur_node->slot_flags){
            free(cur_node);

            #ifdef DEBUG
            map->__dbg_free_cnt+=1;
            #endif
        }

        cur_node=pre_node;
    }

}

int map_new(struct map **map,map_ksize_t length)
{
    *map=malloc(sizeof(struct map));
    if(NULL==*map) return -1;
    
    bzero(*map,sizeof(struct map));

    (*map)->length=length;
   

    return 0;
}

void map_release(struct map *map,map_del_func_t func)
{
    struct __map_list *list=map->list_header,*tmplist;
    
    while(NULL!=list){
        
        if(NULL!=func) func(list->node->data);
        __map_delete(map,list);
        tmplist=list;
        list=list->next;

        free(tmplist);

        #ifdef DEBUG
        map->__dbg_free_cnt+=1;
        #endif
    }
}

int map_add(struct map *map,const char *key,void *data)
{
    struct map_node *tmp_node,*tmp_node_old,*root;
    unsigned char key_v;
    int rs=0;

    root=map->root;
    if(NULL==root){
        rs=__map_node_new(map,&root);
        if(rs) return ENOMEM;
        map->root=root;
    }

    tmp_node_old=root;

    for(int n=0;n<map->length;n++){
        key_v=*key++;
        tmp_node=tmp_node_old->next[key_v];

        if(NULL!=tmp_node){
            tmp_node_old->refcnt[key_v]+=1;
            tmp_node_old=tmp_node;
            continue;
        }
        rs=__map_node_new(map,&tmp_node);
        if(rs) return ENOMEM;
        tmp_node_old->slot_flags+=1;
        tmp_node_old->refcnt[key_v]+=1;
        tmp_node->key_v=key_v;
        tmp_node_old->next[key_v]=tmp_node;
        tmp_node->previous=tmp_node_old;
        tmp_node_old=tmp_node;
    }

    tmp_node->data=data;
    rs=__map_list_add(map,tmp_node);

    return rs;
}

void map_del(struct map *map,const char *key,map_del_func_t func)
{
    unsigned char key_v=0;
    struct map_node *tmpnode=map->root;

    for(int n=0;n<map->length;n++){
        key_v=*key++;
        tmpnode=tmpnode->next[key_v];
        if(NULL==tmpnode) break;
    }

    if(NULL!=tmpnode){
        if(NULL!=func) func(tmpnode->data);
        __map_delete(map,tmpnode->list);
        __map_list_del(map,tmpnode->list);
    }

}

void *map_find(struct map *map,const char *key,char *is_find)
{
    struct map_node *tmp_node;
    unsigned char key_v;
    void *data=NULL;
    

    *is_find=0;
    tmp_node=map->root;

    if(NULL==tmp_node) return NULL;

    for(int n=0;n<map->length;n++){
        key_v=*key++;
        tmp_node=tmp_node->next[key_v];
        if(NULL==tmp_node) break;
    }

    if(NULL!=tmp_node){
        *is_find=1;
        data=tmp_node->data;
    }

    return data;
}

void map_each(struct map *map,map_each_func_t func)
{
    struct __map_list *list=map->list_header,*tmplist;
    
    while(NULL!=list){
        tmplist=list->next;
        func(list->node->data);
        list=tmplist;
    }
}