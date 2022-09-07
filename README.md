# Echo
For the Server socket, the set up sequence for the Stream (e.g. TCP) mode is:

socket()
  |
bind()
  |
listen()
  |
accept()
  |
recv()
  |
send()
  |
close()

Therefore, the same coding sequece is applied for the server socket.


For the Client socket, the set up sequence for the stream mode is:

scoket()
  |
connet()
  |
send()
  |
recv()
  |
close()

Therefore, the same code sequence is applied for the client socket.

For the client socket, the first message was from the defined message "Hello Summer!!", after that an input message is required for the client socket.

The server will not exit after it receive the first message. 

The client or server socket will exit if input message is empty (hit "Enter") or the length of the message is longer than 15.
