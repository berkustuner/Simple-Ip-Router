#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* TO-DO



1- Packet Generator
2- Enqueue, dequeue, drop functions
3- Congestion radio parameter
4- Ask for CR and time.
5. While executing, the code should print below router statistics to the screen every 1 second.
- queue sizes,
- number of packets dropped
- number of high/low priority packets routed successfully
- number of bytes routed successfully

Router has a total 4MB memory. 1MB for each port.

Ports:
10.0.20.x -> port 1
10.0.12.x -> port 2
10.0.50.x -> port 3
10.0.70.x -> port 4

Packet Length = between 100 and 1500 bytes. (will be chosen randomly)
Packet Priority Randomly 0 , 1.

1MB = 1,048,576 bytes

Hexadecimals:
10 => A
0 => 0
12 => C
20 => 14
50 => 32
70 => 46

//!!here i could not figure out how I can separate like 10(A).0(0).20(14). so I took the ip address whole like 10020 then converted it to hex
10.0.20 = 0x2724

It did not make sense when I did it like this so i didnt.
I just used the different parts of IP like 20,12,50,70 to separate the ports.

I want to mention that when the code is executed with low congestion Ratio it gives both high and low prio packets.
But when the congestion Ratio is high all the packets become high prio packets because of the drop function. If you choose simulation time long,
you get all high prio's again. You can see every second what is going on anyways I just wanted to mention it because most cases they come all high prio.

I want to add that according to the congestion ratio, I run the enqueue func first then the dequeue func.
I figured out that this may be the reason it all becomes high prio packets. Because I use the drop function inside of the enqueue function.
I realized my mistake I should have made a function that simplifies the enqueue/dequeue ratio. I assigned 100 to enqueue rate so it will be easier user to
input just the congestion rate then the dequeue rate will be 100 - congestion ratio. Then I will run the enqueue 100 times and dequeue (100 - congestion ratio) times.


Hocam kodumda bir tane sýra var fakat bu sýra her port için 1MB tutup yine port iþlevi görüyor. Dequeue sýrasý da head to tail çalýþýyor.

*/

// Functions that will be used
void enqueue();
void dequeue();
void drop();
//



// Structure for holding the packets' datas
struct packet{
    int dst_ip;
    int packet_len;
    int priority;
};
//

// multi-dimensional array for queue and ports' memories
int queue[10000][3];// since 1MB roughly equal to 1,000,000 bytes, if the packet length minimum is 100, 1,000,000/100 = then we will need maximum of 10,000 queue length.
int port1=0, port2=0, port3=0, port4=0;// at first all the ports' memories are zero.
//

// Variables for intended prints
int droppedPackages = 0;// variable for counting dropped packages. I will use this in an another func than main so I put this here in order it to work.
int lowPrioCount = 0;// to calculate the numbers of packets routed successfully
int highPrioCount = 0;
int bytesRouted = 0; // to calculate the number of bytes routed successfully
int queueSize = 0;// to calculate the queue size
//


// Variables needed for calculating the time
clock_t t1,t2;
long executionTick;// execution tick and time are for calculating every 1 second in order to print every 1 second
double executionTime;
long simulationTick=0;// simulation time will hold the total time passed in order to compare it to input time given so it will stop after it reaches that.
double simulationTime;
double totalSimulationTime;
//



// *****MAIN FUNCTION*****
int main()
{
    srand(time(NULL));// to use the random function
    int ports[] = {20,12,50,70}; // port array. later we will compare the packets ip address to these ports.

// Random Packet Generator
    //int i = rand()%4;    //variable for picking random port. will use this in form of array[i].
    //int packet_len = rand()%1401 + 100;    // random packet length between 100 - 1500
    //int priority = rand()%2;    // random priority low or high(0-1)
    //struct packet newPacket = {ports[i], packet_len, priority};
//


// Making the queue all zeros so it will be easier to arrange it later.
    for(int i=0; i<=10000; i++){
        for(int j=0; j<3; j++){
            queue[i][j] = 0;
        }
    }
//

// Congestion Rate
   int congestionRatio;
   int enqueueRate = 100; /* I gave enqueueRate to be 100. So when the user inputs congestion rate dequeueRate will be 100 - congesRat.
   Then the CR = (enqRate-deqRate)/enqRate * 100 = %CR will be satisfied.*/
   int dequeueRate;

   printf("Enter congestion ratio: ");
   scanf("%d", &congestionRatio);
   dequeueRate = enqueueRate - congestionRatio;
//

// Total Simulation Time
    printf("How long do you want router to run(in seconds): ");
    scanf("%lf",&totalSimulationTime);
//




// Our MAIN Loop. It includes enqueue, dequeue and drop functions. Also it prints every second without waiting.
    while(simulationTime<=totalSimulationTime){
        long t1 = clock();
        //printf("\n%d %d %d %d\n", port1, port2, port3, port4);
        // ENQUEUE PART
        for(int count = 0; count<enqueueRate; count++){
            int i = rand()%4;    //variable for picking random port. will use this in form of array[i].
            int packet_len = rand()%1401 + 100;    // random packet length between 100 - 1500
            int priority = rand()%2;    // random priority low or high(0-1)
            struct packet newPacket = {ports[i], packet_len, priority};// new packet generated
            enqueue(newPacket);
        // ENQUEUE PART ENDS
        }

        // DEQUEUE PART
        for(int count = 0; count<dequeueRate; count++){
            dequeue();
        }
        //

        // FINDING THE QUEUE SIZE
        queueSize = 0; // I assign this to zero because if I dont queueSize will stack after each while loop
        for(int i=0; i<=10000; i++){
            if(queue[i][0]!=0){
            queueSize++;
            }
        }
    // FINDING THE QUEUE SIZE END

        // Printing the values part ==>
        /*So I tried to get the execution time for my each value. Then I constantly added up the exe. time until it reaches 1s.
         If exe. time >= 1 then it prints. After that I assigned it to 0 seconds again. This way I did not delay the main function. */
        long t2 = clock();

        simulationTick += (float)(t2-t1);
        simulationTime = (double)simulationTick/CLOCKS_PER_SEC;

        executionTick += (float)(t2-t1);
        executionTime = (double)executionTick/CLOCKS_PER_SEC;

        if(executionTime>=1){
            printf("\n Queue Size: %d\n",queueSize);
            printf("\n Dropped Packages: %d\n",droppedPackages);
            printf("\n Low Prio Packages: %d\n",lowPrioCount);
            printf("\n High Prio Packages: %d\n",highPrioCount);
            printf("\n Number of Bytes Routed: %d\n",bytesRouted);
            executionTick=0;
        }
        // Printing part end


    }





    // You can check here to know if it is working properly
        /*printf("%6s %6s %14s \n","Port" ,"PacketLength", "Priority");
        for(int i=0; i<100; i++){
          printf("%5d %5d %15d\n", queue[i][0], queue[i][1], queue[i][2]);
        }*/
    //





    return 0;

}
// *****MAIN FUNCTION ENDS*****



