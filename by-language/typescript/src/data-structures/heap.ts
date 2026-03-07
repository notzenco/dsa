/**
 * Binary Heap (Min-Heap and Max-Heap)
 *
 * COMPLEXITY:
 * | Operation           | Time        | Space       |
 * |---------------------|-------------|-------------|
 * | push                | O(log n)    | O(1)        |
 * | pop                 | O(log n)    | O(1)        |
 * | peek                | O(1)        | O(1)        |
 * | heapify (build)     | O(n)        | O(1)        |
 * | size                | O(1)        | O(1)        |
 */

export class MinHeap<T> {
  private _data: T[] = [];

  get size(): number {
    return this._data.length;
  }

  get isEmpty(): boolean {
    return this._data.length === 0;
  }

  private _parent(i: number): number {
    return (i - 1) >> 1;
  }

  private _leftChild(i: number): number {
    return 2 * i + 1;
  }

  private _rightChild(i: number): number {
    return 2 * i + 2;
  }

  private _swap(i: number, j: number): void {
    [this._data[i], this._data[j]] = [this._data[j], this._data[i]];
  }

  private _bubbleUp(index: number): void {
    while (index > 0) {
      const parent = this._parent(index);
      if (this._data[index] < this._data[parent]) {
        this._swap(index, parent);
        index = parent;
      } else {
        break;
      }
    }
  }

  private _bubbleDown(index: number): void {
    const size = this._data.length;
    while (this._leftChild(index) < size) {
      let smallest = index;
      const left = this._leftChild(index);
      const right = this._rightChild(index);

      if (this._data[left] < this._data[smallest]) {
        smallest = left;
      }

      if (right < size && this._data[right] < this._data[smallest]) {
        smallest = right;
      }

      if (smallest !== index) {
        this._swap(index, smallest);
        index = smallest;
      } else {
        break;
      }
    }
  }

  push(value: T): void {
    this._data.push(value);
    this._bubbleUp(this._data.length - 1);
  }

  pop(): T {
    if (this.isEmpty) {
      throw new Error("Pop from empty heap");
    }

    const minVal = this._data[0];
    const last = this._data.pop()!;

    if (this._data.length > 0) {
      this._data[0] = last;
      this._bubbleDown(0);
    }

    return minVal;
  }

  peek(): T {
    if (this.isEmpty) {
      throw new Error("Peek from empty heap");
    }
    return this._data[0];
  }

  toArray(): T[] {
    return [...this._data];
  }

  toSortedArray(): T[] {
    const temp = new MinHeap<T>();
    temp._data = [...this._data];
    const result: T[] = [];
    while (!temp.isEmpty) {
      result.push(temp.pop());
    }
    return result;
  }

  clear(): void {
    this._data = [];
  }

  static heapify<T>(items: T[]): MinHeap<T> {
    const heap = new MinHeap<T>();
    heap._data = [...items];
    for (let i = (heap._data.length >> 1) - 1; i >= 0; i--) {
      heap._bubbleDown(i);
    }
    return heap;
  }
}

export class MaxHeap<T> {
  private _data: T[] = [];

  get size(): number {
    return this._data.length;
  }

  get isEmpty(): boolean {
    return this._data.length === 0;
  }

  private _parent(i: number): number {
    return (i - 1) >> 1;
  }

  private _leftChild(i: number): number {
    return 2 * i + 1;
  }

  private _rightChild(i: number): number {
    return 2 * i + 2;
  }

  private _swap(i: number, j: number): void {
    [this._data[i], this._data[j]] = [this._data[j], this._data[i]];
  }

  private _bubbleUp(index: number): void {
    while (index > 0) {
      const parent = this._parent(index);
      if (this._data[index] > this._data[parent]) {
        this._swap(index, parent);
        index = parent;
      } else {
        break;
      }
    }
  }

  private _bubbleDown(index: number): void {
    const size = this._data.length;
    while (this._leftChild(index) < size) {
      let largest = index;
      const left = this._leftChild(index);
      const right = this._rightChild(index);

      if (this._data[left] > this._data[largest]) {
        largest = left;
      }

      if (right < size && this._data[right] > this._data[largest]) {
        largest = right;
      }

      if (largest !== index) {
        this._swap(index, largest);
        index = largest;
      } else {
        break;
      }
    }
  }

