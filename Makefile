echoall: echoserver echoclient
echoserver: echoserver.c
    gcc -o echoserver echoserver.c
echoclient: echoclient.c
    gcc -o echoclient echoclient.c
