package dsa.datastructures;

import java.util.*;

public class LfuCache<K, V> {

    private static class Node<K, V> {
        K key;
        V value;
        int frequency;
        Node<K, V> prev;
        Node<K, V> next;

        Node(K key, V value) {
            this.key = key;
            this.value = value;
            this.frequency = 1;
        }
    }

    private static class DoublyLinkedList<K, V> {
        Node<K, V> head;
        Node<K, V> tail;
        int size;

        DoublyLinkedList() {
            head = new Node<>(null, null);
            tail = new Node<>(null, null);
            head.next = tail;
            tail.prev = head;
            size = 0;
        }

        void addToFront(Node<K, V> node) {
            node.next = head.next;
            node.prev = head;
            head.next.prev = node;
            head.next = node;
            size++;
        }

        void removeNode(Node<K, V> node) {
            node.prev.next = node.next;
            node.next.prev = node.prev;
            size--;
        }

        Node<K, V> removeLast() {
            if (size == 0) return null;
            Node<K, V> last = tail.prev;
            removeNode(last);
            return last;
        }

        boolean isEmpty() {
            return size == 0;
        }
    }

    private final int capacity;
    private final Map<K, Node<K, V>> keyToNode;
    private final Map<Integer, DoublyLinkedList<K, V>> frequencyMap;
    private int minFrequency;

    public LfuCache(int capacity) {
        if (capacity <= 0) throw new IllegalArgumentException("Capacity must be positive");
        this.capacity = capacity;
        this.keyToNode = new HashMap<>();
        this.frequencyMap = new HashMap<>();
        this.minFrequency = 0;
    }

    public Optional<V> get(K key) {
        Node<K, V> node = keyToNode.get(key);
        if (node == null) return Optional.empty();
        updateFrequency(node);
        return Optional.of(node.value);
    }

    public void put(K key, V value) {
        Node<K, V> existing = keyToNode.get(key);
        if (existing != null) {
            existing.value = value;
            updateFrequency(existing);
            return;
        }

        if (keyToNode.size() >= capacity) {
            evictLFU();
        }

        Node<K, V> newNode = new Node<>(key, value);
        newNode.frequency = 1;
        keyToNode.put(key, newNode);

        frequencyMap.computeIfAbsent(1, k -> new DoublyLinkedList<>()).addToFront(newNode);
        minFrequency = 1;
    }

    public int size() {
        return keyToNode.size();
    }

    public int capacity() {
        return capacity;
    }

    public boolean containsKey(K key) {
        return keyToNode.containsKey(key);
    }

    public void clear() {
        keyToNode.clear();
        frequencyMap.clear();
        minFrequency = 0;
    }

    private void updateFrequency(Node<K, V> node) {
        int oldFreq = node.frequency;
        DoublyLinkedList<K, V> oldList = frequencyMap.get(oldFreq);
        oldList.removeNode(node);

        if (oldList.isEmpty()) {
            frequencyMap.remove(oldFreq);
            if (minFrequency == oldFreq) {
                minFrequency++;
            }
        }

        node.frequency++;
        frequencyMap.computeIfAbsent(node.frequency, k -> new DoublyLinkedList<>()).addToFront(node);
    }

    private void evictLFU() {
        DoublyLinkedList<K, V> minFreqList = frequencyMap.get(minFrequency);
        if (minFreqList == null) return;

        Node<K, V> evicted = minFreqList.removeLast();
        if (evicted == null) return;

        keyToNode.remove(evicted.key);

        if (minFreqList.isEmpty()) {
            frequencyMap.remove(minFrequency);
        }
    }
}
