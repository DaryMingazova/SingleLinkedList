SingleLinkedList
-----------------
A singly linked list. A sequential iterator (Forward itertor) is written for it. A strict security guarantee is provided.


Description
-----------
Some features of the implementation of this single-linked list:

- Implemented a default constructor that creates an empty list.
- Implemented the **getSize** method, which returns the number of items in the list.
- Implemented the **isEmpty** method, which returns true if the list is empty, and false otherwise.
- Implemented the **PushFront** method, which inserts an element at the beginning of a singly linked list.
- Implemented the **Clear** method, which clears the list and does not throw exceptions.
- When the list is destroyed, all its elements are deleted.
- Implemented support for iterating through the elements of the container **SingleLinkedList** using the template class **BasicIterator**, on the basis of which constant and non-constant iterators of the list are declared.
- Implemented comparison operations ==, !=, <, >, <=, >=.
- Implemented the exchange of the contents of two lists using the **swap** method and the template function **swap**.
- Implemented the construction of a singly linked list based on **initializer_list**.
- Implemented a reliable copy constructor and assignment operation (assignment operation provides a strict guarantee of exception safety - if an exception is thrown during assignment, the contents of the left argument of the assignment operation should remain unchanged).
- Implemented the **popFront** method, which deletes the first element of a non-empty list in time **O(1)** (does not throw exceptions).
- The **insertAfter** method is implemented - during **O(1)** inserts a new value into the list after the element referenced by the iterator passed to **insertAfter** (the method provides a strict guarantee of exception safety).
- Implemented the **EraseAfter** method - in time **O(1)** removes from the list the element following the element referenced by the iterator passed to **insertAfter** (does not throw exceptions).
- Implemented methods **before_begin** and **cbefore_begin** - return iterators referring to a fictitious position before the first element of the list. Such an iterator is used as a parameter for the **insertAfter** and **EraseAfter** methods when it is necessary to insert or delete an element at the beginning of the list (this iterator cannot be dereferenced).

Instructions for use 
----------------------------
Transfer the file to your project and use **SingleLinkedList** as a singly linked list in addition to the standard bidirectional - list.
