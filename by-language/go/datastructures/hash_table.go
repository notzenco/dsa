package datastructures

import (
	"fmt"
	"hash/fnv"
)

const (
	htDefaultCapacity    = 16
	htLoadFactorChaining = 0.75
	htLoadFactorOpen     = 0.5
)

// --- Hash Table with Chaining ---

type htChainEntry[K comparable, V any] struct {
	Key   K
	Value V
}

type HashTableChaining[K comparable, V any] struct {
	buckets  [][]htChainEntry[K, V]
	size     int
	capacity int
}

func NewHashTableChaining[K comparable, V any]() *HashTableChaining[K, V] {
	return &HashTableChaining[K, V]{
		buckets:  make([][]htChainEntry[K, V], htDefaultCapacity),
		capacity: htDefaultCapacity,
	}
}

func (ht *HashTableChaining[K, V]) Size() int {
	return ht.size
}

func (ht *HashTableChaining[K, V]) IsEmpty() bool {
	return ht.size == 0
}

func (ht *HashTableChaining[K, V]) hash(key K) int {
	h := fnv.New64a()
	fmt.Fprint(h, key)
	return int(h.Sum64() % uint64(ht.capacity))
}

func (ht *HashTableChaining[K, V]) Put(key K, value V) {
	if float64(ht.size+1)/float64(ht.capacity) > htLoadFactorChaining {
		ht.resize(ht.capacity * 2)
	}

	idx := ht.hash(key)
	for i, entry := range ht.buckets[idx] {
		if entry.Key == key {
			ht.buckets[idx][i].Value = value
			return
		}
	}
	ht.buckets[idx] = append(ht.buckets[idx], htChainEntry[K, V]{key, value})
	ht.size++
}

func (ht *HashTableChaining[K, V]) Get(key K) (V, bool) {
	idx := ht.hash(key)
	for _, entry := range ht.buckets[idx] {
		if entry.Key == key {
			return entry.Value, true
		}
	}
	var zero V
	return zero, false
}

func (ht *HashTableChaining[K, V]) Delete(key K) bool {
	idx := ht.hash(key)
	for i, entry := range ht.buckets[idx] {
		if entry.Key == key {
			ht.buckets[idx] = append(ht.buckets[idx][:i], ht.buckets[idx][i+1:]...)
			ht.size--
			return true
		}
	}
	return false
}

func (ht *HashTableChaining[K, V]) Contains(key K) bool {
	_, ok := ht.Get(key)
	return ok
}

func (ht *HashTableChaining[K, V]) Keys() []K {
	keys := make([]K, 0, ht.size)
	for _, bucket := range ht.buckets {
		for _, entry := range bucket {
			keys = append(keys, entry.Key)
		}
	}
	return keys
}

func (ht *HashTableChaining[K, V]) Values() []V {
	values := make([]V, 0, ht.size)
	for _, bucket := range ht.buckets {
		for _, entry := range bucket {
			values = append(values, entry.Value)
		}
	}
	return values
}

func (ht *HashTableChaining[K, V]) Clear() {
	ht.buckets = make([][]htChainEntry[K, V], htDefaultCapacity)
	ht.capacity = htDefaultCapacity
	ht.size = 0
}

func (ht *HashTableChaining[K, V]) resize(newCapacity int) {
	oldBuckets := ht.buckets
	ht.buckets = make([][]htChainEntry[K, V], newCapacity)
	ht.capacity = newCapacity
	ht.size = 0

	for _, bucket := range oldBuckets {
		for _, entry := range bucket {
			ht.Put(entry.Key, entry.Value)
		}
	}
}

// --- Hash Table with Open Addressing (Linear Probing) ---

type htOpenEntry[K comparable, V any] struct {
	Key       K
	Value     V
	Occupied  bool
	Tombstone bool
}

type HashTableOpenAddressing[K comparable, V any] struct {
	entries  []htOpenEntry[K, V]
	size     int
	capacity int
}

