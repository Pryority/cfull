# Webserver

## Instructions

### Step 1: Set Up Project Structure

Create a directory for your project, let's call it webserver. Inside this directory, create the following files:

- _main.cpp_: The main C++ source code for the web server.
- _index.html_: The landing page for the personal website.
- _about.html_: An "About Me" page for the personal website.

> /webserver
> |-- main.cpp
> |-- index.html
> |-- about.html

### Step 2: Write HTML Content

Open index.html and about.html in a text editor and add some basic HTML content to each file. For example:

`index.html`:

```html
<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>My Personal Website</title>
  </head>
  <body>
    <h1>Welcome to My Personal Website!</h1>
    <p>This is the landing page.</p>
    <a href="/about.html">About Me</a>
  </body>
</html>
```

`about.html`:

```html
<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>About Me</title>
  </head>
  <body>
    <h1>About Me</h1>
    <p>This is the "About Me" page.</p>
  </body>
</html>
```

### Step 3: Write the C++ Server Code

Open main.cpp and add the C++ code for the web server. You can use the code provided in the previous response.

#### I: Include Necessary Headers

In your main.cpp file, include the necessary headers:

```c
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;
```

#### II: Define Server Constants

Define constants for the server settings:

```c
const int PORT = 8080;
const string WEB_ROOT = "./";
```

Ensure that you create a directory named webserver in your project directory. This is where your HTML files will be stored.

#### III: Implement Request Handler

Implement a function to handle incoming HTTP requests:

```c
void handleClient(int clientSocket) {}
```

##### i) Read the HTTP request

```c
char buffer[1024];
memset(buffer, 0, sizeof(buffer));
read(clientSocket, buffer, sizeof(buffer));
```

##### ii) Parse the request to get the requested file path

```c
istringstream request(buffer);
string requestLine;
getline(request, requestLine);
```

##### iii) Extract the requested file path from the request line

```c
istringstream requestLineStream(requestLine);
string method, filepath, httpVersion;
requestLineStream >> method >> filepath >> httpVersion;
```

##### iv) Add conditional statements to add routes to different pages

```c
if (filePath == "/" || filePath.empty()) {
    filePath = "/index.html";
} else if (filePath == "/about") {
    filePath = "/about.html";
} else {
    filePath = "/index.html";
}
```

##### v) Build the full path to the requested file

```c
string fullPath = WEB_ROOT + filePath;
```

##### vi) Add conditional statements for server responses

```c
ifstream file(fullPath);
if (!file.is_open()) {
    // If the file doesn't exist, respond with a 404 Not Found
    string notFoundResponse = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";
    notFoundResponse += "<html><body><h1>404 Not Found</h1></body></html>";
    write(clientSocket, notFoundResponse.c_str(), notFoundResponse.length());
} else {
    // If the file exists, read its content and respond with a 200 OK
    ostringstream fileContent;
    fileContent << file.rdbuf();

    string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    response += fileContent.str();
    write(clientSocket, response.c_str(), response.length());
}
```

##### vii) At the end of the request handler, close the client socket

```c
close(clientSocket);
```

#### IV: Implement the remaining server logic in the main function

```c
int main() {
    /*... inside here  ...*/
}
```

##### i) Create a socket

```c
int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
if (serverSocket == -1) {
    cerr << "Error creating socket\n";
    return 1;
}
```

##### ii) Bind the socket to a port

```c
sockaddr_in serverAddress;
serverAddress.sin_family = AF_INET;
serverAddress.sin_addr.s_addr = INADDR_ANY;
serverAddress.sin_port = htons(PORT);

if (::bind(serverSocket), reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress) == -1) {
  cerr << "Error listening on port " << PORT << "\n";
  close(serverSocket);
  return 1;
}

cout << "Server listening on port " << PORT << "...\n";
```
##### ii) Bind the socket to a port
iListen for connections on the binded
```c
sockaddr_in serverAddress;
serverAddress.sin_family = AF_INET;
serverAddress.sin_addr.s_addr = INADDR_ANY;
serverAddress.sin_port = htons(PORT);

if (::bind(serverSocket), reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress) == -1) {
  cerr << "Er10
```

##### iv) Accept and handle incoming connections

```c
while (true) {
  sockaddr_in clientAddress;
  socklen_t clientAddressLen = sizeof(clientAddress);
  int clientSocket = accept(serverSocket, reinterpret_cast<struct socketaddr*>(&clientAddress), &clientAddressLen);

  if (clientSocket == -1) {
    cerr << "Error accepting connections\n";
    continue;
  }
}
```

##### v) Handle requests with the client handler we made earlier

```c
while (true) {
  /* --- Previous code from iii) --- */

  handleClient(clientSocket);
}
```

##### vi) Close the server socket

```c
int main() {
  /* --- Previous Code --- */

  close(serverSocket);

  return 0;
}
```

### Step 4: Compile the Code

Compile the C++ code using a C++ compiler. If you are using g++, navigate to the webserver directory in the terminal and run the following command:

```bash

g++ -o webserver main.cpp
```

This will create an executable file named webserver.

### Step 5: Run the Server

Run the compiled executable:

```bash

./webserver
```

This will start the web server on port 8080.

### Step 6: Test the Personal Website

Open a web browser and visit <http://localhost:8080/> to view the landing page. Click on the "About Me" link to navigate to the about page.
