#ifndef BUFFER_H
#define BUFFER_H

template <class T>
class Buffer{
private:
    static const int buffer_size = 10;
    T buffer[buffer_size];
    int read_position = 0;
    int write_position = 0;
    mutex mtx;
    condition_variable buffer_not_full;
    condition_variable buffer_not_empty;
public:
    void add(T t) {
        unique_lock<mutex> lock(mtx);
        while(((write_position + 1) % buffer_size) == read_position) {
            buffer_not_full.wait(lock);
        }
        buffer[write_position] = t;
        write_position = (write_position + 1) % buffer_size;
        buffer_not_empty.notify_all();
        lock.unlock();
    }

    T fetch() {
        unique_lock<mutex> lock(mtx);
        while(write_position == read_position) {
            buffer_not_empty.wait(lock);
        }
        T t = buffer[read_position];
        read_position = (read_position + 1) % buffer_size;
        buffer_not_full.notify_all();
        lock.unlock();
        return t;
    }
};

#endif