/*FTP Client*/

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/*for getting file size using stat()*/
#include <sys/stat.h>

/*for sendfile()*/
#include <sys/sendfile.h>

/*for O_RDONLY*/
#include <fcntl.h>

#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 3) {
    cerr << "syntax: app ip port" << endl;
    return EXIT_FAILURE;
  }

  struct sockaddr_in serv_addr;
  struct stat obj;
  int sock;
  int choice;
  char buf[100], filename[20];
//  char command[5];
  int k, size, status;
  int filehandle;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    cerr << "socket creation failed" << endl;
    return EXIT_FAILURE;
  }

  struct hostent *server = gethostbyname(argv[1]);
  if (server == NULL) {
    std::cerr << "ERROR, no such host\n";
    return EXIT_FAILURE;
  }

  serv_addr.sin_family = AF_INET;
  int portno = atoi(argv[2]);
  serv_addr.sin_port = htons(portno);
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

  k = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (k == -1) {
    cerr << "Connect Error" << endl;
    return EXIT_FAILURE;
  }
  int i = 1;
  while (1) {
    printf("\nEnter a choice:\n1- get\n2- put\n3- pwd\n4- ls\n5- cd\n6- quit\n");
    scanf("%d", &choice);
    switch (choice) {
      case 1: {
        printf("Enter filename to get: ");
        scanf("%s", filename);
        strcpy(buf, "get ");
        strcat(buf, filename);
        send(sock, buf, 100, 0);
        recv(sock, &size, sizeof(int), 0);
        if (!size) {
          printf("No such file on the remote directory\n\n");
          break;
        }
        std::vector<char> f(size);
        recv(sock, f.data(), size, 0);
        while (1) {
          filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
          if (filehandle == -1) {
            sprintf(filename + strlen(filename), "%d",
                    i);  // needed only if same directory is used for both server and client
          } else
            break;
        }
        write(filehandle, f.data(), size);
        close(filehandle);
        strcpy(buf, "cat ");
        strcat(buf, filename);
        system(buf);
        break;
      }
      case 2:
      {
        printf("Enter filename to put to server: ");
        scanf("%s", filename);
        filehandle = open(filename, O_RDONLY);
        if (filehandle == -1) {
          printf("No such file on the local directory\n\n");
          break;
        }
        strcpy(buf, "put ");
        strcat(buf, filename);
        send(sock, buf, 100, 0);
        stat(filename, &obj);
        size = obj.st_size;
        send(sock, &size, sizeof(int), 0);
        sendfile(sock, filehandle, NULL, size);
        recv(sock, &status, sizeof(int), 0);
        if (status)
          printf("File stored successfully\n");
        else
          printf("File failed to be stored to remote machine\n");
        break;
      }
      case 3:
      {
        strcpy(buf, "pwd");
        send(sock, buf, 100, 0);
        recv(sock, buf, 100, 0);
        printf("The path of the remote directory is: %s\n", buf);
        break;
      }
      case 4:
      {
        strcpy(buf, "ls");
        send(sock, buf, 100, 0);
        recv(sock, &size, sizeof(int), 0);
        std::vector<char> f(size);
        recv(sock, f.data(), size, 0);
        filehandle = creat("temp.txt", O_WRONLY);
        write(filehandle, f.data(), size);
        close(filehandle);
        printf("The remote directory listing is as follows:\n");
        system("cat temp.txt");
        break;
      }
      case 5:
      {
        strcpy(buf, "cd ");
        printf("Enter the path to change the remote directory: ");
        scanf("%s", buf + 3);
        send(sock, buf, 100, 0);
        recv(sock, &status, sizeof(int), 0);
        if (status)
          printf("Remote directory successfully changed\n");
        else
          printf("Remote directory failed to change\n");
        break;
      }
      case 6:
      {
        strcpy(buf, "quit");
        send(sock, buf, 100, 0);
        recv(sock, &status, 100, 0);
        if (status) {
          printf("Server closed\nQuitting..\n");
          return EXIT_SUCCESS;
        }
        printf("Server failed to close connection\n");
      }
    }
  }

  return EXIT_SUCCESS;
}
