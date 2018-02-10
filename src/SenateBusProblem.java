import java.util.concurrent.Semaphore;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.concurrent.ThreadLocalRandom;


public class SenateBusProblem {

    int simulation_speed = 100;               //increase this to fast forward the simulation.1 is normal speed.

    int riders_waiting = 0;                 //count of riders in the boarding area
    Semaphore mutex = new Semaphore(1);     //to protect waiting riders
    Semaphore bus = new Semaphore(0);       //to signal arrival of a bus
    Semaphore boarded = new Semaphore(0);   // to signal a rider has been boarded

    

    class Bus implements Runnable {

        int myId;

        public Bus(int id){
            this.myId = id;
        }


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
                System.out.println("Bus "+ myId + " departed");

            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    class Rider implements Runnable {

        int myId;

        public Rider(int id){
            this.myId = id;
        }

        @Override
        public void run() {
            try {
                mutex.acquire();
                riders_waiting++;
                mutex.release();

                bus.acquire();
                System.out.println("Rider "+ myId +" got boarded");
                boarded.release();

            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        }
    }

    class RideCreator implements Runnable{
        int rider_id=0;
        int mean_time=(30000/simulation_speed)*2;
        double lambda = 2;
        @Override
        public void run(){
            while(true){
                try {
                    rider_id++;
                    Rider new_rider = new Rider(rider_id);
                    new Thread(new_rider).start();
                    double currRand = ThreadLocalRandom.current().nextDouble(0, 5);
                    int rand_time = (int)(mean_time * lambda * Math.exp(currRand * -1 * lambda));
                    Thread.sleep(rand_time);
                } catch (InterruptedException ex) {
                    Logger.getLogger(SenateBusProblem.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }

    class BusCreator implements Runnable{
        int bus_id=0;
        int mean_time=(1200000/simulation_speed)*2;
        double lambda = 2;
        @Override
        public void run(){
            while(true){
                try {
                    double currRand = ThreadLocalRandom.current().nextDouble(0, 5);;
                    int rand_time = (int)(mean_time*lambda * Math.exp(currRand * -1 * lambda));
                    Thread.sleep(rand_time);
                    bus_id++;
                    Bus new_bus = new Bus(bus_id);
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
