#include <iostream>
#include <fstream>
#include <ctime>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "str_switch.h"

using namespace std;

#ifdef _WIN32
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#endif




string decode_cmd(string str)
{
    string mass;

    string split;
    int size_smd =0;


    for(int i=0; i < str.size();i++)
    {
        cout << "str[i] = " <<static_cast<unsigned char>(str[i])<<endl;

        switch (static_cast<unsigned char>(str[i]))
        {
        case 255:
            mass += "IAC ";
            break;
        case 254:
            mass += "DONT ";
            break;
        case 253:
            mass += "DO ";
            break;
        case 252:
            mass += "WONT ";
            break;
        case 251:
            mass +="WILL ";
            break;
        case 250:
            mass +="SB ";
            break;
        case 249:
            mass +="GO ";
            break;
        case 248:
            mass +="EL ";
            break;
        case 247:
            mass +="EC ";
            break;
        case 246:
            mass +="AYT ";
            break;
        case 245:
            mass +="AO ";
            break;
        case 244:
            mass +="IP ";
            break;
        case 243:
            mass +="BRK ";
            break;
        case 242:
            mass +="DATA ";
            break;
        case 241:
            mass +="NO ";
            break;
        case 240:
            mass +="SE ";
            break;



        default:
            mass += "ECHO ";
        }

    }

    return mass;
}

string cmd(string str)
{
    string mass;

    string split;
    int size_smd =0;
    for(int i=0; i < str.size();i++)
    {
        if(str[i] ==' ')
        {
            continue;

        }

        split += str[i];

        SWITCH(split)
        {
            CASE("IAC"):
                mass.push_back(0xff);
            split = "";
            break;
            CASE("DONT"):
                mass.push_back(0xfe);
            split = "";
            break;
            CASE("DO"):
                mass.push_back(0xfd);
            split = "";
            break;
            CASE("WONT"):
                mass.push_back(0xfc);
            split = "";
            break;
            CASE("WILL"):
                mass.push_back(0xfb);
            split = "";
            break;
            CASE("SB"):
                mass.push_back(0xfa);
            split = "";
            break;
            CASE("GA"):
                mass.push_back(0xf9);
            split = "";
            break;
            CASE("EL"):
                mass.push_back(0xf8);
            split = "";
            break;
            CASE("EC"):
                mass.push_back(0xf7);
            split = "";
            break;
            CASE("AYT"):
                mass.push_back(0xf6);
            split = "";
            break;
            CASE("AO"):
                mass.push_back(0xf5);
            split = "";
            break;
            CASE("IProcess"):
                mass.push_back(0xf4);
            split = "";
            break;
            CASE("Break"):
                mass.push_back(0xf3);
            split = "";
            break;
            CASE("DataMark"):
                mass.push_back(0xf2);
            split = "";
            break;
            CASE("NOP"):
                mass.push_back(0xf1);
            split = "";
            break;
            CASE("SE"):
                mass.push_back(0xf0);
            split = "";
            break;
            CASE("STATUS"):
                mass.push_back(2);
            split = "";
            break;
        }


    }

    return mass;
}


void read(SOCKET &server, char* buff,int sleep)
{
    memset(buff,'\0',20000);

    Sleep(sleep);

    auto bRecv = recv(server, buff, 20000, 0);
    if ((bRecv == SOCKET_ERROR) || (bRecv == 0)) {
        cout << "Disconnected from the server." << endl;
    }
}

int send(SOCKET &server,int &iR,string command)
{
    iR = send(server, command.c_str(), (u_int)command.length(), 0);
    if (iR == SOCKET_ERROR)
    {
        cout << "Couldn't send data." << endl;
        closesocket(server);
        return 0;
    }
}



