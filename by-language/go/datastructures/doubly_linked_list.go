package datastructures

// DLLNode is a node in a doubly linked list.
type DLLNode[T comparable] struct {
	Value T
	Prev  *DLLNode[T]
	Next  *DLLNode[T]
}

// DoublyLinkedList is a doubly linked list with head and tail pointers.
type DoublyLinkedList[T comparable] struct {
	head   *DLLNode[T]
	tail   *DLLNode[T]
	length int
}

func NewDoublyLinkedList[T comparable]() *DoublyLinkedList[T] {
	return &DoublyLinkedList[T]{}
}

func (l *DoublyLinkedList[T]) Len() int {
	return l.length
}

func (l *DoublyLinkedList[T]) IsEmpty() bool {
	return l.length == 0
}

func (l *DoublyLinkedList[T]) Head() *DLLNode[T] {
	return l.head
}

func (l *DoublyLinkedList[T]) Tail() *DLLNode[T] {
	return l.tail
}

func (l *DoublyLinkedList[T]) Prepend(value T) *DLLNode[T] {
	node := &DLLNode[T]{Value: value, Next: l.head}
	if l.head != nil {
		l.head.Prev = node
	} else {
		l.tail = node
	}
	l.head = node
	l.length++
	return node
}

func (l *DoublyLinkedList[T]) Append(value T) *DLLNode[T] {
	node := &DLLNode[T]{Value: value, Prev: l.tail}
	if l.tail != nil {
		l.tail.Next = node
	} else {
		l.head = node
	}
	l.tail = node
	l.length++
	return node
}

func (l *DoublyLinkedList[T]) Insert(index int, value T) bool {
	if index < 0 || index > l.length {
		return false
	}
	if index == 0 {
		l.Prepend(value)
		return true
	}
	if index == l.length {
		l.Append(value)
		return true
	}

	current := l.getNode(index)
	node := &DLLNode[T]{Value: value, Prev: current.Prev, Next: current}
	current.Prev.Next = node
	current.Prev = node
	l.length++
	return true
}

func (l *DoublyLinkedList[T]) InsertAfter(node *DLLNode[T], value T) *DLLNode[T] {
	if node == nil {
		return nil
	}
	newNode := &DLLNode[T]{Value: value, Prev: node, Next: node.Next}
	if node.Next != nil {
		node.Next.Prev = newNode
	} else {
		l.tail = newNode
	}
	node.Next = newNode
	l.length++
	return newNode
}

func (l *DoublyLinkedList[T]) InsertBefore(node *DLLNode[T], value T) *DLLNode[T] {
	if node == nil {
		return nil
	}
	newNode := &DLLNode[T]{Value: value, Prev: node.Prev, Next: node}
	if node.Prev != nil {
		node.Prev.Next = newNode
	} else {
		l.head = newNode
	}
	node.Prev = newNode
	l.length++
	return newNode
}

func (l *DoublyLinkedList[T]) PopFirst() (T, bool) {
	if l.head == nil {
		var zero T
		return zero, false
	}
	value := l.head.Value
	l.head = l.head.Next
	if l.head != nil {
		l.head.Prev = nil
	} else {
		l.tail = nil
	}
	l.length--
	return value, true
}

func (l *DoublyLinkedList[T]) PopLast() (T, bool) {
	if l.tail == nil {
		var zero T
		return zero, false
	}
	value := l.tail.Value
	l.tail = l.tail.Prev
	if l.tail != nil {
		l.tail.Next = nil
	} else {
		l.head = nil
	}
	l.length--
	return value, true
}

func (l *DoublyLinkedList[T]) RemoveNode(node *DLLNode[T]) T {
	if node.Prev != nil {
		node.Prev.Next = node.Next
	} else {
		l.head = node.Next
	}
	if node.Next != nil {
		node.Next.Prev = node.Prev
	} else {
		l.tail = node.Prev
	}
	l.length--
	return node.Value
}

func (l *DoublyLinkedList[T]) MoveToFront(node *DLLNode[T]) {
	if node == l.head {
		return
	}
	l.RemoveNode(node)
	node.Prev = nil
	node.Next = l.head
	if l.head != nil {
		l.head.Prev = node
	}
	l.head = node
	if l.tail == nil {
		l.tail = node
	}
	l.length++
}

func (l *DoublyLinkedList[T]) MoveToBack(node *DLLNode[T]) {
	if node == l.tail {
		return
	}
	l.RemoveNode(node)
	node.Next = nil
	node.Prev = l.tail
	if l.tail != nil {
		l.tail.Next = node
	}
	l.tail = node
	if l.head == nil {
		l.head = node
	}
	l.length++
}

func (l *DoublyLinkedList[T]) Find(value T) *DLLNode[T] {
	current := l.head
	for current != nil {
		if current.Value == value {
			return current
		}
		current = current.Next
	}
	return nil
}

func (l *DoublyLinkedList[T]) Get(index int) (T, bool) {
	if index < 0 || index >= l.length {
		var zero T
		return zero, false
	}
	node := l.getNode(index)
	return node.Value, true
}

func (l *DoublyLinkedList[T]) ToSlice() []T {
	result := make([]T, 0, l.length)
	current := l.head
	for current != nil {
		result = append(result, current.Value)
		current = current.Next
	}
	return result
}

