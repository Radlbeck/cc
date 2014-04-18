/* CPU scheduler implementation using a multi level feedback queue
 */
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <semaphore.h>
#include <queue>
using namespace std;

#define MAX_REQUESTS 10

sem_t mutex_cout, mutex_cpu, mutex_threads;
queue<int> cpu_active_jobs;
int threads_complete = 0;
unsigned int max_cpu_queue = 0;

queue<int> parse_arguments(char *infile, queue<int> job_list)
{
	ifstream file (infile);								// open file into an input file stream
	if(file.is_open()){
		string word;
		while(file >> word){							// iterate through all the strings in the file
			job_list.push(atoi(word.c_str()));			// push them into a queue
		}
	}

	return job_list;
}

void *requestThread(void *argument)
{
	queue<int> *job_list = (queue<int>*)argument;

	// TEST
	sem_wait(&mutex_cout);
	cout << "request thread created: " << job_list->front() << endl;
	sem_post(&mutex_cout);
	// end TEST

	while(!job_list->empty()){							// while jobs exist load into active job list
		sem_wait(&mutex_cpu);
		if(cpu_active_jobs.size() < max_cpu_queue){		// abide by max CPU queue size			
			cpu_active_jobs.push(job_list->front());


			// TEST
			sem_wait(&mutex_cout);
			cout << "request we pushed and popped: " << job_list->front() << endl;
			sem_post(&mutex_cout);
			// end TEST

			job_list->pop();
		}
		sem_post(&mutex_cpu);
	}
	sem_wait(&mutex_threads);
	threads_complete++;
	sem_post(&mutex_threads);
	
	return NULL;
}

void *serviceThread(void *argument)
{
	int requester_amount = *(int*) argument;
	bool is_completed = false;
	queue<int> active_jobs_lvl2;
	queue<int> active_jobs_lvl3;

	// TEST
	sem_wait(&mutex_cout);
	cout << "servicing thread created" << endl;
	sem_post(&mutex_cout);
	// end TEST

	sem_wait(&mutex_threads);
	is_completed = (threads_complete < requester_amount)? false : true;
	sem_post(&mutex_threads);

	while(!is_completed){
		/* level one -> Q = 10 */
		sem_wait(&mutex_cpu);			// grab a new job if needed
		if(!cpu_active_jobs.empty()){
			int current_job = cpu_active_jobs.front();

			current_job -= 10;			// decrement by Q
			if(current_job > 0){		// send to next level
				active_jobs_lvl2.push(current_job); 				
			}else{						// else keep positive for output
				current_job = 0;
			}
			// output
			sem_wait(&mutex_cout);
			cout << "Job received level 1: " << cpu_active_jobs.front() << " remaining: "<< current_job << endl;
			sem_post(&mutex_cout);
			// output
			cpu_active_jobs.pop();
		}
		sem_post(&mutex_cpu);

		/* level two -> Q = 20 */
		if(!active_jobs_lvl2.empty()){
			int current_job = active_jobs_lvl2.front();

			current_job -= 20;			// decrement by Q
			if(current_job > 0){		// send to next level
				active_jobs_lvl3.push(current_job); 				
			}else{						// else keep positive for output
				current_job = 0;
			}
			// output
			sem_wait(&mutex_cout);
			cout << "Job received level 2: " << active_jobs_lvl2.front() << " remaining: "<< current_job << endl;
			sem_post(&mutex_cout);
			// output
			active_jobs_lvl2.pop();
		}

		/* level three -> Q = 40 */
		if(!active_jobs_lvl3.empty()){
			int current_job = active_jobs_lvl3.front();

			current_job -= 40;			// decrement by Q
			if(current_job > 0){		// send to next level
				active_jobs_lvl3.push(current_job); 				
			}else{						// else keep positive for output
				current_job = 0;
			}
			// output
			sem_wait(&mutex_cout);
			cout << "Job received level 3: " << active_jobs_lvl3.front() << " remaining: "<< current_job << endl;
			sem_post(&mutex_cout);
			// output
			active_jobs_lvl3.pop();
		}

		sem_wait(&mutex_cpu);				// give other threads a chance to grab the mutex
		sem_wait(&mutex_threads);			// servicer is done when there are no requesters and no active jobs
		is_completed = 
			(threads_complete >= requester_amount && 
				cpu_active_jobs.empty() && 
				active_jobs_lvl2.empty() && 
				active_jobs_lvl3.empty())? 
			true : false;
		sem_post(&mutex_threads);
		sem_post(&mutex_cpu);
	}
	
	return NULL;
}

int main(int argc, char * argv[])
{	
	sem_init(&mutex_cout, 0, 1);
	sem_init(&mutex_cpu, 0, 1);
	sem_init(&mutex_threads, 0, 1);
	queue<int> job_list[MAX_REQUESTS];
	pthread_t  requester[MAX_REQUESTS], server;
	int request_thread_count = 0;

	/* parse and organize user input */
	if(argc < 2) {cout << "Not enough arguments" << endl; exit(EXIT_FAILURE);}			// send error if incorrect input
	max_cpu_queue = atoi(argv[1]);														// set max CPU queue amount
 	for(int i = 2; i < argc; i++){				// loop through requester thread files and return job lengths in a queue
 		job_list[request_thread_count] = parse_arguments(argv[i], job_list[request_thread_count]);
 		request_thread_count++;
 	}

 	/* create requester and servicing threads */
 	for(int i = 0; i < request_thread_count; i++){
 		pthread_create(&requester[i], NULL, requestThread, (void *) &job_list[i]);
 	}
 	pthread_create(&server, NULL, serviceThread, (void *) &request_thread_count);

 	/* wait for threads to join*/
 	for(int i = 0; i < request_thread_count; i++){
 		pthread_join(requester[i], NULL);
 	}
 	pthread_join(server, NULL);

 	/* clean up */
 	sem_destroy(&mutex_cout);
 	sem_destroy(&mutex_cpu);
 	sem_destroy(&mutex_threads);

	exit(EXIT_SUCCESS);
}
