#Socket Programming Project
***
<br>
- [Socket Programming Project](#socket-programming-project)
- [Student Info](#student-info)
  - [Student Name: Rui Wu](#student-name)
  - [StudentID: *********](#studentid)
- [My work](#My Work)
- [How to run?](#how-to-run)
- [How does it work?](#how-does-it-work)
- [Format of the messages](#format-of-the-messages)
- [Idiosyncrasy](#idiosyncrasy)
- [Reused Code](#reused-code)

<br>

# Student Info
## Student Name: Rui Wu
## StudentID: *******
<br>

#My work
Every parts in the project except extra credit part.
<br>

#How to run?
Firstly, run makefile, then get the exec files.
<br>

Start serverM, serverC, serverCS, serverEE, client in this order. Then the server side will run and will never stop unless you try to stop it. Only 3 times authentication faile, client will shut down.
<br>

#How it works
In serverM, it will encrypt client's information to send to the serverC. In the serverC, it

will check cred.txt and see whether the information is matched. If match, it will send a flag 2 

to serverM, and if it is not successful it will send flag 0 or 1 to serverM. The serverM will 

relay the flag to the client. If the flag is 0 or 1, client will be noticed that retype their 

username and password. client only has 3 times to try, after using 3 times, client will close 

socket. If it is successful, it will enter the next loop which to enter coursename and 

catergory. After serverM recieve this message, it will check to send what server site.

The serverCS and serverEE will check this message in their dictionary. If match, they will send

the right message to the serverM. If not,  they will send the error flag to the serverM. The

last, serverM will relay this message to the client, and client will show the message they want 

to know.

<br>

# Format of the messages  
|  From to          |  |
|  ----  | ----  |
| client to serverM  | username, password|
| serverM to serverC  | encrypt_username and encrypt_password|
| serverC to serverM  | flag|
||   0 means username doesn't match
||     1 means password doesn't match
||     2 means success
| serverM to client  | flag|
|client receive| flag|
|  | check flag|
| client to serverM  | coursename and category|
|serverM to serverCS/serverEE| coursename and category|
| serverCS/serverEE to serverM  | message|
| serverM to client  | message|

<br>

# Idiosyncrasy

1. Input check, if there is any issue in client, it will loop, but the server will still run.
2. The userinformation are stored in serverM, serverC/CS/EE will send multiple times.  
   
<br>

# Reused Code  

I referenced Beej Guide Network Programming to write some functions like sendto, accept.etc and I have them commented in my code.
