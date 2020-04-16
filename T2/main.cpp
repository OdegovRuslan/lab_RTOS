#include <fstream>
#include <iostream>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

const static int BUF_SIZE = 1000;
pthread_barrier_t barrier;
int n;

struct Data {
    pthread_barrier_t barrier;
    int* buffer_lkg;
    char* buffer_t;
    char* buffer;
};


void* f(void* arc){
	Data* d = reinterpret_cast<Data*>(arc);

	for (int i = 0; i < BUF_SIZE/n; ++i)
    {
    	d->buffer[i] = d->buffer_t[i] ^ (d->buffer_lkg[i] % 127);
    }

	pthread_barrier_wait(&d->barrier);
}


int main(int argc, char *argv[]){
	n = sysconf(_SC_NPROCESSORS_ONLN);

	char* buffer = new char[BUF_SIZE+1];
	char* buffer_t = new char[BUF_SIZE+1];
	int* buffer_lkg = new int[BUF_SIZE+1];

	int fd = open(argv[1], O_RDONLY, S_IWRITE | S_IREAD);
	if (fd == -1) {
	    cout <<" err"<< endl;
	    return 0;
	}
	read(fd, buffer_t, BUF_SIZE);
	close(fd);

	ifstream lkg_file("lkg.txt", ios::binary);
	int i = 0;
	while(!lkg_file.eof()){
	    lkg_file >> buffer_lkg[i];
	    i++;
	}
	lkg_file.close();


    pthread_t* p = new pthread_t[n];
    Data* d = new Data[n];

    int b = pthread_barrier_init(&barrier, NULL, n+1);

    for (int i = 0; i < n; ++i)
    {	
    	d[i].buffer_lkg = new int[(BUF_SIZE / n) + 1];
    	d[i].buffer_t = new char[(BUF_SIZE / n) + 1];
    	d[i].buffer = new char[(BUF_SIZE / n) + 1];
    	d[i].barrier = barrier;

    	for (int j = 0; j < (BUF_SIZE/n); ++j)
    	{
    	    d[i].buffer_lkg[j] = buffer_lkg[j + (BUF_SIZE/n)*i];
    		d[i].buffer_t[j] = buffer_t[j + (BUF_SIZE/n)*i];
    	}

    	pthread_create(&p[i], NULL, f, &d[i]);
    }

    pthread_barrier_wait(&barrier);

    for (int i = 0; i < n; ++i)
    {
    	for (int j = 0; j < (BUF_SIZE/n); ++j)
    	{
    		buffer[i*(BUF_SIZE/n)+j] = d[i].buffer[j];
    	}

    }

	fd = open(argv[2], O_WRONLY | O_CREAT, S_IWRITE | S_IREAD);
	    if (fd == -1) {
	        cout << "Cannot open Output File" << endl;
	        return 0;
	    }

	write(fd, buffer, BUF_SIZE);
	close(fd);

    delete buffer, buffer_t, buffer_lkg;
    return 0;
}