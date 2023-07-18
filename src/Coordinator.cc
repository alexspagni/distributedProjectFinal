#include <string.h>
#include <omnetpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include "OperationMessage_m.h"
#include "shuffle_m.h"
#include "ackMessage_m.h"
#include "backOnline_m.h"
#include "shufflerAckCoordinator_m.h"
#include "reducerAckCoordinatorMessage_m.h"
#include "backOnlineReducer_m.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
using namespace omnetpp;

/**
 * Derive the Txc1 class from cSimpleModule. In the Tictoc1 network,
 * both the `tic' and `toc' modules are Txc1 objects, created by OMNeT++
 * at the beginning of the simulation.
 */
class Coordinator : public cSimpleModule
{
private:
    simtime_t timeout = 10;
    cMessage *timeoutEvent;
    int numberNode;

    int executionMode=0;
    int num_of_partitions;
    int length_absolute_path = 100;
    char** partitionLocations;

public:
    Coordinator() : cSimpleModule() {
        partitionLocations = nullptr;
        operationsToDoArray=nullptr;
        }
    virtual ~Coordinator() {
            if (partitionLocations != nullptr) {
                for (int i = 0; i < num_of_partitions; i++) {
                    delete[] partitionLocations[i];
                }
                delete[] partitionLocations;
            }
        }
    int numberOfReducers=5;
    std::vector<int> reducersPartitionAssociation={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1} ;
    std::vector<int> reducersPartitionAssociationTemp={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1} ;
    std::vector<int> reducersActive ;
    const int key_value_generated = 5;

    char operationToDo[20] = "key add 5";
    int *arrayNodes;
    int *failedNodes;

    //char partitionLocations[num_of_partitions][length_absolute_path];
    int indexForPartitionElaborated = -2;
    int *backOnlineNodes;
    char **operationsToDoArray;
    int globalIndexOfOperation=0;
    std::vector<simtime_t> timesMessages;
    std::vector<std::string> partitionsNames;

   // std::vector<std::string> partitionsNames;
   // int reducersActive[100] = {[0 ... 99] = 1};// tutti i reducer sono attivi
    int numberOfShuffler;
  //  int *nodeMessageSent;
   // char operationsToDoArray2[30][20];

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void createPartitions(char absolutePath[100]);
    int checkLettersDrawn(char letterUsed[], char caracterDrawn);
    virtual void generateMessage(char partition[100], char operationToPerform[20], int nodeToSend);
    virtual void shufflerMessage(char partition[100], char operationToPerform[20], int nodeToSend);
    virtual void shufflerMessageModified(const char partition[100], char operationToPerform[20], int nodeToSend);
    virtual int findNextPartition(const char *partition);
    virtual int actualPartitionFunction(const char *partition);
    bool workDone();
    bool workDoneReducer();
    void createFailedArray();
    int reasseamblePartitions();
    int checkNodeAvailable();
    void printFailedNodes();
    void printPartitions();
    void printReducedPartitions();
    void printReducersActive();
    void checkRemainingPartitions();
    void printOnlineNodes();
    void nextOperation();
    void nextOperationReduce();
    void cleanPartitions();
    int reasseamblePartitionsWhenNodesComeBack(int nodeNumber);
    void printMessageTimes();
    void cleanPartitions(int nodeNumber);
    int checkNodeAssegnedAlready(int nodeNumber);
    void initializeReducers();
    void findPartitionNotElaborated();
    void initializeArrayOfNamePartitions();
    int findPartitionReduced(shufflerAckCoordinatorMessage *msgArrived );
    int findPartitionReduced2(reducerAckCoordinatorMessage *msgArrived );
    void resetReducerPartitions();
    bool allReducersDown();
    int countNumberOfOperations();
};

// The module class needs to be registered with OMNeT++
Define_Module(Coordinator);

