/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <vector>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <iostream>

using namespace std;
vector<thread> lt_thread;
vector<int> lt_sock;
struct fd_set master_set, working_set;
int max_sock;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void work_thd()
{
    int rc = 0;
    int desc_ready;
    struct timeval timeout;
    timeout.tv_sec = 3 * 60;
    timeout.tv_usec = 0;
    char buffer[800];
    int close_conn, end_server = 0;
    while(true)
    {
        // Copy master_set into working_set
        memcpy(&working_set, &master_set, sizeof(master_set));

        printf("Waiting on select()...\n");
        rc = select(max_sock+1, &working_set, NULL, NULL, &timeout);
        if (rc < 0)
        {
            perror("  select() failed\n");
            break;
        }
        if (rc == 0)
        {
            printf("  select() timed out. End program.\n");
            break;
        }

        desc_ready = rc; // number of sds ready in working_set

        // Check each sd in working_set
        for (int i = 0; i <= max_sock+1 && desc_ready > 0; ++i)
        {
            // Check to see if this sd is ready
            if (FD_ISSET(i, &working_set))
            {
                printf("  Descriptor %d is avaliable\n", i);
                    do
                    {
                        rc = recv(i, buffer, sizeof(buffer), 0);

                        // Receive data on sd "i", until failure occurs
                        if (rc < 0)
                        {
                            // Normal failure
                            if (errno != EWOULDBLOCK)
                            {
                                perror("  recv() failed\n");
                                close_conn = true;
                            }
                            break;
                        }

                        // The connection has been closed by the client
                        if (rc == 0)
                        {
                            printf("  Connection closed\n");
                            close_conn = true;
                            break;
                        }

                        /* Receiving data succeeded and echo it back
                           the to client */
                        int len = rc;
                        printf("  %d bytes received\n", len);
                        rc = send(i, buffer, len, 0);
                        if (rc < 0)
                        {
                            perror("  send() failed");
                            close_conn = true;
                            break;
                        }
                    }
                    while (true);
            }
        }
    }

}

void recv_msg(int sock)
{
    char buffer[256];
    int n = read(sock,buffer,255);
    while (n >=0)
    {
        printf("%s\n",buffer);
        n = read(sock,buffer,255);
        if( n > 0)
        {
            char buf[] = "Get your message";
             write(sock,buf,strlen(buf));
        }
    }
    if (n < 0) 
         error("ERROR reading from socket");
}

void write_msg(int sockfd)
{
    char buffer[256];
    while(true)
    {
        printf("Please enter the message: ");
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        int n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
        {
             error("ERROR writing to socket");
             break;
        }
    }
    
}


int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (::bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
    max_sock = listen(sockfd,5);
    cout << "max: " << max_sock;

     thread thd1(work_thd);
     // Intialize sd set
    FD_ZERO(&master_set);
    

     while(true)
     {
        clilen = sizeof(cli_addr);
         newsockfd = accept(sockfd, 
                     (struct sockaddr *) &cli_addr, 
                     &clilen);
         if (newsockfd < 0) 
              error("ERROR on accept");
         // thread accept_thd( recv_msg, newsockfd);
         // lt_thread.push_back( std::move(accept_thd));
          FD_SET(newsockfd, &master_set);
     }
     
   
     close(newsockfd);
     close(sockfd);

     for (auto& th : lt_thread) 
        th.join();
    thd1.join();

     return 0; 
}
