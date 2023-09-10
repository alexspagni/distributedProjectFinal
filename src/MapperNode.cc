/*
 * mapperNode.cc
 *
 *  Created on: 9 apr 2023
 *      Author: spagn
 */

#include <string.h>
#include <omnetpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> // Per la funzione malloc
#include <time.h>
#include "OperationMessage_m.h"
#include "ackMessage_m.h"

#include "backOnline_m.h"
#include "shuffle_m.h"
using namespace omnetpp;

class MapperNode : public cSimpleModule
{
private:
    simtime_t timeout;
    cMessage *timeoutEvent;
    cMessage *backOnlineMessage;
    int n=0;
    OperationMessage *msgArrived;
    ShuffleMessage *shuffleMessage;
    int probabilityOfFailure;
    OperationMessage *messageSaved=nullptr;
 int numOfMessages;
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void createPartitions(char absolutePath[100]);

    virtual void mapOperation(OperationMessage *msg, const char *operationToPerform, int modifierValue);
    void writeOnFile(int *arrayKey, int *arrayValue, OperationMessage *msg);
    void ackMessageFunction(OperationMessage *msg);
    void changeKeyOperation(OperationMessage *msg, const char *operationToPerform, int modifierValue);
    void generateTimeout();
    void finish();
};

// The module class needs to be registered with OMNeT++
Define_Module(MapperNode);

void MapperNode::initialize()
{

    numOfMessages=0;
    WATCH(numOfMessages);
    timeout = 10.0;
    timeoutEvent = new cMessage("timeoutEvent");
    backOnlineMessage = new cMessage("backOnline");
    probabilityOfFailure=par("probabilityOfFailure");

}


void MapperNode::finish()
{

    EV << "Message sent or received by mapper "<<getIndex()<<" is " << numOfMessages << endl;
recordScalar("#sent", numOfMessages);


}
void MapperNode::handleMessage(cMessage *msg)
{

    if (msg == timeoutEvent)
    {
        EV<<"I received a timeout message and i'm node "<<getIndex() <<"\n";
        cancelEvent(timeoutEvent);
        int random = rand() % 100;

        // il nodo decide se fallire o meno
        if (random >probabilityOfFailure )
        {
            numOfMessages+=1;


            EV << "I'm able to perform the required, the number generated is "<<random <<"\n";
            if(messageSaved!=nullptr){
                char buffer[11];
                EV <<"Operation to perform: " << messageSaved->getOperationToDo() << " ,partition to read: " << messageSaved->getPartitionToRead() << ",node to be sent: " << messageSaved->getNodeToSend() << "\n";
                const char *messageComplete = messageSaved->getOperationToDo();
                strncpy(buffer, messageComplete, sizeof(buffer) - 1);

                const char *operationType = strtok(buffer, " ");
                const char *operationToPerform = strtok(NULL, " ");
                int modifierValue = strtol(strtok(NULL, " "), NULL, 10);
               // EV<<modifierValue<<"\n";
                if (opp_strcmp(operationType, "map") == 0)
                {
                    mapOperation(messageSaved, operationToPerform, modifierValue);
                }

                else
                {
                    changeKeyOperation(messageSaved, operationToPerform, modifierValue);
                }
            }
            else{
                EV<<"no message arrived"<<"\n";
            }

        }
        else {// nel caso in cui il nodo fallisca deve generare un messaggio di back online message in modo da poter ritornare "alive"
            EV <<" I'm mapper: " << getIndex()<<" and i failed "<<"\n";



            timeout = 15.0;
            scheduleAt(simTime() + timeout, backOnlineMessage);


        }
    }
    else{
       // EV<<"sono qui\n";
        // controllo se il messaggio arrivato è di tipo operation message
        numOfMessages+=1;
        msgArrived = dynamic_cast<OperationMessage *>(msg);
        if (msgArrived!=nullptr){
            EV<<"I received a message of type OperationMessage"<<"\n";
            messageSaved=check_and_cast<OperationMessage *>(msg);
            EV << "I'm node " << getIndex() << " ,operation:" << messageSaved->getOperationToDo() << " ,partition:" << messageSaved->getPartitionToRead() << " ,node to be sent: " << messageSaved->getNodeToSend() << "\n";

        // cancelEvent(timeoutEvent);
           timeout = 1.0;
           scheduleAt(simTime() + timeout, timeoutEvent);
        }

        else {
            // il nodo è tornato online e lo dice al coordinatore
            EV<<"I'm mapper: "<<getIndex()<<" and i received a message of type Back Online Message"<<"\n";
            backOnline *backOnlineMessage= new backOnline("backOnlineMessage");
            backOnlineMessage->setNodeNumber(getIndex());
            send(backOnlineMessage,"out");
            numOfMessages+=1;
        }

    }


}

void MapperNode::writeOnFile(int *arrayKey, int *arrayValue, OperationMessage *msg)
{
    FILE *filePointerWriting;
    filePointerWriting = fopen(msg->getPartitionToRead(), "w");
    int j = 0;
    while (arrayKey[j] != -1)
    {
        j++;
    }
    for (int i = 0; i < j; i++)
    {
        char keyValuePair[20];
        sprintf(keyValuePair, "%d %d", arrayKey[i], arrayValue[i]);
        fprintf(filePointerWriting, "%s\n", keyValuePair);
    }
    fclose(filePointerWriting);
}
// segnalo al coordinatore che ho svolto il mio lavoro sulla partizione
void MapperNode::ackMessageFunction(OperationMessage *msg)
{
    ackMessage *msgToSend = new ackMessage("MsgAcknoledgement");
    msgToSend->setNodeNumber(getIndex());
    msgToSend->setPerformed(true);
    msgToSend -> setPartition(msg ->getPartitionToRead());
    EV << msgToSend->getNodeNumber() <<" "<< msgToSend->getPerformed() << " "<< msgToSend->getPartition();
    numOfMessages++;
    send(msgToSend, "out");
}

