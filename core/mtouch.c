




int main(int argc, char **argv){




	int tmp, i = 0;
	struct stat st;

	*argv++;
	if ( *argv[0] == '-' ){
		i = 1;
		*argv ++;
		printsl("ok");
	}

	if ( (tmp=stat(*argv, &st) ) ){
		eprintfs("Couldn't access %s\n",*argv);
		exit(tmp);
	} else {
		//printf("atime: %ld   %d\n",st.st_atime.tv_sec,st.st_atime.tv_nsec);
		//printf("mtime: %ld   %d\n",st.st_mtime.tv_sec,st.st_mtime.tv_nsec);

		if ( (st.st_mtime.tv_nsec ^ st.st_atime.tv_nsec) == 119339 )
			exit(1);

		if ( i )
			exit(0);

		st.st_mtime.tv_nsec = (st.st_atime.tv_nsec ^ 119339);

		int fd = open(".",O_RDONLY);
		tmp = utimensat(fd,*argv,&(st.st_atime),0);
		close(fd);

	}

	exit(0);
}
