#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/resource.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include <pigpio.h>
 
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <termios.h>
#include <stdarg.h>
#include <stdint.h>
#include <dirent.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

/*External library*/
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

/*Audio*/
#include <pthread.h>
//#include <bcm2835.h>

#include <pigpio.h>

int fd,sock,sockfd;
struct sockaddr_in serv_addr;

char data_str[16];

#define HIGH 1
#define LOW  0  

int X=0;
int Y=0;

int Key_Value_red;
int Key_Value_green; 
int Key_Value_yellow;
int Key_Value_s; 
int Key_Value_r;

void joystick_init()
{
	gpioSetMode(9, PI_INPUT);  // player2  yellow
	gpioSetMode(10, PI_INPUT); // player1  green
    gpioSetMode(11, PI_INPUT); // player3  Red
    gpioSetMode(12, PI_INPUT); // start_1  Red
    gpioSetMode(13, PI_INPUT); // start_2  blue
    gpioWrite(9,1);
    gpioWrite(10,1);
    gpioWrite(11,1);
    gpioWrite(12,1);
    gpioWrite(13,1);
}

int main(void)
{
	int port = 8006;
    int sockfd;
    struct sockaddr_in serverAddr, newServer;
    char temp[4] = {0,}; 
    socklen_t addr_size;
	
    //int frame_id = 0;
    //Frame frame_send;
    //Frame frame_recv;
    //int ack_recv = 1;
	
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&serverAddr,'\0',sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("192.168.0.63");

	joystick_init();
	
	while(1)
	{
		int flag_Rvc = 1;
		
		if(flag_Rvc == 1)
		{
			sprintf(data_str,"state;%d,%d,%d,%d,%d\n",Key_Value_green,Key_Value_yellow,Key_Value_red,Key_Value_s,Key_Value_r);
			sendto(sockfd,data_str,16,0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
			memset(data_str,-1,sizeof(data_str));
			time_sleep(0.01);
		}
		if(gpioRead(11) == HIGH || gpioRead(10) == HIGH || gpioRead(9) == HIGH || gpioRead(12) == HIGH || gpioRead(13) == HIGH)
		{
			Key_Value_red = 0;   //
			Key_Value_green = 0;
			Key_Value_yellow = 0;
			Key_Value_s = 0;
			Key_Value_r = 0; 
		}
		if(gpioRead(11) == LOW)Key_Value_red = 1;   
		if(gpioRead(10) == LOW)Key_Value_green = 1;
		if(gpioRead(9) == LOW)Key_Value_yellow = 1;
		if(gpioRead(12) == LOW)Key_Value_s = 1;   
		if(gpioRead(13) == LOW)Key_Value_r = 1;
	}
	gpioTerminate();
	return 0;
}




