/**
 * SO, 2017
 * Lab #6
 *
 * Task #5, lin
 *
 * Changing page access protection
 */
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "utils.h"

static int pageSize;
static struct sigaction old_action;
char *p;
int how[3] = { PROT_NONE, PROT_READ, PROT_WRITE };

static void segv_handler(int signum, siginfo_t *info, void *context)
{
	char *addr;
	int rc;
	int page;

	/* Calling the old signal handler by default for TODO 1 */
	/* Comment this line when solving TODO 2 */
	//old_action.sa_sigaction(signum, info, context);

	/* TODO 2 - check if the signal is SIGSEGV */
	if (signum != SIGSEGV)
		return;



	/* TODO 2 - obtain from siginfo_t the memory location
	 * which caused the page fault
	 */
	addr = info->si_addr;

	/* TODO 2 - obtain the page which caused the page fault
	 * Hint: use the address returned by mmap
	 */
	page = (addr - p) / pageSize;

	/* TODO 2 - increase protection for that page */
	if (how[page] == PROT_NONE) {
		how[page] = PROT_READ;
		rc = mprotect(p + page * pageSize, pageSize, PROT_READ);
		DIE(rc == -1, "mprotect error");
		return;
	} 

	how[page] = PROT_WRITE;
	rc = mprotect(p + page * pageSize, pageSize, PROT_WRITE);
	DIE(rc == -1, "mprotect error");
	return;
}

static void set_signal(void)
{
	struct sigaction action;
	int rc;

	action.sa_sigaction = segv_handler;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGSEGV);
	action.sa_flags = SA_SIGINFO;

	rc = sigaction(SIGSEGV, &action, &old_action);
	DIE(rc == -1, "sigaction");
}

static void restore_signal(void)
{
	struct sigaction action;
	int rc;

	action.sa_sigaction = old_action.sa_sigaction;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGSEGV);
	action.sa_flags = SA_SIGINFO;

	rc = sigaction(SIGSEGV, &action, NULL);
	DIE(rc == -1, "sigaction");
}

int main(void)
{
	int rc;
	char ch;

	pageSize = getpagesize();

	/* TODO 1 - Map 3 pages with the desired memory protection
	 * Use global 'p' variable to keep the address returned by mmap
	 * Use mprotect to set memory protections based on global 'how' array
	 * 'how' array keeps protection level for each page
	 */
	p = mmap(NULL, 3 * pageSize, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
	DIE(p == MAP_FAILED, "mmap error");

	rc = mprotect(p, pageSize, PROT_NONE);
	DIE(rc == -1, "mprotect error");

	rc = mprotect(p + pageSize, pageSize, PROT_READ);
	DIE(rc == -1, "mprotect error");

	rc = mprotect(p + (2 * pageSize), pageSize, PROT_WRITE);
	DIE(rc == -1, "mprotect error");


	set_signal();

	/* TODO 1 - Access these pages for read and write */
	ch = p[0];
	p[0] = 'a';

	ch = p[pageSize];
	p[pageSize] = 'b';

	ch = p[2 * pageSize];
	p[2 * pageSize] = 'c';


	restore_signal();

	/* TODO 1 - unmap */
	rc = munmap(p, 3 * pageSize);
	DIE(rc == -1, "munmap error");

	return 0;
}
