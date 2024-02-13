#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cerrno>
#include <stdio.h>

using namespace std;

const int PORT = 8080;
const string WEB_ROOT = "./views";
const int MAX_NUM_CONNECTIONS = 10;
const unordered_map<string, string> routes = {
    {"/", "index.html"},
    {"/about", "about.html"},
    {"/login", "login.html"},
    {"/contact", "contact.html"}};

void handleClient(int clientSocket);
int initializeServerSocket();

int requirePortBindOK(int &serverSocket, sockaddr_in &serverAddress)
{
    if (::bind(serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        printf("\t🚢🚫 ==> ERROR BINDING on port %d\n", PORT);
        close(serverSocket);
        return 1;
    }

    return 0;
}

int requireListeningOK(int &serverSocket)
{
    if (listen(serverSocket, MAX_NUM_CONNECTIONS) == -1)
    {
        printf("\t📡🚫 ==> ERROR LISTENING on port %d\n", PORT);
        close(serverSocket);
        return 1;
    }

    return 0;
}

int requireSocketCreatedOK(int &serverSocket)
{
    if (serverSocket == -1)
    {
        printf("\t📞🚫 ==> ERROR CREATING SOCKET on port %d\n", PORT);
        return 1;
    }

    return 0;
}

int main()
{
    int serverSocket = initializeServerSocket();

    if (serverSocket == -1)
    {
        return 1;
    }

    printf("\t📡 ==> OK SERVER LISTENING on port %d...\n", PORT);

    /* -- CLIENT HANDLER LOOP  ---------------- */
    while (true)
    {
        sockaddr_in clientAddress;
        socklen_t clientAddressLen = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr *>(&clientAddress), &clientAddressLen);

        if (clientSocket == -1)
        {
            printf("\t🔌🚫 ==> ERROR accepting connections: %s\n", strerror(errno));
            continue;
        }

        handleClient(clientSocket);
    }

    close(serverSocket);

    return 0;
}

void handleClient(int clientSocket)
{
    /* -- CLIENT HANDLER CONFIG ------------------------ */
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    read(clientSocket, buffer, sizeof(buffer));

    istringstream request(buffer);
    string requestLine;
    getline(request, requestLine);

    istringstream requestLineStream(requestLine);
    string method, filePath, httpVersion;
    requestLineStream >> method >> filePath >> httpVersion;

    printf("\t🔌✅ ==> ACCEPTED CONNECTION for file: %s\n", filePath.c_str());

    /* -- ROUTES ------------------------ */
    auto it = routes.find(filePath);
    if (it != routes.end())
    {
        // printf("it: %s\n", it->first.c_str());
        // printf("it->second: %s\n", it->second.c_str());
        filePath = it->second;
    }
    else
    {
        filePath = "/404.html";
    }

    string fullPath = WEB_ROOT + "/" + filePath;
    // printf("fullPath: %s\n", fullPath.c_str());

    /* -- SERVER RESPONSES -------------- */
    ifstream file(fullPath);
    if (!file.is_open())
    {
        // If the file doesn't exist, respond with a 404 Not Found
        string notFoundResponse = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";
        notFoundResponse += "<html><body><h1>404 Not Found</h1></body></html>";
        write(clientSocket, notFoundResponse.c_str(), notFoundResponse.length());
    }
    else
    {
        // If the file exists, read its content and respond with a 200 OK
        ostringstream fileContent;
        fileContent << file.rdbuf();

        string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        response += fileContent.str();
        write(clientSocket, response.c_str(), response.length());
    }

    close(clientSocket);
}

int initializeServerSocket()
{
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1)
    {
        perror("\t📞🚫 ==> ERROR CREATING SOCKET");
        return -1;
    };
    if (requirePortBindOK(serverSocket, serverAddress) != 0)
    {
        return -1;
    };
    if (requireListeningOK(serverSocket) != 0)
    {
        return -1;
    };

    return serverSocket;
}