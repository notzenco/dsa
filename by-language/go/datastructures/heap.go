package datastructures

// MinHeap is a min-heap of ordered elements.
type MinHeap[T Ordered] struct {
	data []T
}

func NewMinHeap[T Ordered]() *MinHeap[T] {
	return &MinHeap[T]{}
}

func MinHeapFromSlice[T Ordered](items []T) *MinHeap[T] {
	h := &MinHeap[T]{data: make([]T, len(items))}
	copy(h.data, items)
	for i := len(h.data)/2 - 1; i >= 0; i-- {
		h.siftDown(i)
	}
	return h
}

func (h *MinHeap[T]) Size() int     { return len(h.data) }
func (h *MinHeap[T]) IsEmpty() bool { return len(h.data) == 0 }

func (h *MinHeap[T]) Peek() (T, bool) {
	if len(h.data) == 0 {
		var zero T
		return zero, false
	}
	return h.data[0], true
}

func (h *MinHeap[T]) Push(val T) {
	h.data = append(h.data, val)
	h.siftUp(len(h.data) - 1)
}

func (h *MinHeap[T]) Pop() (T, bool) {
	if len(h.data) == 0 {
		var zero T
		return zero, false
	}
	val := h.data[0]
	last := len(h.data) - 1
	h.data[0] = h.data[last]
	h.data = h.data[:last]
	if len(h.data) > 0 {
		h.siftDown(0)
	}
	return val, true
}

func (h *MinHeap[T]) PushPop(val T) T {
	if len(h.data) > 0 && h.data[0] < val {
		val, h.data[0] = h.data[0], val
		h.siftDown(0)
	}
	return val
}

func (h *MinHeap[T]) Replace(val T) (T, bool) {
	if len(h.data) == 0 {
		var zero T
		return zero, false
	}
	old := h.data[0]
	h.data[0] = val
	h.siftDown(0)
	return old, true
}

func (h *MinHeap[T]) ToSortedSlice() []T {
	tmp := &MinHeap[T]{data: make([]T, len(h.data))}
	copy(tmp.data, h.data)
	result := make([]T, 0, len(h.data))
	for tmp.Size() > 0 {
		val, _ := tmp.Pop()
		result = append(result, val)
	}
	return result
}

func (h *MinHeap[T]) siftUp(i int) {
	for i > 0 {
		parent := (i - 1) / 2
		if h.data[i] < h.data[parent] {
			h.data[i], h.data[parent] = h.data[parent], h.data[i]
			i = parent
		} else {
			break
		}
	}
}

func (h *MinHeap[T]) siftDown(i int) {
	n := len(h.data)
	for {
		smallest := i
		left := 2*i + 1
		right := 2*i + 2
		if left < n && h.data[left] < h.data[smallest] {
			smallest = left
		}
		if right < n && h.data[right] < h.data[smallest] {
			smallest = right
		}
		if smallest == i {
			break
		}
		h.data[i], h.data[smallest] = h.data[smallest], h.data[i]
		i = smallest
	}
}

// MaxHeap is a max-heap of ordered elements.
type MaxHeap[T Ordered] struct {
	data []T
}

func NewMaxHeap[T Ordered]() *MaxHeap[T] {
	return &MaxHeap[T]{}
}

func MaxHeapFromSlice[T Ordered](items []T) *MaxHeap[T] {
	h := &MaxHeap[T]{data: make([]T, len(items))}
	copy(h.data, items)
	for i := len(h.data)/2 - 1; i >= 0; i-- {
		h.siftDown(i)
	}
	return h
}

func (h *MaxHeap[T]) Size() int     { return len(h.data) }
func (h *MaxHeap[T]) IsEmpty() bool { return len(h.data) == 0 }

func (h *MaxHeap[T]) Peek() (T, bool) {
	if len(h.data) == 0 {
		var zero T
		return zero, false
	}
	return h.data[0], true
}

func (h *MaxHeap[T]) Push(val T) {
	h.data = append(h.data, val)
	h.siftUp(len(h.data) - 1)
}

func (h *MaxHeap[T]) Pop() (T, bool) {
	if len(h.data) == 0 {
		var zero T
		return zero, false
	}
	val := h.data[0]
	last := len(h.data) - 1
	h.data[0] = h.data[last]
	h.data = h.data[:last]
	if len(h.data) > 0 {
		h.siftDown(0)
	}
	return val, true
}

func (h *MaxHeap[T]) ToSortedSlice() []T {
	tmp := &MaxHeap[T]{data: make([]T, len(h.data))}
	copy(tmp.data, h.data)
	result := make([]T, 0, len(h.data))
	for tmp.Size() > 0 {
		val, _ := tmp.Pop()
		result = append(result, val)
	}
	return result
}

