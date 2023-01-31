#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iomanip>
#include <ctype.h>
#include <vector>
#include<bits/stdc++.h>

using namespace std;

#define LOCAL_HOST "127.0.0.1" // Host address
#define SERVER_PORT 25283 // TCP Port
#define BUFLEN 1024 // Length of socket stream buffer

struct sockaddr_in m_tcp;
int m_tcp_sockfd;

int client_portNum;

string userInfom, findCourse;

struct sockaddr_in client;
int client_sockfd;
// char userName[1000];
// char passWords[1000];
char userInfo[1000];
char auFlag[1025];
char selectFlag[1000];



char final_info[BUFLEN+1];


vector<string> Category;


int recvLen1;



void setCategory(){
    string temp1 = "Credit";
    string temp2 = "Professor";
    string temp3 = "Days";
    string temp4 = "CourseName";
    Category.push_back(temp1);
    Category.push_back(temp2);
    Category.push_back(temp3);
    Category.push_back(temp4);
}

void initial_TCP(){
    //creat socket//
    m_tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //connect socket//
    m_tcp.sin_family = AF_INET;

    m_tcp.sin_port = htons(SERVER_PORT);

    m_tcp.sin_addr.s_addr = inet_addr(LOCAL_HOST);

    if(connect(m_tcp_sockfd, (struct sockaddr *)&m_tcp, sizeof(m_tcp)) == -1){
        perror("Error connection to server");
        exit(EXIT_FAILURE);
    }
}

void combineTwoStr(string input1, string input2){
    userInfom = input1 + ',' + input2;
}

void sendUserinToM(){
    cout << "runSendM" << endl;
    strcpy(userInfo, userInfom.c_str());
    if(send(m_tcp_sockfd, userInfo, sizeof(userInfo), 0) == -1){
        perror("Error sending data to server socket");
        close(client_sockfd);
        exit(EXIT_FAILURE);
    }
    //get dynamic client port number
    socklen_t len = sizeof(client);
    if (getsockname(m_tcp_sockfd, (struct sockaddr *)&client, &len) == -1){
        perror("Error getting client port number");
        close(client_sockfd);
        exit(EXIT_FAILURE);
    }
    else{
        client_portNum = ntohs(client.sin_port);
    }
}

// void sendPasswordsToM(string input){
//     cout << "ERRor" << endl;
//     passwords = input;
//     strcpy(passWords, passwords.c_str());
//     if(send(m_tcp_sockfd, passWords, sizeof(passWords), 0) == -1){
//         perror("Error sending data to server socket");
//         close(client_sockfd);
//         exit(EXIT_FAILURE);
//     }
//     //get dynamic client port number
//     int client_portNum;
//     socklen_t len = sizeof(client);
//     if (getsockname(m_tcp_sockfd, (struct sockaddr *)&client, &len) == -1){
//         perror("Error getting client port number");
//         close(client_sockfd);
//         exit(EXIT_FAILURE);
//     }
//     else{
//         client_portNum = ntohs(client.sin_port);
//     }
// }

void recvAuflagFromM(){
    if ( (recvLen1 = recv(m_tcp_sockfd, auFlag, 1025, 0)) == -1){
        perror("Error receiving message from serverM");
        close(client_sockfd);
        exit(EXIT_FAILURE);
    }
    auFlag[recvLen1] = '\0';
}

void sendCourseCodeAndCategory(){
    memset(selectFlag, 0, sizeof(selectFlag));
    strcpy(selectFlag, userInfom.c_str());
    if(send(m_tcp_sockfd, selectFlag, sizeof(selectFlag), 0) == -1){
        perror("Error sending data to server socket");
        close(client_sockfd);
        exit(EXIT_FAILURE);
    }

    //get dynamic client port number
    socklen_t len = sizeof(client);
    if (getsockname(m_tcp_sockfd, (struct sockaddr *)&client, &len) == -1){
        perror("Error getting client port number");
        close(client_sockfd);
        exit(EXIT_FAILURE);
    }
    else{
        client_portNum = ntohs(client.sin_port);
    }
}

