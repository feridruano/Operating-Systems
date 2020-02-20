## Lab 03 Description

Develop a program that creates a three level hierarchy of processes.

#### Process Tags (Numbers)

- **"1"** annotates the root process.
  - **"1.1"** annotates the first child of **"1"**.
    - **"1.1.1"** annotates the first child of **"1.1"**.
    - **"1.1.2"** annotates the second child of **"1.1"**.
  - **"1.2"** annotates the second child of **"1"**.
    - **"1.2.1"** annotates the first child of **"1.2"**.
    - **"1.2.2"** annotates the second child of **"1.2"**.



#### Notes

Root process waits until all children processes are terminated.

All children and grandchildren process must morph into **iam** process.

​	**iam** processes is the  ?

 Use **proc_hier.c** to call **execlp()** to programmatically activate the **iam** program.

**Process Tags** are created by passing command line arguments to the **iam** program.



#### Running Program

1. Open 4 terminals and record each session individually.
2. Choose a **root** terminal to execute `$./proc_hier`.
3. Switch to a second terminal and use $ `ps -elf | grep "proc_hier\|iam"` command to find all **iam** processes. Then, verify the process hierarchy.
4. Use `gdb` on the remaining terminals to attach the two children (1.1, 1.2) of the root.
   1. `$ sudo gdb -p <process id> iam`
   2. Then, change the variable value of **i** in each terminal to break the endless loop.
   3. `gdb> set var i=1`
   4. Continue running the process
   5. `gdb> continue`
   6. Note that the process will be blocked when it executes the subsequent **waitpid()** command.
5. Back on the second terminal, use `kill` command to terminate the two **iam** processes that are of child process **1.1**.
   1. `$ kill -KILL <process id of 1.1.1> <process id of 1.1.2>`
6. On the terminal with child **1.1** attached, verify that the processes unblock and terminate after its children are terminated.
7. Repeat step 5 and 6, but for child process **1.2**.
   1. `$ kill -KILL <process id of 1.2.1> <process id of 1.2.2>`
   2. Verify that the first terminal (root proc_hier) unblocks and terminates after both of its children are terminated.
8. Exit scripting in all terminals



### Programming Lab Notes

`fork` is a system call that makes a complete copy of the running processes. The return value allows use to determine whether the new process is a child (= 0), parent (> 0), or error (< 0).

`exec` is a system call that is used after a `fork` to replace the process's memory space with a new program. The new process becomes the "currently running process".

`getpid` returns the process id of the calling process (parent).

**Process Termination**

Process asks operating system to **exit** (delete it).

- Process passes output data from child to parent via **wait**.
- Process's resources are deallocated.

Parent may terminate child processes (**abort**)

- Child process exceeds allocated resources.
- Child process is no longer required.
- Some operating systems terminate children processes if parent is terminated.

Terminated process may enter "zombie" state while operating system is performing termination-related cleanup.