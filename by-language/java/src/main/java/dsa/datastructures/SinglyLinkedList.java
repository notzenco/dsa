package dsa.datastructures;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.Set;

public class SinglyLinkedList<T> {

    private static class Node<T> {
        T data;
        Node<T> next;

        Node(T data) {
            this.data = data;
            this.next = null;
        }
    }

    private Node<T> head;
    private Node<T> tail;
    private int size;

    public SinglyLinkedList() {
        this.head = null;
        this.tail = null;
        this.size = 0;
    }

    public void prepend(T value) {
        Node<T> newNode = new Node<>(value);
        if (head == null) {
            head = newNode;
            tail = newNode;
        } else {
            newNode.next = head;
            head = newNode;
        }
        size++;
    }

    public void append(T value) {
        Node<T> newNode = new Node<>(value);
        if (tail == null) {
            head = newNode;
            tail = newNode;
        } else {
            tail.next = newNode;
            tail = newNode;
        }
        size++;
    }

    public void insert(int index, T value) {
        if (index < 0 || index > size) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        }
        if (index == 0) {
            prepend(value);
            return;
        }
        if (index == size) {
            append(value);
            return;
        }
        Node<T> prev = getNode(index - 1);
        Node<T> newNode = new Node<>(value);
        newNode.next = prev.next;
        prev.next = newNode;
        size++;
    }

    public T popFirst() {
        if (head == null) {
            throw new NoSuchElementException("List is empty");
        }
        T value = head.data;
        head = head.next;
        if (head == null) {
            tail = null;
        }
        size--;
        return value;
    }

    public T popLast() {
        if (head == null) {
            throw new NoSuchElementException("List is empty");
        }
        if (head == tail) {
            T value = head.data;
            head = null;
            tail = null;
            size--;
            return value;
        }
        Node<T> current = head;
        while (current.next != tail) {
            current = current.next;
        }
        T value = tail.data;
        tail = current;
        tail.next = null;
        size--;
        return value;
    }

    public T pop(int index) {
        if (index < 0 || index >= size) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        }
        if (index == 0) {
            return popFirst();
        }
        if (index == size - 1) {
            return popLast();
        }
        Node<T> prev = getNode(index - 1);
        T value = prev.next.data;
        prev.next = prev.next.next;
        size--;
        return value;
    }

    public boolean remove(T value) {
        if (head == null) {
            return false;
        }
        if (head.data != null ? head.data.equals(value) : value == null) {
            popFirst();
            return true;
        }
        Node<T> current = head;
        while (current.next != null) {
            if (current.next.data != null ? current.next.data.equals(value) : value == null) {
                if (current.next == tail) {
                    tail = current;
                }
                current.next = current.next.next;
                size--;
                return true;
            }
            current = current.next;
        }
        return false;
    }

    public int find(T value) {
        Node<T> current = head;
        int index = 0;
        while (current != null) {
            if (current.data != null ? current.data.equals(value) : value == null) {
                return index;
            }
            current = current.next;
            index++;
        }
        return -1;
    }

    public T get(int index) {
        if (index < 0 || index >= size) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        }
        return getNode(index).data;
    }

    public void clear() {
        head = null;
        tail = null;
        size = 0;
    }

    public void reverse() {
        if (size <= 1) {
            return;
        }
        tail = head;
        Node<T> prev = null;
        Node<T> current = head;
        while (current != null) {
            Node<T> next = current.next;
            current.next = prev;
            prev = current;
            current = next;
        }
        head = prev;
    }

    public List<T> toList() {
        List<T> result = new ArrayList<>();
        Node<T> current = head;
        while (current != null) {
            result.add(current.data);
            current = current.next;
        }
        return result;
    }

    public static <T> SinglyLinkedList<T> fromList(List<T> list) {
        SinglyLinkedList<T> sll = new SinglyLinkedList<>();
        for (T item : list) {
            sll.append(item);
        }
        return sll;
    }

    public int size() {
        return size;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public boolean contains(T value) {
        return find(value) != -1;
    }

    public void set(int index, T value) {
        if (index < 0 || index >= size) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        }
        getNode(index).data = value;
    }

    public Object[] toArray() {
        Object[] result = new Object[size];
        Node<T> current = head;
        int i = 0;
        while (current != null) {
            result[i++] = current.data;
            current = current.next;
        }
        return result;
    }

    public boolean isPalindrome() {
        if (size <= 1) return true;

        // Find middle using slow/fast pointers
        Node<T> slow = head;
        Node<T> fast = head;
        while (fast.next != null && fast.next.next != null) {
            slow = slow.next;
            fast = fast.next.next;
        }

        // Reverse second half
        Node<T> prev = null;
        Node<T> current = slow.next;
        while (current != null) {
            Node<T> next = current.next;
            current.next = prev;
            prev = current;
            current = next;
        }

        // Compare first half with reversed second half
        Node<T> left = head;
        Node<T> right = prev;
        boolean result = true;
        while (right != null) {
            if (left.data != null ? !left.data.equals(right.data) : right.data != null) {
                result = false;
                break;
            }
            left = left.next;
            right = right.next;
        }

        // Restore second half
        current = prev;
        prev = null;
        while (current != null) {
            Node<T> next = current.next;
            current.next = prev;
            prev = current;
            current = next;
        }
        slow.next = prev;

        return result;
    }

    public static <T extends Comparable<T>> SinglyLinkedList<T> mergeSorted(
            SinglyLinkedList<T> list1, SinglyLinkedList<T> list2) {
        SinglyLinkedList<T> result = new SinglyLinkedList<>();
        Node<T> a = list1.head;
        Node<T> b = list2.head;
        while (a != null && b != null) {
            if (a.data.compareTo(b.data) <= 0) {
                result.append(a.data);
                a = a.next;
            } else {
                result.append(b.data);
                b = b.next;
            }
        }
        while (a != null) {
            result.append(a.data);
            a = a.next;
        }
        while (b != null) {
            result.append(b.data);
            b = b.next;
        }
        return result;
    }

    public static <T> SinglyLinkedList<T> fromArray(T[] array) {
        SinglyLinkedList<T> list = new SinglyLinkedList<>();
        for (T item : array) {
            list.append(item);
        }
        return list;
    }

    public <R> SinglyLinkedList<R> map(java.util.function.Function<T, R> mapper) {
        SinglyLinkedList<R> result = new SinglyLinkedList<>();
        Node<T> current = head;
        while (current != null) {
            result.append(mapper.apply(current.data));
            current = current.next;
        }
        return result;
    }

    public SinglyLinkedList<T> filter(java.util.function.Predicate<T> predicate) {
        SinglyLinkedList<T> result = new SinglyLinkedList<>();
        Node<T> current = head;
        while (current != null) {
            if (predicate.test(current.data)) {
                result.append(current.data);
            }
            current = current.next;
        }
        return result;
    }

    public <R> R reduce(R identity, java.util.function.BiFunction<R, T, R> accumulator) {
        R result = identity;
        Node<T> current = head;
        while (current != null) {
            result = accumulator.apply(result, current.data);
            current = current.next;
        }
        return result;
    }

    // ---- Special methods ----

    public T getMiddle() {
        if (head == null) {
            throw new NoSuchElementException("List is empty");
        }
        Node<T> slow = head;
        Node<T> fast = head;
        while (fast.next != null && fast.next.next != null) {
            slow = slow.next;
            fast = fast.next.next;
        }
        return slow.data;
    }

    public boolean hasCycle() {
        if (head == null) {
            return false;
        }
        Node<T> slow = head;
        Node<T> fast = head;
        while (fast != null && fast.next != null) {
            slow = slow.next;
            fast = fast.next.next;
            if (slow == fast) {
                return true;
            }
        }
        return false;
    }

    public T getNthFromEnd(int n) {
        if (n <= 0 || n > size) {
            throw new IllegalArgumentException("n must be between 1 and " + size);
        }
        Node<T> fast = head;
        Node<T> slow = head;
        for (int i = 0; i < n; i++) {
            fast = fast.next;
        }
        while (fast != null) {
            slow = slow.next;
            fast = fast.next;
        }
        return slow.data;
    }

    public void removeDuplicates() {
        if (head == null) {
            return;
        }
        Set<T> seen = new HashSet<>();
        Node<T> current = head;
        seen.add(current.data);
        while (current.next != null) {
            if (seen.contains(current.next.data)) {
                if (current.next == tail) {
                    tail = current;
                }
                current.next = current.next.next;
                size--;
            } else {
                seen.add(current.next.data);
                current = current.next;
            }
        }
    }

    // ---- Private helpers ----

    private Node<T> getNode(int index) {
        Node<T> current = head;
        for (int i = 0; i < index; i++) {
            current = current.next;
        }
        return current;
    }
}
