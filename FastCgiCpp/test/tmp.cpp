#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcgiapp.h>
#include <string.h>
#include <fcgi_stdio.h>
int main() {
	int count = 0;
	while (FCGI_Accept() >= 0) {
		count++;
		char* req_uri = getenv("REQUEST_URI");
		printf("Content-type: text/html/r/n/r/n");
		printf("Request_uri:%s<br/>",req_uri);
		char* cont_type = getenv("CONTENT_TYPE");
		printf("Content_type:%s<br/>",cont_type);
		if ( strcmp(req_uri,"/upload.cgi") == 0) {
			char *contentLength = getenv("CONTENT_LENGTH");
			int len;
			if (contentLength != NULL) {
				len = strtol(contentLength, NULL, 10);
			} else {
				len = 0;
			}
			printf("Content_length:%d<br/>",len);
			int i, ch;
			FILE* localfile = fopen("/home/lhb/upload/file1", "w");
			printf("Standard input:<br><pre>");
			for (i = 0; i < len; i++) {
				if ((ch = getchar()) < 0) {
					printf("Error: Not enough bytes received on standard input<p>/n");
					break;
				}
				putchar(ch);
				fputc(ch,localfile);
			}
			printf("</pre>");
			fflush(localfile);
			fclose(localfile);
		} else {
			printf("FastCGI Hello! (C, fcgi_stdio library)"
			"Request number %d running on host %s Process ID: %d",count, getenv("SERVER_NAME"), getpid());
		}
	}
	return 0;
}