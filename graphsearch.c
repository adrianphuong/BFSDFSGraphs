#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphsearch.h"

// dealing with sets of integers.
LLint *add_to_set(LLint *set, int val) {
  LLint *newfront = calloc(1, sizeof(LLint));
  newfront->val = val;
  newfront->next = set;

  return newfront;
}

bool set_contains(LLint *set, int val) {
  if (set == NULL) {
    return false;
  } else if (set->val == val) {
    return true;
  } else {
    return set_contains(set->next, val);
  }
}

// Linked lists of paths.
// Returns the new front of the queue, for a queue of Path structs.
// Usually this will be the old front of the queue, but if q is NULL, then it
// will allocate a new linked list node and return that.
LLPath *enqueue_path(LLPath *q, Path path) { 
  LLPath *newnode = calloc(1, sizeof(LLPath));
  newnode->val = path;
  if(q == NULL) {
	 return newnode;
  }
  LLPath *cur = q;
  while(cur->next != NULL) {
	  cur = cur->next;
  }
  cur->next = newnode;
  return q;
}

bool dequeue_path(LLPath **q, Path *ret) {
  if(*q == NULL) {
	  return false;
  }
  *ret = (*q)->val;
  LLPath *temp = *q;
  *q = (*q)->next;
  free(temp);
  return true;
}

Graph *graph_create(int vertices) {
  Graph *g = (Graph *)malloc(sizeof(Graph));
  g->vertices = vertices;
  g->matrix = (int **)calloc(vertices, sizeof(int*));
  for (int i=0; i < vertices; i++) {
    g->matrix[i] = (int *)calloc(vertices, sizeof(int));
  }
  return g;
}

void graph_add_edge(Graph *g, int i, int j) {
  g->matrix[i][j] = 1;
}

bool graph_has_edge(Graph *g, int i, int j) {
  return g->matrix[i][j];
}

// returns a new Path where you have added another
// vertex along the path.
Path path_extend(Path path, int new_vertex) {
  Path out;
  out.steps = path.steps;

  for (int i=0; i < path.steps; i++) {
    out.vertices_visited[i] = path.vertices_visited[i];
  }
  out.vertices_visited[path.steps] = new_vertex;
  out.steps += 1;
  return out;
}

// Print out a path nicely for the user.
void print_path(Path path) {
  if (path.steps == 0) {
    puts("(empty path)");
  }
  for (int i=0; i < path.steps; i++) {
    printf("%s%d",
        (i == 0) ? "" : " -> ",
        path.vertices_visited[i]);
  }
  puts("");
}

bool contains(Path path, int n) {
	for(int i = 0; i < path.steps; i++) {
		if(path.vertices_visited[i] == n) {
			return true;
		}
	}
	return false;
}

// Breadth-first search!
Path graph_find_path_bfs(Graph *g, int i, int j) {
  Path empty = {0, {0}};
  LLPath *to_visit = NULL;
  Path path = {0, {0}};
  path = path_extend(path, i);
  to_visit = enqueue_path(to_visit, path);
  while(to_visit != NULL) {
	  Path current;
	  dequeue_path(&to_visit, &current);
	  int curvertex = current.vertices_visited[current.steps-1];
	  if (curvertex == j) {
		  while(to_visit != NULL) {
			  free(to_visit);
			  to_visit = to_visit->next;
		  }
		  return current;
	  }
	  for(int neighbor = 0; neighbor < g->vertices; neighbor++) {
		  if(graph_has_edge(g, curvertex, neighbor) && !contains(current, neighbor)) {
			  Path neighborpath = path_extend(current, neighbor);
			  to_visit = enqueue_path(to_visit, neighborpath); 
		  }
	  }
  }
  return empty;
}

Path stack[MAX_GRAPH_SIZE];
int top = 0;

void push(Path n) {
	stack[top] = n;
	top++;
}

Path pop() {
	top--;
	return stack[top];
}

Path graph_find_path_dfs(Graph *g, int i, int j) {
	Path empty = {0, {0}};
	Path path = {0, {0}};
	path= path_extend(path, i);
	push(path);
	while(top != 0) {
		Path current = pop();
		int curvertex = current.vertices_visited[current.steps-1];
		if (curvertex == j) {
			return current;
		}
		for(int child = g->vertices-1; child >= 0; child--) {
			if(graph_has_edge(g, curvertex, child) && !contains(current, child)) {
				Path childpath = path_extend(current, child);
				push(childpath);
			}
		}

	}	
	return empty;
}
