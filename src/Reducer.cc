/*
 * Reducer.cc
 *
 *  Created on: 28 mag 2023
 *      Author: spagn
 */



#include <cstring>
#include <map>
#include <string.h>
#include <omnetpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "OperationMessage_m.h"
#include "ackMessage_m.h"
#include "backOnlineReducer_m.h"
#include "shuffle_m.h"
#include "keysValueMessage_m.h"
#include "messageToReducer_m.h"
#include "reducerAckCoordinatorMessage_m.h"
using namespace omnetpp;

class Reducer : public cSimpleModule
{
private:
    simtime_t timeout;
    cMessage *timeoutEvent;
    cMessage *backOnlineMessage;
    cQueue messageQueue;
    int probabilityOfFailure;
    //cMessage *backOnlineMessage;
    std::map<int, int> keyValuePair;
    int numOfMessages;
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void writeOnFile(messageToReducer *keysValueMessage);
    void ackCoordinator();
    void scheduleTimeout();
    void sendMessage(messageToReducer *message);
    void finish();
    void emptyQueue();

};

// The module class needs to be registered with OMNeT++
Define_Module(Reducer);

void Reducer::initialize()
{
    numOfMessages=0;

    WATCH(numOfMessages);
    timeout = 10.0;
    timeoutEvent = new cMessage("timeoutEvent");
    backOnlineMessage = new cMessage("backOnline");
    probabilityOfFailure=par("probabilityOfFailure");
}


void  Reducer::emptyQueue() {
       while (!messageQueue.isEmpty()) {
           messageToReducer *msg = dynamic_cast<messageToReducer*>(messageQueue.pop());
           delete msg;  // Remember to delete the message to avoid memory leaks
       }
   }
void Reducer::finish()
{
    EV << "Message sent or received by reducer "<<getIndex()<<" is " << numOfMessages << endl;
    recordScalar("#sentReducer", numOfMessages);
}
void Reducer::handleMessage(cMessage *msg)
{

    if (msg == timeoutEvent)
           {
            EV<<"Message of type timeout is arrived"<<"\n";
           // EV<<"un messaggio TimeoutEvent e' arrivato"<<"\n";
            cancelEvent(timeoutEvent);
            int random = rand() % 100;

            // il nodo decide se fallire o meno
            if (random >probabilityOfFailure)
            {
                numOfMessages+=1;
                EV<<"I'm reducer: "<<getIndex()<<" and i'm able to perform the function require, because the number generated is: "<<random<<"\n";
                ackCoordinator();

            }
            else{
                EV<<"Reducer failed"<<"\n";
                timeout = 15.0;
                scheduleAt(simTime() + timeout, backOnlineMessage);
                emptyQueue() ;
            }
           }
    else{
        messageToReducer *msgReceived = dynamic_cast<messageToReducer *>(msg);
        if (msgReceived!=nullptr){
            numOfMessages+=1;
            EV<<"Message of type Message To Reducer is arrived"<<"\n";
        EV << "I'm reducer: " << getIndex() << " ,partition to work: " << msgReceived->getPartitionToReduce() << "\n";
              messageQueue.insert(msgReceived);
              timeout = 1.0;
              scheduleTimeout();
        }
        else{
            // il nodo è tornato online e lo dice al coordinatore
               EV<<"I'm reducer: "<<getIndex()<<" and i received a message of type Back Online Message Reducer"<<"\n";
               backOnlineReducer *backOnlineMessage= new backOnlineReducer("backOnlineMessage");
               backOnlineMessage->setNodeNumber(getIndex());
               send(backOnlineMessage,"out");
               numOfMessages+=1;
        }
    }
}

void Reducer::scheduleTimeout() {
        // Controlla se è già stato programmato un messaggio di timeout
        if (timeoutEvent && !timeoutEvent->isScheduled()) {
            // Programmazione del messaggio di timeout
           // simtime_t timeout = simTime() + 1.0;  // Esempio di timeout dopo 5 secondi
            scheduleAt(simTime() +timeout, timeoutEvent);
            EV << "Timeout message programmed for " <<endl;
        }
    }
void Reducer::ackCoordinator(){
    while (!messageQueue.isEmpty()) {
        messageToReducer *message = dynamic_cast<messageToReducer *>(messageQueue.pop());
        EV<<"partitions"<<message->getPartitionToReduce()<<"\n";
        writeOnFile(message);
       if (message != nullptr) {
         sendMessage(message);
       }
       numOfMessages+=1;

       }
}

void Reducer::sendMessage(messageToReducer *message){
    reducerAckCoordinatorMessage *msg=new reducerAckCoordinatorMessage("reducerAckCoordinatorMessage");
            msg->setReducerSender(getIndex());
            msg->setPartitionReduced(message->getPartitionToReduce());

            send(msg, "out");
}
void Reducer::writeOnFile(messageToReducer *msg){
        const int bufferLength = 255, numberKeyValue = 2;
        int value=0,key=-1;
        char buffer[bufferLength];
        char absolutePath[100];
        FILE *filePointerWriting, *checkFileExistence,*filePointerReading;
        strcpy(absolutePath, msg->getPartitionToReduce());
        checkFileExistence = fopen(absolutePath, "r");
        if (checkFileExistence != NULL) {
               // printf("Il file esiste.\n");
                fclose(checkFileExistence);

                 filePointerReading= fopen(absolutePath, "r");
                 int index=0;
                 while (fgets(buffer, bufferLength, filePointerReading))
                     {

                         char *token = strtok(buffer, " ");
                         // loop through the string to extract all other tokens
                         int i = 0;
                         while (i < numberKeyValue)
                         {
                             if (i == 0)
                             {
                                 key = strtol(token, NULL, 10);


                                 //indexKey++;
                             }
                             if (i == 1)
                             {
                                 value = strtol(token, NULL, 10);



                             }

                             token = strtok(NULL, " ");
                             i++;
                         }
                         if (keyValuePair.find(key) != keyValuePair.end()) {
                             keyValuePair[key]+=value;
                         } else {
                             keyValuePair[key]=value;
                         }
                     }

            }
        fclose(filePointerReading);
        filePointerWriting= fopen(absolutePath, "w");
        char keyValuePairContainer[20];
         for (const auto& pair : keyValuePair) {
                 const int& key1 = pair.first;
                 const int& value1 = pair.second;

                 EV<<"Key and value written after processing on partition: "<<absolutePath<<"\n";
                 EV<< "Key: " << key1 << ", value: " << value1 <<"\n";
                 sprintf(keyValuePairContainer, "%d %d", key1, value1);
                 fprintf(filePointerWriting, "%s\n", keyValuePairContainer);
             }
         keyValuePair.clear();
         fclose(filePointerWriting);
}

