#ifndef SCOPE_H
#define SCOPE_H

#include "symbol.h"
#include "hash_table.h"

struct ht_list {
	struct hash_table *ht;
	int level;
	int var_count;
	struct ht_list *next;
};


void            scope_enter();	// push a new (empty) hash table on to the stack
void            scope_exit();	// pop one from the stack
int 			scope_level();	//  the number of hash tables in the current stack
int				scope_var_count();	//  the number of variables in this hashtable
void            scope_bind( const char *name, struct symbol *s ); // insert into the current scope an entry binding a name to a symbol object
struct symbol * scope_lookup( const char *name ); // search the stack of hash tables, looking for the closest instance of a matching definition.
struct symbol *	scope_lookup_current( const char *name );

#endif