bool telnet_connect(SOCKET &server,int &iR)
{
    string command;
    char buff[20000] = {0};

    cout << "Start telnet connect: 192.168.128.76" <<endl;
    //1
    command.clear();
    command.push_back(0xff);
    command.push_back(0xfb);
    command.push_back(0x18);
    command.push_back(0xff);
    command.push_back(0xfb);
    command.push_back(0x1f);

    send(server,iR,command);

    read(server,buff,300);

    //2
    command.clear();
    command.push_back(0xff);
    command.push_back(0xfc);
    command.push_back(0x20);
    command.push_back(0xff);
    command.push_back(0xfc);
    command.push_back(0x23);
    command.push_back(0xff);
    command.push_back(0xfb);
    command.push_back(0x27);
    command.push_back(0xff);
    command.push_back(0xfc);
    command.push_back(0x24);

    //cout <<    decode_cmd(command) <<endl;
    send(server,iR,command);

    read(server,buff,300);

    //3
    command.clear();
    command.push_back(0xff);
    command.push_back(0xfa);
    command.push_back(0x1f);
    command.push_back('\0');
    command.push_back(0x50);
    command.push_back('\0');
    command.push_back(0x19);
    command.push_back(0xff);
    command.push_back(0xf0);

    //cout <<    decode_cmd(command) <<endl;
    send(server,iR,command);

    command.clear();
    command.push_back(0xff);
    command.push_back(0xfa);
    command.push_back(0x27);
    command.push_back('\0');
    command.push_back(0xff);
    command.push_back(0xf0);
    command.push_back(0xff);
    command.push_back(0xfa);
    command.push_back(0x18);
    command.push_back('\0');
    command.push_back(0x41);
    command.push_back(0x4e);
    command.push_back(0x53);
    command.push_back(0x49);
    command.push_back(0xff);
    command.push_back(0xf0);

    //cout <<    decode_cmd(command) <<endl;
    send(server,iR,command);

    read(server,buff,300);

    //4
    command.clear();
    command.push_back(0xff);
    command.push_back(0xfd);
    command.push_back(0x03);

    //cout <<    decode_cmd(command) <<endl;
    send(server,iR,command);

    command.clear();
    command.push_back(0xff);
    command.push_back(0xfb);
    command.push_back(0x01);

    command.push_back(0xff);
    command.push_back(0xfe);
    command.push_back(0x05);

    command.push_back(0xff);
    command.push_back(0xfc);
    command.push_back(0x21);

    //cout <<    decode_cmd(command) <<endl;
    send(server,iR,command);

    read(server,buff,300);

    //5
    command.clear();
    command.push_back(0xff);
    command.push_back(0xfc);
    command.push_back(0x01);

    //cout <<    decode_cmd(command) <<endl;
    send(server,iR,command);

    command.clear();
    command.push_back(0xff);
    command.push_back(0xfd);
    command.push_back(0x01);

    //cout <<    decode_cmd(command) <<endl;
    send(server,iR,command);

    read(server,buff,300);

    //6
    command.clear();
    command.push_back(0xff);
    command.push_back(0xfc);
    command.push_back(0x01);

    //cout <<    decode_cmd(command) <<endl;
    send(server,iR,command);

    command.clear();
    command.push_back(0xff);
    command.push_back(0xfd);
    command.push_back(0x01);

    //cout <<    decode_cmd(command) <<endl;
    send(server,iR,command);

    string str_login(buff);


    if(str_login == "login: ")
    {
        return true;
    }

    return false;
}


