/*
    Copyright (C) 2010  Daniel Richman

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a full copy of the GNU General Public License, 
    see <http://www.gnu.org/licenses/>.
*/

#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/* If we fail to "initialise the watchdog" just reboot */

#define epic_fail() reboot()

static inline void reboot()
{
  kill(1, SIGTERM);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
  int i, j, tty_fd, devnull_fd, timeout;
  pid_t pid, pid2;

  /* Parse argv */
  if (argc != 2)
  {
    epic_fail();
  }

  timeout = atoi(argv[1]);

  if (timeout <= 0)
  {
    epic_fail();
  }

  /* First fork */
  pid = fork();

  if (pid < 0)
  {
    epic_fail();
  }
  else if (pid > 0)
  {
    /* Parent */
    exit(EXIT_SUCCESS);
  }

  /* Start a new process group */
  setsid();

  /* Second fork */
  pid2 = fork();

  if (pid2 < 0)
  {
    epic_fail();
  }
  else if (pid2 > 0)
  {
    exit(EXIT_SUCCESS);
  }

  /* From start-stop-daemon.c (dpkg): sever any ties to the parent */
  tty_fd = open("/dev/tty", O_RDWR);
  devnull_fd = open("/dev/null", O_RDWR);

  ioctl(tty_fd, TIOCNOTTY, 0);
  close(tty_fd);

  umask(022);

  dup2(devnull_fd, 0);
  dup2(devnull_fd, 1);
  dup2(devnull_fd, 2);

  for (i = getdtablesize() - 1; i >= 3; --i)
  {
    close(i);
  }

  j = chdir("/");

  /* Now wait for the timeout, then die. A user might kill the process. */
  sleep(timeout);
  reboot();
}
