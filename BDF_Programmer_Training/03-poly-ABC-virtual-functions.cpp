/*
 * @Author: Ning Zhang
 * @Date: 2024-05-23 12:53:39
 * @Last Modified by: Ning Zhang
 * @Last Modified time: 2024-05-23 13:44:42
 */

#include "include/polymorphism_example.h"

using namespace BDF_programmer_training::poly_example04;

class Pi_Calculator : public OpenMP_Algorithm_Template_Base
{
public:
    Pi_Calculator(int _end_indx) : OpenMP_Algorithm_Template_Base()
    {
        begin_indx = 1;
        end_indx = _end_indx;
        local_res.resize(num_of_threads, 0.0);
    }

    virtual void Initialize() noexcept
    {
        std::cout << "Pi_Calculator::Initialize()" << std::endl;
    }
    virtual void Finish() noexcept
    {
        auto res = std::accumulate(local_res.begin(), local_res.end(), 0.0);
        res = sqrt(res * 6.0);
        printf("Pi = %.10f\n", res);
    }
    virtual void Initialize_local() noexcept
    {
        auto thread_id = OMP_THREAD_LABEL;
        local_res[thread_id] = 0.0;
    }
    virtual void Finish_local() noexcept
    {
#pragma omp critical
        {
            std::cout << "thread " << OMP_THREAD_LABEL << " finished" << std::endl;
        }
    }
    virtual void Do_Task(size_t _task_id)
    {
        auto thread_id = OMP_THREAD_LABEL;
        local_res[thread_id] += 1.0 / (double(_task_id) * double(_task_id));
    }
    virtual bool Judge_Update_Data() noexcept { return false; }
    virtual bool Judge_Update_Data_Force() noexcept { return false; }
    virtual void Update_Data() {}
    virtual void Update_Data_Force() {}

protected:
    std::vector<double> local_res;
};

int main()
{
    Pi_Calculator pi_calculator(100000000);
    pi_calculator.Run();
    return 0;
}