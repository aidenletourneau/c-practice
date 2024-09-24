/* COP 3502C Assignment 1
This program is written by: Aiden Letourneau */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct RegisteredVehicle RegisteredVehicle;

typedef struct Garage{ //for one garage
  char* garage_name; //to be used for garage name
  int total_capacity; //total capacity of the garage
  int current_count; // number of vehicles in the garage in a particular time
  RegisteredVehicle** parked_vehicles; //list of parked vehicles in a particular time
} Garage;

typedef struct RegisteredVehicle{ //for one registered vehicle
	char* license_plate; //to be used for string license plate# of the vehicle
	char* owner_name; //to be used for storing owner name
	Garage* current_garage;
} RegisteredVehicle;

typedef struct Campus{
  Garage** garages; //list of garages in campus
  int total_garages; //number of garages in the campus
  RegisteredVehicle** registered_vehicles; //list of registered vehicles
  int total_registered_vehicles; //total number of registered vehicles
} Campus;

Garage* createGarage(const char* name, int capacity); //working on
RegisteredVehicle* createRegisteredVehicle(const char* license, const char* owner);
void registerVehicle(Campus* campus, const char* license, const char* owner);
void parkVehicle(Garage* garage, RegisteredVehicle* vehicle);
int removeVehicleFromGarage(Garage* garage, const char* license);
RegisteredVehicle* searchVehicleByLicense(const Campus* campus, const char* license);
Garage* searchGarageByName(const Campus* campus, const char* name);
int countTotalVehicles(const Campus* campus);
int resizeGarage(Garage* garage, int new_capacity);
int relocateVehicle(Campus* campus, const char* license, const char* target_garage_name);
void displayVehiclesByOwner(const Campus* campus, const char* owner_name);
int removeGarage(Campus* campus, const char* garage_name);
void generateGarageUtilizationReport(const Campus* campus);
Garage* getLeastUtilizedGarage(const Campus* campus);
void freeMemory(Campus* campus);


int main(void){

	//initialize campus
	Campus* campus = (Campus*)malloc(sizeof(Campus));
	int numGarages, numVehicles, numCommands;
	scanf("%d %d %d", &numGarages, &numVehicles, &numCommands);
	campus->total_garages = numGarages;
	campus->total_registered_vehicles = numVehicles;
	campus->garages = (Garage**) malloc(sizeof(Garage*) * numGarages);
	campus->registered_vehicles = (RegisteredVehicle**) malloc(sizeof(RegisteredVehicle*) * numVehicles);


	//Create Garages
	for (int i = 0; i < numGarages; i++){
		char garageName[22];
		int capacity;
		scanf("%22s %d", garageName, &capacity);
		campus->garages[i] = createGarage(garageName, capacity);
	}

	//Create Vehicles
	for (int i = 0; i < numVehicles; i++){
		char license[22];
		char owner[22];
		scanf("%s %s", license, owner);
		campus->registered_vehicles[i]  = createRegisteredVehicle(license, owner);
	}

		

	//Run Commands
	for (int i = 0; i < numCommands; i++){
		char command[22];
		scanf("%s", command);

		// PARK
		if (strcmp(command, "PARK") == 0){
			char license[22];
			char garageName[22];
			scanf("%s %s", license, garageName);
			RegisteredVehicle* vehicle = searchVehicleByLicense(campus, license);
			Garage* garage = searchGarageByName(campus, garageName);
						
			parkVehicle(garage, vehicle);
		}

		// UTILIZATION_REPORT
		else if(strcmp(command, "UTILIZATION_REPORT") == 0){
			generateGarageUtilizationReport(campus);
		}

		// RESIZE
		else if(strcmp(command, "RESIZE") == 0){
			char garageName[22];
			int newCapacity;
			scanf("%s %d", garageName, &newCapacity);
			Garage* garage = searchGarageByName(campus, garageName);
			if (resizeGarage(garage, newCapacity)){
					printf("SUCCESS\n");
			}
			else{ printf("FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n");
			}
		}

		// SEARCH_OWNER
		else if(strcmp(command, "SEARCH_OWNER") == 0){
			char name[22];
			scanf("%s", name);
			displayVehiclesByOwner(campus, name);
		}

		// RELOCATE
		else if(strcmp(command, "RELOCATE") == 0){
			char license[22];
			char garageName[22];
			scanf("%s %s", license, garageName);
			if(relocateVehicle(campus, license, garageName)){
				printf("RELOCATION SUCCESSFUL.\n");
			}
		}

		// COUNT_TOTAL
		else if (strcmp(command, "COUNT_TOTAL") == 0){
			int total = 0;
			for (int i = 0; i < campus->total_garages; i++){
				if(campus->garages[i] != NULL){
					total += campus->garages[i]->current_count;
				}
			}
			printf("%d\n", total);
		}

		// REGISTER_VEHICLE
		else if (strcmp(command, "REGISTER_VEHICLE") == 0){
			char license[22];
			char name[22];
			scanf("%s %s", license, name);
			registerVehicle(campus, license, name);
			printf("REGISTERED\n");
		}

		// REMOVE_GARAGE
		else if (strcmp(command, "REMOVE_GARAGE") == 0){
			char name[22];
			scanf("%s", name);
			if(removeGarage(campus, name)){
				printf("%s REMOVED\n", name);
			}
			else{
				printf("%s NOT FOUND\n", name);
			}
		} 

		// REMOVE_VEHICLE_GARAGE
		else if(strcmp(command, "REMOVE_VEHICLE_GARAGE") == 0){
			char license[22];
			scanf("%s", license);
			RegisteredVehicle* vehicle = searchVehicleByLicense(campus, license);
			if (vehicle->current_garage == NULL){
				printf("NOT FOUND IN CAMPUS\n");
			}
			else{
				char prevGarageName[22];
				strcpy(prevGarageName, vehicle->current_garage->garage_name);
				if(removeVehicleFromGarage(vehicle->current_garage, license)){
					printf("REMOVED FROM %s\n", prevGarageName);
					vehicle->current_garage = NULL;
				}
			}
		}
	}
	freeMemory(campus);
  return 0;
}

