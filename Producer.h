#pragma once

#include "BufferQueue.h"
#include <ctime>



class Producer {
public:

    Producer(int unique_id, BufferQueue* bQueue)
    {
        m_uniqueid = unique_id;
        m_bufferQueue = bQueue;
     
        m_count = 0;
    }

    ~Producer()
    {

    }

    void StartProducing()
    {
        sentinal = true;
        m_count = 0;
        pthread_create(&m_thread, NULL, addProduct, this);
    }

    void StopProducing()
    {
        sentinal = false;
        void* ret_val;
        pthread_join(m_thread, &ret_val);
    }

private:
    static void* addProduct(void* param)
    {
        int count = 0;
        VideoCapture cap = VideoCapture(0);
        Producer* _this = (Producer*)param;
        while (_this->sentinal )   
        {
            //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            cap >> _this->my_created_product;
            //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[�s]" << std::endl;
            
            if (count == 0)
            {
                _this->m_bufferQueue->addProductToQueue(_this->my_created_product);
            }
            ++(_this->m_count);
            ++count;
            if (count == 5)
            {
                count = 0;
            }

        }

        return NULL;
}
    bool sentinal;
    BufferQueue* m_bufferQueue;
    pthread_t m_thread;
    Mat my_created_product;
    VideoCapture *m_cap;
    int m_uniqueid;
    int m_count;
};

