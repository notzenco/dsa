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
