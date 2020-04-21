#ifndef TIMER_H 
#define TIMER_H

#include <chrono>
#include <iostream>

    class Timer {
        public:
            Timer(){ m_StartTimePoint = std::chrono::high_resolution_clock::now();}
            ~Timer(){ stop();}

            void stop(){
                std::chrono::high_resolution_clock::time_point EndTimePoint = std::chrono::high_resolution_clock::now();

                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(EndTimePoint - m_StartTimePoint).count();
                double milliSeconds = duration * 0.001;

                std::cout << duration << "us (" << milliSeconds << "ms)\n";
            }
        private:
            std::chrono::high_resolution_clock::time_point m_StartTimePoint;

    };



#endif //TIMER_H