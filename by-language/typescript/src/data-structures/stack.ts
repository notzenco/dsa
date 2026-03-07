/**
 * Stack (LIFO - Last In, First Out)
 *
 * COMPLEXITY:
 * | Operation           | Time        | Space       |
 * |---------------------|-------------|-------------|
 * | push                | O(1)*       | O(1)        |
 * | pop                 | O(1)        | O(1)        |
 * | peek/top            | O(1)        | O(1)        |
 * | isEmpty             | O(1)        | O(1)        |
 * | size                | O(1)        | O(1)        |
 * | getMin (MinStack)   | O(1)        | O(n)        |
 * | getMax (MaxStack)   | O(1)        | O(n)        |
 *
 * *amortized
 */

export class Stack<T> {
  private _data: T[] = [];

  get size(): number {
    return this._data.length;
  }

  get isEmpty(): boolean {
    return this._data.length === 0;
  }

  push(value: T): void {
    this._data.push(value);
  }

  pop(): T {
    if (this.isEmpty) {
      throw new Error("Pop from empty stack");
    }
    return this._data.pop()!;
  }

  peek(): T {
    if (this.isEmpty) {
      throw new Error("Peek from empty stack");
    }
    return this._data[this._data.length - 1];
  }

  clear(): void {
    this._data = [];
  }

  toArray(): T[] {
    return [...this._data];
  }

  toString(): string {
    return `Stack([${this._data.join(", ")}])`;
  }

  [Symbol.iterator](): Iterator<T> {
    let index = 0;
    const data = this._data;
    return {
      next(): IteratorResult<T> {
        if (index < data.length) {
          return { value: data[index++], done: false };
        }
        return { value: undefined as unknown as T, done: true };
      },
    };
  }

  static fromArray<T>(items: T[]): Stack<T> {
    const stack = new Stack<T>();
    for (const item of items) {
      stack.push(item);
    }
    return stack;
  }
}

export class MinStack<T> {
  private _data: T[] = [];
  private _minStack: T[] = [];

  get size(): number {
    return this._data.length;
  }

  get isEmpty(): boolean {
    return this._data.length === 0;
  }

  push(value: T): void {
    this._data.push(value);
    if (
      this._minStack.length === 0 ||
      value <= this._minStack[this._minStack.length - 1]
    ) {
      this._minStack.push(value);
    }
  }

  pop(): T {
    if (this.isEmpty) {
      throw new Error("Pop from empty stack");
    }
    const value = this._data.pop()!;
    if (value === this._minStack[this._minStack.length - 1]) {
      this._minStack.pop();
    }
    return value;
  }

  peek(): T {
    if (this.isEmpty) {
      throw new Error("Peek from empty stack");
    }
    return this._data[this._data.length - 1];
  }

  getMin(): T {
    if (this.isEmpty) {
      throw new Error("getMin from empty stack");
    }
    return this._minStack[this._minStack.length - 1];
  }

  clear(): void {
    this._data = [];
    this._minStack = [];
  }

  toArray(): T[] {
    return [...this._data];
  }

  toString(): string {
    return `MinStack([${this._data.join(", ")}])`;
  }
}

export class MaxStack<T> {
  private _data: T[] = [];
  private _maxStack: T[] = [];

  get size(): number {
    return this._data.length;
  }

  get isEmpty(): boolean {
    return this._data.length === 0;
  }

  push(value: T): void {
    this._data.push(value);
    if (
      this._maxStack.length === 0 ||
      value >= this._maxStack[this._maxStack.length - 1]
    ) {
      this._maxStack.push(value);
    }
  }

  pop(): T {
    if (this.isEmpty) {
      throw new Error("Pop from empty stack");
    }
    const value = this._data.pop()!;
    if (value === this._maxStack[this._maxStack.length - 1]) {
      this._maxStack.pop();
    }
    return value;
  }

  peek(): T {
    if (this.isEmpty) {
      throw new Error("Peek from empty stack");
    }
    return this._data[this._data.length - 1];
  }

  getMax(): T {
    if (this.isEmpty) {
      throw new Error("getMax from empty stack");
    }
    return this._maxStack[this._maxStack.length - 1];
  }

  clear(): void {
    this._data = [];
    this._maxStack = [];
  }

  toArray(): T[] {
    return [...this._data];
  }

  toString(): string {
    return `MaxStack([${this._data.join(", ")}])`;
  }
}

// Utility functions

export function isBalancedParentheses(s: string): boolean {
  const stack = new Stack<string>();
  const pairs: Record<string, string> = { ")": "(", "]": "[", "}": "{" };
  const openers = new Set(["(", "[", "{"]);
  const closers = new Set([")", "]", "}"]);

  for (const char of s) {
    if (openers.has(char)) {
      stack.push(char);
    } else if (closers.has(char)) {
      if (stack.isEmpty || stack.pop() !== pairs[char]) {
        return false;
      }
    }
  }

  return stack.isEmpty;
}

export function evaluatePostfix(expression: string): number {
  const stack = new Stack<number>();
  const operators = new Set(["+", "-", "*", "/"]);
  const tokens = expression.split(/\s+/).filter((t) => t.length > 0);

  for (const token of tokens) {
    if (operators.has(token)) {
      if (stack.size < 2) {
        throw new Error("Invalid expression");
      }
      const b = stack.pop();
      const a = stack.pop();

      switch (token) {
        case "+":
          stack.push(a + b);
          break;
        case "-":
          stack.push(a - b);
          break;
        case "*":
          stack.push(a * b);
          break;
        case "/":
          if (b === 0) {
            throw new Error("Division by zero");
          }
          stack.push(a / b);
          break;
      }
    } else {
      const num = Number(token);
      if (isNaN(num)) {
        throw new Error(`Invalid token: ${token}`);
      }
      stack.push(num);
    }
  }

  if (stack.size !== 1) {
    throw new Error("Invalid expression");
  }

  return stack.pop();
}

export function nextGreaterElement(arr: number[]): number[] {
  const n = arr.length;
  const result = new Array<number>(n).fill(-1);
  const stack = new Stack<number>(); // stack of indices

  for (let i = 0; i < n; i++) {
    while (!stack.isEmpty && arr[stack.peek()] < arr[i]) {
      const idx = stack.pop();
      result[idx] = arr[i];
    }
    stack.push(i);
  }

  return result;
}
