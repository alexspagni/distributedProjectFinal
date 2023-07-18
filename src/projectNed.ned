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
        int n = default(7);
        int numberOfShuffler=default(5);
    gates:
        input in[]; //mapIn
        output out[]; //mapOut
        input inRedux[];
        output outShuf[];
        input inShuffler[];
}


simple MapperNode
{
    parameters:
        int n = default(7);
    gates:

        input in; //mapIn
        output out; //mapOut
}

simple Shuffler
{
    parameters:
        int n = default(7);
        int numberOfShuffler=default(5);
    gates:

        input in; //mapIn
        output out[]; //mapOut
         output outCoordinator;
}

simple Reducer
{
    parameters:
        int n = default(7);
    gates:

        input in[]; //mapIn
        output out; //mapOut
}


network DistributedProject2
{
    parameters:
        int n = default(7);
		
        @display("bgb=429,164");
    submodules:
        Map[n]: MapperNode {
            @display("p=51,76");
        }
        Cor: Coordinator {
            @display("p=244,101");
        }
        Shuf[n]: Shuffler {
            @display("p=301.56,26.04");
        }
        redux[n]: Reducer {
            @display("p=339.36002,115.08");
        }
    connections:

        for i=0..n-1 {
            //collegamenti tra coordinator e mapper
            Cor.out++ --> {  delay = 1000ms; } --> Map[i].in;
            Map[i].out --> {  delay = 1000ms; } --> Cor.in++;
            //collgamenti da reducer a corrdinator
            redux[i].out --> {  delay = 1000ms; } --> Cor.inRedux++;
            //collegamenti da coordinator a shuffler
            Cor.outShuf++ --> {  delay = 1000ms; } --> Shuf[i].in;
            //Collegamenti da shuffler ad ogni reducer
            Shuf[i].out++--> {  delay = 10ms; }--> redux[0].in++ ;
            Shuf[i].out++--> {  delay = 10ms; }--> redux[1].in++ ;
            Shuf[i].out++--> {  delay = 10ms; }--> redux[2].in++ ;
            Shuf[i].out++--> {  delay = 10ms; }--> redux[3].in++ ;
            Shuf[i].out++--> {  delay = 10ms; }--> redux[4].in++ ;
            Shuf[i].outCoordinator--> {  delay = 10ms; }--> Cor.in++ ;
        }
        
    
}
