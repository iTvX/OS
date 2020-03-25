//reference:
// Java Thread join
// https://www.journaldev.com/1024/java-thread-join-example

public class Sudoko {
    public static int num_colOrRow = 9;
    public static int numOfThreads = 9;
    private static int sub_interval = 3;
    private static boolean[] isValid;
    private static int[][] source = {
            {6, 2, 4, 5, 3, 9, 1, 8, 7},
            {5, 1, 9, 7, 2, 8, 6, 3, 4},
            {8, 3, 7, 6, 1, 4, 2, 9, 5},
            {1, 4, 3, 8, 6, 5, 7, 2, 9},
            {9, 5, 8, 2, 4, 7, 3, 6, 1},
            {7, 6, 2, 3, 9, 1, 4, 5, 8},
            {3, 7, 1, 9, 5, 6, 8, 4, 2},
            {4, 9, 6, 1, 8, 2, 5, 7, 3},
            {2, 8, 5, 4, 7, 3, 9, 1, 6}
    };

    public static class validator implements Runnable {
        int row;
        int col;

        validator(int row, int column) {
            this.row = row;
            this.col = column;
        }

        @Override
        public void run() {
            int sumOfAdd = 0;
            int sumOfProduct = 1;

            int i = row;
            while (i < row + sub_interval) {
                int j = col;
                while (j < col + sub_interval) {
                    int num = source[i][j];
                    sumOfAdd = num + sumOfAdd;
                    sumOfProduct *= num;
                    j++;
                }
                i++;
            }

            if (sumOfAdd == 45 && sumOfProduct == 362880) {
                isValid[row + col / 3] = true;
            }
        }
    }

    public static void main(String[] args) {
        isValid = new boolean[numOfThreads];
        Thread[] threads = new Thread[numOfThreads];
        int index = 0;

        for (int col = 0; col < num_colOrRow; col++) {
            for (int row = 0; row < num_colOrRow; row++) {
                if (col % sub_interval == 0 && row % sub_interval == 0) {
                    threads[index++] = new Thread(new validator(col, row));
                }

            }
        }

// run
        for (Thread thread : threads) {
            thread.start();
        }
// wait for all threads

        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (Exception e) {
                String threadName = Thread.currentThread().getName();
                System.out.println("Exception from " + threadName + ".run");
            }
        }

//if one or more than one of isValid array is not valid, then print "Invalid" and return,
// if all of them are valid, then will not going to
// "if statement", it would print "valid" instead.
        for (boolean b : isValid) {
            if (!b) {
                System.out.println("Invalid!");
                return;
            }
        }
        System.out.println("Valid!");
    }
}