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
    int numOfReducers=5;
    OperationMessage *msgArrived;
    ShuffleMessage *shuffleMessage;
    OperationMessage *messageSaved=NULL;
    int arrayKey[100] = {[0 ... 99] = -1};
    int arrayValue[100] = {[0 ... 99] = -1};
    cQueue messageQueue;
    //int numOfPartiotions=10;
    int numOfShuffler=5;
    int totalNumberOfPartitions=10;// partizioni nel momento in cui divido in chunk
    int *arrayOfPartitions=nullptr;
    int *arrayOfReducers=nullptr;
    std::vector<std::string> partitionsNames;
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void getDataFromFile(ShuffleMessage *msg);
    void shufflingOperation(ShuffleMessage *msg);
    void addKeyValuepair(int *arrayKey, int *arrayValue);
    void prepareMessage(ShuffleMessage *msg);
    void sendMessage(int *arrayKey, int reducerToSend, int key,ShuffleMessage *msg);
    int checkReducerAvailable(ShuffleMessage *msg);
    int findClosestReducerAvailable(int reducerToSend,ShuffleMessage *msg);
    void printArrayOfReducers(ShuffleMessage *msg);
    void printArrayOfPartitions(ShuffleMessage *msg);
    void printKeyValuePairExtracted();
    void resetArray(int *array);
    void riarrangeKey(ShuffleMessage *msg);
    void ackMessageCoordinator();
    void sendDataReducer();
    int findReducerToSendData(int index,int count);
    void scheduleTimeout();
    void scheduleTimeoutForReducers();
    int countPartitionsToElaborate(int index);
    void initializeArrayOfPartitions(ShuffleMessage *msg);
    void initializeArrayOfReducers(ShuffleMessage *msg);
    int numOfPartionsRemainedToElaborate();
    int findPartitionToElaborate(int index,int count);
    void initializeArrayOfNamePartitions();
    void sendMessageReducer(int reducerNumber, std::string partitionToElaborate);
    void sendMessageCoordinator(int reducerNumber, std::string partitionToElaborate);
};

// The module class needs to be registered with OMNeT++
Define_Module(Shuffler);

