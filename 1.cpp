#include <iostream>
#include<random>
#include<thread>
#include<vector>
#include<iterator>
#include"timer.h"
#include<mutex>
#include<future>
using namespace std;
class Data{
private:
mutex mutex_;
long double count_;
public:
    Data():count_(0){};
   void add(long double count){
        lock_guard<mutex> lock(mutex_);
        count_+=count;
    }
    long double print(){
        lock_guard<mutex> lock(mutex_);
        return count_;
   }

};
//template<typename Iterator>
void MONTE( Data* data, int begin, int end,int j){
    random_device rd;
    mt19937 RD(rd());
    long double count = 0;
    for(auto i =begin;  i<end; i++){
        uniform_real_distribution<> dis(10.0/6.0*j, 10.0/6.0*(j+1));
        uniform_real_distribution<> dis1(0.0, 10.0);
        if(sqrt(pow(dis(RD),2)+pow(dis1(RD),2))<10.0){
            count++;
        }
    }
    data->add(count);
}
int main() {

    long long int N=1000000000;

    //vector<int> k(6,0);
    int n = thread::hardware_concurrency();
    Data counter;
    vector <thread> thr(n);
    vector<std::future<void>> futures(n);
    Timer parallel;
    for(int j=0; j<n-1; j++) {
        packaged_task<void(Data* , int, int, int)> task(MONTE);
        futures[j] = task.get_future();
        thr[j]=thread(move(task),&counter, N/n*j, N/n*(j+1), j);
    }
    MONTE(&counter, N/n*(n-1), N, thread::hardware_concurrency()-1);
    for(int j=0; j<5; j++) {thr[j].join();}
    long double pi = 4*(counter.print())/N;
    cout<<"Thread pi calculation: "<<pi<<endl;
    cout<<"Time of thread calculation: "<<parallel.print()<<endl;
    double sum = 0;
    random_device rd;
    mt19937 RD(rd());
    Timer line;
    for(int i=0; i<N; i++){
        uniform_real_distribution<> dis(0.0, 10.0);
        uniform_real_distribution<> dis1(0.0, 10.0);
        if(sqrt(pow(dis(RD),2)+pow(dis1(RD),2))<10.0){
            sum++;
        }
    }
    cout<<"Thread pi calculation: "<<4*sum/N<<endl;
    cout<<"Time of thread calculation: "<<line.print()<<endl;
    return 0;
}
