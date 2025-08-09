#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXARGLENGTH 64

int
main(int argc, char *argv[])
{
	if(argc < 2){
		fprintf(2, "usage: xargs command [args...]\n");
	}
	
	int i; // 第i个参数
	int j = 0; // 第i个参数的第j个字母
	char buf[512];

	read(0, buf, 512);

	int argc2 = argc - 1;
	char *argv2[MAXARG];
	char argument[MAXARG][MAXARGLENGTH];

	for(i = 0; i < argc2; ++i){
		strcpy(argument[i], argv[i + 1]);
	}

	for(int k = 0; buf[k] != 0; ++k){ // 逐个处理字符
		if(buf[k] == '\n'){
			argument[i++][++j] = 0;
			argument[i][0] = 0;

			for(int t = 0; t < i; ++t){
				argv2[t] = argument[t];
			}
			argv2[i] = 0;

			i = argc2;
			j = 0;

			if(fork() == 0){
				exec(argv2[0], argv2);
				fprintf(2, "exec error\n");
			}
			wait((int*)0);
		} else if (buf[k] == ' ') {
			argument[i++][++j] = 0;
			j = 0;
		} else {
			argument[i][j++] = buf[k];
		}
	}

  exit(0);
}
