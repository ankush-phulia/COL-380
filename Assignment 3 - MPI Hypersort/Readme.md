## COL 380 - Report 3

#### Running the code

##### Compiling

```bash
bash scripts/compile.sh
```

##### Running

```bash
mpirun sort <input_file.txt> -np <number of processors>
```



#### Code Description

##### Design Decisions 

  1. Use of MPI_Scatter over multiple broadcasts/send-receives to intially distribute the load
  2. Communicators are split into halves at each level to facillitate the ease of pairing up processes

##### Parallelization Strategy

  1. All the processes sorts its sub-array in parallel
  2. Moreover, paired arrays can exchange their high and low portions simultaneously
  3. MPI_Scatter ensures that distribution of data can be done in parallel,broadcasting via MPI_Broadcast also ensures this

##### Load Balancing 

  1. Initially the array is divided equal amongst all processes
  2. Using a median of a sub-array(of process 0 in our case), ensures that that the pivot value is more likely to be closer to the true median than an arbitrarily chosen pivot. More equitable division of list ensures after each phase, no process is likely overloaded

#### Results

##### **Time vs Procs**

| Processes | Time(s) |
| :-------: | :-----: |
|     1     |  7.669  |
|     2     |  5.115  |
|     4     |  2.197  |
|     8     |  1.201  |
|    16     |  0.710  |
|    32     |  4.583  |

##### Time vs Payload

| Payload | Time(s) |
| :-----: | :-----: |
|  2^10   | 0.00302 |
|  2^18   | 0.0072  |
|  2^32   |  2.15   |
