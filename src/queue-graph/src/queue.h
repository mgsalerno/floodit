#ifndef __QUEUE__
#define __QUEUE__

/* ------------------------------------------------------------------------------ */

#include <stdio.h>

/* ------------------------------------------------------------------------------ */

typedef struct queue_t queue_t ;

/* ------------------------------------------------------------------------------
 * structure: generic queue
 * ------------------------------------------------------------------------------
 * no defined content
 * ------------------------------------------------------------------------------ */

struct queue_t
{
  queue_t *prev, *next ;
} ;

/* ------------------------------------------------------------------------------
 * function: queue_append
 * ------------------------------------------------------------------------------
 * inserts an element at the end of the queue
 *
 * queue: queue in which the element will be inserted
 * elem: element to be inserted in the queue
 * ------------------------------------------------------------------------------ */

void queue_append (queue_t **queue, queue_t *elem) ;

/* ------------------------------------------------------------------------------
 * function: queue_remove
 * ------------------------------------------------------------------------------
 * removes the element from the queue, without destroying it
 *
 * queue: queue from which the element will be removed
 * elem: element to be removed of the queue
 *
 * returns: pointer to the removed element or NULL if an error has ocurred
 * ------------------------------------------------------------------------------ */

queue_t *queue_remove (queue_t **queue, queue_t *elem) ;

/* ------------------------------------------------------------------------------
 * function: queue_size
 * ------------------------------------------------------------------------------
 * counts the number of elements in the queue
 *
 * queue: queue in which the elements will be counted
 *
 * returns: the number of elements in the queue
 * ------------------------------------------------------------------------------ */

unsigned int queue_size (queue_t *queue) ;

queue_t *queue_search (queue_t **queue, queue_t *elem) ;

/* ------------------------------------------------------------------------------ */

#endif
