# Synchronous Chat
A synchronous chat where the server and client interact with each other through blocking operations. This means that the server cannot send a message until it receives one from the client, and vice versa.  

## Controls
1. Start the server.  
2. Connect to the server using the client.  
3. Use the /exit command to exit the chat.  

## Design
All core operations are performed in a blocking mode, meaning the server waits for a response from the client before continuing.  
The server and client communicate using a TCP connection.  

## Build
1. Last release CMake, Git  
2. Open folder with your IDE  
3. CMake will automatically pull all required dependencies. However, please note that this process may take quite a long time (especially if dependencies are being downloaded or compiled from scratch).  
