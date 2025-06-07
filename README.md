Write two programs in C: a server program and a client program (which can be executed by potentially many clients –
and they should be able to be served simultaneously by the server), which will be able to communicate with each
other (using UNIX-domain stream sockets) repeatedly as follows:

    The client will send the server a sequence of N integer numbers, which it will read as input from the user.

    The server, after receiving the sequence, will compute the average of the numbers. If the average is greater than 20, it will send back to the client:
    (a) the computed average, and
    (b) an appropriate acceptance message (e.g., 'Sequence Ok').
    Otherwise, it will send only a failure message (e.g., 'Check Failed').

    The client, after each communication, should print the server’s response on the screen and ask the user for the next sequence.

    The communication will end (from the client’s side) when the user indicates that they do not wish to enter another sequence for checking.
