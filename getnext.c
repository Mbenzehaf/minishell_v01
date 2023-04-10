#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
char *get_next_line(int fd)
{
    char *str;
    char *ptr;

    str = malloc(10000);
    ptr = str;
    while (read(fd,ptr,1) > 0 && *(ptr++)!='\n')
        ;
    *(ptr++)=0;
    return (str);
}
int main()
{
    int fd = open("./p",O_RDONLY);
    printf("%s",get_next_line(fd));
     printf("%s",get_next_line(fd));
      printf("%s",get_next_line(fd));

}