void Coordinator::initialize()
{
    int numberOfOperations=countNumberOfOperations();
    operationsToDoArray = new char*[numberOfOperations];
              for (int i = 0; i < numberOfOperations; i++) {
                  operationsToDoArray[i] = new char[20];
              }
  //PROVA

    nlohmann::json json;

      std::ifstream file("C:\\Users\\spagn\\Desktop\\prova\\file.json");

      if (file.is_open()) {
          file >> json;
          file.close();
      } else {
          EV << "Failed to open the JSON file." << "\n";

      }

      // Extract number of partitions from the JSON structure

      int partitions = json["partitions"];
      EV<<partitions<<"\n";
      int numMap = json["map"].size();
      std::string Map[2][numMap];
      int i = 0;
      // Access and manipulate the JSON data
      for (const auto& map : json["map"]) {
          int MapId = map["id"];
          char finalArray[20]="";

          std::string op = map["operation"];

          const char* Operation = op.c_str();
        // EV<<Operation<<"\n";
          std::string num = map["value"];
          const char* numOp = num.c_str();
          //EV<<numOp<<"\n";
          //aggiungi valori estratti a
          //Map[0][i] = Operation;
          //Map[1][i] = to_string(numOp);
          //operationsToDoArray[i] = "Map ";
          char mapString[5]="map ";
          strcat(finalArray, mapString);
          strcat(finalArray, Operation);
         strcat(finalArray, " ");
          strcat(finalArray, numOp);
          //EV<<finalArray<<"\n";
          strcpy(operationsToDoArray[i],finalArray);
        //  strcat(operationsToDoArray2[i],"\0");
          //cout << "worker function " << MapId << " Map operation: " << Map[0][i] << " value: " << Map[1][i] << endl;
        //  EV << operationsToDoArray2[i] << "\n";
         //
          i++;
      }


for (const auto& red : json["changekey"]) {
    char finalArray2[20]="";
 //string op = red["operation"];
 std::string op2 = red["operation"];
 const char* Operation2 = op2.c_str();


 std::string num2 = red["value"];
  const char* numOp2 = num2.c_str();
  //aggiungi valori estratti a
  strcat(finalArray2, "key ");
  strcat(finalArray2, Operation2);
  strcat(finalArray2, " ");
  strcat(finalArray2, numOp2);
  strcpy(operationsToDoArray[i],finalArray2);
  i++;
}

for (const auto& red2 : json["reduce"]) {
    char finalArray3[20]="";
    strcat(finalArray3, "reduce");
    strcpy(operationsToDoArray[i],finalArray3);
      i++;
  }
for (int i=0;i<numberOfOperations;i++){
    EV<<operationsToDoArray[i]<<"\n";
}
 ///////////////////
    num_of_partitions=partitions;
    partitionLocations = new char*[num_of_partitions];
           for (int i = 0; i < num_of_partitions; i++) {
               partitionLocations[i] = new char[length_absolute_path];
           }
    numberNode = par("n");
    numberOfShuffler=par("numberOfShuffler");
    // failed nodes: serve per tenere traccia dei nodi che sono falliti
    //arrayNodes: serve per sapere quale partizioni sono state allocate e a quali nodi
    //backOnlineNodes: serve per capire quali nodi dopo essere falliti sono tornati online.
    failedNodes = (int *)malloc(numberNode * sizeof(int));
    arrayNodes = (int *)malloc(num_of_partitions * sizeof(int));
    backOnlineNodes = (int *)malloc(numberNode * sizeof(int));
    reducersActive.resize(numberOfReducers);
    initializeReducers();
    //Initialize the partitions of chunks for the reduce phase
    initializeArrayOfNamePartitions();
    // set the vector array at the correct dimension
    // timesMessages: serve per tenere traccia del "time clock" a cui è stato ricevuto un certo messaggio o quando è stato mandato ad un certo nodo
    timesMessages.resize(numberNode);
  //  nodeMessageSent=(int *)malloc(num_of_partitions * sizeof(int));
    // initialized failed nodes array
    for (int i = 0; i < numberNode; i++)
    {
        failedNodes[i] = 1;// at first all the nodes are active
        backOnlineNodes[i]=0;// at first any node is failed so no node is online.
        timesMessages[i]=-1;// inizialmente nessun messaggio è stato mandato e quindi nessun time clock registrato
        //nodeMessageSent[i]=0;

    }
    // initialize array nodes
    for (int i = 0; i < num_of_partitions; i++)
    {

        arrayNodes[i] =-2;// se uno degli elementi di arrayNodes è settato a -2 allora vuol dire che quella partizione non è stata assegnata a nessun nodo
       // nodeMessageSent[i]=-1;
    }

  /*  for (int i = 0; i < num_of_partitions; i++)
    {
        EV << arrayNodes[i] << " ";
    }
*/
    for (int i = 0; i < num_of_partitions; i++)
    {
        strcpy(partitionLocations[i], " ");
    }

    char fileName[20] = "partition";
    //char endOfTheName[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    //char charactersUsed[26];
   // strncpy(charactersUsed, " ", sizeof(charactersUsed));

    // vado a creare le partizioni
    for (int i = 0; i < num_of_partitions; i++)
    {
        char absolutePath[100];
        char letter;
        int seen = 0;
        sprintf(absolutePath, "C:\\Users\\spagn\\Desktop\\prova\\partition%i.txt", i);
        strcpy(partitionLocations[i], absolutePath);

        createPartitions(absolutePath);
    }
    for (int i=0;i<num_of_partitions;i++){
        EV<<partitionLocations[i]<<"\n";
    }
    // vado ad inviare i primi messaggi, una partizione per ogni nodo disponibile
    simtime_t currentTime = simTime();//acquisco il tempo corrente

    if (strcmp(operationsToDoArray[0], "reduce")!=0){
        for (int i = 0; i <numberNode; i++)
        {
                if (i<num_of_partitions){
                int nodeToSend=i%numberNode;
                timesMessages[i]=currentTime;
                arrayNodes[i] =nodeToSend;
                //shufflerMessage(partitionLocations[i], operationsToDoArray[globalIndexOfOperation],nodeToSend);
                EV<<"sending data  to the mapper :"<<nodeToSend<<"\n";
                generateMessage(partitionLocations[i], operationsToDoArray[globalIndexOfOperation], nodeToSend);
            }
        }
    }
    else{
        for (int i = 0; i <num_of_partitions; i++)
            {

            int shufflerToSend=i%numberOfShuffler;
            EV<<"No mapper operations, sending data to the shuffler "<<shufflerToSend<<"\n";
            shufflerMessage(partitionLocations[i], operationsToDoArray[globalIndexOfOperation],shufflerToSend);
            }
        reducersActive.clear();
        for (int i=0;i<numberOfReducers;i++){
                reducersActive[i]=-1;
            }
    }

    printPartitions();

    printMessageTimes();
    // set and start the timeout of the coordinator
    timeoutEvent = new cMessage("timeoutEvent");
    scheduleAt(simTime() + timeout, timeoutEvent);
}

