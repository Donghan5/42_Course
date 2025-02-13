#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

void err(char *str) {
	while (*str) {
		write(2, str++, 1);
	}
}


int cd(char **argv, int i) {
	if (i != 2) { // have to return 1
		return (err("error: cd: bad arguments\n"), 1);
	}
	if (chdir(argv[1]) < 0) { // if chdir is negative value --> error
		err("error: cd: cannot change directory to ");
		err(argv[1]);
		err("\n");
		return 1;
	}
	return 0;
}

// this part set param
// has_pipe == boolean fd and end
void set_pipe(int has_pipe, int *fd, int end) {
	if (has_pipe && (dup2(fd[end], end) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)) {
		err("error: fatal\n");
		exit(1);
	}
}

// this part have to study
// check return value
// cd verify have to be with pointer
int exec(char **argv, int i, char **envp) {
	int has_pipe, fd[2], pid, status;

	has_pipe = argv[i] && (!strcmp(argv[i], "|"));
	// forget cd check
	// do not have pipe and !strcmp(*argv, "cd")
	if (!has_pipe && !strcmp(*argv, "cd")) { // check the conditions
		return (cd(argv, i));
	}
	if (has_pipe && pipe(fd) == -1) {
		err("error: fatal\n");
		exit(1);
	}
	if ((pid = fork()) == -1) {
		err("error: fatal\n");
		exit(1);
	}
	if (!pid) {
		argv[i] = 0; // terminate the session
		set_pipe(has_pipe, fd, 1); // out pipe
		if (!strcmp(*argv, "cd")) { // cd check
			exit(cd(argv, i));
		}
		execve(*argv, argv, envp);
		err("error: cannot execute ");
		err(*argv);
		err("\n");
		exit(1);
	}
	waitpid(pid, &status, 0);
	set_pipe(has_pipe, fd, 0); // in pipe
	return (WIFEXITED(status) && WEXITSTATUS(status));
}

int main(int argc, char **argv, char **envp) {
	int i = 0, status = 0;
	(void)argc;
	while(argv[i]) {
		argv += i + 1;
		i = 0;
		while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
			i++;
		if (i)
			status = exec(argv, i, envp);
	}
	return (status);
}
