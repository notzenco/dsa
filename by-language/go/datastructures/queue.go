package datastructures

// Queue is a generic FIFO data structure.
type Queue[T any] struct {
	items []T
}

func NewQueue[T any]() *Queue[T] {
	return &Queue[T]{}
}

func (q *Queue[T]) Enqueue(item T) {
	q.items = append(q.items, item)
}

func (q *Queue[T]) Dequeue() (T, bool) {
	if len(q.items) == 0 {
		var zero T
		return zero, false
	}
	item := q.items[0]
	q.items = q.items[1:]
	return item, true
}

func (q *Queue[T]) Peek() (T, bool) {
	if len(q.items) == 0 {
		var zero T
		return zero, false
	}
	return q.items[0], true
}

func (q *Queue[T]) Rear() (T, bool) {
	if len(q.items) == 0 {
		var zero T
		return zero, false
	}
	return q.items[len(q.items)-1], true
}

func (q *Queue[T]) IsEmpty() bool {
	return len(q.items) == 0
}

func (q *Queue[T]) Size() int {
	return len(q.items)
}

func (q *Queue[T]) Clear() {
	q.items = nil
}

// CircularQueue is a fixed-capacity circular queue.
type CircularQueue[T any] struct {
	items    []T
	head     int
	tail     int
	size     int
	capacity int
}

func NewCircularQueue[T any](capacity int) *CircularQueue[T] {
	return &CircularQueue[T]{
		items:    make([]T, capacity),
		capacity: capacity,
	}
}

func (q *CircularQueue[T]) Enqueue(item T) bool {
	if q.size == q.capacity {
		return false
	}
	q.items[q.tail] = item
	q.tail = (q.tail + 1) % q.capacity
	q.size++
	return true
}

func (q *CircularQueue[T]) Dequeue() (T, bool) {
	if q.size == 0 {
		var zero T
		return zero, false
	}
	item := q.items[q.head]
	q.head = (q.head + 1) % q.capacity
	q.size--
	return item, true
}

func (q *CircularQueue[T]) Peek() (T, bool) {
	if q.size == 0 {
		var zero T
		return zero, false
	}
	return q.items[q.head], true
}

func (q *CircularQueue[T]) Rear() (T, bool) {
	if q.size == 0 {
		var zero T
		return zero, false
	}
	idx := (q.tail - 1 + q.capacity) % q.capacity
	return q.items[idx], true
}

func (q *CircularQueue[T]) IsEmpty() bool {
	return q.size == 0
}

func (q *CircularQueue[T]) IsFull() bool {
	return q.size == q.capacity
}

func (q *CircularQueue[T]) Size() int {
	return q.size
}

func (q *CircularQueue[T]) Capacity() int {
	return q.capacity
}

// Deque is a double-ended queue.
type Deque[T any] struct {
	items []T
}

func NewDeque[T any]() *Deque[T] {
	return &Deque[T]{}
}

func (d *Deque[T]) AddFront(item T) {
	d.items = append([]T{item}, d.items...)
}

func (d *Deque[T]) AddRear(item T) {
	d.items = append(d.items, item)
}

func (d *Deque[T]) RemoveFront() (T, bool) {
	if len(d.items) == 0 {
		var zero T
		return zero, false
	}
	item := d.items[0]
	d.items = d.items[1:]
	return item, true
}

func (d *Deque[T]) RemoveRear() (T, bool) {
	if len(d.items) == 0 {
		var zero T
		return zero, false
	}
	item := d.items[len(d.items)-1]
	d.items = d.items[:len(d.items)-1]
	return item, true
}

func (d *Deque[T]) PeekFront() (T, bool) {
	if len(d.items) == 0 {
		var zero T
		return zero, false
	}
	return d.items[0], true
}

func (d *Deque[T]) PeekRear() (T, bool) {
	if len(d.items) == 0 {
		var zero T
		return zero, false
	}
	return d.items[len(d.items)-1], true
}

func (d *Deque[T]) IsEmpty() bool {
	return len(d.items) == 0
}

func (d *Deque[T]) Size() int {
	return len(d.items)
}

// MonotonicQueue maintains a decreasing order for max queries.
type MonotonicQueueEntry struct {
	Value int
	Index int
}

type MonotonicQueue struct {
	deque []MonotonicQueueEntry
}

func NewMonotonicQueue() *MonotonicQueue {
	return &MonotonicQueue{}
}

func (mq *MonotonicQueue) Push(value, index int) {
	for len(mq.deque) > 0 && mq.deque[len(mq.deque)-1].Value <= value {
		mq.deque = mq.deque[:len(mq.deque)-1]
	}
	mq.deque = append(mq.deque, MonotonicQueueEntry{Value: value, Index: index})
}

func (mq *MonotonicQueue) PopByIndex(index int) {
	if len(mq.deque) > 0 && mq.deque[0].Index <= index {
		mq.deque = mq.deque[1:]
	}
}

func (mq *MonotonicQueue) GetMax() (int, bool) {
	if len(mq.deque) == 0 {
		return 0, false
	}
	return mq.deque[0].Value, true
}

func (mq *MonotonicQueue) IsEmpty() bool {
	return len(mq.deque) == 0
}

// SlidingWindowMaximum returns the maximum in each window of size k.
func SlidingWindowMaximum(arr []int, k int) []int {
	if len(arr) == 0 || k <= 0 {
		return nil
	}
	if k > len(arr) {
		k = len(arr)
	}

	result := make([]int, 0, len(arr)-k+1)
	mq := NewMonotonicQueue()

	for i := 0; i < len(arr); i++ {
		mq.Push(arr[i], i)
		if i >= k-1 {
			mq.PopByIndex(i - k)
			max, _ := mq.GetMax()
			result = append(result, max)
		}
	}

	return result
}
