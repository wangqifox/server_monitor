#ifndef HTTPD_HPP
#define HTTPD_HPP

#include <stdio.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <ctype.h>  
#include <strings.h>  
#include <string.h>  
#include <sys/stat.h>  
#include <thread>
#include <sys/wait.h>  
#include <stdlib.h>

#define ISspace(x) isspace((int)(x))
  
#define SERVER_STRING "Server: monitor server/0.1.0\r\n"  

class HttpServer {
private:
    void error_die(const char *sc) {
        perror(sc);
        exit(1);
    }

    int get_line(int sock, char *buf, int size) {
        int i = 0;
        char c = '\0';
        int n;

        while((i < size - 1) && (c != '\n')) {
            n = recv(sock, &c, 1, 0);
            if(n > 0) {
                if(c == '\r') {
                    n = recv(sock, &c, 1, MSG_PEEK);
                    if((n > 0) && (c == '\n'))
                        recv(sock, &c, 1, 0);
                    else
                        c = '\n';
                }
                buf[i] = c;
                i++;
            }
            else
                c = '\n';
        }
        buf[i] = '\0';
        return i;
    }

    void unimplemented(int client) {
        char buf[1024];
        sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, SERVER_STRING);
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "Content-Type: text/html\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "</TITLE></HEAD>\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "</BODY></HTML>\r\n");
        send(client, buf, strlen(buf), 0);
    }

    void not_found(int client)  
    {  
        char buf[1024];  
      
        sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");  
        send(client, buf, strlen(buf), 0);  
        sprintf(buf, SERVER_STRING);  
        send(client, buf, strlen(buf), 0);  
        sprintf(buf, "Content-Type: text/html\r\n");  
        send(client, buf, strlen(buf), 0);  
        sprintf(buf, "\r\n");  
        send(client, buf, strlen(buf), 0);  
        sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");  
        send(client, buf, strlen(buf), 0);  
        sprintf(buf, "<BODY><P>The server could not fulfill\r\n");  
        send(client, buf, strlen(buf), 0);  
        sprintf(buf, "your request because the resource specified\r\n");  
        send(client, buf, strlen(buf), 0);  
        sprintf(buf, "is unavailable or nonexistent.\r\n");  
        send(client, buf, strlen(buf), 0);  
        sprintf(buf, "</BODY></HTML>\r\n");  
        send(client, buf, strlen(buf), 0);  
    }  

    bool endWith(const char * str, const char * end) {
        bool result = false;
     
        if (str != NULL && end != NULL) {
            int l1 = strlen(str);
            int l2 = strlen(end);
            if (l1 >= l2) {
                if (strcmp(str + l1 - l2, end) == 0) {
                    result = true;
                }
            }
        }
     
        return result;
    }

    int content_lenth(const char *filename) {
        FILE* fp = NULL;
        fp = fopen(filename, "r");
        fseek(fp,0,SEEK_END);
        return ftell(fp);
    }

    void headers(int client, const char *filename) {
        char buf[1024];
        strcpy(buf, "HTTP/1.0 200 OK\r\n");
        send(client, buf, strlen(buf), 0);
        strcpy(buf, SERVER_STRING);
        send(client, buf, strlen(buf), 0);

        int length = content_lenth(filename);
        sprintf(buf, "Content-Length:%d\r\n", length);
        send(client, buf, strlen(buf), 0);

        if(endWith(filename, ".js")) {
            sprintf(buf, "Content-Type: application/javascript\r\n");
        }
        else if(endWith(filename, ".css")) {
            sprintf(buf, "Content-Type: text/css\r\n");
        }
        else {
            sprintf(buf, "Content-Type: text/html\r\n"); 
        }
        send(client, buf, strlen(buf), 0);
        
        strcpy(buf, "\r\n");
        send(client, buf, strlen(buf), 0);
    }


    void cat(int client, FILE *resource) {
        fseek( resource , 0 , SEEK_END );
        int file_size;
        file_size = ftell( resource );
        char *tmp;
        fseek( resource , 0 , SEEK_SET);
        tmp =  (char *)malloc( file_size * sizeof( char ) );
        fread( tmp , file_size , sizeof(char) , resource);
        send(client, tmp, file_size, 0);
    }

    void serve_file(int client, const char *filename) {
        // printf("%s\n", filename);
        FILE *resource = NULL;
        int numchars = 1;
        char buf[1024];

        buf[0] = 'A';
        buf[1] = '\0';
        while((numchars > 0) && strcmp("\n", buf))
            numchars = get_line(client, buf, sizeof(buf));
        resource = fopen(filename, "r");
        if(resource == NULL)
            not_found(client);
        else {
            headers(client, filename);
            cat(client, resource);
        }
        fclose(resource);
    }

    void accept_request(int client) {
        char buf[1024];
        int numchars;
        char method[255];
        char url[255];
        char path[512];
        size_t i, j;
        struct stat st;
        char *query_string = NULL;

        numchars = get_line(client, buf, sizeof(buf));
        i = 0;
        j = 0;
        while(!ISspace(buf[j]) && (i < sizeof(method) - 1)) {
            method[i] = buf[j];
            i++;
            j++;
        }
        method[i] = '\0';

        if(strcasecmp(method, "GET") != 0) {
            unimplemented(client);
            close(client);
            return;
        }

        i = 0;
        while(ISspace(buf[j]) && (j < sizeof(buf)))
            j++;
        while(!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf))) {
            url[i] = buf[j];
            i++;
            j++;
        }
        url[i] = '\0';

        if(strcasecmp(method, "GET") == 0){
            query_string = url;
            while((*query_string != '?') && (*query_string != '\0'))
                query_string++;
            if(*query_string == '?'){
                *query_string = '\0';
                query_string++;
            }
        }

        sprintf(path, "static%s", url);
        if(path[strlen(path) - 1] == '/')
            strcat(path, "index.html");
        if(stat(path, &st) == -1) {
            while((numchars > 0) && strcmp("\n", buf))
                numchars = get_line(client, buf, sizeof(buf));
            not_found(client);
        } else {
            if ((st.st_mode & S_IFMT) == S_IFDIR)
                strcat(path, "/index.html");
            serve_file(client, path);
        }

        close(client);
    }

    //初始化httpd服务，建立套接字，绑定端口监听
    int startup(u_short port) {
        int httpd = 0;
        struct sockaddr_in name;
        httpd = socket(PF_INET, SOCK_STREAM, 0);
        if (httpd == -1)
            error_die("socket");
        int optval = 1;
        setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR, &optval,sizeof(optval));
        memset(&name, 0, sizeof(name));
        name.sin_family = AF_INET;
        name.sin_port = htons(port);
        name.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(httpd, (struct sockaddr*)&name, sizeof(name)) < 0)
            error_die("bind");
        if(port == 0) {
            socklen_t namelen = sizeof(name);
            if(getsockname(httpd, (struct sockaddr*)&name, &namelen) == -1)
                error_die("getsockname");
            port = ntohs(name.sin_port);
        }
        if (listen(httpd, 5) < 0)
            error_die("listen");
        return httpd;
    }

    u_short port;

public:
    HttpServer(u_short port):port(port){}

    void start() {
        int server_sock = -1;
        // u_short port = 9003;
        int client_sock;
        struct sockaddr_in client_name;
        socklen_t client_name_len = sizeof(client_name);

        server_sock = startup(port);
        printf("httpd running on port %d\n", port);

        while(1) {
            client_sock = accept(server_sock, (struct sockaddr *)&client_name, &client_name_len);
            if (client_sock == -1)
                error_die("accept");

            std::thread server_thread(bind(&HttpServer::accept_request, this, client_sock));
            server_thread.join();
        }
        close(server_sock);
    }

};


#endif
