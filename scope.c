#include "scope.h"

struct ht_list *ht_stack_head = NULL; 

// push a new (empty) hash table on to the stack
void scope_enter(){
    struct ht_list *ht_entry = (struct ht_list *) malloc (sizeof(struct ht_list));
    ht_entry->ht = hash_table_create(0, 0);
    ht_entry->level = ht_stack_head->level ? stack->level + 1 : 1;
    if (ht_stack_head && ht_stack_head->level > 1){
        // local
        ht_entry->var_count = ht_stack_head->var_count; 
    } else {
        // global
        ht_entry->var_count = 0;
    }

    ht_entry->next = ht_stack_head;
    ht_stack_head = ht_entry;

}

// pop one from the stack
void scope_exit(){
    if (ht_stack_head->level > 2) {
        ht_stack_head->next->var_count = stack_top->var_count;
    }
    ht_stack_head =  sht_stack_head->next;

}

//  the number of hash tables in the current stack
int scope_level(){
    return stack_top->level;
}

// insert into the current scope an entry binding a name to a symbol object
void scope_bind( const char *name, struct symbol *s ){
    s->which = ht_stack_head->var_count;
    ht_stack_head->num_vars ++;
    if (!hash_table_insert(ht_stack_head->ht, name, s)) {
        fprintf(stdout, "resolve error: failed to insert a duplicate entry.\n");
        exit(1);
    }

}

// search the stack of hash tables, looking for the closest instance of a matching definition.
struct symbol * scope_lookup( const char *name ){
    struct ht_list* curr_ht = ht_stack_head;
    while (curr_ht) {
        struct symbol *s = hash_table_lookup(curr_ht->ht, name);
        if (s) return s;
        curr_ht = curr_ht->next;
    } 
    return NULL;
}

// works like scope lookup except that it only searches the topmost table
struct symbol *	scope_lookup_current( const char *name ){
    return hash_table_lookup(curr_ht->ht, name); 
}