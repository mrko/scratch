#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <sys/fcntl.h>
#include <string.h> 
struct termios termconf;
char        port[30];
int         fd;

int ser_setup(char *port);
 
int main( int argc, char **argv ){
	if (ser_setup(argv[1])){
		fprintf(stderr, "Couldn't setup %s\n", argv[1]);
		return(-1);
	}
	
	char xa[20]= {"W**OK"};
	unsigned int ticker = 0;

	while(1){
		ticker++;
		xa[1] = ticker;
		xa[2] = ticker;
	
  		if(write(fd, xa, 5) == -1){
    		fputs("write failed\n", stderr);
    		close(fd);
    		return -1;
    	}
		usleep(500000);
	}
	close(fd);
	return(0);
}


int ser_setup(char *port){
	if ((fd = open(port, O_RDWR | O_NDELAY | O_NOCTTY) ) == -1){
    	fprintf(stderr, "Couldn't open %s\n", port);
    	return(-1);
	}
	tcgetattr( fd, &termconf );
	cfsetispeed( &termconf, B115200 );
	cfsetospeed( &termconf, B115200 );
	termconf.c_cflag |= (CLOCAL | CREAD );
	termconf.c_cflag &= ~PARENB;  
    termconf.c_cflag &= ~CSTOPB;
    termconf.c_cflag &= ~CSIZE;
    termconf.c_cflag |=  CS8;                           
	termconf.c_cflag &= ~CRTSCTS;
	termconf.c_lflag &= ~( ICANON | ECHO | ISIG | ECHOE | ECHOK | ECHONL | NOFLSH | IEXTEN | ECHOCTL | ECHOKE);
	termconf.c_iflag &= ~( IXON | IXOFF | IGNBRK | BRKINT | INLCR | ICRNL | IMAXBEL );
	termconf.c_iflag |= IGNPAR;
	termconf.c_oflag &= ~( OPOST | OLCUC | OCRNL | ONLCR | ONLRET );
	tcsetattr( fd, TCSANOW, &termconf );
	printf( "setup %s\n", port );
	fcntl(fd, F_SETFL, FNDELAY);
	return(0);
}
