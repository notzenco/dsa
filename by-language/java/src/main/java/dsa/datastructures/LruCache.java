package dsa.datastructures;

import java.util.*;

public class LruCache<K, V> {

    private static class Node<K, V> {
        K key;
        V value;
        Node<K, V> prev;
        Node<K, V> next;

        Node(K key, V value) {
            this.key = key;
            this.value = value;
        }
    }

    private int capacity;
    private final Map<K, Node<K, V>> map;
    private final Node<K, V> head; // dummy head
    private final Node<K, V> tail; // dummy tail
    private int evictionCount;

    public LruCache(int capacity) {
        if (capacity <= 0) throw new IllegalArgumentException("Capacity must be positive");
        this.capacity = capacity;
        this.map = new HashMap<>();
        this.head = new Node<>(null, null);
        this.tail = new Node<>(null, null);
        this.evictionCount = 0;
        head.next = tail;
        tail.prev = head;
    }

    public Optional<V> get(K key) {
        Node<K, V> node = map.get(key);
        if (node == null) return Optional.empty();
        moveToFront(node);
        return Optional.of(node.value);
    }

    public void put(K key, V value) {
        Node<K, V> existing = map.get(key);
        if (existing != null) {
            existing.value = value;
            moveToFront(existing);
            return;
        }

        if (map.size() >= capacity) {
            evictLRU();
        }

        Node<K, V> newNode = new Node<>(key, value);
        addToFront(newNode);
        map.put(key, newNode);
    }

    public int size() {
        return map.size();
    }

    public int capacity() {
        return capacity;
    }

    public boolean containsKey(K key) {
        return map.containsKey(key);
    }

    public Optional<V> peek(K key) {
        Node<K, V> node = map.get(key);
        if (node == null) return Optional.empty();
        return Optional.of(node.value);
    }

    public Optional<V> remove(K key) {
        Node<K, V> node = map.remove(key);
        if (node == null) return Optional.empty();
        removeNode(node);
        return Optional.of(node.value);
    }

    public List<K> keys() {
        List<K> result = new ArrayList<>();
        Node<K, V> current = head.next;
        while (current != tail) {
            result.add(current.key);
            current = current.next;
        }
        return result;
    }

    public List<V> values() {
        List<V> result = new ArrayList<>();
        Node<K, V> current = head.next;
        while (current != tail) {
            result.add(current.value);
            current = current.next;
        }
        return result;
    }

    public List<Map.Entry<K, V>> entries() {
        List<Map.Entry<K, V>> result = new ArrayList<>();
        Node<K, V> current = head.next;
        while (current != tail) {
            result.add(Map.entry(current.key, current.value));
            current = current.next;
        }
        return result;
    }

    public V getOrDefault(K key, V defaultValue) {
        Node<K, V> node = map.get(key);
        if (node == null) return defaultValue;
        moveToFront(node);
        return node.value;
    }

    public void putAll(Map<K, V> entries) {
        for (Map.Entry<K, V> entry : entries.entrySet()) {
            put(entry.getKey(), entry.getValue());
        }
    }

    public void resize(int newCapacity) {
        if (newCapacity <= 0) {
            throw new IllegalArgumentException("Capacity must be positive");
        }
        this.capacity = newCapacity;
        while (map.size() > capacity) {
            evictLRU();
        }
    }

    public int getEvictionCount() {
        return evictionCount;
    }

    public void clear() {
        map.clear();
        head.next = tail;
        tail.prev = head;
    }

    private void addToFront(Node<K, V> node) {
        node.next = head.next;
        node.prev = head;
        head.next.prev = node;
        head.next = node;
    }

    private void removeNode(Node<K, V> node) {
        node.prev.next = node.next;
        node.next.prev = node.prev;
    }

    private void moveToFront(Node<K, V> node) {
        removeNode(node);
        addToFront(node);
    }

    private void evictLRU() {
        Node<K, V> lru = tail.prev;
        if (lru == head) return;
        removeNode(lru);
        map.remove(lru.key);
        evictionCount++;
    }
}