int telnet(string user,string pass)
{
    WSADATA wsaData;
    auto iR = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iR != 0){
        return 1;
    }
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    addrinfo* result;
    int res = getaddrinfo("192.168.128.76", "23", &hints, &result); // 94.142.241.111

    if (res != 0)
    {
        std::cerr << "failed to getaddrinfo, error " << res << std::endl;
        WSACleanup();
        return 1;
    }



    SOCKET server = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (server == INVALID_SOCKET) {
        closesocket(server); return 1;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.128.76"); //192.168.128.76
    addr.sin_port = htons(23);


    //cout << "Username: " << user;
    //cout << endl;
    //cout << "Password: " << pass;
    //cout << endl;

    cout << "Connecting..." << endl;
    if ((connect(server, result->ai_addr,  (int)result->ai_addrlen)) == SOCKET_ERROR) {
        cout << "Couldn't connect."<< endl;
        closesocket(server);
        Sleep(2000);
        return 0;
    }


    freeaddrinfo(result);
    int bRecv = 0;
    char buff[20000] = {0};




    bRecv = recv(server, buff, sizeof(buff), 0);
    if ((bRecv == SOCKET_ERROR) || (bRecv == 0)) {
        cout << "Disconnected from the server." << endl;
        WSACleanup();
        return 0;
    }

    //cout <<"bRecv:" << bRecv << endl;
    //cout <<"Read:" << string(buff) << endl;

    /*
    string data = string(buff);
    string msg;
    for(int i=0;i < data.size();i++)
    {
        cout << "data = " << to_string(char(data[i]))<<endl;

        if(data[i] == -3 && data[i-1] == -1)
        {
            msg += data[i+1];
        }
    }
    */

    // cout << msg <<endl;
    //cout <<    decode_cmd(data) <<endl;

    string command;

    if(telnet_connect(server,iR)  == false)
    {
        cout << "Connet ERROR"<<endl;
        WSACleanup();
        closesocket(server);
        return -1;
    }


    ofstream out;

    time_t now;
    struct tm* timeinfo;

    time(&now);
    timeinfo = localtime(&now);


    char data[80];

    strftime(data,sizeof(data),"Temperatur_%d_%m_%Y___%H_%M_%S",timeinfo);

    out.open(string(data)+".csv");

    string FileName(data);


    //отправка логина после коннекта
    command.clear();
    command.append(user);
    command.append("\r\n");
    send(server,iR,command);

    read(server,buff,300);

    //отправка пароля после логина и коннекта
    command.clear();
    command.append(pass);
    command.append("\r\n");
    send(server,iR,command);

    read(server,buff,300);


    if(buff[2] =='#')
    {

        while (true) {

            string str_temp;
            string temp;


            command.clear();
            command.append("xadc-status");
            command.append("\r\n");
            send(server,iR,command);
            read(server,buff,1000);

            //
            time_t now;
            struct tm* timeinfo;

            time(&now);
            timeinfo = localtime(&now);


            char data[80] = {0};
            char timeColumn[80] = {0};

            strftime(data,sizeof(data),"%d.%m.%Y",timeinfo);
            strftime(timeColumn,sizeof(timeColumn),"%H:%M:%S",timeinfo);
            //


            for(int i=0;i<sizeof(buff);i++)
            {
                if(buff[i-1] == '\r' && buff[i] == '\0')
                {
                   buff[i] = '\r';
                }
            }


            str_temp = string(buff);

            string str;
            for(int i=0; i < str_temp.size();i++)
            {
                str +=str_temp[i];

                if(str_temp[i] == '\n')
                {
                    str = "";
                    continue;
                }

                SWITCH(str)
                {
                    CASE("temp     ="):
                    {
                        for(int j=i+1; j <str_temp.size();j++)
                        {
                            if(str_temp[j] == '(')
                            {
                                i = str_temp.size()+1;
                                break;
                            }

                            if(str_temp[j] == ' ')
                            {
                                continue;
                            }

                            temp += str_temp[j];
                        }
                        break;
                    }
                }
            }

            //Записать найденную темпиратуру в файл

            cout << string(timeColumn) << " : tem = " << temp<<endl;

            string row;

            row = string(data) + ";" + string(timeColumn) +";"+ temp +="\n";
            out << row.c_str();
            out.close();
            out.open(FileName+".csv",ios::app);
        }


    }


//    int counter = 1;

//    while (true) {
//        cout <<endl;
//        //    command.clear();
//        //    command.append("xadc-status");
//        //    command.append("\r\n");

//        Sleep(500);

//        bRecv = recv(server, buff, 20000, 0);
//        if ((bRecv == SOCKET_ERROR) || (bRecv == 0)) {
//            cout << "Disconnected from the server." << endl;
//            break;
//        }
//        //cout << bRecv <<" ) READ : "<< decode_cmd(buff) <<endl;


//        cout << bRecv <<" ) READ : "<< string(buff) <<endl;
//        memset(buff,'\0',sizeof (buff));
//        // Sleep(500);
//        // system("cls");
//        // send(server, command.c_str(), (u_int)command.length(), 0);


//        counter++;

//        switch (counter) {
//        case 1:
//            command.clear();
//            command.push_back(0xff);
//            command.push_back(0xfb);
//            command.push_back(0x18);
//            command.push_back(0xff);
//            command.push_back(0xfb);
//            command.push_back(0x1f);

//            cout <<    decode_cmd(command) <<endl;
//            send(server,iR,command);


//            break;
//        case 2:
//            command.clear();
//            command.push_back(0xff);
//            command.push_back(0xfc);
//            command.push_back(0x20);
//            command.push_back(0xff);
//            command.push_back(0xfc);
//            command.push_back(0x23);
//            command.push_back(0xff);
//            command.push_back(0xfb);
//            command.push_back(0x27);
//            command.push_back(0xff);
//            command.push_back(0xfc);
//            command.push_back(0x24);

//            cout <<    decode_cmd(command) <<endl;
//            send(server,iR,command);

//            break;
//        case 3:
//            command.clear();
//            command.push_back(0xff);
//            command.push_back(0xfa);
//            command.push_back(0x1f);
//            command.push_back('\0');
//            command.push_back(0x50);
//            command.push_back('\0');
//            command.push_back(0x19);
//            command.push_back(0xff);
//            command.push_back(0xf0);

//            cout <<    decode_cmd(command) <<endl;
//            send(server,iR,command);

//            command.clear();
//            command.push_back(0xff);
//            command.push_back(0xfa);
//            command.push_back(0x27);
//            command.push_back('\0');
//            command.push_back(0xff);
//            command.push_back(0xf0);
//            command.push_back(0xff);
//            command.push_back(0xfa);
//            command.push_back(0x18);
//            command.push_back('\0');
//            command.push_back(0x41);
//            command.push_back(0x4e);
//            command.push_back(0x53);
//            command.push_back(0x49);
//            command.push_back(0xff);
//            command.push_back(0xf0);

//            cout <<    decode_cmd(command) <<endl;
//            send(server,iR,command);
//            break;

//        case 4:
//            command.clear();
//            command.push_back(0xff);
//            command.push_back(0xfd);
//            command.push_back(0x03);

//            cout <<    decode_cmd(command) <<endl;
//            send(server,iR,command);

//            command.clear();
//            command.push_back(0xff);
//            command.push_back(0xfb);
//            command.push_back(0x01);

//            command.push_back(0xff);
//            command.push_back(0xfe);
//            command.push_back(0x05);

//            command.push_back(0xff);
//            command.push_back(0xfc);
//            command.push_back(0x21);

//            cout <<    decode_cmd(command) <<endl;
//            send(server,iR,command);
//            break;
//        case 5:
//            command.clear();
//            command.push_back(0xff);
//            command.push_back(0xfc);
//            command.push_back(0x01);

//            cout <<    decode_cmd(command) <<endl;
//            send(server,iR,command);

//            command.clear();
//            command.push_back(0xff);
//            command.push_back(0xfd);
//            command.push_back(0x01);

//            cout <<    decode_cmd(command) <<endl;
//            send(server,iR,command);
//            break;

//        case 6:
//            command.clear();
//            command.push_back(0xff);
//            command.push_back(0xfc);
//            command.push_back(0x01);

//            cout <<    decode_cmd(command) <<endl;
//            send(server,iR,command);

//            command.clear();
//            command.push_back(0xff);
//            command.push_back(0xfd);
//            command.push_back(0x01);

//            cout <<    decode_cmd(command) <<endl;
//            send(server,iR,command);

//            command.clear();
//            command.append(user);
//            command.append("\r\n");
//            send(server,iR,command);

//            break;

//            case 7:
//            command.clear();
//            command.append(pass);
//            command.append("\r\n");
//            send(server,iR,command);
//            break;

//        default:

//            command.clear();
//            command.append("xadc-status");
//            command.append("\r\n");
//            send(server,iR,command);
//            break;
//        }
 //   }

    return 0;
}





int main()
{
//    ofstream out;

//    time_t now;
//    struct tm* timeinfo;

//    time(&now);
//    timeinfo = localtime(&now);


//    char data[80];

//    strftime(data,sizeof(data),"Temperatur_%d_%m_%Y___%H_%M_%S",timeinfo);

//    out.open(string(data)+".csv");

    telnet("root","111");



    system("pause");
    return 0;
}
