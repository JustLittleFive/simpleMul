# CS205 C/ C++ Programming - Project 1 A Simple Calculator
**Name:** Lv Yue  
**SID:** 11710420

## Part1: Analysis  
Project goal: Implement a calculator which can multiply two numbers.  
Using C++ * operator is easy to implement the perpose.  
However, to avoid overflow of basic type or precision loss of floating point number, we choose to simulate basic handwritten vertical multiplication, time complexity O(n^2).
Further more, there is Schönhage–Strassen algorithm, which core idea is *divide and conquer*. Its time complexity can reduce to O(n^log<sub>2</sub>3).  

## Part2: 
