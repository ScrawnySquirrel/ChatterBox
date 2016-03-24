#include "clientsrc.h"
#include <QDebug>

int sockNum = -1;
Client *parentx;
MainWindow *mw;

clientSrc::clientSrc(QWidget *parent, QWidget *grandparent)//std::string ipAddr, int portNum = DEFAULT_PORT)
{
    //host = ipAddr.c_str();
    //strcpy(host, ipAddr);
    //port = portNum;
    //sockNum = 0;
    parentx = qobject_cast<Client*>(parent);
    mw = qobject_cast<MainWindow*>(grandparent);
}



void *readMsg(void *sock){
    int sockNum = *((int*)sock);
    int n, bytes_to_read;
    MsgStr *MsgRcv, rcvform;
    MsgRcv = &rcvform;



    qDebug("RcvThread");
    while(1){
        n = 0;
        bytes_to_read = sizeof(MsgStr);


        while ((n = recv (sockNum, (MsgStr *)MsgRcv, bytes_to_read, 0)) < bytes_to_read)
        {
            MsgRcv += n;
            bytes_to_read -= n;

        }
        qDebug()<<"THE RECEIVE INFO! " << MsgRcv->type << ", " << MsgRcv->name << ", ";
        switch(MsgRcv->type){

            case MSG_NAME:
                qDebug("change name\n");
                break;
            case MSG_QUIT:
                qDebug("sb quit");
                break;
            case MSG_CONN:
                qDebug("%s enter the chat room\n", MsgRcv->name);
                std::stringstream temp;
                temp << MsgRcv->name <<" is connected";
                mw->set_status("CONN");
                break;
            case MSG_MESG:
                QString s(MsgRcv->msgTxt);
                parentx->print_msg(s);
                //qDebug ("[%s]  %s\n", MsgRcv->name, MsgRcv->msgTxt);
                break;

        }

    }
    return (0);
}


void clientSrc::clientStart()
{
 //   int sd = 0;
    struct hostent	*hp;
    struct sockaddr_in server;
  pthread_t p_thread;

    // Create the socket
    if ((sockNum = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Cannot create socket");
        return;
    }
//	memset(&recvMsg, 0, sizeof(struct messageStr));
    bzero((char *)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if ((hp = gethostbyname(host.c_str())) == NULL)
    {
        qWarning("Unknown server address\n");
        return;
    }
    bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

    // Connecting to the server
    if (connect (sockNum, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        qDebug("Can't connect to server\n");
        perror("connect");
        return;
    }

    mw->set_status("Connected!");
    sendPersonalInfo();
    recvList();


    pthread_create(&p_thread, NULL, readMsg, (void *)&sockNum);
/*    if (thr_id < 0)
    {
        qWarning("thread create error : ");
        return;
    }
    */
        qDebug("\t - read thread created...\n");

    //close(sockNum);
}

void clientSrc::recvList(){

  Clients *listRcv = list;
  int n = 0, bytes_to_read;
  bytes_to_read = sizeof(list);

    while ((n = recv (sockNum, (MsgStr *)listRcv, bytes_to_read, 0)) < bytes_to_read)
    {
      listRcv += n;
      bytes_to_read -= n;
    }
    //displayList();

}



void clientSrc::sendPersonalInfo(){

    MsgStr *myInfo, infoTxt;
    myInfo = &infoTxt;
    //createMsg(myInfo, MSG_CONN, nickname.c_str());

    myInfo->type = MSG_CONN;
    strcpy(myInfo->name, nickname.c_str());
    strcpy(myInfo->msgTxt, "");
    send (sockNum, (void *)myInfo, sizeof(MsgStr) ,0);

}

void clientSrc::createMsg(MsgStr *mesgForm, int typeNum, const char *message){
    mesgForm->type = typeNum;
    strcpy(mesgForm->name, message);
    strcpy(mesgForm->msgTxt, message);

}
void clientSrc::writeMsg(std::string message){

    qDebug()<<"WRITE socket number : "<<sockNum<<"\n";
    MsgStr *msgPtr, msgTxt;
    msgPtr = &msgTxt;
    createMsg(msgPtr, MSG_MESG, message.c_str());
    send(sockNum, (void *)msgPtr, sizeof(MsgStr), 0);


}



