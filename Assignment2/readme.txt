README

Mohammad Omar Zahir - zahirm1 - 400255963
Mahad Aziz - azizm17 - 400250379

Part 1 - Address Translation:
Understanding the logic of how FIFO works to determine the frame numbers was done by both Omar & Mahad. Omar initially figured out how the frame numbers must begin with 0, and Mahad figured out how to implement the functionality.
Both Omar & Mahad figured out how to implement the Page Table and how the frame numbers will get stored in it.
The calculating of the values for the physical address, page offset, and other parameters required to determine the output were inspired by the tutorials but were implemented by both Omar & Mahad.
Determining the logic for resetting the frame numbers once it has reached 128 was done by Omar.
Determining how the value should be updated if a page number has an old frame number, and when it should not, was identified and implemented by Mahad.
The overall simplifying and shortening of the entire code for part 1 to make it simpler was done by Omar.

Part 2 - TLB:
The search_TLB function and the understanding and implementation of it was done by Omar.
The TLB_Add function and the understanding and implementation of it was done by Omar.
The TLB_Update function and the understanding and implementation of it was done by Mahad.
The merging of the TLB functionality with our initial code with just the page table was understood and implemented by both Omar & Mahad.
Determining the logic of when to add to the TLB and how to implement it was done by Omar.
Determining the logic of when to update the TLB and how to implement it was done by Mahad.
Determining the frame number when the page number exists in the TLB was done by Mahad.

Part 3 - Handling Page Faults
Understanding how the copying from Backing Store works and the logic behind was discovered by both Omar & Mahad.
The implementing of the memcopy functionality and how to store and index the values correctly from the backing store was implemented by Mahad
Calculating the value from the memory array and what the value represented from the physical memory address was worked and implemented by both Omar and Mahad.

Commenting:
Mahad commented the beginning part of the code relating to the individual functions for the TLB and anything outside the main function
Omar commented the main function and all respective logic and parameters within it