//initialize new garage with appropriate data and memory
Garage* createGarage(const char* name, int capacity){
	Garage* newGarage = (Garage*)malloc(sizeof(Garage));
	newGarage->current_count = 0;
	newGarage->total_capacity = capacity;
	newGarage->parked_vehicles = (RegisteredVehicle**)calloc(capacity, sizeof(RegisteredVehicle*));
	for(int i = 0; i < capacity; i++){
		newGarage->parked_vehicles[i] = NULL;
	}
	newGarage->garage_name = (char*)malloc(sizeof(char)*(strlen(name) + 1));
	strcpy(newGarage->garage_name, name);
	return newGarage;
}

//initialize new vehicle with appropriate data and memory
RegisteredVehicle* createRegisteredVehicle(const char* license, const char* owner){
	RegisteredVehicle* newVehicle = (RegisteredVehicle*) malloc(sizeof(RegisteredVehicle));
	newVehicle->license_plate = (char*)malloc(sizeof(char) * (strlen(license)+1));
	newVehicle->owner_name = (char*)malloc(sizeof(char) * (strlen(owner)+1));
	newVehicle->current_garage = NULL;
	strcpy(newVehicle->license_plate, license);
	strcpy(newVehicle->owner_name, owner);
	return newVehicle;
}

// add new vehicle to system and update campus
void registerVehicle(Campus* campus, const char* license, const char* owner){
	campus->total_registered_vehicles += 1;
	campus->registered_vehicles = (RegisteredVehicle**)realloc(campus->registered_vehicles, campus->total_registered_vehicles * sizeof(RegisteredVehicle*));
	campus->registered_vehicles[campus->total_registered_vehicles - 1] = createRegisteredVehicle(license, owner);
}

//check if garage is full, finds the next index of empty/NULL and parks the vehicle there
void parkVehicle(Garage* garage, RegisteredVehicle* vehicle){
	if (garage->current_count == garage->total_capacity){
		printf("FULL\n");
		return;
	}
	garage->current_count += 1;
	int indexToInsert = 0;
	while(1){
		if (!garage->parked_vehicles[indexToInsert]){
			break;
		}
		indexToInsert++;
	}
	garage->parked_vehicles[indexToInsert] = vehicle;
	vehicle->current_garage = garage;
	printf("PARKED\n");
}

//iterates through all registered vehicle to find specified one
RegisteredVehicle* searchVehicleByLicense(const Campus* campus, const char* license){
	for(int i = 0; i < campus->total_registered_vehicles; i++){
		if (strcmp(campus->registered_vehicles[i]->license_plate, license) == 0){
			return campus->registered_vehicles[i];
		}
	}
	return NULL;
}

//iterates through all garages to find specified one
Garage* searchGarageByName(const Campus* campus, const char* name){
	for(int i = 0; i < campus->total_garages; i++){
		if (strcmp(campus->garages[i]->garage_name, name) == 0){
			return campus->garages[i];
		}
	}

}

//checks if resize is appropriate, then reallocates the park_vehicles memory
int resizeGarage(Garage* garage, int new_capacity){
	if (new_capacity < garage->current_count){
		return 0;
	}
	garage->total_capacity = new_capacity;
	garage->parked_vehicles = realloc(garage->parked_vehicles, new_capacity * sizeof(RegisteredVehicle*));
	return 1;
}

//iterates through campus->garages to find first not NULL index, then iterates through all garages and compares utility, if less then set the index equal to that
Garage* getLeastUtilizedGarage(const Campus* campus){
	int indexOfLeastUtilizedGarage = -1;
	for (int i = 0; i < campus->total_garages; i++){
		if (campus->garages[i] != NULL){
			indexOfLeastUtilizedGarage = i;
			break;
		}
	}
	for (int i = 0; i < campus->total_garages; i ++){
		if ((campus->garages[i] != NULL) && ((double)campus->garages[i]->current_count)/((double)campus->garages[i]->total_capacity) < ((double)campus->garages[indexOfLeastUtilizedGarage]->current_count)/((double)campus->garages[indexOfLeastUtilizedGarage]->total_capacity)){
			indexOfLeastUtilizedGarage = i;
		}
	}
	return campus->garages[indexOfLeastUtilizedGarage];
}

