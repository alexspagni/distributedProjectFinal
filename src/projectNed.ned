//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

//package projectdistributedsystem;

//@license(LGPL);
//package projectdistributedsystem;
simple Coordinator
{
    parameters:
     //   int numberOfWorkers = default(7);
        int numberOfShuffler=default(1);
        int numberOfReducers=default(10);
        int keyValuePairs=default(2);
        double totalExecutionTime @unit("s") = default(0s);
        int totalNumberOfMessagesCoordinator=default(0);
        int numberOfWorkers= default(10);
    gates:
        input in[]; //mapIn
        output out[]; //mapOut
        input inRedux[];
        output outShuf;
        input inShuffler;
}


simple MapperNode
{
    parameters:
        int n = default(7);
        int probabilityOfFailure=default(20);
    gates:

        input in; //mapIn
        output out; //mapOut
}

simple Shuffler
{
    parameters:
        //int n = default(7);
        int numOfShuffler=default(1);
        int numOfReducer=default(10);
    gates:

        input in; //in coordinator
        output out[]; //out coordinator
         output outCoordinator;// out Coordinator
}

simple Reducer
{
    parameters:
       // int n = default(7);
        int probabilityOfFailure=default(20);
    gates:

        input in; //shuffler in
        output out; //out Coordinator
}


network DistributedProject2
{
    parameters:
        int numberOfWorkers;
        int numberOfShuffler;
        int numberOfReducer;
        double executionTimeShufflerReducer @unit(ms);
        double executionTimeCoordinatorOtherComponent @unit(ms);
		
        @display("bgb=429,164");
    submodules:
        Map[numberOfWorkers]: MapperNode {
            @display("p=51,76");
        }
        Cor: Coordinator {
            @display("p=244,101");
        }
        Shuf: Shuffler {
            @display("p=301.56,26.04");
        }
        redux[numberOfReducer]: Reducer {
            @display("p=339.36002,115.08");
        }
    connections:

        for i=0..numberOfWorkers-1 {
            //collegamenti tra coordinator e mapper
            Cor.out++ --> {  delay = parent.executionTimeCoordinatorOtherComponent; } --> Map[i].in;
            Map[i].out --> {  delay = parent.executionTimeCoordinatorOtherComponent; } --> Cor.in++;
           
        }
       
            Cor.outShuf --> {  delay = parent.executionTimeCoordinatorOtherComponent; } --> Shuf.in;
            Shuf.outCoordinator--> {  delay = parent.executionTimeShufflerReducer; }--> Cor.inShuffler ;
            
        
        for i=0..numberOfReducer-1 {
         
            //collgamenti da reducer a corrdinator
            redux[i].out --> {  delay = parent.executionTimeCoordinatorOtherComponent; } --> Cor.inRedux++;
            
        }
       
         for i=0..numberOfReducer-1 {
         
             //Collegamenti da shuffler ad ogni reducer
            Shuf.out++--> {  delay = parent.executionTimeShufflerReducer; }--> redux[i].in ;


            
            
        }
    
}
