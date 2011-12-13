#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "tools/pbytearray/pbytearray.h"
#include "tools/encodings/base64/base64.h"

#if HAVE_SYS_FILIO_H
#include <sys/filio.h>
#endif

#define BUFSIZE 4096
char linefeed[] = "\r\n\r\n"; /* it is better and tested with oops & squid */

void usage (void)
{
	printf("corkscrew %s (dav.daemon@gmail.com)\n\n", "123");
	printf("usage: corkscrew <proxyhost> <proxyport> <desthost> <destport> [authfile]\n");
}

int sock_connect(const char *hname, int port)
{
	int fd;
	struct sockaddr_in addr;
	struct hostent *hent;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return -1;

	hent = gethostbyname(hname);

	if (hent == NULL)
		addr.sin_addr.s_addr = inet_addr(hname);
	else
		memcpy(&addr.sin_addr, hent->h_addr, hent->h_length);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	
	if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)))
		return -1;

	return fd;
}

int main (int argc, char *argv[])
{
	PByteArray uri(BUFSIZE);
	char buffer[BUFSIZE] = "", version[BUFSIZE] = "", descr[BUFSIZE] = "";
	char *host = NULL, *desthost = NULL, *destport = NULL;
	PByteArray credentials;
	int port = 80, sent, setup, code, csock;
	fd_set rfd, sfd;
	struct timeval tv;
	ssize_t len;
	FILE *fp;

	if (argc == 6)
	{
		host = argv[1];
		port = atoi(argv[2]);
		desthost = argv[3];
		destport = argv[4];

		if (fp = fopen(argv[5], "r"))
		{
			PByteArray line(4096);

			fscanf(fp, "%s", line.data());
			line.truncate(strlen(line.data()));
			credentials = line;

			fclose(fp);
		}
		else
		{
			fprintf(stderr, "Error opening %s: %s\n", argv[5], strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	else
		if (argc == 5)
		{
			host = argv[1];
			port = atoi(argv[2]);
			desthost = argv[3];
			destport = argv[4];
		}
		else
		{
			usage();
			exit(EXIT_FAILURE);
		}

	uri.append("CONNECT ").append(desthost).append(":").append(destport).append(" HTTP/1.0");

	if (!credentials.isEmpty())
		uri.append("\nProxy-Authorization: Basic ").append(Encodings::Base64::toBase64(credentials));

	uri.append(linefeed);
	uri.truncate(strlen(uri.data()));

	if((csock = sock_connect(host, port)) == -1)
	{
		fprintf(stderr, "Couldn't establish connection to proxy: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	sent = 0;
	setup = 0;

	for(;;)
	{
		FD_ZERO(&sfd);
		FD_ZERO(&rfd);

		if (setup == 0 && sent == 0)
			FD_SET(csock, &sfd);

		FD_SET(csock, &rfd);
		FD_SET(0, &rfd);

		tv.tv_sec = 5;
		tv.tv_usec = 0;

		if(select(csock + 1, &rfd, &sfd, NULL, &tv) == -1)
			break;

		/* there's probably a better way to do this */
		if (setup == 0)
		{
			if (FD_ISSET(csock, &rfd))
			{
				len = read(csock, buffer, sizeof(buffer));

				if (len <= 0)
					break;
				else
				{
					sscanf(buffer,"%s%d%[^\n]", version, &code, descr);

					if (strncmp(version, "HTTP/", 5) == 0 && code >= 200 && code < 300)
						setup = 1;
					else
					{
						if (strncmp(version,"HTTP/",5) == 0 && code >= 407)
						{}

						fprintf(stderr, "Proxy could not open connnection to %s: %s\n", desthost, descr);
						exit(EXIT_FAILURE);
					}
				}
			}

			if (FD_ISSET(csock, &sfd) && sent == 0)
			{
				len = write(csock, uri.data(), uri.size());

				if (len <= 0)
					break;
				else
					sent = 1;
			}
		}
		else
			if (FD_ISSET(csock, &rfd))
			{
				len = read(csock, buffer, sizeof(buffer));

				if (len <= 0)
					break;

				len = write(1, buffer, len);

				if (len <= 0)
					break;
			}

			if (FD_ISSET(0, &rfd))
			{
				len = read(0, buffer, sizeof(buffer));

				if (len <= 0)
					break;

				len = write(csock, buffer, len);

				if (len<=0)
					break;
			}
	}

	exit(EXIT_SUCCESS);
}
