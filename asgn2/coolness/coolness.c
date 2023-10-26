//Including standard io library, math library and standard library
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//The following function returns the coolness given a float temperature and a float speed
float coolness(float temp, float speed){
	float coolness = (35.74 + (0.6215 * temp) - (35.75 * (pow(speed, 0.16))) + ((0.4275 * temp) * (pow(speed, 0.16))));
	return coolness;
}


//The following function prints the header that will be used in our table that the program will return
void print_header(){
	printf("Temp\tWind\tCoolness\n");
}


//The following function prints the lines within our table that will be show the temp than the speed than the coolness
void print_line(float temp, float speed, float coolness){
	printf("%.1f\t%.1f\t%.1f\n", temp, speed, coolness);
}

int main(int argc, char**argv) {
	//Checking if there are more than two arguments passed
	if(argc > 3){
		printf("Usage: ./coolness [temperature] [wind speed]\n");
		exit(1);
	}
	//If there are two arguments
	if(argc == 3){
		//Changing char data into floats
		float temp = (float)atof(argv[1]);
		float speed = (float)atof(argv[2]);
		//Calculating coolness with the two arguments as temperature and speed
		float cool = coolness(temp, speed);
		//If temp < -99 or > 50 or speed is less than 0.5, print error
		if(((temp < -99) || (temp > 50)) || (speed < 0.5)){
                        printf("Error: Coolness. Acceptable input values are -99<=T<=50 and 0.5<=V.\n");
                        exit(1);
                }
		//Print table
		print_header();
		print_line(temp, speed, cool);
	//If there is one argument
	} else if(argc == 2){
		//Changing char data into float temp
		float temp = (float)atof(argv[1]);
		//If temp is greater than 50 or less than -99 print error
		if(((temp < -99) || (temp > 50))){
                        printf("Error: Coolness. Acceptable input values are -99<=T<=50 and 0.5<=V.\n");
                        exit(1);
                }
		//Print header
		print_header();
		//For loop to print the lines
		for(float speed = 5.0; speed <= 15.0; speed += 5){
			float cool = coolness(temp, speed);
			print_line(temp, speed, cool);
		}
	//If there are no arguments
	} else if(argc == 1){
		//Print header
		print_header();
		//For loop to print table
		for(float temp = -10.0; temp <= 40.0; temp += 10){
			for(float speed = 5.0; speed <= 15.0; speed += 5){
				float cool = coolness(temp, speed);
				print_line(temp, speed, cool);	
			}
			if(temp < 40.0){	
			printf("\n");
			}
		}	
	}	
	return 0;
}