//preform appropriate checks, then removes the vehicle from one garage and parks it at another
int relocateVehicle(Campus* campus, const char* license, const char* target_garage_name){
	int indexOfVehicle= -1;
	int indexOfGarage = -1;
	
	// Garage Checks
	for (int i = 0; i < campus->total_garages; i++){
		if (strcmp(target_garage_name, campus->garages[i]->garage_name) == 0){
			indexOfGarage = i;
			break;
		}
	}
	if (indexOfGarage == -1){
		printf("%s NOT FOUND.\n", target_garage_name);
		return 0;
	}
	if (campus->garages[indexOfGarage]->current_count == campus->garages[indexOfGarage]->total_capacity){
		printf("%s IS FULL.\n", target_garage_name);
		return 0;
	}
	
	// Vehicle Checks
	for (int i = 0; i < campus->total_registered_vehicles; i++){
		if((strcmp(license, campus->registered_vehicles[i]->license_plate) == 0)){
			indexOfVehicle = i;
			break;
		}
	}
	if (indexOfVehicle == -1){
		printf("NO VEHICLE FOUND\n");
		return 0;
	}
	else if (campus->registered_vehicles[indexOfVehicle]->current_garage == NULL){
		printf("%s NOT IN CAMPUS.\n", license);
		return 0;
	}

	//remove the vehicle from the list of parked cars
	removeVehicleFromGarage(campus->registered_vehicles[indexOfVehicle]->current_garage, campus->registered_vehicles[indexOfVehicle]->license_plate);

	parkVehicle(campus->garages[indexOfGarage], campus->registered_vehicles[indexOfVehicle]);
	
	return 1;

}

// iterates through all vehicles then sets that vehicles current vehicle to null, that index of parked_vehicles to null and the decrements the current count
int removeVehicleFromGarage(Garage* garage, const char* license){
	for (int i = 0; i < garage->current_count; i ++){
		if (strcmp(license, garage->parked_vehicles[i]->license_plate) == 0){
			garage->parked_vehicles[i]->current_garage = NULL;
			garage->parked_vehicles[i] = NULL;
			garage->current_count --;
			return 1;
		}
	}
	return 0;
}

int removeGarage(Campus* campus, const char* garage_name){
	// search for garage
	for (int i = 0; i < campus->total_garages; i ++){
		if((campus->garages[i] != NULL) && (strcmp(garage_name, campus->garages[i]->garage_name) == 0)){
			// set each parked vehicles current garage = NULL
			for (int j = 0; j < campus->garages[i]->total_capacity; j ++){
				if (campus->garages[i]->parked_vehicles[j] != NULL){
					campus->garages[i]->parked_vehicles[j]->current_garage = NULL;
				}
			}
			//free appropriate memory
			free(campus->garages[i]->garage_name);
			free(campus->garages[i]->parked_vehicles);
			free(campus->garages[i]);
			campus->garages[i] = NULL;
			return 1;
		}
	}
	return 0;
}

//prints data for each non null garage and calls getLeastUtilizedGarage
void generateGarageUtilizationReport(const Campus* campus){
	for (int i = 0; i < campus->total_garages; i++){
		if(campus->garages[i] != NULL){
			printf("Garage: %s, Capacity: %d, Occupied: %d, Utilization: %.2f%%\n", campus->garages[i]->garage_name, campus->garages[i]->total_capacity, campus->garages[i]->current_count, ((double)campus->garages[i]->current_count)/((double)campus->garages[i]->total_capacity)*100);
		}
	}
	printf("Least Utilized: %s\n", getLeastUtilizedGarage(campus)->garage_name);
}

void freeMemory(Campus* campus){
	// free garages
	for (int i = 0; i < campus->total_garages; i++){
		if (campus->garages[i] != NULL){
			free(campus->garages[i]->garage_name);
			free(campus->garages[i]->parked_vehicles);
			free(campus->garages[i]);
			campus->garages[i] = NULL;
		}
	}
	free(campus->garages);
	// free registered vehicles
	for (int i = 0; i < campus->total_registered_vehicles; i++){
		free(campus->registered_vehicles[i]->license_plate);
		free(campus->registered_vehicles[i]->owner_name);
		free(campus->registered_vehicles[i]);
	}
	free(campus->registered_vehicles);

	// free campus
	free(campus);
}

//loops through each vehicle to find matching owner names
void displayVehiclesByOwner(const Campus* campus, const char* owner_name){
	int hasCar = 0;
	for(int i = 0; i < campus->total_registered_vehicles; i++){
		if (strcmp(owner_name, campus->registered_vehicles[i]->owner_name) == 0){
			hasCar = 1;
			printf("%s ", campus->registered_vehicles[i]->license_plate);
			if (campus->registered_vehicles[i]->current_garage != NULL){
				printf("%s\n", campus->registered_vehicles[i]->current_garage->garage_name);
			}
			else {
				printf("NOT ON CAMPUS\n");
			}
		}
	}
	if(!hasCar){
		printf("NO REGISTERED CAR BY THIS OWNER\n");
	}
}
