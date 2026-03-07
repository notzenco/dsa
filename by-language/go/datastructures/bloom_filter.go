package datastructures

import (
	"crypto/md5"
	"crypto/sha256"
	"encoding/binary"
	"math"
)

// BloomFilter is a space-efficient probabilistic data structure.
type BloomFilter struct {
	bits     []bool
	size     int
	numHash  int
	count    int
}

func NewBloomFilter(expectedItems int, falsePositiveRate float64) *BloomFilter {
	size := optimalSize(expectedItems, falsePositiveRate)
	numHash := optimalHashes(size, expectedItems)
	return &BloomFilter{
		bits:    make([]bool, size),
		size:    size,
		numHash: numHash,
	}
}

func NewBloomFilterWithSize(size, numHash int) *BloomFilter {
	return &BloomFilter{
		bits:    make([]bool, size),
		size:    size,
		numHash: numHash,
	}
}

func optimalSize(n int, p float64) int {
	m := -float64(n) * math.Log(p) / (math.Log(2) * math.Log(2))
	return int(math.Ceil(m))
}

func optimalHashes(m, n int) int {
	k := float64(m) / float64(n) * math.Log(2)
	return int(math.Max(1, math.Round(k)))
}

func (bf *BloomFilter) getHashes(item string) []int {
	h1 := md5Hash(item)
	h2 := sha256Hash(item)

	hashes := make([]int, bf.numHash)
	for i := 0; i < bf.numHash; i++ {
		combined := h1 + uint64(i)*h2
		hashes[i] = int(combined % uint64(bf.size))
	}
	return hashes
}

func md5Hash(s string) uint64 {
	h := md5.Sum([]byte(s))
	return binary.BigEndian.Uint64(h[:8])
}

func sha256Hash(s string) uint64 {
	h := sha256.Sum256([]byte(s))
	return binary.BigEndian.Uint64(h[:8])
}

func (bf *BloomFilter) Add(item string) {
	for _, idx := range bf.getHashes(item) {
		bf.bits[idx] = true
	}
	bf.count++
}

func (bf *BloomFilter) Contains(item string) bool {
	for _, idx := range bf.getHashes(item) {
		if !bf.bits[idx] {
			return false
		}
	}
	return true
}

func (bf *BloomFilter) Count() int {
	return bf.count
}

func (bf *BloomFilter) FalsePositiveRate() float64 {
	setBits := 0
	for _, b := range bf.bits {
		if b {
			setBits++
		}
	}
	ratio := float64(setBits) / float64(bf.size)
	return math.Pow(ratio, float64(bf.numHash))
}

// CountingBloomFilter supports deletions.
type CountingBloomFilter struct {
	counters []int
	size     int
	numHash  int
	count    int
}

func NewCountingBloomFilter(expectedItems int, falsePositiveRate float64) *CountingBloomFilter {
	size := optimalSize(expectedItems, falsePositiveRate)
	numHash := optimalHashes(size, expectedItems)
	return &CountingBloomFilter{
		counters: make([]int, size),
		size:     size,
		numHash:  numHash,
	}
}

func (cbf *CountingBloomFilter) getHashes(item string) []int {
	h1 := md5Hash(item)
	h2 := sha256Hash(item)

	hashes := make([]int, cbf.numHash)
	for i := 0; i < cbf.numHash; i++ {
		combined := h1 + uint64(i)*h2
		hashes[i] = int(combined % uint64(cbf.size))
	}
	return hashes
}

func (cbf *CountingBloomFilter) Add(item string) {
	for _, idx := range cbf.getHashes(item) {
		cbf.counters[idx]++
	}
	cbf.count++
}

func (cbf *CountingBloomFilter) Contains(item string) bool {
	for _, idx := range cbf.getHashes(item) {
		if cbf.counters[idx] == 0 {
			return false
		}
	}
	return true
}

func (cbf *CountingBloomFilter) Remove(item string) bool {
	if !cbf.Contains(item) {
		return false
	}
	for _, idx := range cbf.getHashes(item) {
		if cbf.counters[idx] > 0 {
			cbf.counters[idx]--
		}
	}
	cbf.count--
	return true
}

func (cbf *CountingBloomFilter) Count() int {
	return cbf.count
}
