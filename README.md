# hash-table-implementation
implements hash tables based either on a dynamic array or an array of Linked List

Description:

The Hash table allows us to store and manage key_value pairs 
with ease, providing essential functions like insertions, lookup,
and removal. The program also handles collisons resolution using 
chaining, and also maintains optimal load factor by resizing itself 
when necessary. The implementation also includes a simple and effective 
hash function which distribuites your key uniformly, ensuring quick
access to your data. The program is also overall perfect for users who
need a fast, reliable, and user-friendly hash table without diving into the 
complexities of the code. try it out now!

Instructions: 

 1) first unzip the assignment5 by doing this in the terminal: unzip assignment5.

 2) then run the "make" in the terminal to compile all the code.
 
 3) finally type "./test_ht" which will the run program with all the test cases. 

Limitations:

overall the hash table implementation in C provides functionaility like insertions,
lookup, and also removal while automatically handling collision resultions and resizing.
Overall, it has many limitations, including a lack of support for custom hash functions,
absense of memeory managment for individual elements, no safety, no support, and a resizing 
strategy that doesn't shrink the table.
