/*
 * shuffler.c
 *
 *  Created on: 28 mag 2023
 *      Author: spagn
 */


#include <string.h>
#include <omnetpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "OperationMessage_m.h"
#include "ackMessage_m.h"
#include "backOnline_m.h"
#include "shuffle_m.h"
#include "keysValueMessage_m.h"
#include "messageToReducer_m.h"
#include "shufflerAckCoordinator_m.h"
using namespace omnetpp;

class Shuffler : public cSimpleModule
{
private:
    simtime_t timeout;
    cMessage *timeoutEvent;
    cMessage *timeoutEventReducers;
    cMessage *backOnlineMessage;
    int numOfReducers;
    OperationMessage *msgArrived;
    ShuffleMessage *shuffleMessage;
    OperationMessage *messageSaved=NULL;
    cQueue messageQueue;
    //int numOfPartiotions=10;
    int numOfShuffler;
    int totalNumberOfPartitions=10;// partizioni nel momento in cui divido in chunk
    int *arrayOfPartitions=nullptr;
    int *arrayOfReducers=nullptr;
    std::vector<std::string> partitionsNames;
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void shufflingOperation(ShuffleMessage *msg);
    int checkReducerAvailable(ShuffleMessage *msg);
    void printArrayOfReducers(ShuffleMessage *msg);
    void printArrayOfPartitions(ShuffleMessage *msg);
    void riarrangeKey(ShuffleMessage *msg);
    void sendDataReducer();
    void scheduleTimeout();
    void scheduleTimeoutForReducers();
    void initializeArrayOfPartitions(ShuffleMessage *msg);
    void initializeArrayOfReducers(ShuffleMessage *msg);
    int numOfPartionsRemainedToElaborate();
    void initializeArrayOfNamePartitions();
    void sendMessageReducer(int reducerNumber, std::string partitionToElaborate);
    void sendMessageCoordinator(int reducerNumber, std::string partitionToElaborate);
    int  findReducer(int lastReducerFound);
};

// The module class needs to be registered with OMNeT++
Define_Module(Shuffler);

void Shuffler::initialize()
{
    numOfReducers=par("numOfReducer");
    numOfShuffler=par("numOfShuffler");
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");
    timeoutEventReducers = new cMessage("timeoutEventReducers");
    arrayOfPartitions = (int *)malloc(10 * sizeof(int));//ten because i can group the numbers in 10 partitions
    arrayOfReducers = (int *)malloc(numOfReducers * sizeof(int));

    initializeArrayOfNamePartitions();
}
// Used to inizialize chunks of data that need to be read or created
void Shuffler::initializeArrayOfNamePartitions(){
           partitionsNames.insert(partitionsNames.begin(), "C:\\Users\\spagn\\Desktop\\prova\\reduceFileOfKeys0.txt");
           partitionsNames.insert(partitionsNames.begin()+1, "C:\\Users\\spagn\\Desktop\\prova\\reduceFileOfKeys1.txt");
           partitionsNames.insert(partitionsNames.begin()+2, "C:\\Users\\spagn\\Desktop\\prova\\reduceFileOfKeys2.txt");
           partitionsNames.insert(partitionsNames.begin()+3, "C:\\Users\\spagn\\Desktop\\prova\\reduceFileOfKeys3.txt");
           partitionsNames.insert(partitionsNames.begin()+4, "C:\\Users\\spagn\\Desktop\\prova\\reduceFileOfKeys4.txt");
           partitionsNames.insert(partitionsNames.begin()+5, "C:\\Users\\spagn\\Desktop\\prova\\reduceFileOfKeys5.txt");
           partitionsNames.insert(partitionsNames.begin()+6, "C:\\Users\\spagn\\Desktop\\prova\\reduceFileOfKeys6.txt");
           partitionsNames.insert(partitionsNames.begin()+7, "C:\\Users\\spagn\\Desktop\\prova\\reduceFileOfKeys7.txt");
           partitionsNames.insert(partitionsNames.begin()+8, "C:\\Users\\spagn\\Desktop\\prova\\reduceFileOfKeys8.txt");
           partitionsNames.insert(partitionsNames.begin()+9, "C:\\Users\\spagn\\Desktop\\prova\\reduceFileOfKeys9.txt");

         /*  for (int i=0;i<10;i++){
               EV<<partitionsNames[i]<<" ";
           }
           EV<<"\n";
    */
}
// per capire quali partizioni sono state elaborate
void Shuffler::initializeArrayOfPartitions(ShuffleMessage *msg){
        for (int i=0;i<10;i++){
            arrayOfPartitions[i]=msg->getPartitionElaborated(i);
        }

}
// per capire quali reducer sono ancora attivi
void Shuffler::initializeArrayOfReducers(ShuffleMessage *msg){
        for (int i=0;i<numOfReducers;i++){
            arrayOfReducers[i]=msg->getReducerAvailable(i);
        }
}

