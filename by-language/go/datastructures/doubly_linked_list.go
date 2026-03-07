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

func DLLFromSlice[T comparable](items []T) *DoublyLinkedList[T] {
	l := NewDoublyLinkedList[T]()
	for _, item := range items {
		l.Append(item)
	}
	return l
}
