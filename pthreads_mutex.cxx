/*
 * pthreads_mutex.cxx
 * 
 * Copyright 2016 olegartys <olegartys@olegartys-HP-Pavilion-15-Notebook-PC>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>

#include <unistd.h>
#include <pthread.h>

pthread_mutex_t    mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_spinlock_t spin_mutex;
pthread_rwlock_t   rw_r_mutex = PTHREAD_RWLOCK_INITIALIZER;
pthread_rwlock_t   rw_w_mutex = PTHREAD_RWLOCK_INITIALIZER;

void* f_thread1(void*) { pthread_mutex_lock(&mutex); std::cout << "1 is free\n"; return NULL; }
void* f_thread2(void*) { pthread_spin_lock(&spin_mutex); std::cout << "2 is free\n"; return NULL; }
void* f_thread3(void*) { pthread_rwlock_wrlock(&rw_r_mutex); std::cout << "3 is free\n"; return NULL; }
void* f_thread4(void*) { pthread_rwlock_rdlock(&rw_w_mutex); std::cout << "4 is free\n"; return NULL; }

int main(int argc, char **argv)
{
	pthread_mutex_lock(&mutex);
	
	pthread_spin_init(&spin_mutex, PTHREAD_PROCESS_PRIVATE);
	pthread_spin_lock(&spin_mutex);
	pthread_rwlock_rdlock(&rw_r_mutex);
	pthread_rwlock_wrlock(&rw_w_mutex);
	
	pthread_t threads[4];
	void*(*thread_funcs[4])(void*) = {f_thread1, f_thread2, f_thread3, f_thread4};
	for (int i = 0; i < 4; ++i) {
		pthread_create(&threads[i], NULL, thread_funcs[i], NULL);
	}
	
	sleep(15);
	pthread_mutex_unlock(&mutex);
	pthread_spin_unlock(&spin_mutex);
	pthread_rwlock_unlock(&rw_r_mutex);
	pthread_rwlock_unlock(&rw_w_mutex);
	
	return 0;
}

