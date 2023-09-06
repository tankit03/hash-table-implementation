/*
 * In this file, you will write the structures and functions needed to
 * implement a hash table.  Feel free to implement any helper functions
 * you need in this file to implement a hash table.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: tanish hupare
 * Email: huparet@oregonstate.edu
 */

#include <stdlib.h>

#include "dynarray.h"
#include "list.h"
#include "hash_table.h"



struct ht{

    int size;
    struct dynarray* array;

};

struct ht_node{

    void* value;
    void* key;

};

/*
 * This function should allocate and initialize an empty hash table and
 * return a pointer to it.
 */



struct ht* ht_create(){
   struct ht* hash_t = malloc(sizeof(struct ht));
   hash_t->size = 0;
   hash_t->array = dynarray_create(); 


    for(int i = 0; i < get_capacity(hash_t->array); i++){
        struct list* l = list_create();
        dynarray_insert(hash_t->array, l);
    }

    return hash_t;

}


/*

the function first calculates the hash number for the given key using the provided 
converted function and looks up corresponding list in the dynamic array. if a list 
is found it goes through the list to find a matching hash table node based on the key.
when the node is found, the function returns a pointer to the hash table node. if no
matching node is found, the function returns NULL

*/

struct ht_node* get_hashnode(struct ht* ht, void* key, int (*convert)(void*)){

    int hashnumber = ht_hash_func(ht, key, convert);

    struct list* lst = dynarray_get(ht->array, hashnumber);

    if(lst != NULL){

       struct node* nd = get_head(lst);

       while(nd != NULL){
           struct ht_node* ht_nd = get_val(nd);


           if(convert(ht_nd->key) == convert(key)){
               return ht_nd;
           }
           nd = get_next(nd);
       }
    }
    return NULL;
}

/*
 * This function should free the memory allocated to a given hash table.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the hash table.  That is the responsibility of the caller.
 *
 * Params:
 *   ht - the hash table to be destroyed.  May not be NULL.
 */
void ht_free(struct ht* ht){
    
    for(int i = 0; i < dynarray_size(ht->array); i++){
        struct list* lst = dynarray_get(ht->array, i);
        free_list(lst);
        
    }


    dynarray_free(ht->array);
    free(ht);
    
}

/*
 * This function should return 1 if the specified hash table is empty and
 * 0 otherwise.
 *
 * Params:
 *   ht - the hash table whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if ht is empty and 0 otherwise.
 */
int ht_isempty(struct ht* ht){

    if(ht->size == 0){

        return 1;

    }else{

        return 0;
    }

    
}


/*
 * This function returns the size of a given hash table (i.e. the number of
 * elements stored in it, not the capacity).
 */
int ht_size(struct ht* ht){
    
   return ht->size;

}


/*
 * This function takes a key, maps it to an integer index value in the hash table,
 * and returns it. The hash algorithm is totally up to you. Make sure to consider
 * Determinism, Uniformity, and Speed when design the hash algorithm
 *
 * Params:
 *   ht - the hash table into which to store the element.  May not be NULL.
 *   key - the key of the element to be stored
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
int ht_hash_func(struct ht* ht, void* key, int (*convert)(void*)){
   

    int idx = convert(key);
    int hash_code = idx % dynarray_size(ht->array);

    return hash_code;

    
}


/*
 * This function should insert a given element into a hash table with a
 * specified key.  Note that you cannot have two same keys in one hash table.
 * If the key already exists, update the value associated with the key.  
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * Resolution of collisions is requried, use either chaining or open addressing.
 * If using chaining, double the number of buckets when the load factor is >= 4
 * If using open addressing, double the array capacity when the load factor is >= 0.75
 * load factor = (number of elements) / (hash table capacity)
 *
 * Params:
 *   ht - the hash table into which to insert an element.  May not be NULL.
 *   key - the key of the element
 *   value - the value to be inserted into ht.
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */




