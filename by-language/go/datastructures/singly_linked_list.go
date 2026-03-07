package datastructures

// SLLNode is a node in a singly linked list.
type SLLNode[T comparable] struct {
	Value T
	Next  *SLLNode[T]
}

// SinglyLinkedList is a singly linked list with head and tail pointers.
type SinglyLinkedList[T comparable] struct {
	head   *SLLNode[T]
	tail   *SLLNode[T]
	length int
}

func NewSinglyLinkedList[T comparable]() *SinglyLinkedList[T] {
	return &SinglyLinkedList[T]{}
}

func (l *SinglyLinkedList[T]) Len() int {
	return l.length
}

func (l *SinglyLinkedList[T]) IsEmpty() bool {
	return l.length == 0
}

func (l *SinglyLinkedList[T]) Head() *SLLNode[T] {
	return l.head
}

func (l *SinglyLinkedList[T]) Tail() *SLLNode[T] {
	return l.tail
}

func (l *SinglyLinkedList[T]) Prepend(value T) {
	node := &SLLNode[T]{Value: value, Next: l.head}
	l.head = node
	if l.tail == nil {
		l.tail = node
	}
	l.length++
}

func (l *SinglyLinkedList[T]) Append(value T) {
	node := &SLLNode[T]{Value: value}
	if l.tail == nil {
		l.head = node
		l.tail = node
	} else {
		l.tail.Next = node
		l.tail = node
	}
	l.length++
}

func (l *SinglyLinkedList[T]) Insert(index int, value T) bool {
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

	prev := l.head
	for i := 0; i < index-1; i++ {
		prev = prev.Next
	}
	node := &SLLNode[T]{Value: value, Next: prev.Next}
	prev.Next = node
	l.length++
	return true
}

func (l *SinglyLinkedList[T]) PopFirst() (T, bool) {
	if l.head == nil {
		var zero T
		return zero, false
	}
	value := l.head.Value
	l.head = l.head.Next
	if l.head == nil {
		l.tail = nil
	}
	l.length--
	return value, true
}

func (l *SinglyLinkedList[T]) PopLast() (T, bool) {
	if l.head == nil {
		var zero T
		return zero, false
	}
	if l.head == l.tail {
		value := l.head.Value
		l.head = nil
		l.tail = nil
		l.length--
		return value, true
	}

	current := l.head
	for current.Next != l.tail {
		current = current.Next
	}
	value := l.tail.Value
	current.Next = nil
	l.tail = current
	l.length--
	return value, true
}

func (l *SinglyLinkedList[T]) Pop(index int) (T, bool) {
	if index < 0 || index >= l.length {
		var zero T
		return zero, false
	}
	if index == 0 {
		return l.PopFirst()
	}
	if index == l.length-1 {
		return l.PopLast()
	}

	prev := l.head
	for i := 0; i < index-1; i++ {
		prev = prev.Next
	}
	value := prev.Next.Value
	prev.Next = prev.Next.Next
	l.length--
	return value, true
}

func (l *SinglyLinkedList[T]) Remove(value T) bool {
	if l.head == nil {
		return false
	}
	if l.head.Value == value {
		l.PopFirst()
		return true
	}
	current := l.head
	for current.Next != nil {
		if current.Next.Value == value {
			if current.Next == l.tail {
				l.tail = current
			}
			current.Next = current.Next.Next
			l.length--
			return true
		}
		current = current.Next
	}
	return false
}

func (l *SinglyLinkedList[T]) Find(value T) int {
	current := l.head
	for i := 0; current != nil; i++ {
		if current.Value == value {
			return i
		}
		current = current.Next
	}
	return -1
}

func (l *SinglyLinkedList[T]) Get(index int) (T, bool) {
	if index < 0 || index >= l.length {
		var zero T
		return zero, false
	}
	current := l.head
	for i := 0; i < index; i++ {
		current = current.Next
	}
	return current.Value, true
}

func (l *SinglyLinkedList[T]) Reverse() {
	var prev *SLLNode[T]
	current := l.head
	l.tail = l.head

	for current != nil {
		next := current.Next
		current.Next = prev
		prev = current
		current = next
	}
	l.head = prev
}

func (l *SinglyLinkedList[T]) GetMiddle() (T, bool) {
	if l.head == nil {
		var zero T
		return zero, false
	}
	slow := l.head
	fast := l.head
	for fast.Next != nil && fast.Next.Next != nil {
		slow = slow.Next
		fast = fast.Next.Next
	}
	return slow.Value, true
}

func (l *SinglyLinkedList[T]) HasCycle() bool {
	if l.head == nil {
		return false
	}
	slow := l.head
	fast := l.head
	for fast != nil && fast.Next != nil {
		slow = slow.Next
		fast = fast.Next.Next
		if slow == fast {
			return true
		}
	}
	return false
}

func (l *SinglyLinkedList[T]) GetNthFromEnd(n int) (T, bool) {
	if n <= 0 || n > l.length {
		var zero T
		return zero, false
	}
	slow := l.head
	fast := l.head
	for i := 0; i < n; i++ {
		if fast == nil {
			var zero T
			return zero, false
		}
		fast = fast.Next
	}
	for fast != nil {
		slow = slow.Next
		fast = fast.Next
	}
	return slow.Value, true
}

func (l *SinglyLinkedList[T]) RemoveDuplicates() {
	if l.head == nil {
		return
	}
	seen := make(map[T]bool)
	seen[l.head.Value] = true
	current := l.head

	for current.Next != nil {
		if seen[current.Next.Value] {
			if current.Next == l.tail {
				l.tail = current
			}
			current.Next = current.Next.Next
			l.length--
		} else {
			seen[current.Next.Value] = true
			current = current.Next
		}
	}
}

