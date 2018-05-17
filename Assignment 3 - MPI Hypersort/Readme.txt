Lab Report 4
Ankush Phulia 
2014CS50279

Compilation : "bash compile.sh"
Run : "mpirun hypersort <input_file.txt> -np <number of processors>"

Basic Implementation follows the hypersort design as described in the paper

Design Decisions : 
	1. Use of MPI_Scatter over multiple broadcasts/send-receives to intially distribute the load
	2. Communicators are split into halves at each level to facillitate the ease of pairing up processes

Parallelization Strategy :
	1. All the processes sorts its sub-array in parallel
	2. Moreover, paired arrays can exchange their high and low portions simultaneously
	3. MPI_Scatter ensures that distribution of data can be done in parallel,broadcasting via MPI_Broadcast also ensures this

Load Balancing : 
	1. Initially the array is divided equall amongst all processes
	2. Using a median of a sub-array(of process 0 in our case), ensures that that the pivot value is more likely to be closer to the true median than an arbitrarily chosen pivot. More equitable division of list ensures after each phase, no process is likely overloaded

Results :
1. Time vs Procs
	Procs : 1		Time : 7.66906
	Procs : 2		Time : 5.11508
	Procs : 4		Time : 2.19754
	Procs : 8		Time : 1.2007
	Procs : 16		Time : 0.710012
	Procs : 32		Time : 4.58364
2. Time vs Payload
	Payload : ~2^10		Time : 0.00302
	Payload : ~2^18		Time : 0.00726
	Payload : ~2^32		Time : 2.15