func (h *MaxHeap[T]) siftUp(i int) {
	for i > 0 {
		parent := (i - 1) / 2
		if h.data[i] > h.data[parent] {
			h.data[i], h.data[parent] = h.data[parent], h.data[i]
			i = parent
		} else {
			break
		}
	}
}

func (h *MaxHeap[T]) siftDown(i int) {
	n := len(h.data)
	for {
		largest := i
		left := 2*i + 1
		right := 2*i + 2
		if left < n && h.data[left] > h.data[largest] {
			largest = left
		}
		if right < n && h.data[right] > h.data[largest] {
			largest = right
		}
		if largest == i {
			break
		}
		h.data[i], h.data[largest] = h.data[largest], h.data[i]
		i = largest
	}
}

// HeapSort sorts a slice using heap sort.
func HeapSort[T Ordered](arr []T) {
	n := len(arr)
	// Build max heap
	for i := n/2 - 1; i >= 0; i-- {
		heapSortSiftDown(arr, n, i)
	}
	// Extract elements
	for i := n - 1; i > 0; i-- {
		arr[0], arr[i] = arr[i], arr[0]
		heapSortSiftDown(arr, i, 0)
	}
}

func heapSortSiftDown[T Ordered](arr []T, n, i int) {
	for {
		largest := i
		left := 2*i + 1
		right := 2*i + 2
		if left < n && arr[left] > arr[largest] {
			largest = left
		}
		if right < n && arr[right] > arr[largest] {
			largest = right
		}
		if largest == i {
			break
		}
		arr[i], arr[largest] = arr[largest], arr[i]
		i = largest
	}
}

// NLargest returns the n largest elements from a slice.
func NLargest[T Ordered](arr []T, n int) []T {
	if n >= len(arr) {
		result := make([]T, len(arr))
		copy(result, arr)
		HeapSort(result)
		// Reverse to get descending
		for i, j := 0, len(result)-1; i < j; i, j = i+1, j-1 {
			result[i], result[j] = result[j], result[i]
		}
		return result
	}

	h := NewMinHeap[T]()
	for _, v := range arr {
		if h.Size() < n {
			h.Push(v)
		} else {
			h.PushPop(v)
		}
	}
	return h.ToSortedSlice()
}

// PriorityQueueItem is an item in a priority queue with priority and stable ordering.
type PriorityQueueItem[T any] struct {
	Value    T
	Priority int
	index    int // insertion order for stable sorting
}

// PriorityQueue is a min-priority queue with stable ordering.
type PriorityQueue[T any] struct {
	items   []PriorityQueueItem[T]
	counter int
}

func NewPriorityQueue[T any]() *PriorityQueue[T] {
	return &PriorityQueue[T]{}
}

func (pq *PriorityQueue[T]) Size() int     { return len(pq.items) }
func (pq *PriorityQueue[T]) IsEmpty() bool { return len(pq.items) == 0 }

func (pq *PriorityQueue[T]) Enqueue(value T, priority int) {
	item := PriorityQueueItem[T]{Value: value, Priority: priority, index: pq.counter}
	pq.counter++
	pq.items = append(pq.items, item)
	pq.pqSiftUp(len(pq.items) - 1)
}

func (pq *PriorityQueue[T]) Dequeue() (T, int, bool) {
	if len(pq.items) == 0 {
		var zero T
		return zero, 0, false
	}
	item := pq.items[0]
	last := len(pq.items) - 1
	pq.items[0] = pq.items[last]
	pq.items = pq.items[:last]
	if len(pq.items) > 0 {
		pq.pqSiftDown(0)
	}
	return item.Value, item.Priority, true
}

func (pq *PriorityQueue[T]) Peek() (T, int, bool) {
	if len(pq.items) == 0 {
		var zero T
		return zero, 0, false
	}
	return pq.items[0].Value, pq.items[0].Priority, true
}

func (pq *PriorityQueue[T]) Clear() {
	pq.items = nil
	pq.counter = 0
}

func (pq *PriorityQueue[T]) pqLess(i, j int) bool {
	if pq.items[i].Priority != pq.items[j].Priority {
		return pq.items[i].Priority < pq.items[j].Priority
	}
	return pq.items[i].index < pq.items[j].index
}

func (pq *PriorityQueue[T]) pqSiftUp(i int) {
	for i > 0 {
		parent := (i - 1) / 2
		if pq.pqLess(i, parent) {
			pq.items[i], pq.items[parent] = pq.items[parent], pq.items[i]
			i = parent
		} else {
			break
		}
	}
}

func (pq *PriorityQueue[T]) pqSiftDown(i int) {
	n := len(pq.items)
	for {
		smallest := i
		left := 2*i + 1
		right := 2*i + 2
		if left < n && pq.pqLess(left, smallest) {
			smallest = left
		}
		if right < n && pq.pqLess(right, smallest) {
			smallest = right
		}
		if smallest == i {
			break
		}
		pq.items[i], pq.items[smallest] = pq.items[smallest], pq.items[i]
		i = smallest
	}
}