void Shuffler::handleMessage(cMessage *msg)
{


    ShuffleMessage *shuffleMessage = dynamic_cast<ShuffleMessage *>(msg);
    if (shuffleMessage!=nullptr){

        initializeArrayOfPartitions(shuffleMessage);
        initializeArrayOfReducers(shuffleMessage);

        EV<<"I received a message of type Shuffle Message"<<"\n";
        EV << "i'm shuffler: " << " ,Operation to perform: " << shuffleMessage->getOperationToDo() << " ,partition to work: " << shuffleMessage->getPartitionToRead() << ",node to be sent: " << shuffleMessage->getNodeToSend() << "\n";
       messageQueue.insert(shuffleMessage);
       printArrayOfReducers(shuffleMessage);
       printArrayOfPartitions(shuffleMessage);
       scheduleTimeout();

    }
    if (msg == timeoutEvent)
       {
        while (!messageQueue.isEmpty()) {
            ShuffleMessage *message = dynamic_cast<ShuffleMessage *>(messageQueue.pop());
                                   if (message != nullptr) {
                                      if (message->getCreatePartition()){

                                          shufflingOperation(message);
                                          scheduleTimeoutForReducers();
                                      }
                                      else{

                                          scheduleTimeoutForReducers();
                                      }

                                   }
        }
       }
    if (msg == timeoutEventReducers)
           {
            EV<<"Sending data to the corresponding reducer"<<"\n";
            sendDataReducer();



           }
}
void Shuffler::scheduleTimeout() {
        // Controlla se è già stato programmato un messaggio di timeout
        if (timeoutEvent && !timeoutEvent->isScheduled()) {
            // Programmazione del messaggio di timeout

            scheduleAt(simTime() + timeout, timeoutEvent);

        }
    }
void Shuffler::scheduleTimeoutForReducers() {
        // Controlla se è già stato programmato un messaggio di timeout
        if (timeoutEventReducers && !timeoutEventReducers->isScheduled()) {
            // Programmazione del messaggio di timeout
           // simtime_t timeout = simTime() + 1.0;  // Esempio di timeout dopo 5 secondi
            scheduleAt(simTime()+timeout, timeoutEventReducers);
           // EV << "Messaggio di timeout programmato per " << timeout << endl;
        }
    }
void Shuffler::printArrayOfReducers(ShuffleMessage *msg)
{
EV<<"List of reducers available, 1=Available -1=Unavailable"<<"\n";
for (int i=0;i<msg->getReducerAvailableArraySize();i++){
    if(msg->getReducerAvailable(i)!=0){
    EV<<"Reducer "<<i<< " "<<msg->getReducerAvailable(i)<<"\n ";
    }
}
//EV<<"\n";
}
void Shuffler::printArrayOfPartitions(ShuffleMessage *msg)
{
EV<<"List of partition elabarated, -1=elaborated, -2 or any other number= not elaborated"<<"\n";
for (int i=0;i<msg->getPartitionElaboratedArraySize();i++){
    EV<< "Partition 1 "<<msg->getPartitionElaborated(i)<<" \n";
}
//EV<<"\n";
}

void Shuffler::shufflingOperation(ShuffleMessage *msg){

   EV << "Message arrived i'm the shuffler: " << " ,Operation: " << msg->getOperationToDo() << " ,partition: " << msg->getPartitionToRead() << ",node to be sent:: " << msg->getNodeToSend() << "\n";
    riarrangeKey(msg);

}

// per trovare il numero di partizioni rimanenti da elaborare
int Shuffler::numOfPartionsRemainedToElaborate(){
    int count=0;
   for (int i=0;i<totalNumberOfPartitions;i++){
       if (arrayOfPartitions[i]==-2){
           count++;
       }
   }
   return count;
}