void MapperNode::mapOperation(OperationMessage *msg, const char *operationToPerform, int modifierValue)
{
    int maxSize=100;
    int currentSize=0;
    FILE *filePointer;
    const int bufferLength = 255, numberKeyValue = 2;
    char buffer[bufferLength];
    int *arrayKey=(int *)malloc(maxSize*sizeof(int));
   //int arrayKey[100] = {[0 ... 99] = -1};
    int *arrayValue=(int *)malloc(maxSize*sizeof(int));
    //EV<<modifierValue<<"\n";
    for (int i=0;i<maxSize;i++){
        arrayKey[i]=-1;
        arrayValue[i]=-1;
    }
    filePointer = fopen(msg->getPartitionToRead(), "r+");
    int indexKey = 0, indexValue = 0;
    while (fgets(buffer, bufferLength, filePointer))
    {

        char *token = strtok(buffer, " ");
        // loop through the string to extract all other tokens
        int i = 0;
        if (maxSize==currentSize){
            maxSize=maxSize*2;
            arrayKey=(int *)realloc(arrayKey, maxSize*sizeof(int));
            arrayValue=(int *)realloc(arrayValue, maxSize*sizeof(int));
        }
        while (i < numberKeyValue)
        {

            if (i == 0)
            {
                arrayKey[indexKey] = strtol(token, NULL, 10);
                EV << "Key " << arrayKey[indexKey] << " ";
                indexKey++;
            }
            if (i == 1)
            {
                int value = strtol(token, NULL, 10);
                EV << "Old value " << value <<" ";
                if (strcmp(operationToPerform, "add") == 0)
                {
                    EV<<", ADD operation performed "<<" ";
                    value = value + modifierValue;
                }
                else if (strcmp(operationToPerform, "mul") == 0)
                {
                    EV<<", MUL operation performed "<<" ";
                    value = value * modifierValue;
                }
                else if (strcmp(operationToPerform, "sub") == 0)
                {
                    EV<<", SUB operation performed "<<" ";
                    value = value - modifierValue;
                }

                arrayValue[indexValue] = value;

                EV << ", New value " << arrayValue[indexValue] << "\n";

                indexValue++;
            }

            token = strtok(NULL, " ");

            i++;
        }
        currentSize++;
    }
    fclose(filePointer);

    writeOnFile(arrayKey, arrayValue, msg);

    ackMessageFunction(msg);
}

void MapperNode::changeKeyOperation(OperationMessage *msg, const char *operationToPerform, int modifierKey)
{

    int maxSize=100;
    int currentSize=0;
    FILE *filePointer;
    const int bufferLength = 255, numberKeyValue = 2;
    char buffer[bufferLength];
    int *arrayKey=(int *)malloc(maxSize*sizeof(int));
   //int arrayKey[100] = {[0 ... 99] = -1};
    int *arrayValue=(int *)malloc(maxSize*sizeof(int));
    for (int i=0;i<maxSize;i++){
           arrayKey[i]=-1;
           arrayValue[i]=-1;
       }
    filePointer = fopen(msg->getPartitionToRead(), "r+");
    int indexKey = 0, indexValue = 0;
    while (fgets(buffer, bufferLength, filePointer))
    {

        char *token = strtok(buffer, " ");

        int i = 0;
        if (maxSize==currentSize){
            maxSize=maxSize*2;
            arrayKey=(int *)realloc(arrayKey, maxSize*sizeof(int));
            arrayValue=(int *)realloc(arrayValue, maxSize*sizeof(int));
        }
        while (i < numberKeyValue)
        {
            if (i == 0)
            {
                int newKey;
                int key = strtol(token, NULL, 10);
                EV << " Old key " << key << " ";
                if (strcmp(operationToPerform, "add") == 0)
                {
                    EV<<", ADD operation performed "<<" ";
                            newKey = key + modifierKey;
                }
                else if (strcmp(operationToPerform, "mul") == 0)
                {
                    EV<<", MUL operation performed"<<" ";
                            newKey = key * modifierKey;
                }
                else if (strcmp(operationToPerform, "sub") == 0)
                {

                    EV<<", SUB operation performed"<<" ";
                            newKey = key - modifierKey;
                }

                arrayKey[indexKey] = newKey;

                EV << ", New key " << arrayKey[indexKey] << " ";

                indexKey++;
            }

            if (i == 1)
            {
                arrayValue[indexValue] = strtol(token, NULL, 10);

                EV << " Old value " << arrayValue[indexValue];
                EV << ", New value " << arrayValue[indexValue] << "\n ";
                indexValue++;
            }

            token = strtok(NULL, " ");

            i++;
        }
        currentSize++;
    }
    fclose(filePointer);

    writeOnFile(arrayKey, arrayValue, msg);

    ackMessageFunction(msg);
}
