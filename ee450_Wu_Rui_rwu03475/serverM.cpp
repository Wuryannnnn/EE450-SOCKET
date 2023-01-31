#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctime>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <iomanip>
#include <map>

using namespace std;

#define LOCAL_HOST "127.0.0.1" // Host address
#define serverC_PORT 21283
#define serverCS_PORT 22283
#define serverEE_PORT 23283
#define M_UDP_PORT 24283 
#define M_Client_TCP_PORT 25283
#define MAXDATASIZE 1024 // max number of bytes we can get at once
#define BACKLOG 1 // max number of connections allowed on the incoming queue
#define FAIL -1
#define BUFLEN 1024

//store origin client infom
char userInfom[BUFLEN];

//store encry infom
char userName[1000];
char passWords[1000];

//judge infom valide
char flag[BUFLEN];

//coursecode
char selectflag[BUFLEN];
char category[BUFLEN];



char ResponseInfo[BUFLEN];

int recvLen1, recvLen2, sendLen1, sendLen2, recvLen, sendLen, recvLen3;
string nowflag, coursecode; //falg

string nowusername, nowpasswords;//userinfo_origin

string nowcourseCode, nowcourseCate;//user_input



struct sockaddr_in m_tcp_client, m_udp, client, serverC, serverCS, serverEE;
int m_tcp_client_fd, m_udp_fd, new_tcp_client_fd;