  push(value: T): void {
    this._data.push(value);
    this._bubbleUp(this._data.length - 1);
  }

  pop(): T {
    if (this.isEmpty) {
      throw new Error("Pop from empty heap");
    }

    const maxVal = this._data[0];
    const last = this._data.pop()!;

    if (this._data.length > 0) {
      this._data[0] = last;
      this._bubbleDown(0);
    }

    return maxVal;
  }

  peek(): T {
    if (this.isEmpty) {
      throw new Error("Peek from empty heap");
    }
    return this._data[0];
  }

  toArray(): T[] {
    return [...this._data];
  }

  toSortedArray(): T[] {
    const temp = new MaxHeap<T>();
    temp._data = [...this._data];
    const result: T[] = [];
    while (!temp.isEmpty) {
      result.push(temp.pop());
    }
    return result;
  }

  clear(): void {
    this._data = [];
  }

  static heapify<T>(items: T[]): MaxHeap<T> {
    const heap = new MaxHeap<T>();
    heap._data = [...items];
    for (let i = (heap._data.length >> 1) - 1; i >= 0; i--) {
      heap._bubbleDown(i);
    }
    return heap;
  }
}

export class PriorityQueue<T> {
  private _heap: [number, number, T][] = [];
  private _counter = 0;

  get size(): number {
    return this._heap.length;
  }

  get isEmpty(): boolean {
    return this._heap.length === 0;
  }

  private _swap(i: number, j: number): void {
    [this._heap[i], this._heap[j]] = [this._heap[j], this._heap[i]];
  }

  private _less(i: number, j: number): boolean {
    const a = this._heap[i];
    const b = this._heap[j];
    if (a[0] !== b[0]) return a[0] < b[0];
    return a[1] < b[1];
  }

  private _bubbleUp(index: number): void {
    while (index > 0) {
      const parent = (index - 1) >> 1;
      if (this._less(index, parent)) {
        this._swap(index, parent);
        index = parent;
      } else {
        break;
      }
    }
  }

  private _bubbleDown(index: number): void {
    const size = this._heap.length;
    while (2 * index + 1 < size) {
      let smallest = index;
      const left = 2 * index + 1;
      const right = 2 * index + 2;

      if (this._less(left, smallest)) {
        smallest = left;
      }

      if (right < size && this._less(right, smallest)) {
        smallest = right;
      }

      if (smallest !== index) {
        this._swap(index, smallest);
        index = smallest;
      } else {
        break;
      }
    }
  }

  enqueue(item: T, priority: number): void {
    this._heap.push([priority, this._counter++, item]);
    this._bubbleUp(this._heap.length - 1);
  }

  dequeue(): T {
    if (this.isEmpty) {
      throw new Error("Dequeue from empty queue");
    }

    const item = this._heap[0][2];
    const last = this._heap.pop()!;

    if (this._heap.length > 0) {
      this._heap[0] = last;
      this._bubbleDown(0);
    }

    return item;
  }

  peek(): T {
    if (this.isEmpty) {
      throw new Error("Peek from empty queue");
    }
    return this._heap[0][2];
  }

  clear(): void {
    this._heap = [];
    this._counter = 0;
  }
}

export function heapSort<T>(arr: T[]): T[] {
  const heap = MinHeap.heapify(arr);
  return heap.toSortedArray();
}

export function nLargest<T>(arr: T[], n: number): T[] {
  if (n <= 0) return [];
  if (n >= arr.length) return [...arr].sort((a, b) => (a < b ? 1 : a > b ? -1 : 0));

  const heap = new MinHeap<T>();

  for (const item of arr) {
    if (heap.size < n) {
      heap.push(item);
    } else if (item > heap.peek()) {
      heap.pop();
      heap.push(item);
    }
  }

  return heap.toSortedArray().reverse();
}

export function nSmallest<T>(arr: T[], n: number): T[] {
  if (n <= 0) return [];
  if (n >= arr.length) return [...arr].sort((a, b) => (a < b ? -1 : a > b ? 1 : 0));

  const heap = new MaxHeap<T>();

  for (const item of arr) {
    if (heap.size < n) {
      heap.push(item);
    } else if (item < heap.peek()) {
      heap.pop();
      heap.push(item);
    }
  }

  return heap.toSortedArray().reverse();
}
