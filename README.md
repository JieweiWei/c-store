c-store
=======

c-store is a column-related database coding in c language.


Development environment
-----------------------

ubuntu 12.04 (32bit)


Test Environment
----------------

ubuntu 10.04/12.04/13.04/14.04 (32bit)


Preliminary work
----------------

1.Put the <orders.tbl> into the current directory.

2.Put the <customer.tbl> into the current directory.


Run
---

change directory to the project directory

1.Enter the directory in the terminal.

2.Enter [make] to compile and link the program.

3.Enter [./bin/db help] to view cstore command.

4.Enter [./bin/db load orders] to load  orders.

5.Enter [./bin/db load customer] to load customer.

6.Enter [./bin/db retrieve orders] to retrieve orders.

7.Enter [./bin/db compress orders 1] to compress the first column of orders.

8.Enter [./bin/db join] to join the orders and customer.

9.Enter [./bin/db count] to count orders.

10.Enter [make clean] to remove .o file and executable files.


