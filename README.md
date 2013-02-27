Cproxyserver
============
 =>HOW TO USE
   1.all folder in code folder. Client and server are in different folder.
    2.run make file. =>Make 
    3. ./proxy <port_num>
    4. server start listening for connection.
    5. connection with server using web client and talnet.
    6. send appropriate request which is given in question
    7. server show request link

 =>REQUEST TYPE
   
   <METHOD> <URL or PATH> <HTTP VERSION>
    Host: <HOSTNAME>
   => both are given in input

   GET http://www.iitg.ernet.in/ HTTP/1.0
    Host: www.iitg.ernet.in
            or
    GET / HTTP/1.0
    Host: www.iitg.ernet.in

=>EXIT FROM SERVER 
  1. server continue running till we manually not stop it
   2. ctrl + c using for exit from server
 
=> LIMITATIONS
  1.It does not handle concurrent requests, only sequential requests.  
   2.Only HTTP/1.0 implemented.
   3.Only “GET” method is implemented. 
   4.Connection: keep-alive not implemented. 
   5.And client request and after getting data it's exit.

=>FEATURES
   1.Accept GET feature and for other like POST ,PUT ,HEAD it's give 501 (NOT implemented error).
    2. port can be used after 1000 .
    3. accept all HTTP header but forward only HTTP/1.0 version
    4. close client after request complete
    5. used only default 80 port.
    6. NOT IMPLEMENTED Proxy works behind another proxy (without authentication). NOTE: Proxy details have to be specified in the code using a macro, file: http_proxy.cpp
=> IMPLEMENTATION DETAILS 
   1.client.c
      1. use the structure addrinfo .
         2. use AF_INET6 to force IPv6
        3. loop through all the results and connect to the first we can
   means using getaddrinfo we using hostname to ipheader.
    4.finding a socketfd and connect it to server.
    5. and read request from stdin.
      First line in request contain GET format
      second line “Host: <host_name>“ contain
    and the data send to server in one string format and “ ” between fistline and second line
   6.using write send it to server .
    7. and last read message data from server.
      And that's in html format
   
  2.server.c
   1.A simple server in the internet domain using TCP 
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection.
    2. In main first checking argument , after we made socket id using socket stream.
   2. change port_no to integer format.
   3. after this we bind the address of client.
   4. listen to the port.
   5. after it accept connection for infinite user till we will not close the server.
   6.calling dostuff function .
   7.DOSTUFF()
  There is a separate instance of this function 
  for each connection.  It handles all communication 
  once a connnection has been established.
  8.
   we pass in this function connection sock id.
  9.here we read request from client and parsing it's request in given format using strok_r function.
  10.when we get host name then we have to pass it to proxy server.
  11. so we make new socket connection for proxy server and connect it to particular host.
  12. after connect to particular host it send data to client
  13.server side it's print Request by client: <request>


example

  1.
    GET http://www.iitg.ernet.in/ HTTP/1.0                          
    Host: www.iitg.ernet.in

2.    
   GET http://jatinga.iitg.ernet.in/ HTTP/1.0
   Host: jatinga.iitg.ernet.in

  3.GET http://www.google.com/ HTTP/1.0
    Host: www.google.com 


