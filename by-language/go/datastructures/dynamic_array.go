package datastructures

const (
	defaultCapacity = 8
	growthFactor    = 2
	shrinkThreshold = 0.25
)

// DynamicArray is a resizable array implementation.
type DynamicArray[T comparable] struct {
	data     []T
	size     int
	capacity int
}

func NewDynamicArray[T comparable]() *DynamicArray[T] {
	return &DynamicArray[T]{
		data:     make([]T, defaultCapacity),
		capacity: defaultCapacity,
	}
}

func NewDynamicArrayWithCapacity[T comparable](capacity int) *DynamicArray[T] {
	if capacity < 1 {
		capacity = 1
	}
	return &DynamicArray[T]{
		data:     make([]T, capacity),
		capacity: capacity,
	}
}

func (a *DynamicArray[T]) Size() int {
	return a.size
}

func (a *DynamicArray[T]) Capacity() int {
	return a.capacity
}

func (a *DynamicArray[T]) IsEmpty() bool {
	return a.size == 0
}

func (a *DynamicArray[T]) Get(index int) (T, bool) {
	if index < 0 || index >= a.size {
		var zero T
		return zero, false
	}
	return a.data[index], true
}

func (a *DynamicArray[T]) Set(index int, value T) bool {
	if index < 0 || index >= a.size {
		return false
	}
	a.data[index] = value
	return true
}

func (a *DynamicArray[T]) Append(value T) {
	if a.size == a.capacity {
		a.resize(a.capacity * growthFactor)
	}
	a.data[a.size] = value
	a.size++
}

func (a *DynamicArray[T]) Insert(index int, value T) bool {
	if index < 0 || index > a.size {
		return false
	}
	if a.size == a.capacity {
		a.resize(a.capacity * growthFactor)
	}
	copy(a.data[index+1:a.size+1], a.data[index:a.size])
	a.data[index] = value
	a.size++
	return true
}

func (a *DynamicArray[T]) Pop() (T, bool) {
	if a.size == 0 {
		var zero T
		return zero, false
	}
	a.size--
	value := a.data[a.size]

	if a.size > 0 && a.size <= int(float64(a.capacity)*shrinkThreshold) && a.capacity > defaultCapacity {
		newCap := a.capacity / growthFactor
		if newCap < defaultCapacity {
			newCap = defaultCapacity
		}
		a.resize(newCap)
	}
	return value, true
}

func (a *DynamicArray[T]) Remove(value T) bool {
	for i := 0; i < a.size; i++ {
		if a.data[i] == value {
			a.removeAt(i)
			return true
		}
	}
	return false
}

func (a *DynamicArray[T]) removeAt(index int) {
	copy(a.data[index:a.size-1], a.data[index+1:a.size])
	a.size--

	if a.size > 0 && a.size <= int(float64(a.capacity)*shrinkThreshold) && a.capacity > defaultCapacity {
		newCap := a.capacity / growthFactor
		if newCap < defaultCapacity {
			newCap = defaultCapacity
		}
		a.resize(newCap)
	}
}

func (a *DynamicArray[T]) Find(value T) int {
	for i := 0; i < a.size; i++ {
		if a.data[i] == value {
			return i
		}
	}
	return -1
}

func (a *DynamicArray[T]) Contains(value T) bool {
	return a.Find(value) != -1
}

func (a *DynamicArray[T]) Reverse() {
	for i, j := 0, a.size-1; i < j; i, j = i+1, j-1 {
		a.data[i], a.data[j] = a.data[j], a.data[i]
	}
}

func (a *DynamicArray[T]) Clear() {
	a.size = 0
	a.data = make([]T, defaultCapacity)
	a.capacity = defaultCapacity
}

func (a *DynamicArray[T]) ToSlice() []T {
	result := make([]T, a.size)
	copy(result, a.data[:a.size])
	return result
}

func (a *DynamicArray[T]) resize(newCapacity int) {
	newData := make([]T, newCapacity)
	copy(newData, a.data[:a.size])
	a.data = newData
	a.capacity = newCapacity
}

func DynamicArrayFromSlice[T comparable](items []T) *DynamicArray[T] {
	cap := defaultCapacity
	for cap < len(items) {
		cap *= growthFactor
	}
	a := &DynamicArray[T]{
		data:     make([]T, cap),
		size:     len(items),
		capacity: cap,
	}
	copy(a.data, items)
	return a
}
