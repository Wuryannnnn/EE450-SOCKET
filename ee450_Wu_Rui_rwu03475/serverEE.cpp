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
#define ServerEE_PORT 23283
#define ServerM_PORT 24283
#define BUFLEN 1024 // Length of socket stream buffer

int recvLen1;
int findindex;
int nowpos;


// char category[BUFLEN];

vector<string> course_info;
vector<vector<string>> res_vectorOfCategory;



string recvCatergory, recvCourseCode;



string recvRequest;
char Request[1025];
char Response[1000];
char errorInfo[1000];



struct sockaddr_in m_udp, serverEE;
int serverEE_sockfd;



void setM(){
    m_udp.sin_family = AF_INET;
    //M Port #
    m_udp.sin_port = htons(ServerM_PORT);
    //M IP ADDR - INADDR_LOOPBACK refers to localhost ("127.0.0.1")
    m_udp.sin_addr.s_addr = inet_addr(LOCAL_HOST);
}

void init_UDP(){
    // *** 1. CREATE SOCKET ***
    if ( (serverEE_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
        perror("Error creating UDP socket");
        exit(EXIT_FAILURE);
    }

    serverEE.sin_family = AF_INET;
    
    serverEE.sin_port = htons(ServerEE_PORT);

    serverEE.sin_addr.s_addr = inet_addr(LOCAL_HOST);

    if (::bind(serverEE_sockfd, (struct sockaddr *) &serverEE, sizeof(serverEE)) == -1 ){
        close(serverEE_sockfd);
        perror("Error binding UDP socket");
        exit(EXIT_FAILURE);
    }
}


void constrcutCourseinfo(){
    string line;
    const char* fileNamePath = "./ee.txt";
    ifstream infile;
    infile.open(fileNamePath);
    if(infile.fail()){
        cout << "file open fails or not exist" << endl;
    }else{
        while(getline(infile, line)){
            course_info.push_back(line);
        }
    }
    infile.close();
        
}

void splitinfo(){
    string temp;
    vector<string> temp1;
    int pos;
    for(int i = 0; i < course_info.size(); i++){
        for(int j = 0; j < course_info[i].size(); j++){
            pos = course_info[i].find(',');
            temp = course_info[i].substr(0, pos);
            course_info[i] = course_info[i].substr(pos+1);
            temp1.push_back(temp);
        }
        temp1.push_back(course_info[i]);
        res_vectorOfCategory.push_back(temp1);
        temp1.clear();
    }
}

void recvRequestFromM(){
    memset(Request, 0, sizeof(Request));
    socklen_t m_len = sizeof(m_udp);
    string res;
    if((recvLen1 = recvfrom(serverEE_sockfd, Request, sizeof(Request), 0, (struct sockaddr *)&m_udp, &m_len)) < 1){
        perror("Error receiving from serverM");
        exit(EXIT_FAILURE);
    }
    Request[recvLen1] = '\0';
    res = string(Request);
    int pos;
    for(int i = 0; i < res.size(); i++){
        pos = res.find(',');
        recvCourseCode = res.substr(0, pos);
        recvCatergory = res.substr(pos+1);
    }
    if(recvCatergory == "Credit"){
        findindex = 1;
    }else if(recvCatergory == "Professor"){
        findindex = 2;
    }else if(recvCatergory == "Days"){
        findindex = 3;
    }else if(recvCatergory == "CourseName"){
        findindex = 4;
    }else{
        findindex = 5;
    }
}

void getInfo(){
    string temp;
    if(findindex != 5){
        temp =  res_vectorOfCategory[nowpos][findindex];
        strcpy(Response, temp.c_str());
        cout << "The course information has been found: The <" << recvCatergory << "> of <" << recvCourseCode << "> is <" << Response << ">." <<endl;
    }else{
        cout << "Wrong Category" << endl;
    }
}

bool checkCategory(){
    for(int i = 0; i < res_vectorOfCategory.size(); i++){
        if(recvCourseCode == res_vectorOfCategory[i][0]){
            nowpos = i;
            return true;
        }
    }
    return false;
}

// void recvRequestFromM(){
//     socklen_t m_len = sizeof(m_udp);

//     if((recvLen1 = recvfrom(serverCS_sockfd, Request, BUFLEN, 0, (struct sockaddr *)&m_udp, &m_len)) < 1){
//         perror("Error receiving from serverM");
//         exit(EXIT_FAILURE);
//     }
//     Request[recvLen1] = '\0';
//     if(Request[0] != '\0'){
//         recvRequest = string(Request);
//     }
// }

void sendValidToM(){
    int sendLen;
    if((sendLen = sendto(serverEE_sockfd, Response, sizeof(Response), 0, (struct sockaddr *)&m_udp, sizeof(struct sockaddr_in))) == -1){
        perror("Error sending UDP response to ServerM from ServerCS");
        exit(EXIT_FAILURE);
    }
    memset(Response, 0, sizeof(Response));
}



void sendUnValidToM(){
    int temp = 2;
    int sendLen;
    sprintf(errorInfo, "%d", temp);
    if((sendLen = sendto(serverEE_sockfd, errorInfo, sizeof(errorInfo), 0, (struct sockaddr *)&m_udp, sizeof(struct sockaddr_in))) == -1){
        perror("Error sending UDP response to ServerM from ServerCS");
        exit(EXIT_FAILURE);
    }
    memset(errorInfo, 0, sizeof(errorInfo));
}

int main(){
    init_UDP();
    setM();
    cout << "The ServerEE is up and running using UDP on port "<< ServerEE_PORT << endl;
    constrcutCourseinfo();
    splitinfo();
    while(1){
        recvRequestFromM();
        cout << "The Server<EE> received a request from the Main Server about the <" << recvCatergory << "> of <" << recvCourseCode << ">." << endl;
        if(checkCategory()){
            getInfo();
            sendValidToM();
        }else{
            cout << "Didn’t find the course: <" << recvCourseCode << ">." << endl;
            sendUnValidToM();
            cout << "The Server<EE> finished sending the response to the Main Server." << endl;
        }
    }
}