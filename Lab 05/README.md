### Lab 05 - Task Description

Matrix operations are fundamental tools in Computer Graphics (CG). CG objects can be represented as sets of vertices (i.e., *n*-dimensional vectors with *n* being usually 4), and can be transformed (for example to animate them) through a composition of matrix operations. Matrix multiplication is the most common operation that is repeated millions of times when the object passes through the geometric pipeline. For efficiency, many of the operations are done concurrently. Nowadays, that is done by multicore hardware on a video card, however, it all started as software libraries some years ago...

In this exercise you will get a taste of the concurrent processing using threads by recreating the history: your task is to implement multiplication of matrices concurrently computing the elements of the product.

Lets say that we want to multiply matrix ***A\*** with the dimension ***m\*** by ***k\*** by a matrix ***B\*** with the dimension ***k\*** by ***n\***. To obtain the product, say matrix ***C\***, we compute dot products for each row of ***A\*** and each column of ***B***; i.e., to compute a specific element ***cij\*** of the matrix ***C\***, we compute a dot product of row ***ai\*** from the matrix ***A\*** and the column ***bj\*** from the matrix ***B***.

![LaTeX: c_{ij}=\sum_{l=0}^{k} a_{il}b_{lj}](https://cilearn.csuci.edu/equation_images/c_%257Bij%257D%253D%255Csum_%257Bl%253D0%257D%255E%257Bk%257D%2520a_%257Bil%257Db_%257Blj%257D)c i j = ∑ l = 0 k a i l b l j

The application should accept input in the following format:

- the first line contains three numbers that specify the sizes of the matrices; for example, ***m***, ***k***, and ***n***, followed by
- ***m\*** lines of ***k\*** numbers that represent matrix ***A\***, and in turn followed by
- ***k\*** lines of ***n\*** numbers that represent matrix ***B\***.

Note that the program should check the validity of ***m\***, ***k\***, and ***n\***, to ensure that the matrices ***A\*** and ***B\*** can be legally multiplied.

#### Sample Input:

```
3 2 3 
1 4 
2 5 
3 6 
8 7 6 
5 4 3
```

The program should output the content of both input matrices followed by their product as shown here:

#### Sample Output:

```
MATRIX A 
1 4 
2 5 
3 6 
      
MATRIX B 
8 7 6 
5 4 3 
      
MATRIX A x B 
28 23 18 
41 34 27 
54 45 36
```

### Implementation

Your implementation must not use any global variables, since any use of global variables prevents the capability to multiply any number of matrix pairs concurrently. It is a common approach to stay away from global declarations while working with threads. Of course, global data can still be used as part of the application logic to share information between multiple threads as long as it does not iterfere with the concurrent computation.

In your implementation, you must use ***m\*n*** single unique threads to compute the values of every element ***cij\*** of the product matrix. Therefore, you must pass a pointer to the following structure:

```
struct matrixCell 
{ 
   int i; 
   int j; 
   int k; 
   int **a; 
   int **b; 
   int **c; 
};
```

as a parameter to pthread_create(). The integer i is the index of the row in the matrix ***A\***, j is the index of the column in the matrix ***B\***, and k indicates the number of elements in the row and - at the same time - the number of elements in the column. The pointers will point to matrices ***A\***, ***B\***, and ***C\*** that will be placed in dynamically allocated space in the function allocatAndLoadMatrices(). The pointer **a points to the matrix ***A\***, **b to ***B\***, and **c to ***C\***. The function takes pointers to these pointers (i.e., pointers to two-dimensional arrays) as parameters (hence ***a, ***b, and ***c). You must allocate space for all rows and all columns in every of the arrays, so they can hold the corresponding matrices. The function loadMatrix() is an auxiliary function that focuses on loading one matrix at a time.

You MUST NOT use thread_join() after the creation of each thread, since that would cause sequential execution of the threads, and there would be no concurrency whatsoever. However, you must wait for the threads to complete in another place before printing the result, since if you do not, then the product matrix ***C\*** will be only partially calculated as the program may exit before all threads manage to complete their execution.

The archive [matrixMult.zip](https://cilearn.csuci.edu/courses/12323/files/1381361/download?wrap=1) contains starting code for your implementation. Implement all missing parts as indicated in the comments. You must not change the code.

Check this Web site for ideas on dynamic allocation of matrices: [https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/ (Links to an external site.)](https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/).

### Submission

You must submit the following:

- the signed source code,
- the MakeLists.txt file with which you built your application, and
- multiple scripts of robust tests that confirm that your program is capable of multiplying any anumber of matrix pairs concurrently.