// void recvCourseflagAndInfo(){
//     if ( (recvLen1 = recv(m_tcp_sockfd, authCourse, BUFLEN, 0)) == -1){
//         perror("Error receiving message from serverM");
//         close(client_sockfd);
//         exit(EXIT_FAILURE);
//     }
//     authCourse[recvLen1] = '\0';
// }

// void sendCategory(string input){
//     strcpy(Des_category, input.c_str());
//     if(send(m_tcp_sockfd, Des_category, 1024, 0) == -1){
//         perror("Error sending data to server socket");
//         close(client_sockfd);
//         exit(EXIT_FAILURE);
//     }

//     //get dynamic client port number
//     int client_portNum;
//     socklen_t len = sizeof(client);
//     if (getsockname(m_tcp_sockfd, (struct sockaddr *)&client, &len) == -1){
//         perror("Error getting client port number");
//         close(client_sockfd);
//         exit(EXIT_FAILURE);
//     }
//     else{
//         client_portNum = ntohs(client.sin_port);
//     }
// }

// void sendRequestToM(string input){
//     strcpy(requestInfo, input.c_str());
//     if(send(m_tcp_sockfd, requestInfo, sizeof(Des_category), 0) == -1){
//         perror("Error sending data to server socket");
//         close(client_sockfd);
//         exit(EXIT_FAILURE);
//     }

//     //get dynamic client port number
//     int client_portNum;
//     socklen_t len = sizeof(client);
//     if (getsockname(m_tcp_sockfd, (struct sockaddr *)&client, &len) == -1){
//         perror("Error getting client port number");
//         close(client_sockfd);
//         exit(EXIT_FAILURE);
//     }
//     else{
//         client_portNum = ntohs(client.sin_port);
//     }
// }

void recvResponseInfo(){
    memset(final_info, 0, sizeof(final_info));
    if ( (recvLen1 = recv(m_tcp_sockfd, final_info, BUFLEN+1, 0)) == -1){
        perror("Error receiving message from serverM");
        close(client_sockfd);
        exit(EXIT_FAILURE);
    }
    final_info[recvLen1] = '\0';
}

bool check_username(string username){
    if(username.size() < 5 || username.size() > 50){
        return false;
    }
    else{
        for(int i = 0; i < username.size(); i++){
            if(isupper(username[i])){
                return false;
            }
            else if(!isalpha(username[i])){
                return false;
            }
        }
    }
    return true;
}

bool check_passwords(string passwords){
    if(passwords.size() < 5 || passwords.size() > 50){
        return false;
    }
    else{
        return true;
    }
}

bool check_courcode(string input){
    if(input.size() != 5){
        cout << "You must enter more than 5 words" << endl;
        return false;
    }
    else{
        if((input[0] == 'E' && input[1] == 'E') || (input[0] == 'C' && input[1] == 'S')){
            // coursecode = input;
            return true;
        }else{
            cout << "You must enter EE or CS" << endl;
            return false;
        }
    }
}


bool check_categoryvalidation(string input){
    // int now = 0;
    // string temp;
    // int size = 0;
    // std::vector<string>::iterator it;
    // while(!input.empty()){
    //     auto pos = input.find(",");
    //     if(pos != string::npos){
    //         size = input.size() - pos - 1;
    //         temp = input.substr(0, size);
    //         input = input.substr(pos+1);
    //         it = std::find(Category.begin(), Category.end(), temp);
    //         if(it == Category.end()){
    //             return false;
    //         }
    //     }else{
    //         it = std::find(Category.begin(), Category.end(), input);
    //         if(it == Category.end()){
    //             return false;
    //         }
    //         break;
    //     }
    // }
    // return true;
    std::vector<string>::iterator it;
    it = std::find(Category.begin(), Category.end(), input);
    if(it == Category.end()){
        cout << "You must enter the same type of the remind" << endl;
        return false;
    }else{
        // category = input;
        return true;
    }
}

