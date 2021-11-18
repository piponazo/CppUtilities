/* Toy app where to learn about how to implement a minimal TCP server (for testing purposes)
 */
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>  // for sendfile()
#include <sys/socket.h>
#include <sys/stat.h>  // for getting file size using stat()
#include <unistd.h>

/*for O_RDONLY*/
#include <fcntl.h>

#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 2) {
    cerr << "syntax: app port" << endl;
    return EXIT_FAILURE;
  }

  struct sockaddr_in serv_addr, cli_addr;
  struct stat obj;
  int sock1, sock2;
  char buf[100], command[5], filename[20];
  int k, i, size, c;
  int filehandle;

  sock1 = socket(AF_INET, SOCK_STREAM, 0);
  if (sock1 == -1) {
    cerr << "Socket creation failed" << endl;
    return EXIT_FAILURE;
  }

  int portno = atoi(argv[1]);

  /* setup the host_addr structure for use in bind call */
  // server byte order
  serv_addr.sin_family = AF_INET;

  // automatically be filled with current host's IP address
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  // convert short integer value for port must be converted into network byte order
  serv_addr.sin_port = htons(portno);

  k = bind(sock1, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (k == -1) {
    cerr << "Binding error" << endl;
    return EXIT_FAILURE;
  }

  k = listen(sock1, 1);
  if (k == -1) {
    cerr << "Listen failed" << endl;
    return EXIT_FAILURE;
  }
  socklen_t clilen = sizeof(cli_addr);
  sock2 = accept(sock1, (struct sockaddr *)&cli_addr, &clilen);
  i = 1;
  while (1) {
    recv(sock2, buf, 100, 0);
    sscanf(buf, "%s", command);
    if (!strcmp(command, "ls")) {
      system("ls >temps.txt");
      i = 0;
      stat("temps.txt", &obj);
      size = obj.st_size;
      send(sock2, &size, sizeof(int), 0);
      filehandle = open("temps.txt", O_RDONLY);
      sendfile(sock2, filehandle, NULL, size);
    } else if (!strcmp(command, "get")) {
      sscanf(buf, "%s%s", filename, filename);
      stat(filename, &obj);
      filehandle = open(filename, O_RDONLY);
      size = obj.st_size;
      if (filehandle == -1)
        size = 0;
      send(sock2, &size, sizeof(int), 0);
      if (size)
        sendfile(sock2, filehandle, NULL, size);

    } else if (!strcmp(command, "put")) {
      int c = 0;
      sscanf(buf + strlen(command), "%s", filename);
      recv(sock2, &size, sizeof(int), 0);
      i = 1;
      while (1) {
        filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
        if (filehandle == -1) {
          sprintf(filename + strlen(filename), "%d", i);
        } else
          break;
      }
      std::vector<char> f(size);
      recv(sock2, f.data(), size, 0);
      c = write(filehandle, f.data(), size);
      close(filehandle);
      send(sock2, &c, sizeof(int), 0);
    } else if (!strcmp(command, "pwd")) {
      system("pwd>temp.txt");
      i = 0;
      FILE *f = fopen("temp.txt", "r");
      while (!feof(f))
        buf[i++] = fgetc(f);
      buf[i - 1] = '\0';
      fclose(f);
      send(sock2, buf, 100, 0);
    } else if (!strcmp(command, "cd")) {
      if (chdir(buf + 3) == 0)
        c = 1;
      else
        c = 0;
      send(sock2, &c, sizeof(int), 0);
    }

    else if (!strcmp(command, "bye") || !strcmp(command, "quit")) {
      printf("FTP server quitting..\n");
      i = 1;
      send(sock2, &i, sizeof(int), 0);
      exit(0);
    }
  }
  return 0;
}