// MergeHeaps merges two min heaps into one.
func MergeHeaps[T Ordered](h1, h2 *MinHeap[T]) *MinHeap[T] {
	combined := make([]T, 0, h1.Size()+h2.Size())
	combined = append(combined, h1.data...)
	combined = append(combined, h2.data...)
	return MinHeapFromSlice(combined)
}

// KWayMerge merges k sorted slices into one sorted slice.
func KWayMerge[T Ordered](lists [][]T) []T {
	totalLen := 0
	for _, l := range lists {
		totalLen += len(l)
	}
	result := make([]T, 0, totalLen)

	entries := make([]kwayEntry[T], 0)
	for i, l := range lists {
		if len(l) > 0 {
			entries = append(entries, kwayEntry[T]{l[0], i, 0})
		}
	}

	// Build min heap
	n := len(entries)
	for i := n/2 - 1; i >= 0; i-- {
		kwaySiftDown(entries, n, i)
	}

	for len(entries) > 0 {
		result = append(result, entries[0].val)
		e := entries[0]
		nextIdx := e.elemIdx + 1
		if nextIdx < len(lists[e.listIdx]) {
			entries[0] = kwayEntry[T]{lists[e.listIdx][nextIdx], e.listIdx, nextIdx}
			kwaySiftDown(entries, len(entries), 0)
		} else {
			entries[0] = entries[len(entries)-1]
			entries = entries[:len(entries)-1]
			if len(entries) > 0 {
				kwaySiftDown(entries, len(entries), 0)
			}
		}
	}
	return result
}

type kwayEntry[T Ordered] struct {
	val     T
	listIdx int
	elemIdx int
}

func kwaySiftDown[T Ordered](entries []kwayEntry[T], n, i int) {
	for {
		smallest := i
		left := 2*i + 1
		right := 2*i + 2
		if left < n && entries[left].val < entries[smallest].val {
			smallest = left
		}
		if right < n && entries[right].val < entries[smallest].val {
			smallest = right
		}
		if smallest == i {
			break
		}
		entries[i], entries[smallest] = entries[smallest], entries[i]
		i = smallest
	}
}

// MedianFinder tracks the running median using two heaps.
type MedianFinder struct {
	maxHeap *MaxHeap[float64] // lower half
	minHeap *MinHeap[float64] // upper half
}

func NewMedianFinder() *MedianFinder {
	return &MedianFinder{
		maxHeap: NewMaxHeap[float64](),
		minHeap: NewMinHeap[float64](),
	}
}

func (mf *MedianFinder) AddNum(num float64) {
	mf.maxHeap.Push(num)

	// Ensure maxHeap top <= minHeap top
	if mf.maxHeap.Size() > 0 && mf.minHeap.Size() > 0 {
		maxTop, _ := mf.maxHeap.Peek()
		minTop, _ := mf.minHeap.Peek()
		if maxTop > minTop {
			val, _ := mf.maxHeap.Pop()
			mf.minHeap.Push(val)
		}
	}

	// Balance sizes: maxHeap can have at most 1 more than minHeap
	if mf.maxHeap.Size() > mf.minHeap.Size()+1 {
		val, _ := mf.maxHeap.Pop()
		mf.minHeap.Push(val)
	}
	if mf.minHeap.Size() > mf.maxHeap.Size() {
		val, _ := mf.minHeap.Pop()
		mf.maxHeap.Push(val)
	}
}

func (mf *MedianFinder) FindMedian() float64 {
	if mf.maxHeap.Size() > mf.minHeap.Size() {
		val, _ := mf.maxHeap.Peek()
		return val
	}
	maxTop, _ := mf.maxHeap.Peek()
	minTop, _ := mf.minHeap.Peek()
	return (maxTop + minTop) / 2.0
}

func (mf *MedianFinder) Size() int {
	return mf.maxHeap.Size() + mf.minHeap.Size()
}

// NSmallest returns the n smallest elements from a slice.
func NSmallest[T Ordered](arr []T, n int) []T {
	if n >= len(arr) {
		result := make([]T, len(arr))
		copy(result, arr)
		HeapSort(result)
		return result
	}

	h := NewMaxHeap[T]()
	for _, v := range arr {
		if h.Size() < n {
			h.Push(v)
		} else if v < h.data[0] {
			h.data[0] = v
			h.siftDown(0)
		}
	}
	result := h.ToSortedSlice()
	// Reverse for ascending order
	for i, j := 0, len(result)-1; i < j; i, j = i+1, j-1 {
		result[i], result[j] = result[j], result[i]
	}
	return result
}