void Coordinator::initializeArrayOfNamePartitions(){
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

          /* for (int i=0;i<10;i++){
               EV<<partitionsNames[i]<<" ";
           }
           EV<<"\n";
*/
}
void Coordinator::initializeReducers()
{
    for (int i=0;i<numberOfReducers;i++){
        reducersActive[i]=1;
    }
}
int Coordinator::findNextPartition(const char *partition)
{
    int partitionIndex = 0;
    char buffer[100];
    strncpy(buffer, partition, sizeof(buffer) - 1);

    while (strcmp(partitionLocations[partitionIndex], buffer) != 0)
    {
        partitionIndex++;
    }
    int nextPartitionIndex = -1;
    if (partitionIndex + numberNode < num_of_partitions)
    {
        if (arrayNodes[partitionIndex + numberNode]!=-1){
            nextPartitionIndex = partitionIndex + numberNode;
        }

    }

    return nextPartitionIndex;
}
// viene utilizzata per capire su quale partizione ha lavorato il nodo da cui il coordinator ha ricevuto il messaggio
int Coordinator::actualPartitionFunction(const char *partition)
{

    int indexPartition = 0;
    char buffer[100];
    strncpy(buffer, partition, sizeof(buffer) - 1);
    while (strcmp(partitionLocations[indexPartition], buffer) != 0)
    {
        indexPartition++;
    }
    return indexPartition;
}
//Controllo se tutte le partizioni sono state elaborate con successo
bool Coordinator::workDone()
{
    bool result = false;

    for (int i = 0; i < num_of_partitions; i++)
    {
        if (arrayNodes[i] != -1)
        {
            return result;
        }
    }
    result = true;
    return result;
}
bool Coordinator::workDoneReducer()
{
    bool result = false;

    for (int i = 0; i < 10; i++)
    {
        if (reducersPartitionAssociation[i]!= -1)
        {
            return result;
        }
    }
    result = true;
    return result;
}
// Va a prendere la prima partizione disponibile
int Coordinator::reasseamblePartitions()
{
    for (int i = 0 ; i < num_of_partitions; i++)
    {
        // EV<< i<< " ";
        if (arrayNodes[i] == -2)
        {

           // indexForPartitionElaborated = i;
            return i;
            //   EV<< "\n";
        }
    }

    return -1;
}
// Funzione che va a cercare l'ultima partizione che era stata assegnata al nodo, se la trova la restituisce
int Coordinator::reasseamblePartitionsWhenNodesComeBack(int nodeNumber)
{
    for (int i = 0 ; i < num_of_partitions; i++)
    {
        // EV<< i<< " ";
        if (arrayNodes[i] == nodeNumber)
        {

           // indexForPartitionElaborated = i;
            return i;
            //   EV<< "\n";
        }
    }

    return -1;
}
//NON USATA
void Coordinator::checkRemainingPartitions()
{
    bool remainingPartitions = false;
    for (int i = indexForPartitionElaborated + 1; i < num_of_partitions; i++)
    {
        // EV<< i<< " ";
        if (arrayNodes[i] != -1)
        {

            remainingPartitions = true;
            //   EV<< "\n";
        }
    }

    if (remainingPartitions == false)
    {
        indexForPartitionElaborated = -1;
    }
    EV << " " << remainingPartitions << " " << indexForPartitionElaborated << "\n";
}
/*
int Coordinator::reasseamblePartitions()
{
    for (int i = indexForPartitionElaborated + 1; i < num_of_partitions; i++)
    {
        // EV<< i<< " ";
        if (arrayNodes[i] != -1)
        {

            indexForPartitionElaborated = i;
            return indexForPartitionElaborated;
            //   EV<< "\n";
        }
    }

    return -1;
}
void Coordinator::checkRemainingPartitions()
{
    bool remainingPartitions = false;
    for (int i = indexForPartitionElaborated + 1; i < num_of_partitions; i++)
    {
        // EV<< i<< " ";
        if (arrayNodes[i] != -1)
        {

            remainingPartitions = true;
            //   EV<< "\n";
        }
    }

    if (remainingPartitions == false)
    {
        indexForPartitionElaborated = -1;
    }
    EV << " " << remainingPartitions << " " << indexForPartitionElaborated << "\n";
}
*/
// Controllo se ci sono ancora nodi "alive"
int Coordinator::checkNodeAvailable()
{
    int counter = 0;
    for (int i = 0; i < numberNode; i++)
    {
        if (failedNodes[i] == 0)
        {
            counter++;
        }
    }
    return counter;
}
void Coordinator::printFailedNodes()
{
     EV<<"The list below shows which nodes have been failed and which not, 0=failed,1= not failed"<<"\n";
    for (int i = 0; i < numberNode; i++)
    {
        EV << " failed node :" << i << " " << failedNodes[i] << "\n";
    }
}
void Coordinator::printPartitions()
{
    EV<<"How the partitions are assigned at the beginnng"<<"\n";
    for (int i = 0; i < num_of_partitions; i++)
    {
        EV << " partitions :" << i << " " << arrayNodes[i] << "\n";
    }
}
void Coordinator::printReducedPartitions()
{
    for (int i = 0; i < 10; i++)
    {
        EV << " reducer partitions :" << i << " " << reducersPartitionAssociation[i] << "\n";
    }
}
void Coordinator::printReducersActive()
{
    for (int i = 0; i < numberOfReducers; i++)
    {
        EV << " reducer :" << i << " " << reducersActive[i] << "\n";
    }
}
void Coordinator::printOnlineNodes()
{
    for (int i = 0; i < numberNode; i++)
    {
        EV << " online node :" << i << " " << backOnlineNodes[i] << "\n";
    }
}
void Coordinator::printMessageTimes()
{
    EV<<"Taking track of the times of messagess exchanged beetween the mappers and the coordinator "<<"\n";
    for (int i = 0; i < numberNode; i++)
    {
        EV << " last message sent to node :" << i << " at time " << timesMessages[i] << "\n";
    }
}
bool Coordinator::allReducersDown(){
    int count=0;
for (int i=0;i<numberOfReducers;i++){
    if (reducersActive[i]==-1){
        count++;
    }
}
if(count==numberOfReducers){
    return true;
}
else
{
 return false;
}
}
// Questa funzione mi serve per settare le partizioni che erano state assegnate ai nodi falliti come "libere" e quindi riassegnabili
void Coordinator::cleanPartitions(int nodeNumber){
    for (int i=0;i<num_of_partitions;i++){
        if (arrayNodes[i]==nodeNumber){
            arrayNodes[i]=-2;
        }
    }
}
// Questa funzione serve a controllare se ad un nodo è già stata assegnata una partizione oppure no
int Coordinator::checkNodeAssegnedAlready(int nodeNumber){
    for (int i=0;i<num_of_partitions;i++){
           if (arrayNodes[i]==nodeNumber){
               return 1;
           }
       }
    return 0;
}
void Coordinator::createFailedArray()
{
    executionMode=1;
    if(executionMode==0){
        // Nel caso di executionMode=0 ad ogni nodo venivano assegnate le partizioni applicando semplicemente il modulo.
        for (int j = 0; j < numberNode; j++)
        {
            for (int i = 0; i < num_of_partitions; i++)
            {
                if (arrayNodes[i] == j)
                {
                    failedNodes[j] = 0;
                    arrayNodes[i]=-2;// settare a -2 significa liberare la partizione.
                }

            }
        }

        for (int i=0;i<numberNode;i++){
            backOnlineNodes[i]=0;
        }
    }
    //EV<<"actual time"<<simTime()<<"\n";
    //printMessageTimes();

    if (executionMode==1){
        for (int i=0;i<numberNode;i++){
            /**Ser ogni nodo controllo quando è arrivato l'ultimo messaggio.
             * Se l'ultimo messaggio è arrivato più di tre secondi fa e a questo nodo era stata assegnata effettivamente una partizione allora vuol dire che il nodo è fallito.
             * Se nell'array timesMessagge nell'indece corrispondente al nodo c'è "-1" significa che al nodo in question non era stata assegnata nessuna partizione
            **/
            if ((simTime()-timesMessages[i])>3 && timesMessages[i]!=-1){
                //EV<<simTime()-timesMessages[i]<<" ";
                failedNodes[i]=0;// segno che il nodo è fallito

            }
        }
        //EV<<"\n";
    }
    for (int i=0;i<numberNode;i++){
        if (failedNodes[i]==0){// per tutti i nodi falliti.
            cleanPartitions(i);
        }

    }
   /* for (int i=0;i<numberNode;i++){
        if (failedNodes[i]!=0){}
        timesMessages[i]=-1;
    }*/
   // printMessageTimes();
//executionMode=1;
}