func NewHashTableOpenAddressing[K comparable, V any]() *HashTableOpenAddressing[K, V] {
	return &HashTableOpenAddressing[K, V]{
		entries:  make([]htOpenEntry[K, V], htDefaultCapacity),
		capacity: htDefaultCapacity,
	}
}

func (ht *HashTableOpenAddressing[K, V]) Size() int {
	return ht.size
}

func (ht *HashTableOpenAddressing[K, V]) IsEmpty() bool {
	return ht.size == 0
}

func (ht *HashTableOpenAddressing[K, V]) hash(key K) int {
	h := fnv.New64a()
	fmt.Fprint(h, key)
	return int(h.Sum64() % uint64(ht.capacity))
}

func (ht *HashTableOpenAddressing[K, V]) Put(key K, value V) {
	if float64(ht.size+1)/float64(ht.capacity) > htLoadFactorOpen {
		ht.resize(ht.capacity * 2)
	}

	idx := ht.hash(key)
	for i := 0; i < ht.capacity; i++ {
		pos := (idx + i) % ht.capacity
		entry := &ht.entries[pos]
		if !entry.Occupied || entry.Tombstone {
			entry.Key = key
			entry.Value = value
			entry.Occupied = true
			entry.Tombstone = false
			ht.size++
			return
		}
		if entry.Key == key {
			entry.Value = value
			return
		}
	}
}

func (ht *HashTableOpenAddressing[K, V]) Get(key K) (V, bool) {
	idx := ht.hash(key)
	for i := 0; i < ht.capacity; i++ {
		pos := (idx + i) % ht.capacity
		entry := &ht.entries[pos]
		if !entry.Occupied {
			break
		}
		if !entry.Tombstone && entry.Key == key {
			return entry.Value, true
		}
	}
	var zero V
	return zero, false
}

func (ht *HashTableOpenAddressing[K, V]) Delete(key K) bool {
	idx := ht.hash(key)
	for i := 0; i < ht.capacity; i++ {
		pos := (idx + i) % ht.capacity
		entry := &ht.entries[pos]
		if !entry.Occupied {
			break
		}
		if !entry.Tombstone && entry.Key == key {
			entry.Tombstone = true
			ht.size--
			return true
		}
	}
	return false
}

func (ht *HashTableOpenAddressing[K, V]) Contains(key K) bool {
	_, ok := ht.Get(key)
	return ok
}

func (ht *HashTableOpenAddressing[K, V]) Clear() {
	ht.entries = make([]htOpenEntry[K, V], htDefaultCapacity)
	ht.capacity = htDefaultCapacity
	ht.size = 0
}

func (ht *HashTableOpenAddressing[K, V]) resize(newCapacity int) {
	oldEntries := ht.entries
	ht.entries = make([]htOpenEntry[K, V], newCapacity)
	ht.capacity = newCapacity
	ht.size = 0

	for _, entry := range oldEntries {
		if entry.Occupied && !entry.Tombstone {
			ht.Put(entry.Key, entry.Value)
		}
	}
}

// --- Hash Set ---

type HashSet[K comparable] struct {
	table *HashTableChaining[K, struct{}]
}

func NewHashSet[K comparable]() *HashSet[K] {
	return &HashSet[K]{table: NewHashTableChaining[K, struct{}]()}
}

func (s *HashSet[K]) Add(key K) {
	s.table.Put(key, struct{}{})
}

func (s *HashSet[K]) Contains(key K) bool {
	return s.table.Contains(key)
}

func (s *HashSet[K]) Remove(key K) bool {
	return s.table.Delete(key)
}

func (s *HashSet[K]) Size() int {
	return s.table.Size()
}

func (s *HashSet[K]) IsEmpty() bool {
	return s.table.IsEmpty()
}

func (s *HashSet[K]) Items() []K {
	return s.table.Keys()
}

func (s *HashSet[K]) Clear() {
	s.table.Clear()
}
