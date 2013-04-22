/*
 * tstree.h - Ternary Search Tree
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

#ifndef _TSTree_h
#define _TSTree_h

typedef struct TSTree {
  char splitchar;
  struct TSTree *low;
  struct TSTree *equal;
  struct TSTree *high;
  void *value;
} TSTree;

TSTree *TSTree_insert(TSTree *root, const char *key, size_t len, void *value);

void *TSTree_search(TSTree *root, const char *key, size_t len);

void *TSTree_search_prefix(TSTree *root, const char *key, size_t len);

void TSTree_destroy(TSTree *root);

#endif
