package dsa.datastructures;

import java.util.*;
import java.util.function.LongSupplier;

public class TtlCache<K, V> {

    private static class CacheEntry<V> {
        V value;
        long expirationTime;

        CacheEntry(V value, long expirationTime) {
            this.value = value;
            this.expirationTime = expirationTime;
        }

        boolean isExpired(long currentTime) {
            return currentTime >= expirationTime;
        }
    }

    private final Map<K, CacheEntry<V>> map;
    private final int capacity;
    private final long defaultTtlMillis;
    private final LongSupplier clock;

    /**
     * Creates a TTL cache using System.currentTimeMillis() as the clock.
     */
    public TtlCache(int capacity, long defaultTtlMillis) {
        this(capacity, defaultTtlMillis, System::currentTimeMillis);
    }

    /**
     * Creates a TTL cache with an injectable clock (for testing).
     */
    public TtlCache(int capacity, long defaultTtlMillis, LongSupplier clock) {
        if (capacity <= 0) throw new IllegalArgumentException("Capacity must be positive");
        if (defaultTtlMillis <= 0) throw new IllegalArgumentException("TTL must be positive");
        this.capacity = capacity;
        this.defaultTtlMillis = defaultTtlMillis;
        this.clock = clock;
        this.map = new LinkedHashMap<>();
    }

    public Optional<V> get(K key) {
        CacheEntry<V> entry = map.get(key);
        if (entry == null) return Optional.empty();
        if (entry.isExpired(clock.getAsLong())) {
            map.remove(key);
            return Optional.empty();
        }
        return Optional.of(entry.value);
    }

    public void put(K key, V value) {
        put(key, value, defaultTtlMillis);
    }

    public void put(K key, V value, long customTtlMillis) {
        cleanup();
        // If key already exists, remove it first (so we don't double-count capacity)
        map.remove(key);
        if (map.size() >= capacity) {
            // Evict the oldest entry (first inserted that remains)
            Iterator<K> it = map.keySet().iterator();
            if (it.hasNext()) {
                it.next();
                it.remove();
            }
        }
        long expirationTime = clock.getAsLong() + customTtlMillis;
        map.put(key, new CacheEntry<>(value, expirationTime));
    }

    public int size() {
        cleanup();
        return map.size();
    }

    public boolean containsKey(K key) {
        CacheEntry<V> entry = map.get(key);
        if (entry == null) return false;
        if (entry.isExpired(clock.getAsLong())) {
            map.remove(key);
            return false;
        }
        return true;
    }

    public void clear() {
        map.clear();
    }

    /**
     * Removes all expired entries from the cache.
     */
    public void cleanup() {
        long now = clock.getAsLong();
        map.entrySet().removeIf(e -> e.getValue().isExpired(now));
    }
}