void Coordinator::cleanPartitions(){
    for (int i=0;i<num_of_partitions;i++){
        arrayNodes[i]=-2;
    }
    createFailedArray();

}
void Coordinator::nextOperation(){
cleanPartitions();
for (int i = 0; i < numberNode; i++)
   {
    if (failedNodes[i]!=0){
        arrayNodes[i] =i;
        timesMessages[i]=simTime();
        generateMessage(partitionLocations[i], operationsToDoArray[globalIndexOfOperation], i);
    }
   }

   timeoutEvent = new cMessage("timeoutEvent");
   scheduleAt(simTime() + timeout, timeoutEvent);

}

void Coordinator::nextOperationReduce(){
cleanPartitions();
for (int i = 0; i < numberNode; i++)
   {
    if (failedNodes[i]!=0){
        arrayNodes[i] =i;
        timesMessages[i]=simTime();
      //  generateMessage(partitionLocations[i], operationsToDoArray[globalIndexOfOperation], i);
    }
   }
   executionMode=2;
   timeoutEvent = new cMessage("timeoutEvent");
   scheduleAt(simTime() + timeout, timeoutEvent);

}
void Coordinator::handleMessage(cMessage *msg)
{
    if (msg != timeoutEvent)
    {
        EV<<msg->getArrivalGate()<<"\n";
        ackMessage *msgArrived2 = dynamic_cast<ackMessage *>(msg);
        if (msgArrived2 != nullptr)// dopo che ho controllato se il cast a ack message è andato bene vado ad eseguire le operazioni che dovrebbero essere fatte nel caso in cui sia arrivato questo messaggio
        {

            if (checkNodeAvailable() == 0 && executionMode==0)
            {
                ackMessage *msgArrived = check_and_cast<ackMessage *>(msg);
                // significa che il nodo che ha inviato il messaggio ha completato il suo lavoro
                timesMessages[msgArrived->getNodeNumber()]=-1;
                EV << "i'm the coordinator and i received a message from mapper " << msgArrived->getNodeNumber() << " , the partition on which the work was performed is: " << msgArrived->getPartition() << "\n";
                EV << "Anyone of the nodes have been failed from my perspective so i continue to assign the partitions in ascendant order\n";
                //cancelEvent(timeoutEvent);
                // qui sto andando a capire su quale partizione il mapper ha lavorato e aggiorno l'array che tiene traccia di tutte le partizioni su cui si è lavorato
                int actualPartition = actualPartitionFunction(msgArrived->getPartition());
                arrayNodes[actualPartition] = -1; // vado a dire che questa partizione è stata completata con successo.
                // quì va a trovare la prossima partizione su cui il nodo deve lavorare
                int nextPartition = findNextPartition(msgArrived->getPartition());
                // vado a settare nell'array che tiene traccia di tutte le partizione completate e assegnate la nuova partizione su cui il nodo deve lavorare
                arrayNodes[nextPartition] = msgArrived->getNodeNumber();

               // scheduleAt(simTime() + timeout, timeoutEvent);
                simtime_t currentTime = simTime();//acquisco il tempo corrente
                // Se il nodo da cui mi è arrivato il messaggio deve lavorare su ulteriori partizioni
                if (nextPartition != -1)
                {
                    EV << "The number of the next partition assigned to mapper : "<<msgArrived->getNodeNumber()<<" is " << nextPartition << " and its name is "<<partitionLocations[nextPartition]<<"\n";
                    timesMessages[msgArrived->getNodeNumber()]=currentTime;
                    generateMessage(partitionLocations[nextPartition], operationsToDoArray[globalIndexOfOperation], msgArrived->getNodeNumber());
                }

                else
                {
                    EV << "Since no other partitions can be assigned to the mapper "<<msgArrived->getNodeNumber()<<" its work is done for now";
                }
                printMessageTimes();
            }
            else
            {

                // situazione in cui alcuni nodi sono falliti mentre altri no.
               // EV << "Alcuni nodi sono falliti e quindi devo andare a gestire le partizioni in maniera differente\n";
                ackMessage *msgArrived = check_and_cast<ackMessage *>(msg);
                // significa che il nodo che ha inviato il messaggio ha completato il suo lavoro
                timesMessages[msgArrived->getNodeNumber()]=-1;
                EV << "i'm the coordinator and i received a message from mapper " << msgArrived->getNodeNumber() << ", the partition on which the work was performed is:  " << msgArrived->getPartition() << "\n";
               // cancelEvent(timeoutEvent);
                //Trovo la prossima partizione
                int actualPartition = actualPartitionFunction(msgArrived->getPartition());
                arrayNodes[actualPartition] = -1; // vado a dire che questa partizione è stata completata con successo.
                //scheduleAt(simTime() + timeout, timeoutEvent);
                //if (indexForPartitionElaborated != -1)
                //{

                simtime_t currentTime = simTime();//acquisco il tempo corrente
                // dato che alcuni nodi sono falliti le partizioni non possono essere più assegnate a multipli del indice del nodo, e quindi devo trovare un altro modo per assegnarle
                int partition = reasseamblePartitions();
                if (partition != -1)// Il nodo deve lavorare su un'altra partizione o sono già state tutte assegnate o completate?
                {
                    timesMessages[msgArrived->getNodeNumber()]=currentTime;
                    EV << "The number of the next partition assigned to mapper : "<<msgArrived->getNodeNumber() << " is " << partition << " and its name is "<<partitionLocations[partition]<<"\n";
                    arrayNodes[partition] = msgArrived->getNodeNumber();
                    generateMessage(partitionLocations[partition], operationsToDoArray[globalIndexOfOperation], msgArrived->getNodeNumber());
                    //nodeMessageSent[indexForPartitionElaborated]=msgArrived->getNodeNumber();
                }
                else
                {
                    EV << "Since no other partitions can be assigned to the mapper "<<msgArrived->getNodeNumber()<<" its work is done for now";
                }
                printPartitions();
                EV<<"\n";
                printMessageTimes();
               // }
                // checkRemainingPartitions();
            }
        }
        backOnline *newMessageArrived = dynamic_cast<backOnline *>(msg);

        if (newMessageArrived != nullptr && strcmp(operationsToDoArray[globalIndexOfOperation], "reduce")!=0)// siamo nella situazione il cui uno dei nodi che sono falliti, sono tornati online.
        {

            executionMode=1;
            backOnline *backOnlineMessage = check_and_cast<backOnline *>(msg);
            EV<<"I'm the coordinator and i received a Back Online message from " << backOnlineMessage->getNodeNumber()<<"\n ";
          //  EV << "back onlineMessage from: " << backOnlineMessage->getNodeNumber()<<"\n ";
            int nodeNumber=backOnlineMessage->getNodeNumber();
           // backOnlineNodes[nodeNumber]=1;
            //Dato che il nodo è tornato online devo andare a dirlo anche all'array che tiene traccia dei nodi falliti.
            failedNodes[nodeNumber]=1;
            printFailedNodes();
            printMessageTimes();
         //   scheduleAt(simTime() + timeout, timeoutEvent);
            simtime_t currentTime = simTime();//acquisco il tempo corrente
            // Vado a cercare l'ultima partizione che era stata assegnata al nodo.
           int partition = reasseamblePartitionsWhenNodesComeBack(backOnlineMessage->getNodeNumber());
           if (partition==-1){
               // Se l'ultima partizione che era stata assegnata al nodo che è tornato online è già stata riassegnata, allora gli cerco un altra partizione
               partition=reasseamblePartitions();
           }
            if (partition != -1)
            {
                // Vado a settare il time clock a cui viene inviato il messaggio al nodo.
                timesMessages[backOnlineMessage->getNodeNumber()]=currentTime;
                EV << "This mapper has to perform a computation on the partition number " << partition << " that correspond to " << partitionLocations[partition] << "\n";
                arrayNodes[partition] = backOnlineMessage->getNodeNumber();
                generateMessage(partitionLocations[partition], operationsToDoArray[globalIndexOfOperation], backOnlineMessage->getNodeNumber());
                //nodeMessageSent[indexForPartitionElaborated]=msgArrived->getNodeNumber();

            }
            else
            {
                // Nel caso in cui non ci siano più partizione da elaborare allora il nodo rimarrà inattivo
                timesMessages[backOnlineMessage->getNodeNumber()]=-1;
                //EV << "no more partition to elaborate, your work is done";
            }
            printPartitions();
            printMessageTimes();
        }
        shufflerAckCoordinatorMessage *msgArrived = dynamic_cast<shufflerAckCoordinatorMessage *>(msg);
        if (msgArrived!=nullptr){
            EV<<"Message arrived from shuffler"<<"\n";
            EV<<"Partition: "<<msgArrived->getPartitionToReduce()<<" assigned to reducer: "<<msgArrived->getReducerSent()<<"\n";
           // partitionReducerAssociation[msgArrived->getPartitionToReduce()] = msgArrived->getReducerSent();
            int partitionIndex=findPartitionReduced(msgArrived);
            reducersPartitionAssociation[partitionIndex]=msgArrived->getReducerSent();
            reducersActive[msgArrived->getReducerSent()]=-1;

        }

         reducerAckCoordinatorMessage *msgArrived3 = dynamic_cast<reducerAckCoordinatorMessage *>(msg);
                if (msgArrived3!=nullptr){
                    EV<<"Message arrived from reducer"<<"\n";
                    EV<<"Partition: "<<msgArrived3->getPartitionReduced()<<"processed by reducer: "<<msgArrived3->getReducerSender()<<"\n";
                    reducersActive[msgArrived3->getReducerSender()]=1;
                   int partitionIndex=findPartitionReduced2(msgArrived3);
                    reducersPartitionAssociation[partitionIndex]=-1;
                }
          backOnlineReducer *msgArrived4 = dynamic_cast<backOnlineReducer *>(msg);
          if (msgArrived4!=nullptr){
              EV<<"Back online message received from reducer: "<<msgArrived4->getNodeNumber()<<"\n";
              reducersActive[msgArrived4->getNodeNumber()]=1;
           }
    }
    else// mi è arrivato il messaggio di timeout
    {

        EV << "A timeout event is arrived at the Coordinator"<<"\n";
         //  EV<<"Execution mode"<<executionMode<<"\n";--> SCOMMENTA SE VUOI VEDERE IN CHE EXECUTION MODE SEI
        // delete msg;
        //executionMode=1;--> DOPO RICORDATI DI RIMETTERLO
        if (workDone())// controllo se tutte le partizione sono state elaborate
        {
            EV<<"All the partition have been processed correctly so perform next operation on the list"<<"\n";
            globalIndexOfOperation++;
            EV<<"The next operation to perform is "<<operationsToDoArray[globalIndexOfOperation]<<"\n";
            if ( strcmp(operationsToDoArray[globalIndexOfOperation], "reduce")==0   ){
                executionMode=2;
                nextOperationReduce();
            }
            // esegui prossima operazione sulla lista: map, map, change function, reduce ecc...
            else{
                nextOperation();
            }

        }
        else if (executionMode==3){
            if (workDoneReducer()){
                   EV<<"All reducers partitions has been elaborated"<<"\n";
                   printReducedPartitions();
                   printReducersActive();

            }
            else{
                scheduleAt(simTime() + timeout, timeoutEvent);
                printReducedPartitions();
                printReducersActive();
                if (!allReducersDown()){
                    findPartitionNotElaborated();
                    resetReducerPartitions();
                    printReducedPartitions();
                    printReducersActive();
                }
                else{
                    EV<<"all reducers down, WAIT"<<"\n";
                }

                //reducersActive.clear();

            }
        }
        // fa il primo giro della reduce
        else if (executionMode==2){
            EV<<"Performing the shuffling operation"<<"\n";
            for (int i = 0; i <num_of_partitions; i++)
            {
                int shufflerToSend=i%numberOfShuffler;
                EV<<"Sending data to the shuffler: "<<shufflerToSend<<"\n";
                shufflerMessage(partitionLocations[i], operationsToDoArray[globalIndexOfOperation],shufflerToSend);
            }
            reducersActive.clear();
                    for (int i=0;i<numberOfReducers;i++){
                            reducersActive[i]=-1;
                        }
            executionMode=3;
            scheduleAt(simTime() + timeout, timeoutEvent);
        }
        else if (executionMode==0 || executionMode==1)
        {
            printMessageTimes();
            scheduleAt(simTime() + timeout, timeoutEvent);
            EV << "Not all the partitions have been elaborated so i need to rearrange them on the remaining mappers\n";
            //Una volta scaduto il timeout il coordinator deve andare a vedere quali nodi sono ancora attivi e quali invece sono falliti.
            createFailedArray();
            printFailedNodes();

            for (int i = 0; i < numberNode; i++)
            {
                if (failedNodes[i] != 0 && checkNodeAssegnedAlready(i)==0)
                {
                    // Trovo una partizione per il nodo
                    int partition = reasseamblePartitions();
                    if (partition != -1)
                    {
                        EV<<"This partition is not been elaborated: "<<partitionLocations[partition]<<" and since this mapper: "<<i<<" is not busy, he can try to perform the operation"<<"\n";
                       // EV << "value of index " << partition << " " << partitionLocations[partition] << "\n";
                        simtime_t currentTime = simTime();//acquisco il tempo corrente
                        timesMessages[i]=currentTime;// Setto il tempo a cui al nodo viene inviato il messaggio
                        generateMessage(partitionLocations[partition], operationsToDoArray[globalIndexOfOperation], i);
                        arrayNodes[partition] = i;// in questo modo vado a settare nell'array che tiene traccia delle partizioni assegnate e completate a quale nodo la partizione è stata assegnata
                    }
                    else
                    {
                        //EV << "No more partitions need to be processed in this moment";
                    }
                }
            }
            printPartitions();
            printMessageTimes();
          //  checkRemainingPartitions();
        }
    }
}
void Coordinator::resetReducerPartitions(){
    for (int i=0;i<10;i++){
        if (reducersPartitionAssociation[i]!=-1){
            reducersPartitionAssociation[i]=-2;
        }

    }
}
int Coordinator::findPartitionReduced(shufflerAckCoordinatorMessage *msgArrived ){
for (int i=0;i<10;i++){
    const char*name=partitionsNames[i].c_str();
    if (strcmp(msgArrived->getPartitionToReduce(),name)==0){
        return i;
    }
}

}
int Coordinator::findPartitionReduced2(reducerAckCoordinatorMessage *msgArrived ){
for (int i=0;i<10;i++){
    const char*name=partitionsNames[i].c_str();
    if (strcmp(msgArrived->getPartitionReduced(),name)==0){
        return i;
    }
}

}
void Coordinator::findPartitionNotElaborated(){
    int indexShuffler=0;
    for (int i=0;i<numberOfReducers;i++){

    if (reducersActive[i]==-1){
        //EV<<"sono qui"<<"\n";
        for (int j=0;j<10;j++){
            if (reducersPartitionAssociation[j]==i){
                const char*name=partitionsNames[j].c_str();
                shufflerMessageModified(name, operationsToDoArray[globalIndexOfOperation],indexShuffler%numberOfShuffler);
                EV<<"Partition number "<<j<<" not elaborated "<<"so sending it to the shuffler"<<indexShuffler%numberOfShuffler<<"\n";
               // reducersPartitionAssociationTemp[j]=indexShuffler%numberOfShuffler;
                //reducersActive[indexShuffler%numberOfShuffler]=-1;
                indexShuffler++;
            }
        }
    }
}

}
void Coordinator::createPartitions(char absolutePath[100])
{
    FILE *fptr;
    fptr = fopen(absolutePath, "w");
    for (int i = 0; i < key_value_generated; i++)
    {
        int key = rand() % 10;
        int value = rand() % 50;
        char keyValuePair[20];
        sprintf(keyValuePair, "%d %d", key, value);
        fprintf(fptr, "%s\n", keyValuePair);
    }
    fclose(fptr);
}

