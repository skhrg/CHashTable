#define _BSD_SOURCE
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct table_entry
{
	char *key;
	char *value;
	struct table_entry *next;
};

typedef struct table_entry make_entry;

struct hashtable_data
{
	int size;
	struct table_entry **table;
};

typedef struct hashtable_data new_hashtable;

new_hashtable *create_hashtable(int size)
{
	new_hashtable *hashtable = NULL;
	int i;
	if(size < 1)
  {
    return NULL;
  }
	if((hashtable = malloc(sizeof(new_hashtable))) == NULL)
  {
		return NULL;
	}
	if((hashtable -> table = malloc(sizeof(make_entry *)* size)) == NULL)
  {
		return NULL;
	}
	for(i = 0; i < size; i++)
  {
		hashtable -> table[i] = NULL;
	}
	hashtable -> size = size;
	return hashtable;
}

make_entry *pairgen(char *key, char *value)
{
	make_entry *newpair;
	if((newpair = malloc(sizeof(make_entry))) == NULL)
  {
		return NULL;
	}
	if((newpair -> key = strdup(key)) == NULL)
  {
		return NULL;
	}
	if((newpair -> value = strdup(value)) == NULL)
  {
		return NULL;
	}
	newpair -> next = NULL;
	return newpair;
}

int hash(new_hashtable *hashtable, char *key)
{
	unsigned long int hashvalue;
	int i = 0;
	while(hashvalue < ULONG_MAX && i < strlen(key))
  {
		hashvalue = hashvalue << 8;
		hashvalue += key[i];
		i++;
	}
	return hashvalue % hashtable -> size;
}

char *get_value(new_hashtable *hashtable, char *key)
{
	int bin = 0;
	make_entry *pair;
	bin = hash(hashtable, key);
	pair = hashtable -> table[ bin ];
	while(pair != NULL && pair -> key != NULL && strcmp(key, pair -> key) > 0)
  {
		pair = pair -> next;
	}
	if(pair == NULL || pair -> key == NULL || strcmp(key, pair -> key) != 0)
  {
		return NULL;
	}
  else
  {
		return pair -> value;
	}
}

void set_value(new_hashtable *hashtable, char *key, char *value)
{
	int bin = 0;
	make_entry *newpair = NULL;
	make_entry *next = NULL;
	make_entry *last = NULL;
	bin = hash(hashtable, key);
	next = hashtable -> table[ bin ];

  while(next != NULL && next -> key != NULL && strcmp(key,next -> key) > 0)
  {
		last = next;
		next = next -> next;
	}
	if(next != NULL && next -> key != NULL && strcmp(key, next -> key) == 0)
  {
		free(next -> value);
		next -> value = strdup(value);
	}
  else
  {
		newpair = pairgen(key, value);
		if(next == hashtable -> table[bin])
    {
			newpair -> next = next;
			hashtable -> table[bin] = newpair;
		}
    else if (next == NULL)
    {
			last -> next = newpair;
		}
    else
    {
			newpair -> next = next;
			last -> next = newpair;
		}
	}
}
