package algorithms

import "math"

// GCD returns the greatest common divisor using Euclidean algorithm.
func GCD(a, b int) int {
	if a < 0 {
		a = -a
	}
	if b < 0 {
		b = -b
	}
	for b != 0 {
		a, b = b, a%b
	}
	return a
}

// LCM returns the least common multiple.
func LCM(a, b int) int {
	if a == 0 || b == 0 {
		return 0
	}
	return abs2(a) / GCD(a, b) * abs2(b)
}

func abs2(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

// IsPrime checks if a number is prime.
func IsPrime(n int) bool {
	if n < 2 {
		return false
	}
	if n < 4 {
		return true
	}
	if n%2 == 0 || n%3 == 0 {
		return false
	}
	for i := 5; i*i <= n; i += 6 {
		if n%i == 0 || n%(i+2) == 0 {
			return false
		}
	}
	return true
}

// SieveOfEratosthenes returns all primes up to n.
func SieveOfEratosthenes(n int) []int {
	if n < 2 {
		return nil
	}
	sieve := make([]bool, n+1)
	for i := range sieve {
		sieve[i] = true
	}
	sieve[0], sieve[1] = false, false
	for i := 2; i*i <= n; i++ {
		if sieve[i] {
			for j := i * i; j <= n; j += i {
				sieve[j] = false
			}
		}
	}
	var primes []int
	for i, isPrime := range sieve {
		if isPrime {
			primes = append(primes, i)
		}
	}
	return primes
}

// PrimeFactors returns the prime factorization.
func PrimeFactors(n int) []int {
	if n <= 1 {
		return nil
	}
	var factors []int
	for n%2 == 0 {
		factors = append(factors, 2)
		n /= 2
	}
	for i := 3; i*i <= n; i += 2 {
		for n%i == 0 {
			factors = append(factors, i)
			n /= i
		}
	}
	if n > 1 {
		factors = append(factors, n)
	}
	return factors
}

// FastPow computes base^exp mod modulus using binary exponentiation.
func FastPow(base, exp, modulus int) int {
	if modulus == 1 {
		return 0
	}
	result := 1
	base = base % modulus
	for exp > 0 {
		if exp%2 == 1 {
			result = result * base % modulus
		}
		exp /= 2
		base = base * base % modulus
	}
	return result
}

// EulerTotient computes Euler's totient function φ(n).
func EulerTotient(n int) int {
	result := n
	for p := 2; p*p <= n; p++ {
		if n%p == 0 {
			for n%p == 0 {
				n /= p
			}
			result -= result / p
		}
	}
	if n > 1 {
		result -= result / n
	}
	return result
}

// ExtendedGCD returns gcd, x, y such that a*x + b*y = gcd(a, b).
func ExtendedGCD(a, b int) (int, int, int) {
	if a == 0 {
		return b, 0, 1
	}
	gcd, x1, y1 := ExtendedGCD(b%a, a)
	return gcd, y1 - (b/a)*x1, x1
}

// ModInverse returns the modular multiplicative inverse of a mod m.
// Returns -1 if inverse doesn't exist.
func ModInverse(a, m int) int {
	gcd, x, _ := ExtendedGCD(a, m)
	if gcd != 1 {
		return -1
	}
	return ((x % m) + m) % m
}

// IsPerfectSquare checks if n is a perfect square.
func IsPerfectSquare(n int) bool {
	if n < 0 {
		return false
	}
	root := int(math.Sqrt(float64(n)))
	return root*root == n
}

// IntSqrt returns the integer square root (floor).
func IntSqrt(n int) int {
	if n < 0 {
		return -1
	}
	if n == 0 {
		return 0
	}
	x := int(math.Sqrt(float64(n)))
	// Adjust for floating point errors
	for x*x > n {
		x--
	}
	for (x+1)*(x+1) <= n {
		x++
	}
	return x
}

// NChooseK computes the binomial coefficient C(n, k).
func NChooseK(n, k int) int {
	if k < 0 || k > n {
		return 0
	}
	if k == 0 || k == n {
		return 1
	}
	if k > n-k {
		k = n - k
	}
	result := 1
	for i := 0; i < k; i++ {
		result = result * (n - i) / (i + 1)
	}
	return result
}

// CatalanNumber computes the nth Catalan number.
func CatalanNumber(n int) int {
	if n <= 1 {
		return 1
	}
	return NChooseK(2*n, n) / (n + 1)
}

// CountDivisors returns the number of divisors of n.
func CountDivisors(n int) int {
	if n <= 0 {
		return 0
	}
	count := 0
	for i := 1; i*i <= n; i++ {
		if n%i == 0 {
			count++
			if i != n/i {
				count++
			}
		}
	}
	return count
}

// SumDivisors returns the sum of all divisors of n.
func SumDivisors(n int) int {
	if n <= 0 {
		return 0
	}
	sum := 0
	for i := 1; i*i <= n; i++ {
		if n%i == 0 {
			sum += i
			if i != n/i {
				sum += n / i
			}
		}
	}
	return sum
}

// IsPowerOfTwo checks if n is a power of 2.
func IsPowerOfTwo(n int) bool {
	return n > 0 && n&(n-1) == 0
}

// NextPowerOfTwo returns the next power of 2 >= n.
func NextPowerOfTwo(n int) int {
	if n <= 0 {
		return 1
	}
	n--
	n |= n >> 1
	n |= n >> 2
	n |= n >> 4
	n |= n >> 8
	n |= n >> 16
	n |= n >> 32
	return n + 1
}

// CountBits returns the number of set bits in n.
func CountBits(n int) int {
	count := 0
	for n > 0 {
		count += n & 1
		n >>= 1
	}
	return count
}

// ReverseBits reverses the bits of a 32-bit unsigned integer.
func ReverseBits(n uint32) uint32 {
	var result uint32
	for i := 0; i < 32; i++ {
		result = (result << 1) | (n & 1)
		n >>= 1
	}
	return result
}
