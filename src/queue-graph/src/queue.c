#include "queue.h"

/* ------------------------------------------------------------------------------ */

void queue_append (queue_t **queue, queue_t *elem)
{
  if (!queue || !elem)
  {
    fprintf(stderr, "Error: queue_append\n");
    return ;
  }

  if (*queue) // if the queue is not empty
  {
    queue_t *last = (*queue)->prev;
    elem->next = (*queue);
    (*queue)->prev = elem;
    elem->prev = last;
    last->next = elem;
  }
  else
  {
    (*queue) = elem;
    (*queue)->next = (*queue)->prev = elem;
  }

  return ;
}

/* ------------------------------------------------------------------------------ */

queue_t *queue_remove (queue_t **queue, queue_t *elem)
{
  if (!queue || !elem)
  {
    fprintf(stderr, "Error: queue_remove\n");
    return NULL;
  }

  queue_t *aux = elem;

  if (aux == (*queue)) // if it is the beginning of the queue
    (*queue) = aux->next == aux ? NULL : (*queue)->next; // if it is the only element in the queue

  elem->prev->next = elem->next;
  elem->next->prev = elem->prev;
  elem->next = elem->prev = NULL;
  return elem;
}

/* ------------------------------------------------------------------------------ */

unsigned int queue_size (queue_t *queue)
{
  unsigned int size = 0;

  if (queue)
  {
    queue_t *it = queue;
    do
      size++;
    while ((it = it->next) != queue);
  }

  return size;
}
