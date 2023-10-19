import java.util.Scanner;

class MultithreadingDemo extends Thread {
    public static final int SLEEP_TIME = 100; /* sleeep time in seconds */
    public void run() {
        try {
            // Display the running thread.
            System.out.println("Thread " + Thread.currentThread().getId() + " is running");
            // Put thread to sleep to monitor it.
            Thread.sleep(SLEEP_TIME * 1000);
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}

public class MultithreadingTest {
    public static final int NUM_THREADS = 8; /* number of threads to create */
    public static void main(String[] args) {
        MultithreadingDemo[] demo = new MultithreadingDemo[NUM_THREADS];
        /* Start threads. */
        for (int i = 0; i < NUM_THREADS; i++) {
            System.out.println("Press ENTER to create new thread ...");
            Scanner scanner = new Scanner(System.in);
            scanner.nextLine();
            demo[i] = new MultithreadingDemo();
            demo[i].start();
        }
        /* Wait for threads. */
        for (int i = 0; i < NUM_THREADS; i++) {
            try {
                demo[i].join();
            } catch (Exception e) {
                System.out.println(e);
            }
        }
    }
}
