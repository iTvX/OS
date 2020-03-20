public class test {
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

    private static boolean[] isValid;

    public static class validator implements Runnable {
        int row;
        int col;
        validator(int row, int column) {
            this.row = row;
            this.col = column;
        }

        @Override
        public void run() {
            int sum = 0;
            int product = 1;
            for (int i = row; i < row + 3; i++) {
                for (int j = col; j < col + 3; j++) {
                    int num = source[i][j];
                    sum = num + sum;
                    product *= num;
                }
            }
            if (sum == 45 && product == 362880 ) {
                isValid[row + col/3] = true;
            }
        }
    }

    public static void main(String[] args) {
        int numThreads = 9;
        isValid = new boolean[numThreads];
        Thread[] threads = new Thread[numThreads];
        int index = 0;

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (i%3 == 0 && j%3 == 0) {
                    threads[index++] = new Thread(new validator(i, j));
                }

            }
        }

        // Start threads
        for (int i = 0; i < threads.length; i++) {
            threads[i].start();
        }

        // Wait for all threads to finish
        for (int i = 0; i < threads.length; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        // If any of the entries in the valid array are 0, then the sudoku solution is invalid
        for (int i = 0; i < isValid.length; i++) {
            if (!isValid[i]) {
                System.out.println("Sudoku solution is invalid!");
                return;
            }
        }
        System.out.println("Sudoku solution is valid!");
    }
}