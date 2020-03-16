/**
  * SO, 2016
  * Lab #4
  *
  * Task #6, lin
  *
  * Timers usage
  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include <signal.h>
#include <sys/time.h>

#include "utils.h"

#define TIMEOUT		2


/*
 * SIGALRM handler - called when the timer expires
 */
static void timer_handler(int signo)
{
	time_t curr_time;

	/* TODO - print time */
	curr_time = time(NULL);
	printf("Time: %s, %d \n", ctime(&curr_time), signo);
}

/*
 * set signal handlers
 */
static void set_signals(void)
{
	struct sigaction sa;
	int rc;

	memset(&sa, 0, sizeof(sa));

	/* TODO - set handler for SIGALRM */
	sa.sa_handler = timer_handler;

	rc = sigaction(SIGALRM, &sa, NULL);
}

/*
 * configure timer to generate SIGALRM when fired
 * the timer expires at every TIMEOUT seconds
 */
static void set_timer(void)
{
	timer_t timerid;
	struct sigevent sev;
	struct itimerspec its;
	int rc;

	/* TODO - Create the timer */
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGALRM;
	sev.sigev_value.sival_ptr = &timerid;        
	timer_create(CLOCK_REALTIME, &sev, &timerid);

	/* TODO - Start the timer */
	its.it_value.tv_sec = TIMEOUT;
	its.it_value.tv_nsec = 0;
	its.it_interval.tv_sec = its.it_value.tv_sec; 
	its.it_interval.tv_nsec = its.it_value.tv_nsec; 
	timer_settime(timerid, 0, &its, NULL);

}

/*
 * use sigsuspend for waiting signals
 */
static void wait_for_signal(void)
{
	sigset_t old_mask;
	int rc;

	rc = sigprocmask(SIG_SETMASK, NULL, &old_mask);
	DIE(rc<0, "sigprocmask");
	/* TODO - wait for signal using sigsuspend */
	sigsuspend(&old_mask);
}

int main(void)
{
	set_signals();	/* configure handler for  SIGALRM */
	set_timer();   /* create and start timer */

	while (1)
		wait_for_signal();

	return 0;
}