void initial_ClientTCP(){
    //create socket//
    m_tcp_client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(m_tcp_client_fd < 0){
        perror("Error creating TCP socket");
        exit(EXIT_FAILURE);
    }
    memset(&m_tcp_client, 0, sizeof(m_tcp_client));
    m_tcp_client.sin_family = AF_INET;
    m_tcp_client.sin_port = htons(M_Client_TCP_PORT);
    m_tcp_client.sin_addr.s_addr = inet_addr(LOCAL_HOST);

    //bind socket//
    if(bind(m_tcp_client_fd, (struct sockaddr *)&m_tcp_client, sizeof(m_tcp_client)) == -1){
        close(m_tcp_client_fd);
        perror("Error binding TCP socket");
        exit(EXIT_FAILURE);
    }

    if(listen(m_tcp_client_fd, BACKLOG) == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void initial_UDP(){
    //create socket//
    m_udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(m_udp_fd < 0){
        perror("Error creating UDP socket");
        exit(EXIT_FAILURE);
    }
    memset(&m_udp, 0, sizeof(m_udp));
    m_udp.sin_family = AF_INET;
    m_udp.sin_port = htons(M_UDP_PORT);
    m_udp.sin_addr.s_addr = inet_addr(LOCAL_HOST);

    //bind socket//
    if(::bind(m_udp_fd, (struct sockaddr *)&m_udp, sizeof(m_udp)) == -1){
        close(m_udp_fd);
        perror("Error binding UDP socket");
        exit(EXIT_FAILURE);
    }
}

void setC(){
    serverC.sin_family = AF_INET;
    //M Port #
    serverC.sin_port = htons(serverC_PORT);
    //M IP ADDR - INADDR_LOOPBACK refers to localhost ("127.0.0.1")
    serverC.sin_addr.s_addr = inet_addr(LOCAL_HOST);
}

void setCS(){
    serverCS.sin_family = AF_INET;
    //M Port #
    serverCS.sin_port = htons(serverCS_PORT);
    //M IP ADDR - INADDR_LOOPBACK refers to localhost ("127.0.0.1")
    serverCS.sin_addr.s_addr = inet_addr(LOCAL_HOST);
}

void setEE(){
    serverEE.sin_family = AF_INET;
    //M Port #
    serverEE.sin_port = htons(serverEE_PORT);
    //M IP ADDR - INADDR_LOOPBACK refers to localhost ("127.0.0.1")
    serverEE.sin_addr.s_addr = inet_addr(LOCAL_HOST);
}

void acceptClient(){
    socklen_t clientLen = sizeof(client);
    new_tcp_client_fd = accept(m_tcp_client_fd, (struct sockaddr *)&m_tcp_client, &clientLen);
    if(new_tcp_client_fd == -1){
        perror("Error accepting socket");
        exit(EXIT_FAILURE);
    }
}


void recvUserInFromClient(){
    memset(userInfom, 0, sizeof(userInfom));
    string res;
    recvLen1 = recv(new_tcp_client_fd, userInfom, BUFLEN, 0);
    if(recvLen1 == -1){
        perror("Error receiving message from client");
        close(new_tcp_client_fd);
        exit(EXIT_FAILURE);
    }
    userInfom[recvLen1] = '\0';
    res = string(userInfom);
    int pos;
    for(int i = 0; i < res.size(); i++){
        pos = res.find(',');
        nowusername = res.substr(0, pos);
        nowpasswords = res.substr(pos+1);
    }

    cout << "The main server received the authentication for <" << nowusername << "> using TCP over port <25283>." << endl;
    // cout << nowUsername <<endl;

}

// void recvUserPassFromClient(){
//     // acceptClient();
//     string res;
//     memset(userInfom, 0, sizeof(userInfom));
//     recvLen2 = recv(new_tcp_client_fd, userInfom, BUFLEN, 0);
//     if(recvLen2 == -1){
//         perror("Error receiving message from client");
//         close(new_tcp_client_fd);
//         exit(EXIT_FAILURE);
//     }
//     userInfom[recvLen2] = '\0';
//     // userinformation.username = nowUsername;
//     res = string(userInfom);
//     temp.push_back(res);
//     // for(int i = 0; i < recvLen2; i++){
//     //     nowPasswords += userInfom[i];
//     // }
//     cout << "The main server received the authentication for <" << temp[1] << "> using TCP over port <25283>." << endl;
    
//     // cout << nowPasswords <<endl;
//     // userinformation.passwords = nowPasswords;
//     // close(new_tcp_client_fd);
// }

string encrypt_username(string username){
    for(int i = 0; i < username.size(); i++){
        if(username[i] >= 'a' && username[i] <= 'v'){
            username[i] = username[i]+4;
        }else if(username[i] >= 'w' && username[i] < 'z'){
            username[i] = username[i] - 22;
        }
    }
    return username;
};


string encrypt_passwords(string passwords){
    for(char &i : passwords){
        if(int(i) >= 48 && int(i) <= 57){
            if(int(i) >= 55 && int(i) <= 57){
                i = char(int(i) - 6);
            }else{
                i = char(int(i) + 4);
            }
        }else if(i >= 65 && i <= 90){
            if(int(i) >= 87 && int(i) <= 90){
                i = char(int(i) - 22);
            }else{
                i = char(int(i) + 4);
            }
        }else if(i >= 97 && i <= 122){
            // w-z
            if(int(i) >= 119 && int(i) <=122){
                i = char(int(i) - 22);
            }else{
                i = char(int(i) + 4);
            }
        }
    }
    return passwords;
};

void getEncryp(){
    string userN = encrypt_username(nowusername);
    
    string passW = encrypt_passwords(nowpasswords);
    strcpy(userName, userN.c_str());
    strcpy(passWords, passW.c_str());
}


void sendToC(){
    cout << "The main server sent a authentication request to serverC" << endl;
    // char now_userN[BUFLEN];
    // char now_userP[BUFLEN];
    // memset(&now_userinfo, 0, sizeof(now_userinfo));
    // memcpy(&now_userinfo, &userinformation, sizeof(userinformation));
    // sprintf(now_userN,"%s", userN.c_str());
    // sprintf(now_userP, "%s", passW.c_str());
    sendLen1 = sendto(m_udp_fd, userName, sizeof(userName), 0, (struct sockaddr *) &serverC, sizeof(struct sockaddr_in));
    if(sendLen1 == -1){
        perror("Error sending UDP message1 to serverC from serverM");
        close(new_tcp_client_fd);
        exit(EXIT_FAILURE);
    }

    sendLen1 = sendto(m_udp_fd, passWords, sizeof(passWords), 0, (struct sockaddr *) &serverC, sizeof(struct sockaddr_in));
    if(sendLen1 == -1){
        perror("Error sending UDP message2 to serverC from serverM");
        close(new_tcp_client_fd);
        exit(EXIT_FAILURE);
    }

    cout << "The main server received the feedback from serverC using UDP over port 21283." << endl;
}

void recieveaAuflagFromserverC(){
    memset(flag, 0, sizeof(flag));
    socklen_t serverCLen = sizeof(serverC);
    recvLen = recvfrom(m_udp_fd, flag, BUFLEN, 0, (sockaddr *)&serverC, &serverCLen);
    if(recvLen < 0){
        perror("Error receiving message from serverC.");
        close(new_tcp_client_fd);
        exit(EXIT_FAILURE);
    }
    flag[recvLen] = '\0';
    nowflag = string(flag);
    cout << "The main server received the result of the authentication request from ServerC using UDP over port <24283>." << endl;
}

void show(char flag){
    switch(flag){
        case '0':{
            cout << "the authentication request has failed (no username)" << endl;
            break;
        }
        case '1':{
            cout << "password does not match." << endl;
            break;
        }
        case '2':{
            cout << "authentication request has passed" << endl;
            break;
        }
        default:{
            cout << "Error recieved flag from serverC." << endl;
            break;
        }
    }
}

void sendAuflagToClient(){
    char now_flag[1000];
    strcpy(now_flag, nowflag.c_str());
    int sent = send(new_tcp_client_fd, now_flag, 1000, 0);
    if(sent == -1){
        perror("Connection error");
        close(new_tcp_client_fd);
        exit(1);
    }
    cout << "The main server sent the authentication result to the client." << endl;
    memset(now_flag, 0, sizeof(now_flag));

}

void receiveFromClientRequest(){
    memset(selectflag, 0, sizeof(selectflag));
    recvLen3 = recv(new_tcp_client_fd, selectflag, sizeof(selectflag), 0);
    if(recvLen3 < 0){
        perror("Reading Stream Message Error.");
        exit(1);
    }
    else if(recvLen3 == 0){
        printf("Connection has ended\n");
    }
    selectflag[recvLen3] = '\0';
}

void getCourseCode(){
    coursecode.clear();
    if(selectflag[0] != '\0'){
        for(int i = 0; i < 2; i++){
            coursecode += selectflag[i];
        }
    }
    string res = string(selectflag);
    int pos;
    for(int i = 0; i < res.size(); i++){
        pos = res.find(',');
        nowcourseCode = res.substr(0, pos);
        nowcourseCate = res.substr(pos+1);
    }
}

// void recieveCategoryFromClient(){
//     cout << "run category" << endl;
//     recvLen = recv(new_tcp_client_fd, category, sizeof(category), 0);
//     if(recvLen < 0){
//         perror("Reading Stream Message Error.");
//         exit(1);
//     }
//     else if(recvLen == 0){
//         printf("Connection has ended\n");
//     }
//     category[recvLen] = '\0';
//     usecategory = string(category);
// }

// void sendAucourseToClient(){
//     cout << "This course can not be found" << endl;
//     if(send(new_tcp_client_fd, "404", strlen("404"), 0) == -1){
//         perror("Error sending data to clent.");
//         close(new_tcp_client_fd);
//         exit(EXIT_FAILURE);
//     }
// }

void sendRequestToEE(){
    cout << "The main server sent a request to serverEE" << endl;
    if((sendLen = sendto(m_udp_fd, selectflag, sizeof(selectflag), 0, (struct sockaddr *)&serverEE, sizeof(struct sockaddr_in))) == -1){
        perror("Error sending UDP message to serverEE from the main server");
        close(new_tcp_client_fd);
        exit(EXIT_FAILURE);
    }
    memset(selectflag, 0, sizeof(selectflag));
}

void sendRequestToCS(){
    cout << "The main server sent a request to serverCS" << endl;
    if((sendLen = sendto(m_udp_fd, selectflag, sizeof(selectflag), 0, (struct sockaddr *)&serverCS, sizeof(struct sockaddr_in))) == -1){
        perror("Error sending UDP message to serverCS from the main server");
        close(new_tcp_client_fd);
        exit(EXIT_FAILURE);
    }
    memset(selectflag, 0, sizeof(selectflag));
}


void receiveFromCS(){
    // char recv_credits[BUFLEN];
    // char recv_professor[BUFLEN];
    // char recv_days[BUFLEN];
    // char recv_coursename[BUFLEN];
    memset(ResponseInfo, 0, sizeof(ResponseInfo));
    socklen_t serverCSLen = sizeof(serverCS);

    
        if((recvLen = recvfrom(m_udp_fd, ResponseInfo, BUFLEN, 0, (struct sockaddr *) &serverCS, &serverCSLen )) < 0){
            perror("Error receiving message from ServerCS");
            close(new_tcp_client_fd);
            exit(EXIT_FAILURE);
        }
        ResponseInfo[recvLen] = '\0';
        cout << "The main server received the response from server<CS> using UDP over port <24283>." << endl;
        

        // if((recvLen = recvfrom(m_udp_fd, recv_professor, BUFLEN, 0, (struct sockaddr *) &serverCS, &serverCSLen )) < 0){
        //     perror("Error receiving message from ServerCS");
        //     close(new_tcp_client_fd);
        //     exit(EXIT_FAILURE);
        // }
        // recv_professor[recvLen] = '\0';
        // requestInfo.Professor = string(recv_professor);

        // if((recvLen = recvfrom(m_udp_fd, recv_days, BUFLEN, 0, (struct sockaddr *) &serverCS, &serverCSLen )) < 0){
        //     perror("Error receiving message from ServerCS");
        //     close(new_tcp_client_fd);
        //     exit(EXIT_FAILURE);
        // }
        // recv_days[recvLen] = '\0';
        // requestInfo.Days = string(recv_days);

        // if((recvLen = recvfrom(m_udp_fd, recv_coursename, BUFLEN, 0, (struct sockaddr *) &serverCS, &serverCSLen )) < 0){
        //     perror("Error receiving message from ServerCS");
        //     close(new_tcp_client_fd);
        //     exit(EXIT_FAILURE);
        // }
        // recv_coursename[recvLen] = '\0';
        // requestInfo.CourseName = string(recv_coursename);

        // if(recv_credits[0] != '\0' || recv_professor[0] != '\0' || recv_days[0] != '\0' || recv_coursename[0] != '\0'){
        //     cout << "The main server received the response from server<CS> using UDP over port <22283>." << endl;
        // }
}

void receiveFromEE(){
    // char recv_credits[BUFLEN];
    // char recv_professor[BUFLEN];
    // char recv_days[BUFLEN];
    // char recv_coursename[BUFLEN];

    // int recvFinished = 0;
    memset(ResponseInfo, 0, sizeof(ResponseInfo));
    socklen_t serverEELen = sizeof(serverEE);

        if((recvLen = recvfrom(m_udp_fd, ResponseInfo, BUFLEN, 0, (struct sockaddr *) &serverEE, &serverEELen )) < 0){
            perror("Error receiving message from ServerCS");
            close(new_tcp_client_fd);
            exit(EXIT_FAILURE);
        }
        ResponseInfo[recvLen] = '\0';
        cout << "The main server received the response from server<EE> using UDP over port <24283>." << endl;


        // requestInfo.Credit = string(recv_credits);

        
        // if((recvLen = recvfrom(m_udp_fd, recv_professor, BUFLEN, 0, (struct sockaddr *) &serverEE, &serverEELen )) < 0){
        //     perror("Error receiving message from ServerEE");
        //     close(new_tcp_client_fd);
        //     exit(EXIT_FAILURE);
        // }
        // recv_professor[recvLen] = '\0';
        // requestInfo.Professor = string(recv_professor);

        // if((recvLen = recvfrom(m_udp_fd, recv_days, BUFLEN, 0, (struct sockaddr *) &serverEE, &serverEELen )) < 0){
        //     perror("Error receiving message from ServerEE");
        //     close(new_tcp_client_fd);
        //     exit(EXIT_FAILURE);
        // }
        // recv_days[recvLen] = '\0';
        // requestInfo.Days = string(recv_days);

        // if((recvLen = recvfrom(m_udp_fd, recv_coursename, BUFLEN, 0, (struct sockaddr *) &serverEE, &serverEELen )) < 0){
        //     perror("Error receiving message from ServerEE");
        //     close(new_tcp_client_fd);
        //     exit(EXIT_FAILURE);
        // }
        // recv_coursename[recvLen] = '\0';
        // requestInfo.CourseName = string(recv_coursename);

        // if(recv_credits[0] != '\0' || recv_professor[0] != '\0' || recv_days[0] != '\0' || recv_coursename[0] != '\0'){
        //     cout << "The main server received the response from server<EE> using UDP over port <23283>." << endl;
        // }
}

void sendResponseInfoToClient(){
    cout << "The main server sent the query information to the client." << endl;
    // char res[BUFLEN];
    // memset(&res, 0, sizeof(res));
    // memcpy(&res, &requestInfo, sizeof(requestInfo));
    int sent = send(new_tcp_client_fd, ResponseInfo, sizeof(ResponseInfo), 0);
    if(sent == -1){
        perror("Connection Error");
        close(new_tcp_client_fd);
        exit(1);
    }
    memset(ResponseInfo, 0, sizeof(ResponseInfo));
}


int main(){
    initial_ClientTCP();
    initial_UDP();
    setCS();
    setEE();
    setC();
    // acceptClient();
    int count = 3;
    cout << "The main server is up and running." << endl;
while(1){
    acceptClient();
    while(1){
    recvUserInFromClient();
    // recvUserPassFromClient();
    // cout << nowUsername << endl;
    // cout << encrypt_username(nowUsername) << endl;
    // cout << encrypt_passwords(nowPasswords) << endl;
    // cout << nowUsername << endl;
    getEncryp();
    sendToC();
    recieveaAuflagFromserverC();
    if(nowflag[0] == '0' || nowflag[0] == '1'){
        show(nowflag[0]);
        // acceptClient();
        sendAuflagToClient();
        count--;
        if(count == 0){
            count = 3;
            break;
        }
    }
    else{
        show(nowflag[0]);
        // acceptClient();
        sendAuflagToClient();
        while(1){
            receiveFromClientRequest();
            getCourseCode();
            cout << "The main server received from <" << nowusername << "> to query course <" << nowcourseCode << "> about <" << nowcourseCate << "> using TCP over port <25283>." << endl;
            if(coursecode == "EE"){
                sendRequestToEE();
                receiveFromEE();
            }else if(coursecode == "CS"){
                sendRequestToCS();
                receiveFromCS();
            }
            sendResponseInfoToClient();

        }
    }
    }
    }
}

// int main(){
//     cout << "fuck" << &nowUsername << "you"<<&nowPasswords <<endl;

// }