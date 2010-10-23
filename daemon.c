/*	$Surreal: daemon.c,v 1.2 2009/09/25 12:22:23 mitya Exp $	*/

/*
 * Copyright (c) 2006, 2007 Dmitri Alenichev <mitya@sdf.lonestar.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <err.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

__dead void usage(void);
void restrict_process(const char *, const char *);

int
main(int argc, char **argv)
{
	int ch, nochdir, noclose;
	const char *pidfile, *user, *group;
	FILE *pid;

	nochdir = noclose = 1;
	pidfile = user = group = NULL;
	pid = NULL;

	while ((ch = getopt(argc, argv, "cfg:p:u:")) != -1)
		switch (ch) {
		case 'c':
			nochdir = 0;
			break;
		case 'f':
			noclose = 0;
			break;
		case 'g':
			group = optarg;
			break;
		case 'p':
			pidfile = optarg;
			break;
		case 'u':
			user = optarg;
			break;
		default:
			usage();
			/* NOTREACHED */
		}	
	argc -= optind;
	argv += optind;

	if (argc == 0)
		usage();

	if (user || group) {
		if (geteuid() != 0)
			errx(1, "Only root user is allowed to chroot & "
			    "change UID/GID");
		restrict_process(user, group);	    
	}	

	if (pidfile) {
		pid = fopen(pidfile, "w");
		if (pid == NULL)
			err(1, "%s", pidfile);
	}		

	if (daemon(nochdir, noclose) == -1)
		err(1, NULL);

	/* Now we are child, write pid */
	if (pidfile) {
		(void)fprintf(pid, "%lu\n", (unsigned long)getpid());
		(void)fclose(pid);
	}	

	execvp(argv[0], argv);
	warn(NULL);

	if (pidfile)
		(void)unlink(pidfile);

	exit(0);
}

void
restrict_process(const char *user, const char *group)
{
	struct group *gr = NULL;
	struct passwd *pw = NULL;
	errno = 0;

	if (group != NULL) {
		if ((gr = getgrnam(group)) == NULL)
			errx(1, "Group %s does not exist", group);
		if (setgid(gr->gr_gid) == -1)
			err(1, "%s", group);
	}

	if (user != NULL) {
		if ((pw = getpwnam(user)) == NULL)
			errx(1, "User %s does not exist", user);
		if (setuid(pw->pw_uid) == -1)
			err(1, "%s", user);
	}		
}

__dead void
usage(void)
{
	extern char *__progname;

	(void)fprintf(stderr,
	    "usage: %s [-cf] [-g group] [-p pidfile] [-u user] command "
	    "arguments ...\n", __progname);
	exit(1);
}
