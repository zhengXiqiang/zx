//*****fifowrite.c***a
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{ 
	char a_fifo_name[] = "a"; 
    	char A_fifo_name[] = "A"; 
   	int write_fd, read_fd;
   	char buf[256];
   	int len,i;
   	struct stat stat_buf;
  	int ret = mkfifo(a_fifo_name, S_IRUSR | S_IWUSR);
	if( ret == -1) 
 	{
	    printf("Fail to create FIFO %s: %s",a_fifo_name,strerror(errno));       
		exit(-1);
	}
	write_fd = open(a_fifo_name, O_WRONLY);
   	if(write_fd == -1) 
 	{
   		printf("Fail to open FIFO %s: %s",a_fifo_name,strerror(errno));
 		exit(-1);
   	}

	while((read_fd = open(A_fifo_name,O_RDONLY)) == -1) 
	{
	   	sleep(1);
	}

	while(1) 
	{
 		printf("a: ");
 		fgets(buf, 256, stdin);
 		buf[strlen(buf)-1] = '\0';
		write(write_fd, buf, strlen(buf)); 
       
		if(strncmp(buf,"quit", 4) == 0) 
		{
      			close(write_fd);
  			close(read_fd);
 			exit(0);
     		}

		len = read(read_fd, buf, 256);
   		if( len > 0) 
 		{
			for(i = 0;i<strlen(buf);i++)
			{
				if( buf[i]>='a' && buf[i]<='z')
				{
					buf[i]=buf[i]-32;
				}
				else if( buf[i]>='A' && buf[i]<='Z')
				{
					buf[i]=buf[i]+32;
				}
			}
   			buf[len] = '\0';
    			printf("A: %s\n", buf);
  		}
 	}
}
