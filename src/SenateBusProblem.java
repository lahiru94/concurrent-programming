import java.util.concurrent.Semaphore;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;


public class SenateBusProblem {
    int fast_forward = 100;
    int riders_waiting = 0;                 //count of riders in the boarding area
    Semaphore mutex = new Semaphore(1);     //to protect waiting riders
    Semaphore bus = new Semaphore(0);       //to signal arrival of a bus
    Semaphore boarded = new Semaphore(0);   // to signal a rider has been boarded

    

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
                System.out.println("I departed");

            } catch (InterruptedException e) {
                e.printStackTrace();
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
                System.out.println("I got boarded");
                boarded.release();

            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        }
    }

    class RideCreator implements Runnable{
        int rider_id=0;
        int mean_rider=30000/fast_forward;
        @Override
        public void run(){
            while(true){
                try {
                    Rider new_rider = new Rider();
                    new Thread(new_rider).start();
                    Thread.sleep(mean_rider);
                } catch (InterruptedException ex) {
                    Logger.getLogger(SenateBusProblem.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }

    class BusCreator implements Runnable{
        int bus_id=0;
        int mean_bus=1200000/fast_forward;
        @Override
        public void run(){
            while(true){
                try {
                    Thread.sleep(mean_bus);
                    Bus new_bus = new Bus();
                    new Thread(new_bus).start();
                } catch (InterruptedException ex) {
                    Logger.getLogger(SenateBusProblem.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }


    public static void main(String[] args) {
        SenateBusProblem simulation = new SenateBusProblem();
        new Thread(simulation.new BusCreator()).start();
        new Thread(simulation.new RideCreator()).start();

    }



}
