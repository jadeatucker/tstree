/*
 * test_tstree.c - Ternary Search Tree Test
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

#include "minunit.h"
#include "dbg.h"
#include "tstree.h"

TSTree *root = NULL;

const char *key1 = "T";
const char *key2 = "TEST";
const char *key3 = "TEST3";
const char *key4 = "TSET";
const char *prefix = "TE--";

const char *val1 = "VALUE1";
const char *val2 = "VALUE2";
const char *val3 = "VALUE3";
const char *val4 = "VALUE4";

char *test_insert()
{
  root = TSTree_insert(root, key1, strlen(key1), (void *) val1);
  mu_assert(root != NULL, "Failed to insert value1 at key1");

  root = TSTree_insert(root, key2, strlen(key2), (void *) val2);
  mu_assert(root != NULL, "Failed to insert value2 at key2");

  root = TSTree_insert(root, key3, strlen(key3), (void *) val3);
  mu_assert(root != NULL, "Failed to insert value3 at key3");

  root = TSTree_insert(root, key4, strlen(key4), (void *) val4);
  mu_assert(root != NULL, "Failed to insert value4 at key4");
  
  return NULL;
}

char *test_search()
{
  void *res = TSTree_search(root, key2, strlen(key2));
  mu_assert(res == val2, "Wrong value found for key2");

  res = TSTree_search(root, prefix, strlen(prefix));
  mu_assert(res == NULL, "Should not find value for non-existant key");

  return NULL;
}

char *test_search_prefix()
{
  void *res = TSTree_search_prefix(root, "TE", strlen("TE"));
  mu_assert(res == val2, "Failed to search with prefix");

  res = TSTree_search_prefix(root, prefix, strlen(prefix));
  mu_assert(res == val1, "Failed to search with partial prefix");

  res = TSTree_search_prefix(root, key4, 2);
  mu_assert(res == val4, "Failed to search with prefix length 2")

  return NULL;
}

char *test_destroy()
{
  TSTree_destroy(root);

  return NULL;
}

char *all_tests()
{
  mu_suite_start();

  mu_run_test(test_insert);
  mu_run_test(test_search);
  mu_run_test(test_search_prefix);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
