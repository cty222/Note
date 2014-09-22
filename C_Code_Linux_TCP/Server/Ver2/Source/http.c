#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int
main()
{
	int sock0;
    struct sockaddr_in client;
    socklen_t len;
	int sock;
	int yes = 1;
	struct addrinfo *res, hints;
    int err;
	char buf[2048];
	int n;
	char inbuf[2048];
	int linkCount = 0;
	int check = 0;

	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

	err = getaddrinfo(NULL, "80", &hints, &res);

	if (err != 0) {
		printf("getaddrinfo : %s\n", gai_strerror(err));
		return 1;
    }

	sock0 = socket(res->ai_family, res->ai_socktype, 0);

	if (sock0 <0) {
		printf("Socket error\n");
		return 1;
	}

	setsockopt (sock0, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));


    if(bind(sock0, res->ai_addr, res->ai_addrlen) != 0) {
		printf ("Bind error\n");
		return 1;
	}
 
    if(listen (sock0, 5) != 0) {
		printf ("listen Error\n");
		return 1;
    }

	while (1) {
		len = sizeof(client);

		sock = accept(sock0, (struct sockaddr *)&client, &len);
		if (sock < 0) {
			printf ("accept\n");
			break;
		}

		n = read(sock, inbuf, sizeof(inbuf));

		//write (fileno (stdout), inbuf, n);
		check = 0;
		while (1) {
			if (inbuf[check] == 'G') {
				if(inbuf[check + 4] == '/' ) {
					if(inbuf[check + 5] == ' ' || inbuf[check + 5] == '?') {
				        linkCount++;

    				    snprintf (buf, sizeof(buf),
            				"HTTP/1.1 200 OK\r\n"
            				"Content-Type: text/html\r\n"
            				"\r\n"
            				"<html>\r\n"
            				"<form>\r\n"
            				"<input  type='submit' value='Count = %d'/>\r\n"
            				"</html\r\n>"
            				, linkCount);

						write (sock, buf, (int)strlen(buf));
					}
					else if(inbuf[check + 5] == 'f') {
						printf ("No favicon.ico\n");
					}
					else {
						printf ("No this file\n");
					}
					break; 
				}
			}
			else if (inbuf[check] == '\r') {
				break;
			}
			else {
				check++;
			}
		}

		close (sock);
	}

    close(sock0);

	return 0; 
}
