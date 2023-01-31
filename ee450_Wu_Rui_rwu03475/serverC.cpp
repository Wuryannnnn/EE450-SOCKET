#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <map>
#include <climits>
#include <sstream>

using namespace std;
#define LOCAL_HOST "127.0.0.1" // Host address
#define ServerC_PORT 21283
#define ServerM_PORT 24283
#define BUFLEN 1024 // Length of socket stream buffer



int recvLen1;
string username, passwords;

struct sockaddr_in m_udp, serverC;
int serverC_sockfd, flag;

vector<string> cred_info;
map<string, string> rescred;
map<string, string>::iterator it;

void setM(){
    m_udp.sin_family = AF_INET;
    //M Port #
    m_udp.sin_port = htons(ServerM_PORT);
    //M IP ADDR - INADDR_LOOPBACK refers to localhost ("127.0.0.1")
    m_udp.sin_addr.s_addr = inet_addr(LOCAL_HOST);
}

void init_UDP(){
    // *** 1. CREATE SOCKET ***
    if ( (serverC_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
        perror("Error creating UDP socket");
        exit(EXIT_FAILURE);
    }

    // specify ServerC address

    serverC.sin_family = AF_INET;
    //AWS Port #
    serverC.sin_port = htons(ServerC_PORT);
    //AWS IP ADDR - INADDR_LOOPBACK refers to localhost ("127.0.0.1")
    serverC.sin_addr.s_addr = inet_addr(LOCAL_HOST);

    // *** 2. BIND SOCKET ***

    if (::bind(serverC_sockfd, (struct sockaddr *) &serverC, sizeof(serverC)) == -1 ){
        close(serverC_sockfd);
        perror("Error binding UDP socket");
        exit(EXIT_FAILURE);
    }
}

void recvFromM(){
    char encryUsername[BUFLEN];
    char encryUserPass[BUFLEN];
    memset(encryUsername, 0 , sizeof(encryUsername));
    // cout << "run" << endl;
    socklen_t m_len = sizeof(m_udp);
    if ((recvLen1 = recvfrom(serverC_sockfd, encryUsername, sizeof(encryUsername), 0, (struct sockaddr *) &m_udp, &m_len)) < 1){
        perror("Error receiving from ServerM");
        exit(EXIT_FAILURE);
    }
    encryUsername[recvLen1] = '\0';
    username = string(encryUsername);

    memset(encryUserPass, 0 , sizeof(encryUserPass));
    if ((recvLen1 = recvfrom(serverC_sockfd, encryUserPass, sizeof(encryUserPass), 0, (struct sockaddr *) &m_udp, &m_len)) < 1){
        perror("Error receiving from ServerM");
        exit(EXIT_FAILURE);
    }
    encryUserPass[recvLen1] = '\0';
    passwords = string(encryUserPass);
    cout << "The ServerC received an authentication request from the Main Server." << endl;
}

void constrcutUserinfo(){
    string line;
    ifstream transactionFile("cred.txt",ios::in);
    if(transactionFile.is_open()){
        while(!transactionFile.eof() and getline(transactionFile, line)){
            string userInfo;
            stringstream ss;
            ss << line;
            ss >> userInfo;
            cred_info.push_back(userInfo);
        }
    }
    transactionFile.close();
}

void splitUserinfo(){
    string nowusername;
    string nowpasswords;
    int pos;
    for(int i = 0; i < cred_info.size(); i++){
        pos = cred_info[i].find(',');
        nowusername = cred_info[i].substr(0, pos);
        nowpasswords = cred_info[i].substr(pos+1);
        rescred.insert({nowusername, nowpasswords});
    }
}

int checkValid(){
    for(it = rescred.begin(); it != rescred.end(); ++it){
        if(username == it -> first){
            if(passwords == it -> second){
                flag = 2;
                return flag;
            }else{
                flag = 1;
                return flag;
            }
        }else{
            flag = 0;
        }
    }
    return flag;
}

void sendAuflagToM(){
    int sendLen;
    checkValid();
    char nowflag[1000];
    // memset(nowflag, 0, sizeof(flag));
    // memcpy(nowflag, &flag, sizeof(flag));
    sprintf(nowflag, "%d", flag);
    if((sendLen = sendto(serverC_sockfd, nowflag, 1000, 0, (struct sockaddr *)&m_udp, sizeof(struct sockaddr_in))) == -1){
        perror("Error sending UDP message to serverM from serverC");
        exit(EXIT_FAILURE);
    }
    memset(nowflag, 0, sizeof(nowflag));
    cout << "The ServerC finished sending the response to the Main Server." << endl;
}


int main(){
    setM();
    init_UDP();
    cout << "The ServerC is up and running using UDP on port " << ServerC_PORT << endl;
    constrcutUserinfo();
    splitUserinfo();
    while(1){
        recvFromM();
        sendAuflagToM();
    }
}
    // constrcutUserinfo();
    // splitUserinfo();
    // username = "neqiw";
    // passwords = "6oErwe1w)";
    // int now = checkValid();
    // cout << now << endl;
