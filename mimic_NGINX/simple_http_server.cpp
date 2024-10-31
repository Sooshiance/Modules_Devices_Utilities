#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const int PORT = 8080;
const int BACKLOG = 10;

// handle user's requests
void handle_client(int client_socket)
{
    char buffer[4096];
    
    memset(buffer, 0, sizeof(buffer));
    read(client_socket, buffer, sizeof(buffer));

    std::string request(buffer);
    std::cout << "Request:" << request << std::endl;

    // Parse the request
    std::istringstream request_stream(request);
    std::string method, path;
    request_stream >> method >> path;

    // Serve static files
    if (method == "GET")
    {
        if (path == "/")
        {
            path = "/index.html"; // Default file
        }

        std::ifstream file("." + path);
        if (file)
        {
            std::stringstream file_buffer;
            file_buffer << file.rdbuf();
            std::string response_body = file_buffer.str();

            std::string response =
                "HTTP/1.1 200 OK \n"
                "Content-Length: " +
                std::to_string(response_body.size()) + "\n"
                                                       "Content-Type: text/html \n"
                                                       "\n" +
                response_body;

            write(client_socket, response.c_str(), response.size());
        }
        else
        {
            std::string response =
                "HTTP/1.1 404 Not Found \n"
                "Content-Length: 0\n"
                "\n";
            write(client_socket, response.c_str(), response.size());
        }
    }

    close(client_socket);
}

int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (server_socket < 0)
    {
        std::cerr << "Error opening socket" << std::endl;
        return 1;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    listen(server_socket, BACKLOG);
    
    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true)
    {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket >= 0)
        {
            std::thread(handle_client, client_socket).detach();
        }
        else
        {
            std::cerr << "Error accepting connection" << std::endl;
        }
    }

    close(server_socket);
    
    return 0;
}