void Coordinator::generateMessage(char partition[100], char operationToDo[20], int nodeToSend)
{
    // Create message object and set source and destination field.
    OperationMessage *msg = new OperationMessage("MsgMapper");

    msg->setOperationToDo(operationToDo);
    msg->setNodeToSend(nodeToSend);
    msg->setPartitionToRead(partition);

   send(msg, "out", nodeToSend);
}

void Coordinator::shufflerMessage(char partition[100], char operationToDo[20], int nodeToSend)
{
    // Create message object and set source and destination field.
    ShuffleMessage *msg = new ShuffleMessage("MsgShuffler");
    const char *operation="reduce";
    msg->setOperationToDo(operation);
    msg->setNodeToSend(nodeToSend);
    msg->setPartitionToRead(partition);
    for (int i=0;i<numberOfReducers;i++){
        int value=reducersActive[i];
        msg->setReducerAvailable(i, value);
    }
    // definisco la dimensione del messaggio che sto per inviare
    msg->setPartitionElaboratedArraySize(10);
    for (int i=0;i<10;i++){

     msg->setPartitionElaborated(i, -2);
    }
    msg->setCreatePartition(true);
    send(msg, "outShuf", nodeToSend);


}
void Coordinator::shufflerMessageModified(const char partition[100], char operationToDo[20], int nodeToSend)
{
    // Create message object and set source and destination field.
    ShuffleMessage *msg = new ShuffleMessage("MsgShuffler");
    const char *operation="reduce";
    msg->setOperationToDo(operation);
    msg->setNodeToSend(nodeToSend);
    msg->setPartitionToRead(partition);
    for (int i=0;i<numberOfReducers;i++){
        int value=reducersActive[i];
        msg->setReducerAvailable(i, value);
    }
    // definisco la dimensione del messaggio che sto per inviare
    msg->setPartitionElaboratedArraySize(10);
       for (int i=0;i<10;i++){
           if (  reducersPartitionAssociation[i]>=0){
               msg->setPartitionElaborated(i, -2);
           }
           else{
               msg->setPartitionElaborated(i, -1);
           }

       }
       msg->setCreatePartition(false);
    send(msg, "outShuf", nodeToSend);


}
int Coordinator::checkLettersDrawn(char letterUsed[], char caracterDrawn)
{
    int seen = 0;
    for (int i = 0; i < 26; i++)
    {
        if (strcmp(&letterUsed[i], " ") != 0)
        {
            if (strcmp(&letterUsed[i], &caracterDrawn))
            {
                seen = 1;
            }
        }
    }
    return seen;
}
int  Coordinator::countNumberOfOperations(){
int count=0;
nlohmann::json json;

     std::ifstream file("C:\\Users\\spagn\\Desktop\\prova\\file.json");

     if (file.is_open()) {
         file >> json;
         file.close();
     } else {
         EV << "Failed to open the JSON file." << "\n";

     }

     // Extract number of partitions from the JSON structure

     int partitions = json["partitions"];

     // Access and manipulate the JSON data
     for (const auto& map : json["map"]) {
         int MapId = map["id"];
         count++;
     }


for (const auto& red : json["changekey"]) {
  count++;
}

for (const auto& red2 : json["reduce"]) {
  count++;
 }
return count;
}
