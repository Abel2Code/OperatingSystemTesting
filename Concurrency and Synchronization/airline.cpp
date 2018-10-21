// g++ airline.cpp -o airline
// ./airline 100 5 3 2

#include <iostream>
#include <semaphore.h>
#include <sstream>

int PASSENGERS;
int BAGGAGE_HANDLERS;
int SECURITY_SCREENERS;
int FLIGHT_ATTENDANTS;
int passengersLeft;

sem_t baggageNeeded, baggageDone, securityNeeded, securityDone, seatingNeeded, seatingDone;


void *baggageHandlerLife(void *arg){
  int id = *((int*)(&arg));

for(;;){
    // Wait for a passenger to need baggage processing
    sem_wait(&baggageNeeded);

    // std::cout << "Baggage Handler # " << id << " is helping\n";

    // Let Passenger know that their bag was processed
    sem_post(&baggageDone);
  }
}

void *securityScreenerLife(void *arg){
  int id = *((int*)(&arg));
  for(;;){
    // Wait for a passenger to need security checking
    sem_wait(&securityNeeded);

    // std::cout << "Security Handler # " << id << " is helping\n";

    // Pass a passenger through security
    sem_post(&securityDone);
  }
}

void *flightAttendantLife(void *arg){
  int id = *((int*)(&arg));
  for(;;){
    // Wait for a passenger to need to be seated
    sem_wait(&seatingNeeded);
    // std::cout << "Flight Attendant # " << id << " is helping\n";

    // Seat a passenger
    sem_post(&seatingDone);
  }
}

void *passengerLife(void *arg){
  int id = *((int*)(&arg));

  std::cout << "Passenger # " << id << " arrived at the terminal.\n";

  // Increment BaggageNeeded Semaphore
  sem_post(&baggageNeeded);

  std::cout << "Passenger # " << id << " is waiting at baggage processing.\n";

  // Wait until BagDone Semaphore (Take Resource if Available)

  sem_wait(&baggageDone);

  // Increment SecurityNeeded Semaphore
  sem_post(&securityNeeded);

  std::cout << "Passenger # " << id << " is waiting to be screened by a screener.\n";

  // Wait for SecurityDone Semaphore (Take Resource if available)
  sem_wait(&securityDone);

  // Increment SeatingNeeded Semaphore
  sem_post(&seatingNeeded);

  std::cout << "Passenger # " << id << " is waiting to board the plane by an attendant.\n";

  // Wait for PassengerSeated Semaphore
  sem_wait(&seatingDone);

  std::cout << "Passenger # " << id << " has been seated and relaxes.\n";
  passengersLeft--;
}


int main(int argc, char* argv[]) {
  int createCounter;

  // Set initial person counters

  if(argc < 5){
    std::cout << "ERROR: NOT ENOUGH INPUT\n";
    exit(1);
  }

  // Validating and setting PASSENGERS
  std::istringstream iss( argv[1] );
  if(!(iss >> PASSENGERS)){
    std::cout << "ERROR: INVALID PASSENGER INPUT\n";
    exit(2);
  }

  // passangersLeft decrements as passengers are seated
  passengersLeft = PASSENGERS;

  // Validating and setting BAGGAGE_HANDLERS
  std::istringstream iss2( argv[2] );
  if(!(iss2 >> BAGGAGE_HANDLERS)){
    std::cout << "ERROR: INVALID BAGGAGE_HANDLERS INPUT\n";
    exit(2);
  }

  // Validating and setting SECURITY_SCREENERS
  std::istringstream iss3( argv[3] );
  if(!(iss3 >> SECURITY_SCREENERS)){
    std::cout << "ERROR: INVALID SECURITY_SCREENERS INPUT\n";
    exit(2);
  }

  // Validating and setting FLIGHT_ATTENDANTS
  std::istringstream iss4( argv[4] );
  if(!(iss4 >> FLIGHT_ATTENDANTS)){
    std::cout << "ERROR: INVALID FLIGHT_ATTENDANTS INPUT\n";
    exit(2);
  }

  // Initialize ALL Semaphores at 0
  sem_init(&baggageNeeded, 0, 0);
  sem_init(&baggageDone, 0, 0);
  sem_init(&securityNeeded, 0, 0);
  sem_init(&securityDone, 0, 0);
  sem_init(&seatingNeeded, 0, 0);
  sem_init(&seatingDone, 0, 0);

  pthread_t T[PASSENGERS + BAGGAGE_HANDLERS + SECURITY_SCREENERS + FLIGHT_ATTENDANTS];
  int threadsCreated = 0;

  int rc;

  for(createCounter = 0; createCounter < BAGGAGE_HANDLERS; createCounter++){
    rc = pthread_create(&T[threadsCreated++],NULL,baggageHandlerLife, (void*) createCounter + 1);
    if (rc) {
        std::cout << "Error:unable to create thread," << rc;
        exit(-1);
    }
  }

  for(createCounter = 0; createCounter < SECURITY_SCREENERS; createCounter++){
    rc = pthread_create(&T[threadsCreated++],NULL,securityScreenerLife, (void*) createCounter + 1);
    if (rc) {
        std::cout << "Error:unable to create thread," << rc;
        exit(-1);
    }
  }

  for(createCounter = 0; createCounter < FLIGHT_ATTENDANTS; createCounter++){
    rc = pthread_create(&T[threadsCreated++],NULL,flightAttendantLife, (void*) createCounter + 1);
    if (rc) {
        std::cout << "Error:unable to create thread," << rc;
        exit(-1);
    }
  }

  for(createCounter = 0; createCounter < PASSENGERS; createCounter++){
    rc = pthread_create(&T[threadsCreated++],NULL,passengerLife, (void*) createCounter + 1);
    if (rc) {
        std::cout << "Error:unable to create thread," << rc;
        exit(-1);
    }
  }

  while(passengersLeft > 0){

  }

  std::cout << "Closing Main Thread\n";

}
