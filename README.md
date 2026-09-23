# Sleeping Barber Problem — Process Synchronization

## OS Concept
The OS concept assigned to our team is **Process Synchronization**, studied through the classical **Sleeping Barber Problem**, posed by Edsger W. Dijkstra in 1965.

## Problem Statement
A barbershop has one barber, one barber chair, and a waiting room with N chairs.
- The barber sleeps when there are no customers.
- An arriving customer wakes the barber if he is asleep.
- If the barber is busy, the customer waits on a free chair.
- If all chairs are occupied, the customer leaves.

The challenge is to synchronize the barber and customer processes using semaphores and a mutex, avoiding race conditions, deadlock, and missed wakeups.

## Team Members
- **Harsh Malusare** — Report drafting, coding/implementation, image extraction, slide deck
- **Aliza Farooqui** — Topic selection & drafting, image preparation, partial coding

## Repository Structure
The project source code and report files are organized as follows:

- /src — Source code (Barber & Customer implementation in C)
- /docs — Report, GUI screenshots, references
- README.md

## How to Run
gcc src/barber.c -o barber -lpthread
./barber

## References
See docs/references.md for the full list of research papers used.
