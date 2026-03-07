package datastructures

import "testing"

func TestQueue(t *testing.T) {
	q := NewQueue[int]()

	if !q.IsEmpty() {
		t.Error("new queue should be empty")
	}

	q.Enqueue(1)
	q.Enqueue(2)
	q.Enqueue(3)

	if q.Size() != 3 {
		t.Errorf("expected size 3, got %d", q.Size())
	}

	val, ok := q.Peek()
	if !ok || val != 1 {
		t.Errorf("expected peek 1, got %d", val)
	}

	val, ok = q.Rear()
	if !ok || val != 3 {
		t.Errorf("expected rear 3, got %d", val)
	}

	val, ok = q.Dequeue()
	if !ok || val != 1 {
		t.Errorf("expected dequeue 1, got %d", val)
	}

	val, ok = q.Dequeue()
	if !ok || val != 2 {
		t.Errorf("expected dequeue 2, got %d", val)
	}

	val, ok = q.Dequeue()
	if !ok || val != 3 {
		t.Errorf("expected dequeue 3, got %d", val)
	}

	_, ok = q.Dequeue()
	if ok {
		t.Error("dequeue from empty queue should return false")
	}
}

func TestQueueClear(t *testing.T) {
	q := NewQueue[string]()
	q.Enqueue("a")
	q.Enqueue("b")
	q.Clear()
	if !q.IsEmpty() {
		t.Error("cleared queue should be empty")
	}
}

func TestCircularQueue(t *testing.T) {
	q := NewCircularQueue[int](3)

	if !q.IsEmpty() {
		t.Error("new circular queue should be empty")
	}
	if q.Capacity() != 3 {
		t.Errorf("expected capacity 3, got %d", q.Capacity())
	}

	if !q.Enqueue(1) {
		t.Error("enqueue should succeed")
	}
	if !q.Enqueue(2) {
		t.Error("enqueue should succeed")
	}
	if !q.Enqueue(3) {
		t.Error("enqueue should succeed")
	}
	if q.Enqueue(4) {
		t.Error("enqueue to full queue should fail")
	}
	if !q.IsFull() {
		t.Error("queue should be full")
	}

	val, ok := q.Dequeue()
	if !ok || val != 1 {
		t.Errorf("expected 1, got %d", val)
	}

	// Should be able to add one more now
	if !q.Enqueue(4) {
		t.Error("enqueue should succeed after dequeue")
	}

	val, _ = q.Peek()
	if val != 2 {
		t.Errorf("expected peek 2, got %d", val)
	}

	val, _ = q.Rear()
	if val != 4 {
		t.Errorf("expected rear 4, got %d", val)
	}
}

func TestDeque(t *testing.T) {
	d := NewDeque[int]()

	d.AddRear(1)
	d.AddRear(2)
	d.AddFront(0)

	val, ok := d.PeekFront()
	if !ok || val != 0 {
		t.Errorf("expected front 0, got %d", val)
	}

	val, ok = d.PeekRear()
	if !ok || val != 2 {
		t.Errorf("expected rear 2, got %d", val)
	}

	val, _ = d.RemoveFront()
	if val != 0 {
		t.Errorf("expected 0, got %d", val)
	}

	val, _ = d.RemoveRear()
	if val != 2 {
		t.Errorf("expected 2, got %d", val)
	}

	if d.Size() != 1 {
		t.Errorf("expected size 1, got %d", d.Size())
	}
}

func TestMonotonicQueue(t *testing.T) {
	mq := NewMonotonicQueue()

	mq.Push(3, 0)
	mq.Push(1, 1)
	mq.Push(2, 2)

	max, ok := mq.GetMax()
	if !ok || max != 3 {
		t.Errorf("expected max 3, got %d", max)
	}

	mq.PopByIndex(0) // remove index 0
	max, _ = mq.GetMax()
	if max != 2 {
		t.Errorf("expected max 2, got %d", max)
	}
}

func TestSlidingWindowMaximum(t *testing.T) {
	result := SlidingWindowMaximum([]int{1, 3, -1, -3, 5, 3, 6, 7}, 3)
	expected := []int{3, 3, 5, 5, 6, 7}

	if len(result) != len(expected) {
		t.Fatalf("expected len %d, got %d", len(expected), len(result))
	}

	for i, v := range expected {
		if result[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, result[i])
		}
	}

	// Edge cases
	result = SlidingWindowMaximum([]int{}, 3)
	if result != nil {
		t.Error("empty array should return nil")
	}

	result = SlidingWindowMaximum([]int{1}, 1)
	if len(result) != 1 || result[0] != 1 {
		t.Errorf("single element: expected [1], got %v", result)
	}
}
