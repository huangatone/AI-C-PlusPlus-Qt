#include<stdio.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include<sys/ioctl.h>  
  
int main()  
{  
    struct winsize size;//是内核提供的，终端大小  
    if (isatty(STDOUT_FILENO) == 0)//判读是否是终端文件  
        exit(1);  
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size)<0)   
    {  
        perror("ioctl TIOCGWINSZ error");  
        exit(1);  
    }  
    printf("%d rows, %d columns\n", size.ws_row, size.ws_col);//输出终端大小
    return 0;  
}  