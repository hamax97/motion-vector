# Description. Spiral approach

0. Read both images and store them in a 2D matrix.
1. Enumerate each possible macro-block in the matrix.
2. Save them in a array.
3. Iterate over each macro-block.
4. Substract the position of the current macro-block to each position in the array.
5. Sort the array by minimum position. That is, make the sum of the absolute value of the components (x, y) and sort them in ascending order.
6. Take the first element of the sorted array.
7. Evaluate the sum of the differences of the current macro-block with the new macro-block.
8. If the evaluation is equal to zero, stop.
9. Else, if the evaluation is less than the current value save it.
10. Continue with the next element in the sorted array.
11. When the sorted array is finished, save the best value of the current macro-block.
12. Continue with the next element in the array of macro-blocks.