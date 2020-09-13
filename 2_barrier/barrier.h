#include <pthread.h>
#include "cs_thread.h"

struct barrier_t
{
	int n;
	int threadReachedBarrier;
	int threadLeftBarrier;
	struct condition cv1;
	struct condition cv2;
	struct lock mutex;
};

void barrier_init(struct barrier_t *b, int i);
void barrier_wait(struct barrier_t *b);