/*
 * like -- find similar strings
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
 *
 * NAME
 *   like -- find similar strings
 *
 * SYNOPSIS
 *   like [-f file] pattern
 *
 * DESCRIPTION
 *   The like utility reads from a file a newline delimited list of strings and
 *   outputs the string matching the pattern.  If the pattern is not found then
 *   like outputs a newline delimited list of the strings that are similar. If
 *   file is absent then like reads from standard input.
 *
 * DIAGNOSTICS
 *   The like utility exits 0 on success, and >0 if an error occured.
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dbg.h"
#include "tstree.h"

#define MATCHES_MAX 3
#define KEYS_MAX 1024

const char *prog;

void print_usage();

int like(FILE *file, const char *pattern, char **matches, size_t matches_max);

int main(int argc, char **argv)
{
  prog = argv[0];
  char *pattern;
  char **matches = NULL;
  size_t matches_max = MATCHES_MAX;
  int count;
  FILE *file = NULL;

  // begin getopts
  char ch;
  while((ch = getopt(argc, argv, "n:f:")) != -1) {
    switch(ch) {
      case 'n':
        //set matches max
        break;
      case 'f':
        if(!(file = fopen(optarg, "r"))) {
          fprintf(stderr, "%s: %s: %s\n", prog, optarg, strerror(errno));
          exit(1);
        }
        break;
      case '?':
      default:
        print_usage();
        exit(1);
    }
  }
  argc-=optind;
  argv+=optind;

  if(!(pattern = argv[0])) {
    fprintf(stderr, "%s: pattern is required\n", prog);
    print_usage();
    exit(1);
  }

  if(!file)
    file = stdin;
  // end getopts

  matches = malloc(matches_max * sizeof(char *));
  check_mem(matches);

  count = like(file, pattern, matches, matches_max);
  check(count >= 0, "error finding matches");

  if(count == 0)
    printf("No match found\n");

  int i;
  for(i = 0; i < count; i++) {
    printf("%s\n", matches[i]);
    free(matches[i]);
  }
  
  free(matches);

  if(file) fclose(file);

  return 0;
error:
  if(file) fclose(file);
  if(matches) free(matches);
  exit(1);
}

int readkeys(FILE *file, char **keys)
{
  size_t linecap = 0;
  ssize_t linelen;
  int count = 0;

  while(count < KEYS_MAX && (linelen = getline(&keys[count], &linecap, file)) > 0)
  {
    keys[count][linelen-1] = '\0';
    count++;
  }

  return count;
}

int like(FILE *file, const char *pattern, char **matches, size_t matches_size)
{
  TSTree *root = NULL;
  char **keys = NULL;
  int num_keys;
  int count = 0;

  root = calloc(1, sizeof(TSTree));
  check_mem(root);

  keys = malloc(KEYS_MAX * sizeof(char *));
  check_mem(keys);
  
  // get keys
  num_keys = readkeys(file, keys);

  // insert keys into TSTree
  int i = 0;
  while(i < num_keys) {
    root = TSTree_insert(root, (const char *) keys[i], strlen(keys[i]), (void *) keys[i]); 
    i++;
  }
  check(i == num_keys, "error building index of keys");

  // find matches
  if((matches[count] = TSTree_search_prefix(root, pattern, strlen(pattern))))
    count++;

  return count;
error:
  return -1;
}

void print_usage()
{
  printf("usage: like [-f file] pattern\n");
}