void Shuffler::initialize()
{
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");
    timeoutEventReducers = new cMessage("timeoutEventReducers");
    arrayOfPartitions = (int *)malloc(10 * sizeof(int));//ten because i can group the numbers in 10 partitions
    arrayOfReducers = (int *)malloc(numOfReducers * sizeof(int));
    initializeArrayOfNamePartitions();
}
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
  //  EV<<"sono qui";

    ShuffleMessage *shuffleMessage = dynamic_cast<ShuffleMessage *>(msg);
    if (shuffleMessage!=nullptr){
      //  totalNumberOfPartitions=shuffleMessage->getPartitionElaboratedArraySize();
        initializeArrayOfPartitions(shuffleMessage);
        initializeArrayOfReducers(shuffleMessage);
    //    EV<<"sono qui 2";
        EV<<"I received a message of type Shuffle Message"<<"\n";
        EV << "i'm shuffler: " << getIndex() << " ,Operation to perform: " << shuffleMessage->getOperationToDo() << " ,partition to work: " << shuffleMessage->getPartitionToRead() << ",node to be sent: " << shuffleMessage->getNodeToSend() << "\n";
       messageQueue.insert(shuffleMessage);
       printArrayOfReducers(shuffleMessage);
       printArrayOfPartitions(shuffleMessage);
       scheduleTimeout();
     //  shufflingOperation(shuffleMessage);
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
           // simtime_t timeout = simTime() + 1.0;  // Esempio di timeout dopo 5 secondi
            scheduleAt(simTime() + timeout, timeoutEvent);
           // EV << "Messaggio di timeout programmato per " << timeout << endl;
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
EV<<"List of reducers available"<<"\n";
for (int i=0;i<msg->getReducerAvailableArraySize();i++){
    EV<<"Reducer "<<i<< " "<<msg->getReducerAvailable(i)<<"\n ";
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
void Shuffler::printKeyValuePairExtracted()
{

for (int i=0;i<100;i++){
    if (arrayKey[i]!=-1){
        EV<<"key "<<arrayKey[i]<<" ";
           EV<<"value "<<arrayValue[i]<<"\n ";
    }


}

}
void Shuffler::shufflingOperation(ShuffleMessage *msg){

   EV << "Message arrived i'm shuffler: " << getIndex() << " ,Operation: " << msg->getOperationToDo() << " ,partition: " << msg->getPartitionToRead() << ",node to be sent:: " << msg->getNodeToSend() << "\n";
    //printArrayOfReducers(msg);
    riarrangeKey(msg);
    /*getDataFromFile(msg);
    printKeyValuePairExtracted();
    prepareMessage(msg);
*/

}
int Shuffler::findReducerToSendData(int index,int count){
    int indexReducerFound=-1;
    while((index+count)>=0){
        if (indexReducerFound<numOfReducers-1){
            for(int i=indexReducerFound+1;i<numOfReducers;i++){
                if (arrayOfReducers[i]==1){
                    indexReducerFound=i;
                    index=index-1;
                    break;
                }
            }
        }
        else{
            indexReducerFound=0;
            for(int i=indexReducerFound;i<numOfReducers;i++){
                if (arrayOfReducers[i]==1){
                    indexReducerFound=i;
                    index=index-1;
                    break;
                }
        }

    }

}
    return indexReducerFound;
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
//guardo quante partizioni questo shuffler dovrà elaborare, cioè dovrà gestire sulla base di quelle che sono già state elaborate
int Shuffler::countPartitionsToElaborate(int index){
    int remainedPartitions=numOfPartionsRemainedToElaborate();
    int num=remainedPartitions/numOfShuffler;

    if (num*numOfShuffler==remainedPartitions){// se le partizioni rimaste da elaborare sono pari al numero di shuffler allora sono aposto
        return num;
    }
    else{// Se le partizioni rimaste da elaborare sono maggiori del numero di shuffler allora qualche shuffler dovrà gestire più di una partizione
        if((index+1+num*numOfShuffler)<=remainedPartitions){
            return num+1;
        }
        else{
            return num;
        }
    }

}
int Shuffler::findPartitionToElaborate(int index,int count){

    int partitionFound=-1;
    int cicles=0;
    for (int i=0;i<getIndex();i++){
        cicles=cicles+countPartitionsToElaborate(i);
    }
       while((cicles+count)>0){
           if (partitionFound<totalNumberOfPartitions){
               for(int i=partitionFound+1;i<totalNumberOfPartitions;i++){
                   if (arrayOfPartitions[i]==-2){
                       partitionFound=i;
                       cicles=cicles-1;
                       break;
                   }
               }
           }


   }
       return partitionFound;
}
void Shuffler::sendDataReducer(){

    int remainedPartitions=numOfPartionsRemainedToElaborate();
    EV<<"Number of partitions remained to process "<<remainedPartitions<<"\n";
    // controllo se questo shuffler deve fare inviare una partizione oppure no
    if (remainedPartitions>=(getIndex()+1)){
        int counter=countPartitionsToElaborate(getIndex());
        EV<<"Number of partition that I shuffler: "<<getIndex()<< " have to process is: "<<counter<<"\n";
        for (int i=0;i<counter;i++){
            //EV<<"sono lo shuffler "<<getIndex()<<"\n";
// trovo il reducer a cui inviare i dati
            int reducerFound=findReducerToSendData(getIndex(),i);
            EV<<"reducer to send data is "<<reducerFound<<"\n ";
            //EV<<"sono lo shuffler "<<getIndex()<<"\n";
            // trovo la partizione che dovrà essere elaborata da quel reducer
            int partitionsFound=findPartitionToElaborate(getIndex(),i+1);
            //EV<<"partition to elaborate number"<<partitionsFound<<"\n";
            // Trovo il nome della partizione che dovrà essere elaborata
            std::string partitionsFoundString=partitionsNames.at(partitionsFound);
            EV<<"The partition number found is: "<<partitionsFound<<" and the corresponding name is: "<<partitionsFoundString<<"\n ";
            // Invio un messaggio al reducer corrispondente in modo che inizi ad elaborare la partizione e poi informo anche il coordinator a chi è stata associata la partizione
            sendMessageReducer(reducerFound,partitionsFoundString);
            sendMessageCoordinator(reducerFound,partitionsFoundString);

        }

        EV<<"\n";
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

void Shuffler::ackMessageCoordinator(){

}
/*
void Shuffler::aggregateFunction(KeysValueMessage *msg){
        const int bufferLength = 255, numberKeyValue = 2;
        int value=0;
        char buffer[bufferLength];
        char absolutePath[100];
        FILE *filePointerWriting, *checkFileExistence;
        sprintf(absolutePath, "C:\\Users\\spagn\\Desktop\\prova\\reduceFileOfKey%i.txt", msg->getKey());
        checkFileExistence = fopen(absolutePath, "r");
        if (checkFileExistence != NULL) {
                printf("Il file esiste.\n");
                fclose(checkFileExistence);
                 filePointerWriting= fopen(absolutePath, "w");
                 int index=0;
                 while (fgets(buffer, bufferLength, filePointerWriting))
                     {

                         char *token = strtok(buffer, " ");
                         // loop through the string to extract all other tokens
                         int i = 0;
                         while (i < numberKeyValue)
                         {

                             if (i == 1)
                             {
                                 value = strtol(token, NULL, 10);
                                 while (msg->getValue(index)!=-1){
                                     value = value + msg->getValue(index);
                                 }


                             }

                             token = strtok(NULL, " ");
                             i++;
                         }
                     }


                     char keyValuePair[20];
                     sprintf(keyValuePair, "%d %d", msg->getKey(), value);
                     fprintf(filePointerWriting, "%s\n", keyValuePair);

            }
        else{
            filePointerWriting= fopen(absolutePath, "w");
            char keyValuePair[20];
            int index=0;
            int value=0;
            while (msg->getValue(index)!=-1){
                value = value + msg->getValue(index);
            }
            sprintf(keyValuePair, "%d %d", msg->getKey(), value);
            fprintf(filePointerWriting, "%s\n", keyValuePair);

        }

        fclose(filePointerWriting);
}
*/

int Shuffler::checkReducerAvailable(ShuffleMessage *msg){
    int available=0;
    for (int i=0;i<numOfReducers;i++){
        if (msg->getReducerAvailable(i)!=-1){
            return 1;
        }
    }
return 0;
}
int Shuffler::findClosestReducerAvailable(int reducerToSend,ShuffleMessage *msg){
    int available=0;
    for (int i=reducerToSend-1;i>=0;i--){
       if (msg->getReducerAvailable(i)!=-1){
           return i;
       }
    }
    for (int i=numOfReducers;i>=reducerToSend;i--){
           if (msg->getReducerAvailable(i)!=-1){
               return i;
           }
        }
return -1;
}
void Shuffler::resetArray(int *array){

for (int i=0;i<100;i++){
 array[i]=-1;
}


}
void Shuffler::prepareMessage(ShuffleMessage *msg){
int lenght =sizeof(arrayKey) / sizeof(arrayKey[0]);
int array[100] = {[0 ... 99] = -1};

for (int k=0;k<lenght;k++){
int index=0;
    for (int i=0; i<lenght;i++){

        if (arrayKey[i]!=-1 && arrayKey[i]==k){
            //EV<<"sono qui dentro"<<"\n";
            array[index]=arrayValue[i];
            index++;
        }

    }

if (array[0]!=-1){
    sendMessage(array,k%numOfReducers,k,msg);

}
resetArray(array);
}
}

void Shuffler::sendMessage(int *array, int reducerToSend, int key,ShuffleMessage *msg){
    KeysValueMessage *msgToSend = new KeysValueMessage("keysValueMessage");
    msgToSend->setKey(key);
    msgToSend->setEntireArray(array);
   if (msg->getReducerAvailable(reducerToSend)!=-1){
       msgToSend->setReducerToSend(reducerToSend);

     //  msgToSend -> setPartition(msg ->getPartitionToRead());
       EV << "shuffler "<<getIndex()<<" is sending a message to reducer "<<msgToSend->getReducerToSend() <<" with values that have key: "<< msgToSend->getKey() << " \n";

       for (int i=0;i<100;i++){
           if (msgToSend->getValue(i)!=-1){
               EV <<msgToSend->getValue(i)<<"\n";

           }

       }
      send(msgToSend, "out",reducerToSend);
   }
   else{
       int availableReducer=checkReducerAvailable(msg);
       if (availableReducer){
           int reducerFound=findClosestReducerAvailable(reducerToSend,msg);
           if(reducerFound!=-1){
               msgToSend->setReducerToSend(reducerFound);
               send(msgToSend, "out", reducerFound);
           }
       }

   }

}
void Shuffler::getDataFromFile(ShuffleMessage *msg)
{
    FILE *filePointer;
    const int bufferLength = 255, numberKeyValue = 2;
    char buffer[bufferLength];


    filePointer = fopen(msg->getPartitionToRead(), "r+");
    int indexKey = 0, indexValue = 0;
    while (fgets(buffer, bufferLength, filePointer))
    {

        char *token = strtok(buffer, " ");
        // loop through the string to extract all other tokens
        int i = 0;
        while (i < numberKeyValue)
        {
            if (i == 0)
            {
                arrayKey[indexKey] = strtol(token, NULL, 10);
                EV << "key " << arrayKey[indexKey] << " ";
                indexKey++;
            }
            if (i == 1)
            {
                int value = strtol(token, NULL, 10);


                arrayValue[indexValue] = value;

                EV << "value " << arrayValue[indexValue] << "\n";

                indexValue++;
            }

            token = strtok(NULL, " ");

            i++;
        }
    }
    fclose(filePointer);

    //writeOnFile(arrayKey, arrayValue, msg);

    //ackMessageFunction(msg);
}

