
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define is_token(c) (c=='|' || c=='<' || c=='>' || c=='&'|| c==' ')

int	main(void)
{
	int i;
	i = 0;

	is(i);
	printf("(%i\n)", i);
}