void ht_insert(struct ht* ht, void* key, void* value, int (*convert)(void*)){


    int hashnum = ht_hash_func(ht, key, convert);
    struct node* temp_n = get_head(dynarray_get(ht->array,hashnum));
    float load_fac = (float)ht->size/(dynarray_size(ht->array));



    if(load_fac >= 4){

        int old_cap = dynarray_size(ht->array);
        int new_cap = (dynarray_size(ht->array)) * 2;


        struct dynarray* arr = dynarray_create();
        _dynarray_resize(arr, new_cap);


        for(int i = 0; i < get_capacity(arr); i++){

            dynarray_insert(arr, list_create());

        }

        for(int i = 0; i < old_cap; i++){

            struct list* lis = dynarray_get(ht->array, i);
            struct node* nd = get_head(lis);

            while(nd != NULL){

                struct ht_node* htn = get_val(nd);

                int new_hashnum = ht_hash_func(ht, key, convert);
                //int new_hashnum = convert(htn->key) % new_cap;

                struct list* new_l = dynarray_get(arr, new_hashnum);
                list_insert(new_l, htn);
                nd = get_next(nd);

            }
        }
    

        for(int i =0; i < get_capacity(ht->array); i++){
            list_free(dynarray_get(ht->array, i));
        }
        dynarray_free(ht->array);
        ht->array = arr;
    }

        struct ht_node* htn_temp = get_hashnode(ht, key, convert);

        if(htn_temp != NULL){
            if(htn_temp->key == key){
                htn_temp->value = value;
                return;
            }
        }
        else{

            ht->size++;
            struct list* lst = dynarray_get(ht->array, hashnum);
            struct ht_node* ins = malloc(sizeof(struct ht_node));
            ins->key = key;
            ins->value = value;
            list_insert(lst, ins);

        }
        return;

}




/*
 * This function should search for a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, return the corresponding value (void*) of the element,
 * otherwise, return NULL
 *
 * Params:
 *   ht - the hash table into which to loop up for an element.  May not be NULL.
 *   key - the key of the element to search for
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */




void* ht_lookup(struct ht* ht, void* key, int (*convert)(void*)){

    int hash_num = ht_hash_func(ht,key, convert);

    struct list* l_list = dynarray_get(ht->array, hash_num);

    if(l_list != NULL){
        struct node* node = get_head(l_list);
        
        while(node != NULL){
            struct ht_node* t_ht = get_val(node);

            if(convert(t_ht->key) == convert(key)){
                return t_ht->value;
            }
            node = get_next(node);
        }
    }

    return NULL;

}

/*

this function takes in a pointer to a struct ht, a void pointer to the key being 
searched and a function pointer to convert the key. it then returns a pointers to a 
'struct ht_node' containing the key and its associated data if the key is found, otherwise 
is null
*/


struct ht_node* get_ht(struct ht* ht, void* key, int (*convert)(void*)){

  int hashnum = ht_hash_func(ht, key, convert);

  struct list* lst = dynarray_get(ht->array, hashnum);

  if(lst != NULL){


    struct node* nd = get_head(lst);
    while(lst != NULL){

        struct ht_node* new  = get_val(nd);

        if(convert(new->key) == convert(key)){
            return new;
        }
        nd = get_next(nd);
    }
  }
    return NULL;
}


/*
 * This function should remove a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, remove the element and return, otherwise, do nothing and return 
 *
 * Params:
 *   ht - the hash table into which to remove an element.  May not be NULL.
 *   key - the key of the element to remove
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void ht_remove(struct ht* ht, void* key, int (*convert)(void*)){


    int hashnum = ht_hash_func(ht, key, convert);

    struct list* lst = dynarray_get(ht->array, hashnum);

    struct ht_node* nd = get_hashnode(ht, key, convert);

    if(lst != NULL){

        struct node* nd = get_head(lst);

        while(nd != NULL){
            
            struct ht_node* temp_nd = get_val(nd);

            if(convert(temp_nd->key) == convert(key)){

                node_remove(lst, temp_nd);
                ht->size--;
                free(temp_nd);
                return;

            }
            nd = get_next(nd);

        }

    }
    
    return;

} 
