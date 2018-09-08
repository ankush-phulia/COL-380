## COL 380 - Report 1

#### Running the code

##### Compiling

```bash
bash scripts/compile.sh
```

##### Running

```bash
./par <number of threads, a power of 2 assumed>
```

*Ensure that "input.txt" is in the same directory* 



#### Code Description

##### The key idea

* Balanced binary trees work well as a parallelisation strategy for a variety of algorithms, including prefix sum.
* Given that our input sizes are powers of 2, we can assume number of cores available as a power of 2 as well (or round down to the nearest one).

##### The algorithm

* 2 phases: upsweep and downsweep - 
  * In upsweep we add array elements separated by 0 elements, then 1 element, then 2, then 4 and so on.
  * In downsweep, we swap 2 elements, and also update the value on the larger indexed one ie. a[i] = a[j] & a[j] += a[i], j>i. These elements are n/2, n/4 and so on elements apart.
* This gives us the exclusive prefix sum, that is first element is 0 and last element is omitted (array shifted by 1).
* At each stage of addition or swap/addition, we can do (with log(n) steps each phase) n/2, n/4, n/8, etc parallel operations in upsweep, and 1,2,4, etc. during downsweep
  Run Time : O(n/p + lg p) p = number of processors
  Memory Need : O(n) -Just store the input array, rest in place

##### Implementation

* Used a global array common for all threads, and algorithm runs on this in place, assuming no issues with memory.
* Since upto k threads allowed, array divided into k segments initially, and all our operations (seperated adds, and add/swaps) occur within a segment in sequence. 
  After one step of parallel operations, we join the threads and respawn them, with increased(in upsweep) or decreased(in downsweep) step size(seperation).

##### Parallelisation

* Pthreads are used, with pointers to each kept in an array, in each step (log(n) - 1 steps, where separation doubles or halves), at most 2^(log(k)-1) processors needed for maximum parallelisation. 

* If p is less, then each shares the excess work to be done (which is done in serial). 

**Load balancing** 

* Initially, all (upto n/2) threads (in upsweep) share the equal load, but as soon as they have to add numbers seperated by more than segment size, some threads don't need to work. 
* Similarly in downseep, intially only 1 thread works, but this doubles every step till all threads are working