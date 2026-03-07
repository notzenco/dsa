package dsa.datastructures;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.NoSuchElementException;

public class Stack<T> {
    private final ArrayList<T> data;

    public Stack() {
        this.data = new ArrayList<>();
    }

    public void push(T value) {
        data.add(value);
    }

    public T pop() {
        if (isEmpty()) {
            throw new NoSuchElementException("Stack is empty");
        }
        return data.remove(data.size() - 1);
    }

    public T peek() {
        if (isEmpty()) {
            throw new NoSuchElementException("Stack is empty");
        }
        return data.get(data.size() - 1);
    }

    public boolean isEmpty() {
        return data.isEmpty();
    }

    public int size() {
        return data.size();
    }

    public void clear() {
        data.clear();
    }

    public List<T> toList() {
        return new ArrayList<>(data);
    }

    public boolean contains(T value) {
        return data.contains(value);
    }

    public static <T> Stack<T> fromList(List<T> list) {
        Stack<T> stack = new Stack<>();
        for (T item : list) {
            stack.push(item);
        }
        return stack;
    }

    // ---- MinStack ----
    public static class MinStack<T extends Comparable<T>> {
        private final ArrayList<T> data;
        private final ArrayList<T> minStack;

        public MinStack() {
            this.data = new ArrayList<>();
            this.minStack = new ArrayList<>();
        }

        public void push(T value) {
            data.add(value);
            if (minStack.isEmpty() || value.compareTo(minStack.get(minStack.size() - 1)) <= 0) {
                minStack.add(value);
            }
        }

        public T pop() {
            if (isEmpty()) {
                throw new NoSuchElementException("MinStack is empty");
            }
            T removed = data.remove(data.size() - 1);
            if (removed.compareTo(minStack.get(minStack.size() - 1)) == 0) {
                minStack.remove(minStack.size() - 1);
            }
            return removed;
        }

        public T peek() {
            if (isEmpty()) {
                throw new NoSuchElementException("MinStack is empty");
            }
            return data.get(data.size() - 1);
        }

        public T getMin() {
            if (isEmpty()) {
                throw new NoSuchElementException("MinStack is empty");
            }
            return minStack.get(minStack.size() - 1);
        }

        public boolean isEmpty() {
            return data.isEmpty();
        }

        public int size() {
            return data.size();
        }
    }

    // ---- MaxStack ----
    public static class MaxStack<T extends Comparable<T>> {
        private final ArrayList<T> data;
        private final ArrayList<T> maxStack;

        public MaxStack() {
            this.data = new ArrayList<>();
            this.maxStack = new ArrayList<>();
        }

        public void push(T value) {
            data.add(value);
            if (maxStack.isEmpty() || value.compareTo(maxStack.get(maxStack.size() - 1)) >= 0) {
                maxStack.add(value);
            }
        }

        public T pop() {
            if (isEmpty()) {
                throw new NoSuchElementException("MaxStack is empty");
            }
            T removed = data.remove(data.size() - 1);
            if (removed.compareTo(maxStack.get(maxStack.size() - 1)) == 0) {
                maxStack.remove(maxStack.size() - 1);
            }
            return removed;
        }

        public T peek() {
            if (isEmpty()) {
                throw new NoSuchElementException("MaxStack is empty");
            }
            return data.get(data.size() - 1);
        }

        public T getMax() {
            if (isEmpty()) {
                throw new NoSuchElementException("MaxStack is empty");
            }
            return maxStack.get(maxStack.size() - 1);
        }

        public boolean isEmpty() {
            return data.isEmpty();
        }

        public int size() {
            return data.size();
        }
    }

    // ---- Static utility methods ----

    public static boolean isBalancedParentheses(String s) {
        java.util.Deque<Character> stack = new java.util.ArrayDeque<>();
        for (char c : s.toCharArray()) {
            if (c == '(' || c == '[' || c == '{') {
                stack.push(c);
            } else if (c == ')' || c == ']' || c == '}') {
                if (stack.isEmpty()) {
                    return false;
                }
                char top = stack.pop();
                if ((c == ')' && top != '(') ||
                    (c == ']' && top != '[') ||
                    (c == '}' && top != '{')) {
                    return false;
                }
            }
        }
        return stack.isEmpty();
    }

    public static int evaluatePostfix(String expression) {
        java.util.Deque<Integer> stack = new java.util.ArrayDeque<>();
        String[] tokens = expression.trim().split("\\s+");
        for (String token : tokens) {
            switch (token) {
                case "+": {
                    int b = stack.pop();
                    int a = stack.pop();
                    stack.push(a + b);
                    break;
                }
                case "-": {
                    int b = stack.pop();
                    int a = stack.pop();
                    stack.push(a - b);
                    break;
                }
                case "*": {
                    int b = stack.pop();
                    int a = stack.pop();
                    stack.push(a * b);
                    break;
                }
                case "/": {
                    int b = stack.pop();
                    int a = stack.pop();
                    stack.push(a / b);
                    break;
                }
                default:
                    stack.push(Integer.parseInt(token));
                    break;
            }
        }
        return stack.pop();
    }

    public static int[] nextGreaterElement(int[] arr) {
        int n = arr.length;
        int[] result = new int[n];
        Arrays.fill(result, -1);
        java.util.Deque<Integer> stack = new java.util.ArrayDeque<>();
        for (int i = 0; i < n; i++) {
            while (!stack.isEmpty() && arr[stack.peek()] < arr[i]) {
                result[stack.pop()] = arr[i];
            }
            stack.push(i);
        }
        return result;
    }
}
