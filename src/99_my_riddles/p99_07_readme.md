# Remarks on the performance measurements

The riddle **99_07: in place reverse linked list** is solved with three different algorithms. The first one is allowed to
use both heap and stack space without limitations, the second can only use stack space, and the last one cannot use heap
or stack (at least it cannot scale stack or heap space with the length of the list).

To get an idea of the performance impact I used all algorithm with lists of length:

1 2 4 8 16 32 64 128 256 1024 2048 .. 524288

The csv file `time_99_07.csv` holds the measured time in microseconds.

The file `p99_07_gnuplot_performance.gp` provides a gnuplot recipe to plot the performance from the csv file in a single graph.

The file `p99_07_gnuplot_performance.svg` holds the results. Note that the graph is logarithmic on both X and Y axis. It is shown here:

![image](./p99_07_gnuplot_performance.svg)

Here is the data for the graph above: [time_99_07.csv](./time_99_07.csv).


