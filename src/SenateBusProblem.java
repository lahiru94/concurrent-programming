import java.util.concurrent.Semaphore;
import java.util.logging.Logger;

public class SenateBusProblem {
    private final static Logger LOGGER = Logger.getLogger(SenateBusProblem.class.getName());

    int riders_waiting = 0;                 //count of riders in the boarding area
    Semaphore mutex = new Semaphore(1);     //to protect waiting riders
    Semaphore bus = new Semaphore(0);       //to signal arrival of a bus
    Semaphore boarded = new Semaphore(0);   // to signal a rider has been boarded

    public static void main(String args[]) {
        SenateBusProblem senateBusProblem = new SenateBusProblem();
    }

    class Bus implements Runnable {


        @Override
        public void run() {
            try {
                mutex.acquire();            //bus holds the mutex till the riders get boarded
                int riders_to_board = Math.min(riders_waiting, 50);

                for (int i = 0; i < riders_to_board; i++) {
                    bus.release();
                    boarded.acquire();      //wait till each rider is baorded
                }

                riders_waiting = Math.max(riders_waiting - 50, 0);
                mutex.release();
                LOGGER.info("Bus departed");

            } catch (InterruptedException e) {
                LOGGER.warning("Thread got interrupted");
            }
        }
    }

    class Rider implements Runnable {

        @Override
        public void run() {
            try {
                mutex.acquire();
                riders_waiting++;
                mutex.release();

                bus.acquire();
                LOGGER.info("I boarded");
                boarded.release();

            } catch (InterruptedException e) {
                LOGGER.warning("Thread got interrupted");
            }

        }

    }

}