func (l *SinglyLinkedList[T]) ToSlice() []T {
	result := make([]T, 0, l.length)
	current := l.head
	for current != nil {
		result = append(result, current.Value)
		current = current.Next
	}
	return result
}

// IsPalindrome checks if the linked list values form a palindrome.
func (l *SinglyLinkedList[T]) IsPalindrome() bool {
	if l.length <= 1 {
		return true
	}
	values := l.ToSlice()
	for i, j := 0, len(values)-1; i < j; i, j = i+1, j-1 {
		if values[i] != values[j] {
			return false
		}
	}
	return true
}

// ReverseBetween reverses nodes between positions left and right (0-indexed).
func (l *SinglyLinkedList[T]) ReverseBetween(left, right int) {
	if left >= right || left < 0 || right >= l.length {
		return
	}

	// Build slice, reverse segment, rebuild list
	values := l.ToSlice()
	for i, j := left, right; i < j; i, j = i+1, j-1 {
		values[i], values[j] = values[j], values[i]
	}

	// Rebuild
	l.head = nil
	l.tail = nil
	l.length = 0
	for _, v := range values {
		l.Append(v)
	}
}

// GetCycleStart returns the node where a cycle begins, or nil.
func (l *SinglyLinkedList[T]) GetCycleStart() *SLLNode[T] {
	if l.head == nil {
		return nil
	}
	slow := l.head
	fast := l.head
	for fast != nil && fast.Next != nil {
		slow = slow.Next
		fast = fast.Next.Next
		if slow == fast {
			// Cycle found, find start
			slow = l.head
			for slow != fast {
				slow = slow.Next
				fast = fast.Next
			}
			return slow
		}
	}
	return nil
}

// SortList sorts the linked list using merge sort.
func SortSinglyLinkedList(head *SLLNode[int]) *SLLNode[int] {
	if head == nil || head.Next == nil {
		return head
	}

	// Find middle
	slow := head
	fast := head.Next
	for fast != nil && fast.Next != nil {
		slow = slow.Next
		fast = fast.Next.Next
	}

	mid := slow.Next
	slow.Next = nil

	left := SortSinglyLinkedList(head)
	right := SortSinglyLinkedList(mid)
	return mergeSortedSLL(left, right)
}

func mergeSortedSLL(a, b *SLLNode[int]) *SLLNode[int] {
	dummy := &SLLNode[int]{}
	current := dummy
	for a != nil && b != nil {
		if a.Value <= b.Value {
			current.Next = a
			a = a.Next
		} else {
			current.Next = b
			b = b.Next
		}
		current = current.Next
	}
	if a != nil {
		current.Next = a
	} else {
		current.Next = b
	}
	return dummy.Next
}

// MergeSortedIntLists merges two sorted int singly linked lists.
func MergeSortedIntLists(l1, l2 *SinglyLinkedList[int]) *SinglyLinkedList[int] {
	result := NewSinglyLinkedList[int]()
	a := l1.Head()
	b := l2.Head()
	for a != nil && b != nil {
		if a.Value <= b.Value {
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

// RemoveAllOccurrences removes all nodes with the given value.
func (l *SinglyLinkedList[T]) RemoveAllOccurrences(value T) int {
	count := 0
	for l.head != nil && l.head.Value == value {
		l.PopFirst()
		count++
	}
	if l.head == nil {
		return count
	}
	current := l.head
	for current.Next != nil {
		if current.Next.Value == value {
			if current.Next == l.tail {
				l.tail = current
			}
			current.Next = current.Next.Next
			l.length--
			count++
		} else {
			current = current.Next
		}
	}
	return count
}

// Rotate rotates the list by k positions to the right.
func (l *SinglyLinkedList[T]) Rotate(k int) {
	if l.length <= 1 || k <= 0 {
		return
	}
	k = k % l.length
	if k == 0 {
		return
	}
	// Find the (length - k)th node
	steps := l.length - k
	current := l.head
	for i := 1; i < steps; i++ {
		current = current.Next
	}
	newHead := current.Next
	current.Next = nil
	l.tail.Next = l.head
	l.tail = current
	l.head = newHead
}

// PartitionInts rearranges nodes so values < pivot come before values >= pivot.
// Only works with int linked lists.
func PartitionIntsSLL(l *SinglyLinkedList[int], pivot int) {
	if l.Len() <= 1 {
		return
	}
	var beforeHead, beforeTail, afterHead, afterTail *SLLNode[int]

	current := l.Head()
	for current != nil {
		next := current.Next
		current.Next = nil
		if current.Value < pivot {
			if beforeHead == nil {
				beforeHead = current
				beforeTail = current
			} else {
				beforeTail.Next = current
				beforeTail = current
			}
		} else {
			if afterHead == nil {
				afterHead = current
				afterTail = current
			} else {
				afterTail.Next = current
				afterTail = current
			}
		}
		current = next
	}

	// Rebuild the list from partitioned segments
	values := make([]int, 0, l.Len())
	for n := beforeHead; n != nil; n = n.Next {
		values = append(values, n.Value)
	}
	for n := afterHead; n != nil; n = n.Next {
		values = append(values, n.Value)
	}

	// Clear and rebuild
	l.head = nil
	l.tail = nil
	l.length = 0
	for _, v := range values {
		l.Append(v)
	}
}

func SLLFromSlice[T comparable](items []T) *SinglyLinkedList[T] {
	l := NewSinglyLinkedList[T]()
	for _, item := range items {
		l.Append(item)
	}
	return l
}