func (l *DoublyLinkedList[T]) ToSliceReverse() []T {
	result := make([]T, 0, l.length)
	current := l.tail
	for current != nil {
		result = append(result, current.Value)
		current = current.Prev
	}
	return result
}

// getNode traverses from the closer end for O(n/2) average.
func (l *DoublyLinkedList[T]) getNode(index int) *DLLNode[T] {
	if index < l.length/2 {
		current := l.head
		for i := 0; i < index; i++ {
			current = current.Next
		}
		return current
	}
	current := l.tail
	for i := l.length - 1; i > index; i-- {
		current = current.Prev
	}
	return current
}

// Reverse reverses the entire list in place.
func (l *DoublyLinkedList[T]) Reverse() {
	current := l.head
	for current != nil {
		current.Prev, current.Next = current.Next, current.Prev
		current = current.Prev // was Next before swap
	}
	l.head, l.tail = l.tail, l.head
}

// ForEach applies a function to each value.
func (l *DoublyLinkedList[T]) ForEach(fn func(T)) {
	current := l.head
	for current != nil {
		fn(current.Value)
		current = current.Next
	}
}

// ForEachReverse applies a function to each value in reverse order.
func (l *DoublyLinkedList[T]) ForEachReverse(fn func(T)) {
	current := l.tail
	for current != nil {
		fn(current.Value)
		current = current.Prev
	}
}

// Contains checks if a value exists in the list.
func (l *DoublyLinkedList[T]) Contains(value T) bool {
	return l.Find(value) != nil
}

// RemoveAll removes all nodes with the given value and returns count removed.
func (l *DoublyLinkedList[T]) RemoveAll(value T) int {
	count := 0
	current := l.head
	for current != nil {
		next := current.Next
		if current.Value == value {
			l.RemoveNode(current)
			count++
		}
		current = next
	}
	return count
}

// IsPalindrome checks if the list values form a palindrome.
func (l *DoublyLinkedList[T]) IsPalindrome() bool {
	if l.length <= 1 {
		return true
	}
	front := l.head
	back := l.tail
	for i := 0; i < l.length/2; i++ {
		if front.Value != back.Value {
			return false
		}
		front = front.Next
		back = back.Prev
	}
	return true
}

// Rotate rotates the list k positions to the right.
func (l *DoublyLinkedList[T]) Rotate(k int) {
	if l.length <= 1 || k <= 0 {
		return
	}
	k = k % l.length
	if k == 0 {
		return
	}
	// Find split point: length - k steps from head
	steps := l.length - k
	splitNode := l.head
	for i := 1; i < steps; i++ {
		splitNode = splitNode.Next
	}

	newHead := splitNode.Next
	splitNode.Next = nil
	newHead.Prev = nil
	l.tail.Next = l.head
	l.head.Prev = l.tail
	l.head = newHead
	l.tail = splitNode
}

// Swap swaps the values of two nodes at the given indices.
func (l *DoublyLinkedList[T]) Swap(i, j int) bool {
	if i == j || i < 0 || j < 0 || i >= l.length || j >= l.length {
		return false
	}
	nodeI := l.getNode(i)
	nodeJ := l.getNode(j)
	nodeI.Value, nodeJ.Value = nodeJ.Value, nodeI.Value
	return true
}

// InsertSorted inserts a value into a sorted list maintaining sort order.
func (l *DoublyLinkedList[T]) InsertSorted(value T, less func(T, T) bool) {
	node := &DLLNode[T]{Value: value}
	if l.head == nil || less(value, l.head.Value) {
		l.Prepend(value)
		return
	}
	current := l.head
	for current.Next != nil && !less(value, current.Next.Value) {
		current = current.Next
	}
	node.Prev = current
	node.Next = current.Next
	if current.Next != nil {
		current.Next.Prev = node
	} else {
		l.tail = node
	}
	current.Next = node
	l.length++
}

// MergeSorted merges another sorted DLL into this sorted DLL.
func MergeSortedDLL[T comparable](l1, l2 *DoublyLinkedList[T], less func(T, T) bool) *DoublyLinkedList[T] {
	result := NewDoublyLinkedList[T]()
	a := l1.head
	b := l2.head
	for a != nil && b != nil {
		if less(a.Value, b.Value) || (!less(b.Value, a.Value) && !less(a.Value, b.Value)) {
			result.Append(a.Value)
			a = a.Next
		} else {
			result.Append(b.Value)
			b = b.Next
		}
	}
	for a != nil {
		result.Append(a.Value)
		a = a.Next
	}
	for b != nil {
		result.Append(b.Value)
		b = b.Next
	}
	return result
}

// Copy creates a deep copy of the list.
func (l *DoublyLinkedList[T]) Copy() *DoublyLinkedList[T] {
	result := NewDoublyLinkedList[T]()
	current := l.head
	for current != nil {
		result.Append(current.Value)
		current = current.Next
	}
	return result
}

// Clear removes all elements from the list.
func (l *DoublyLinkedList[T]) Clear() {
	l.head = nil
	l.tail = nil
	l.length = 0
}

func DLLFromSlice[T comparable](items []T) *DoublyLinkedList[T] {
	l := NewDoublyLinkedList[T]()
	for _, item := range items {
		l.Append(item)
	}
	return l
}
