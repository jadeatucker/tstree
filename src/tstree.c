/*
 * tstree.c - Ternary Search Tree Implementation
 * Copyright (C) 2013  Jade Tucker
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <assert.h>

#include "dbg.h"
#include "tstree.h"

static inline TSTree *TSTree_insert_base(TSTree *root, TSTree *node, const char *key, size_t len, void *value)
{
  if(node == NULL) {
    node = (TSTree *) calloc(1, sizeof(TSTree));
    check_mem(node);

    if(root == NULL) {
      root = node;
    }
    
    node->splitchar = *key;
  }

  if(*key < node->splitchar) {
    node->low = TSTree_insert_base(root, node->low, key, len, value);
  } else if(*key == node->splitchar) {
    if(len > 1) {
      node->equal = TSTree_insert_base(root, node->equal, key+1, len-1, value);
    } else {
      assert(node->value == NULL && "Attempt to insert duplicate key");
      node->value = value;
    }
  } else {
    node->high = TSTree_insert_base(root, node->high, key, len, value);
  }

  return node;

error:
  return NULL;
}

TSTree *TSTree_insert(TSTree *root, const char *key, size_t len, void *value)
{
  return TSTree_insert_base(root, root, key, len, value);
}

void *TSTree_search(TSTree *root, const char *key, size_t len)
{
  check(len > 0, "Attempt to get key of length 0");
  check(root != NULL, "Root cannot be NULL");

  TSTree *node = root;
  size_t i = 0;

  while(i < len && node) {
    if(key[i] < node->splitchar) {
      node = node->low;
    } else if(key[i] == node->splitchar) {
      i++;
      if(i < len) node = node->equal;
    } else {
      node = node->high;
    }
  }

  if(node) {
    return node->value;
  }

error:
  return NULL;
}

void *TSTree_search_prefix(TSTree *root, const char *key, size_t len)
{
  check(len > 0, "Attempt to get key of length 0");
  check(root != NULL, "Root cannot be NULL");

  TSTree *last = NULL;
  TSTree *node = root;
  size_t i = 0;

  while(i < len && node) {
    if(key[i] < node->splitchar) {
      node = node->low;
    } else if(key[i] == node->splitchar) {
      if(node->value) last = node;
      i++;
      if(i < len) {
        node = node->equal;
      }
    } else {
      node = node->high;
    }
  }

  node = node ? node : last;

  while(node && !node->value) {
    node = node->equal;
  }

  if(node)
    return node->value;
error:
  return NULL;
}

void TSTree_destroy(TSTree *root)
{
  if(root == NULL) return;

  if(root->low) TSTree_destroy(root->low);

  if(root->equal) TSTree_destroy(root->equal);

  if(root->high) TSTree_destroy(root->high);

  free(root);

  return;
}