// Drop Function

void drop(struct packet a){
    if(a.priority==0){ // if incoming packet is low priority it will be instantly dropped
        droppedPackages++;
    } else if(a.priority==1){ // if incoming packet is high priority it will check if there are low priority packages in the queue
        for(int j=9999; j>=0; j--){
            if(queue[j][0]!=0 && queue[j][2]==0){
                 switch(queue[j][0]){ /* this will replace the low prio packet with incoming high prio packet.
                     I started the for loop from 9999 so it will run tail to head. */
                    case 20:
                        port1=(port1-queue[j][1]+a.packet_len);
                        queue[j][0] = a.dst_ip;
                        queue[j][1] = a.packet_len;
                        queue[j][2] = a.priority;
                        break;
                    case 12:
                        port2=(port2-queue[j][1]+a.packet_len);
                        queue[j][0] = a.dst_ip;
                        queue[j][1] = a.packet_len;
                        queue[j][2] = a.priority;
                        break;
                    case 50:
                        port3=(port3-queue[j][1]+a.packet_len);
                        queue[j][0] = a.dst_ip;
                        queue[j][1] = a.packet_len;
                        queue[j][2] = a.priority;
                        break;
                    case 70:
                        port4=(port4-queue[j][1]+a.packet_len);
                        queue[j][0] = a.dst_ip;
                        queue[j][1] = a.packet_len;
                        queue[j][2] = a.priority;
                        break;
                }
            }
        }
        droppedPackages++;
    }

}
// Drop Function Ends


//Enqueue Function
void enqueue(struct packet a){
    for(int i=0; i<10000; i++){
        if(queue[i][0]==0){ // finds the first all zero line in the queue array(which means empty) and fills it then breaks.
            switch(a.dst_ip){  // switch for updating the queue memory for each port
            case 20:
                if(port1+a.packet_len<=1000000){// checking if the last packet exceeds the memory limit
                port1+=a.packet_len;
                queue[i][0] = a.dst_ip;
                queue[i][1] = a.packet_len;
                queue[i][2] = a.priority;
                break;}else{ // if it exceeds the 1000000 byte memory limit it runs the drop function
                drop(a);
                break;
                }
            case 12:
                if(port2+a.packet_len<=1000000){
                port2+=a.packet_len;
                queue[i][0] = a.dst_ip;
                queue[i][1] = a.packet_len;
                queue[i][2] = a.priority;
                break;} else{
                drop(a);
                break;
                }
            case 50:
                if(port3+a.packet_len<=1000000){
                port3+=a.packet_len;
                queue[i][0] = a.dst_ip;
                queue[i][1] = a.packet_len;
                queue[i][2] = a.priority;
                break;} else{
                drop(a);
                break;}
            case 70:
                if(port4+a.packet_len<=1000000){
                port4+=a.packet_len;
                queue[i][0] = a.dst_ip;
                queue[i][1] = a.packet_len;
                queue[i][2] = a.priority;
                break;} else{drop(a); break;}
            }


            break;
        }
    }

}
// Enq Func Ends




// Dequeue Function
void dequeue(){
    for(int i=10000; i>=0; i--){
        if(queue[i][0]!=0){
            switch(queue[i][0]){ // memory clearing since it dequeues
                case 20:
                    port1-=queue[i][1];
                    break;
                case 12:
                    port2-=queue[i][1];
                    break;
                case 50:
                    port3-=queue[i][1];
                    break;
                case 70:
                    port4-=queue[i][1];
                    break;
            }
        if(queue[i][2]==0){ // high-low prio count
            lowPrioCount++;
        }else if(queue[i][2]==1){
            highPrioCount++;
        }
        bytesRouted+= queue[i][1];
        queue[i][0] = 0; // making it 0's again so it will be considered as empty
        queue[i][1] = 0;
        queue[i][2] = 0;
        break;
        }
    }
}
// Dequeue Func Ends

// Simplification Function
void simplfy(int enqueue, int dequeue){


}



//

