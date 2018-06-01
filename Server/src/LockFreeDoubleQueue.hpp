#ifndef RHIO_LOCKFREEDOUBLEQUEUE_HPP
#define RHIO_LOCKFREEDOUBLEQUEUE_HPP

#include <vector>
#include <atomic>

/**
 * LockFreeDoubleQueue
 *
 * Multiple RealTime (RT) writers appending to 
 * a double buffer while a non RT reader 
 * is accessing the second buffer.
 * The writer operation is lock free with
 * no dynamic allocation (with potentially exceptions
 * due to given template type).
 */
template <typename T>
class LockFreeDoubleQueue
{
    public:

        /**
         * Initialization with maximum 
         * pre-allocated queue buffer size
         */
        LockFreeDoubleQueue(size_t maxSize) :
            _maxSize(maxSize),
            _isWritingTo1(1),
            _stillWriting1(0),
            _stillWriting2(0),
            _length1(0),
            _length2(0),
            _buffer1(),
            _buffer2()
        {
            //Memory pre-allocation
            _buffer1.resize(_maxSize);
            _buffer2.resize(_maxSize);
        }

        /**
         * Swap reading and writing internal buffer.
         * Wait for the writers to complete the writing.
         */
        void swapBufferFromReader()
        {
            //Retrieve writing buffer
            int isOldWriting1 = _isWritingTo1.load();
            //Reset reading buffer size
            if (isOldWriting1) {
                _length2.store(0);
            } else {
                _length1.store(0);
            }
            //Atomically inverse the flag
            _isWritingTo1.fetch_xor(1);
            //Actively wait that all writer complete 
            //writing to old writing buffer
            if (isOldWriting1) {
                while (_stillWriting1.load() != 0) {
                    //Wait
                }
            } else {
                while (_stillWriting2.load() != 0) {
                    //Wait
                }
            }
        }

        /**
         * Direct access to reading (privately own) buffer
         */
        const std::vector<T>& getBufferFromReader() const
        {
            if (_isWritingTo1.load()) {
                return _buffer2;
            } else {
                return _buffer1;
            }
        }

        /**
         * Return the size of currently own by reader buffer
         */
        size_t getSizeFromReader() const
        {
            size_t length;
            if (_isWritingTo1.load()) {
                length = _length2.load();
            } else {
                length = _length1.load();
            }
            
            //Shrink actual size to maximum size since writers 
            //increase length before checking for for length limit
            if (length > _maxSize) {
                length = _maxSize;
            }
            return length;
        }

        /**
         * Append to internal queue the given value.
         * RT, thread safe, no dynamic allocation.
         * If internal buffer is full, given data is dropped.
         * Return false if the value has to be dropped.
         */
        bool appendFromWriter(const T& val)
        {
            bool isOk = false;
            if (_isWritingTo1.load()) {
                //Take writing lock on buffer (wait in swapBuffer)
                _stillWriting1.fetch_add(1);
                //Retrieve next to write index and
                //atomically increase it (after reading)
                size_t index = _length1.fetch_add(1);
                //Write to buffer of not full
                if (index < _maxSize) {
                    _buffer1[index] = val;
                    isOk = true;
                }
                //Release the lock
                _stillWriting1.fetch_sub(1);
            } else {
                //Take writing lock on buffer (wait in swapBuffer)
                _stillWriting2.fetch_add(1);
                //Retrieve next to write index and
                //atomically increase it (after reading)
                size_t index = _length2.fetch_add(1);
                //Write to buffer of not full
                if (index < _maxSize) {
                    _buffer2[index] = val;
                    isOk = true;
                }
                //Release the lock
                _stillWriting2.fetch_sub(1);
            }

            return isOk;
        }

    private:

        /**
         * Maximum data contained by each buffers.
         * Use for pre-dynamic allocation.
         */
        const size_t _maxSize;

        /**
         * If non zero, the writers must write 
         * into internal buffer 1. 
         * If zero, the buffer 2 is used.
         */
        std::atomic<int> _isWritingTo1;
        
        /**
         * Hold the number of writers
         * currently writing to buffer 1 and 2
         */
        std::atomic<int> _stillWriting1;
        std::atomic<int> _stillWriting2;

        /**
         * Index of next available writable
         * in buffer 1 and 2.
         */
        std::atomic<size_t> _length1;
        std::atomic<size_t> _length2;

        /**
         * Internal pre-allocated buffers
         */
        std::vector<T> _buffer1;
        std::vector<T> _buffer2;
};

#endif