// void show(RequestInfo &requestInfo){
//     if(!requestInfo.Coursename.empty()){
//         cout << "The <" + category + "> of <"+ coursecode + "> is <" + requestInfo.Coursename + ">." << endl;
//     }else if(!requestInfo.Credits.empty()){
//         cout << "The <" + category + "> of <"+ coursecode + "> is <" + requestInfo.Credits + ">." << endl;
//     }else if(!requestInfo.Days.empty()){
//         cout << "The <" + category + "> of <"+ coursecode + "> is <" + requestInfo.Days + ">." << endl;
//     }else if(!requestInfo.Professor.empty()){
//         cout << "The <" + category + "> of <"+ coursecode + "> is <" + requestInfo.Professor + ">." << endl;
//     }
// }

int main(){
    initial_TCP();
    cout << "The client is up and running."<<endl;
    setCategory();
    int count = 3;
    string nowusername;
    string nowpasswords;
    string nowcoursecode;
    string nowcategory;
    while(count){
        cout << "Please enter the username: ";
        cin >> nowusername;
        cout << "Please enter the passwords: ";
        cin >> nowpasswords;
        if(!(check_username(nowusername) && check_passwords(nowpasswords))){
            cout << "Username or Passwords can not empty or unvalid.(username must be all alphabet, the length must longer than 5 and less than 50; passwords can be anything except ','. The length of passwords should larger than 5, less than 50.)" << endl;
            continue;
        }
        combineTwoStr(nowusername, nowpasswords);
        cout << "<" << nowusername << "> sent an authentication request to the main server." << endl;
        sendUserinToM();
        recvAuflagFromM();
        if(auFlag[0] == '0'){
            cout << "<"<<nowusername << "> received the result of authentication using TCP over port <"<<client_portNum << ">. Authentication failed: Username does not match" << endl;
            count--;
            cout << "Attempts remaining:<"<< count << ">." << endl;
            memset(userInfo, 0, sizeof(userInfo));
            memset(auFlag, 0, sizeof(auFlag));
            continue;
        }else if(auFlag[0] == '1'){
            cout << "<"<<nowusername << "> received the result of authentication using TCP over port <"<<client_portNum << ">. Authentication failed: Password does not match" << endl;
            count--;
            cout << "Attempts remaining:<"<< count << ">." << endl;
            memset(userInfo, 0, sizeof(userInfo));
            // memset(passWords, 0, sizeof(passWords));
            memset(auFlag, 0, sizeof(auFlag));
            continue;
        }else if(auFlag[0] == '2'){
            cout << "<"<<nowusername << "> received the result of authentication using TCP over port <"<<client_portNum << ">. Authentication is successful" << endl;
            break;
        }
    }
    if(count == 0){
        cout << "Authentication Failed for 3 attempts. Client will shut down." << endl;
        close(client_sockfd);
    }else{
        while(1){
            cout << "Please enter the course code to query: ";
            cin >> nowcoursecode;
            if(!check_courcode(nowcoursecode)){
                continue;
            }else{
                cout << "Please enter the category (Credit / Professor / Days / CourseName): ";
                cin >> nowcategory;
                if(check_categoryvalidation(nowcategory)){
                    combineTwoStr(nowcoursecode, nowcategory);
                    sendCourseCodeAndCategory();
                    cout << "<" << nowusername <<"> sent a request to the main server." << endl;
                    recvResponseInfo();
                    if(final_info[0] != '2'){
                        cout << "The client received the response from the Main server using TCP over port <" << client_portNum << ">." << endl;
                        cout << "The <"<<nowcategory<<"> of <" << nowcoursecode << "> is <" << final_info << ">." << endl;
                        cout << ' ' << endl;
                        cout << "-----Start a new request-----" << endl;                        
                    }else{
                        cout << "Didn’t find the course: <" << nowcoursecode << ">." << endl;
                        cout << ' ' << endl;
                        cout << "-----Start a new request-----" << endl; 
                    }

                }
            }
        }
    }
}
    