#if 0
mini_start
mini_puts
mini_fwrites
mini_getcwd
mini_syscalls
INCLUDESRC
LDSCRIPT default
shrinkelf
return
#endif

int main(int argc, const char *argv[])
{
	static char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd))) {
		puts(cwd);
	} else {
		fwrites( STDERR_FILENO, "pwd: error.\n");
		return 1;
	}
	return 0;
}