int Shuffler::findReducer(int lastReducerFound){
int reducerFound=-1;
    if (lastReducerFound<numOfReducers){
        for (int i=lastReducerFound;i<numOfReducers;i++){
            if (arrayOfReducers[i]==1){
                reducerFound=i;
                break;
            }
        }
    }
    else{
        for (int i=0;i<numOfReducers;i++){
           if (arrayOfReducers[i]==1){
               reducerFound=i;
               break;
           }
       }

    }
    if (reducerFound==-1){
        for (int i=0;i<numOfReducers;i++){
                   if (arrayOfReducers[i]==1){
                       reducerFound=i;
                       break;
                   }
               }
    }
    return reducerFound;
}
void Shuffler::sendDataReducer(){
   // EV<<"SONO QUIII"<<endl;
    int reducerToSendData=-1;
    int remainedPartitions=numOfPartionsRemainedToElaborate();
    EV<<"Number of partitions remained to process "<<remainedPartitions<<"\n";
    // controllo se questo shuffler deve fare inviare una partizione oppure no
    if (remainedPartitions>0){

        for (int i=0;i<10;i++){
            if (arrayOfPartitions[i]==-2){
                int reducerFound=findReducer(reducerToSendData+1);
                EV<<"reducer to send data is "<<reducerFound<<"\n ";
                if (reducerFound==numOfReducers-1){
                    reducerToSendData=-1;
                }
                else{
                    reducerToSendData=reducerFound;
                }

                if (reducerFound>=0){
                    std::string partitionsFoundString=partitionsNames.at(i);
                    EV<<"The partition number found is: "<<i<<" and the corresponding name is: "<<partitionsFoundString<<"\n ";

                    // Invio un messaggio al reducer corrispondente in modo che inizi ad elaborare la partizione e poi informo anche il coordinator a chi è stata associata la partizione
                    sendMessageReducer(reducerFound,partitionsFoundString);
                    sendMessageCoordinator(reducerFound,partitionsFoundString);
                    EV<<"\n";
                }

            }
        }


    }
}

void Shuffler::sendMessageReducer(int reducerNumber, std::string partitionToElaborate){

    messageToReducer *msgToSend = new messageToReducer("messageToReducer");
    msgToSend->setReducerToSend(reducerNumber);
 // prende un puntatore al nome della partizione
    const char *stringPointer=partitionToElaborate.data();
    msgToSend->setPartitionToReduce(stringPointer);
    if(reducerNumber!=-1){
    send(msgToSend, "out", reducerNumber);
    }

}
void Shuffler::sendMessageCoordinator(int reducerNumber, std::string partitionToElaborate){
    shufflerAckCoordinatorMessage *msg = new shufflerAckCoordinatorMessage("shufflerAckCoordinator");
  msg->setReducerSent(reducerNumber);


    const char *stringPointer=partitionToElaborate.data();
    msg->setPartitionToReduce(stringPointer);
    send(msg, "outCoordinator");


}
void Shuffler::riarrangeKey(ShuffleMessage *msg){
    const int bufferLength = 255, numberKeyValue = 2;
    int value=0,key=-1;
    char buffer[bufferLength];
    char absolutePath[100];
    FILE *filePointerReading, *checkFileExistence,*filePointerWriting;
    filePointerReading = fopen(msg->getPartitionToRead(), "r+");
    int indexKey = 0, indexValue = 0;
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
                    EV << "Key extracted: " << key << " ";

                    //indexKey++;
                }
                if (i == 1)
                {
                    value = strtol(token, NULL, 10);


                    EV << ", Value extracted: " << value << "\n";

                  //  indexValue++;
                }

                token = strtok(NULL, " ");

                i++;
            }
            // vado a creare se necessario un file per ogni chunk
            sprintf(absolutePath, "C:\\Users\\spagn\\Desktop\\prova\\reduceFileOfKeys%i.txt",key%10 );
            checkFileExistence = fopen(absolutePath, "r");
            if (checkFileExistence != NULL) {
                           // printf("Il file esiste.\n");
                            fclose(checkFileExistence);
                             filePointerWriting= fopen(absolutePath, "a");
                             char keyValuePair[20];
                             sprintf(keyValuePair, "%d %d", key, value);
                             fputs(keyValuePair, filePointerWriting);
                             fputs("\n", filePointerWriting);

                             fclose(filePointerWriting);
            }
            else{
                filePointerWriting= fopen(absolutePath, "a");
                 char keyValuePair[20];
                 sprintf(keyValuePair, "%d %d", key, value);
                 fputs(keyValuePair, filePointerWriting);
                 fputs("\n", filePointerWriting);

                 fclose(filePointerWriting);
            }

        }
        fclose(filePointerReading);

}



int Shuffler::checkReducerAvailable(ShuffleMessage *msg){
    int available=0;
    for (int i=0;i<numOfReducers;i++){
        if (msg->getReducerAvailable(i)!=-1){
            return 1;
        }
    }
return 0;
}


