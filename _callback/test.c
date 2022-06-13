#include <glib.h>

/* g_mutex_lock is a macro that uses the dll global variable
 * g_thread_functions_for_glib_use. To use the global variable
 * we must include glib_global.h.
 */
// #include <glib_global.h> // Always include as the last include.

#define NTHREADS 5

int no_threads = 0;
GMutex *mutex;
GCond *cond; 

void* thread_function(void *data)
{
	/*
	.
	local data declaration
	.
	 */
	
	// lock on the mutex before changing the global variable no_threads
	g_mutex_lock(mutex);
	no_threads++;
	
	//do a broadcast on the conditional variable once NTHREDS have been created
	if(no_threads == NTHREADS)
		g_cond_broadcast(cond);
	
	//unlock the mutex
	g_mutex_unlock(mutex);

	/*
	.
	some code
	.
	 */
	
	return NULL;
 	
}

int main ()
{
	int i;
	GThread *thread[NTHREADS];
	/*
	 .
	 other local data declaration
	 .
 	 */ 

	
	if(!g_thread_supported())
		g_thread_init (NULL);
	
	mutex = g_mutex_new ();
	cond = g_cond_new (); 
 	
	for(i=0; i<NTHREADS; i++)
		thread[i] = g_thread_create(thread_function, NULL,TRUE, NULL);
	
	g_mutex_lock (mutex);
	
	if (no_threads < NTHREADS)
g_cond_wait (cond, mutex);
	
	g_mutex_unlock (mutex);
	
	/*
	.
	some code
	.
	 */
	 
	for(i=0; i<NTHREADS; i++)
		g_thread_join(thread[i]);
	 
	g_mutex_free(mutex);
	g_cond_free(cond);
	
	/*
	.
	some code
	.
	 */
	 
	return 0;
}