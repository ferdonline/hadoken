/**
 * Copyright (c) 2016, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 * Boost Software License - Version 1.0
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
*
*/
#ifndef _HADOKEN_LATCH_HPP_
#define _HADOKEN_LATCH_HPP_

#include <atomic>
#include <thread>
#include <assert.h>
#include <condition_variable>


namespace hadoken {

namespace thread{

///
/// \brief latch class
///
/// latch ( C++17 implementation )
///
/// ( extract from http://en.cppreference.com/w/cpp/experimental/latch )
/// The latch class is a downward counter of type ptrdiff_t which can be used to synchronize threads. The value of the counter is initialized on creation.
/// Threads may block on the latch until the counter is decremented to zero.
/// There is no possibility to increase or reset the counter, which makes the latch a single-use barrier.
///
///
class latch{
public:
    ///
    /// \brief construct a new latch
    /// \param value : the initial value of the counter, must be non negative
    ///
    inline latch(std::ptrdiff_t value) :
        _counter((value !=0)?(value):(unlocked)),
        _cond(),
        _latch_lock(){
        assert(value >= 0);
    }

    /// default destructor
    ~latch() = default;

    /// count down, without waiting
    /// non blocking
    ///
    /// \param n : value to decremement
    ///
    inline void count_down(std::ptrdiff_t n = 1){
        const std::ptrdiff_t previous_val = _counter.fetch_sub(n);
        __check_and_notify(previous_val - n);
    }


    /// count down and wait until the counter is 0
    /// \param n : value to decremement
    ///
    inline void count_down_and_wait(std::ptrdiff_t n = 1){
        count_down(n);
        wait();
    }

    ///
    /// \brief return  true if the counter reached 0
    ///
    inline bool is_ready() const{
        return (_counter.load() <= unlocked);
    }

    ///
    /// \brief wait untile the counter reach 0
    ///
    inline void wait(){
        // lightweight optimistic two phases unlock
        // avoid race condition on condition variable/mutex destruction
        // if the lifetime of the latch is the same that one of the waiting thread
        while(_counter.load() > unlocked){
            if(_counter.load() > 0){
                std::unique_lock<std::mutex> _l(_latch_lock);
                _cond.wait_for(_l, std::chrono::milliseconds(1));
            }else{
#ifndef HADOKEN_SPIN_NO_YIELD
                std::this_thread::yield();
#else
                // dummy implementation for platforms without sched_yield() support 
                usleep(1);
#endif
            }
        }
    }


private:
    static constexpr std::ptrdiff_t unlocked = -2048;

    inline void __check_and_notify(std::ptrdiff_t v){
        if(v <= 0){
            _cond.notify_all();
            _counter.store(unlocked);
        }
    }



    latch(const latch &) = delete;
    latch & operator=(const latch&) = delete;

    std::atomic<std::ptrdiff_t> _counter;
    std::condition_variable _cond;
    std::mutex _latch_lock;
};


} // thread


} //hadoken

#endif // _HADOKEN_LATCH_HPP_