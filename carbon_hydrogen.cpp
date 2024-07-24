#include <iostream>  
#include <thread>  
#include <mutex>  
#include <condition_variable>
#include <cstring>  
  
class MethaneProducer {  
private:  
    std::mutex mtx;  
    std::condition_variable cv;  
    int hydrogen_count = 0;   
    bool carbon_ready = false;   
  
public:  
    void releaseHydrogen() {  
        std::unique_lock<std::mutex> lock(mtx);  
        cv.wait(lock, [this]() { return hydrogen_count < 4;});    
        std::cout << "H";  
        hydrogen_count++;  
  
        if (hydrogen_count == 4&&carbon_ready) {   
            hydrogen_count = 0; 
            carbon_ready = false;  
            cv.notify_all(); 
        }  
    }  
  
    void releaseCarbon() {  
        std::unique_lock<std::mutex> lock(mtx);  
        cv.wait(lock, [this]() { return  !carbon_ready; });  
        std::cout << "C"; 
        carbon_ready=true;
        if (hydrogen_count == 4&&carbon_ready) {  
            hydrogen_count = 0; 
            carbon_ready = false;  
            cv.notify_all();
        }   
         
    }  
};
int getNumber(char str[1000])
{
	int sum=0;
	for(int i=0;i<strlen(str);i++)
	{
		if((str[i]>='a'&&str[i]<='z')||(str[i]>='A'&&str[i]<='Z'))
		{
			sum++;
		}
	}
	return sum;
}  
  
int main() {  
    MethaneProducer producer;
    char str[1000];
	std::cin.get(str,1000);
	int number= getNumber(str);  
    std::thread hydrogen_threads[number*4/5];  
    std::thread carbon_threads[number/5]; 
    for (int i = 0; i < number/5; ++i) {  
        carbon_threads[i] = std::thread(&MethaneProducer::releaseCarbon, &producer);  
    }   
  
    for (int i = 0; i < number*4/5; ++i) {  
        hydrogen_threads[i] = std::thread(&MethaneProducer::releaseHydrogen, &producer);  
    }  
  
    std::cout << std::endl; 
  
    return 0;  
}