#ifndef __SQUEUE_H__
#define __SQUEUE_H__

#include <queue>
#include <mutex>

template <class T> 
class SQueue {
public:
	SQueue();
	~SQueue();
	void push(T e);
	T pop();
	T front();
	int size();
private:
	std::mutex mMutex;
	std::queue<T> mQueue;
};

template <class T>
SQueue<T>::SQueue() {

}

template <class T>
SQueue<T>::~SQueue() {

}

template <class T>
void SQueue<T>::push(T e) {
	mMutex.lock();
	mQueue.push(e);
	mMutex.unlock();
}

template <class T>
T SQueue<T>::pop() {
	mMutex.lock();
	if (mQueue.size() == 0) {
		mMutex.unlock();
		return NULL;
	}
	else {
		T e = mQueue.front();
		mQueue.pop();
		mMutex.unlock();
		return e;
	}
}

template <class T>
T SQueue<T>::front() {
	mMutex.lock();
	if (mQueue.size() == 0) {
		mMutex.unlock();
		return NULL;
	}
	else {
		T e = mQueue.front();
		mMutex.unlock();
		return e;
	}
}

template <class T>
int SQueue<T>::size() {
	mMutex.lock();
	int size = mQueue.size();
	mMutex.unlock();
	return size;
}

#endif