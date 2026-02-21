# Experimental protocol for C-State benchmark

We will be using `turbostat` to record data. This tool is only available on `x86-64` arch. 

We will record the following metrics :

- residency in each Cstate (software) : % of time spend in the given Cstate
- same as above but with **hardware** residency
- amount of requests for each Cstate : # times linux requested a given Cstate
- energy : ? (to be defined)

We will use the 3 following load types :

1. CPU load : count how many prime numbers inferiors to `N_cpu`
2. memory load : performs a pointer chase for `N_mem` jumps 
3. I/O load : writes `N_io` times 500 MB of random data into a file

Each load function will be run `loop` times, sequencially. The data read by turbostat will be exported to `.txt` file. 

The outline of the experiment is :

1. Definition of parameters (`N_cpu`, `N_mem`, `N_io`, `loop`)
2. Set a cpu idle governors 
3. Set a cpu affinity
4. Run all load test with turbostat recording
5. Export results
6. Rerun the experiment with a different governor

## Note

The new version of `io_load` uses a pre made file of 1Go of random data from `/dev/urandom`. To generate it you can use :

    dd if=/dev/urandom of=random.bin bs=1M count=1024