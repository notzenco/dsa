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

func (h *MinHeap[T]) Size() int    { return len(h.data) }
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

func (h *MaxHeap[T]) Size() int    { return len(h.data) }
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
