package datastructures

// TrieNode is a node in a Trie.
type TrieNode struct {
	Children map[rune]*TrieNode
	IsEnd    bool
	Value    interface{}
	Count    int
}

func newTrieNode() *TrieNode {
	return &TrieNode{Children: make(map[rune]*TrieNode)}
}

// Trie is a prefix tree for string storage and retrieval.
type Trie struct {
	Root *TrieNode
	size int
}

func NewTrie() *Trie {
	return &Trie{Root: newTrieNode()}
}

func (t *Trie) Size() int {
	return t.size
}

func (t *Trie) IsEmpty() bool {
	return t.size == 0
}

func (t *Trie) Insert(word string) {
	node := t.Root
	for _, ch := range word {
		if _, ok := node.Children[ch]; !ok {
			node.Children[ch] = newTrieNode()
		}
		node = node.Children[ch]
		node.Count++
	}
	if !node.IsEnd {
		node.IsEnd = true
		t.size++
	}
}

func (t *Trie) Search(word string) bool {
	node := t.findNode(word)
	return node != nil && node.IsEnd
}

func (t *Trie) StartsWith(prefix string) bool {
	return t.findNode(prefix) != nil
}

func (t *Trie) findNode(prefix string) *TrieNode {
	node := t.Root
	for _, ch := range prefix {
		child, ok := node.Children[ch]
		if !ok {
			return nil
		}
		node = child
	}
	return node
}

func (t *Trie) Delete(word string) bool {
	return t.deleteHelper(t.Root, []rune(word), 0)
}

func (t *Trie) deleteHelper(node *TrieNode, word []rune, depth int) bool {
	if depth == len(word) {
		if !node.IsEnd {
			return false
		}
		node.IsEnd = false
		t.size--
		return len(node.Children) == 0
	}

	ch := word[depth]
	child, ok := node.Children[ch]
	if !ok {
		return false
	}

	child.Count--
	shouldDelete := t.deleteHelper(child, word, depth+1)

	if shouldDelete {
		delete(node.Children, ch)
		return !node.IsEnd && len(node.Children) == 0
	}
	return false
}

func (t *Trie) GetWordsWithPrefix(prefix string) []string {
	node := t.findNode(prefix)
	if node == nil {
		return nil
	}
	var results []string
	t.collectWords(node, prefix, &results)
	return results
}

func (t *Trie) collectWords(node *TrieNode, prefix string, results *[]string) {
	if node.IsEnd {
		*results = append(*results, prefix)
	}
	for ch, child := range node.Children {
		t.collectWords(child, prefix+string(ch), results)
	}
}

func (t *Trie) Autocomplete(prefix string, limit int) []string {
	words := t.GetWordsWithPrefix(prefix)
	if len(words) > limit {
		return words[:limit]
	}
	return words
}

func (t *Trie) LongestCommonPrefix() string {
	node := t.Root
	prefix := ""

	for len(node.Children) == 1 && !node.IsEnd {
		for ch, child := range node.Children {
			prefix += string(ch)
			node = child
		}
	}
	return prefix
}

func (t *Trie) CountWordsWithPrefix(prefix string) int {
	node := t.findNode(prefix)
	if node == nil {
		return 0
	}
	return node.Count
}

// TrieMap stores key-value pairs in a trie with prefix-based operations.
type TrieMap struct {
	root *TrieNode
	size int
}

func NewTrieMap() *TrieMap {
	return &TrieMap{root: newTrieNode()}
}

func (tm *TrieMap) Size() int    { return tm.size }
func (tm *TrieMap) IsEmpty() bool { return tm.size == 0 }

func (tm *TrieMap) Put(key string, value interface{}) {
	node := tm.root
	for _, ch := range key {
		if _, ok := node.Children[ch]; !ok {
			node.Children[ch] = newTrieNode()
		}
		node = node.Children[ch]
	}
	if !node.IsEnd {
		tm.size++
	}
	node.IsEnd = true
	node.Value = value
}

func (tm *TrieMap) Get(key string) (interface{}, bool) {
	node := tm.findNode(key)
	if node == nil || !node.IsEnd {
		return nil, false
	}
	return node.Value, true
}

func (tm *TrieMap) Contains(key string) bool {
	node := tm.findNode(key)
	return node != nil && node.IsEnd
}

func (tm *TrieMap) Delete(key string) bool {
	node := tm.findNode(key)
	if node == nil || !node.IsEnd {
		return false
	}
	node.IsEnd = false
	node.Value = nil
	tm.size--
	return true
}

func (tm *TrieMap) findNode(key string) *TrieNode {
	node := tm.root
	for _, ch := range key {
		child, ok := node.Children[ch]
		if !ok {
			return nil
		}
		node = child
	}
	return node
}

func (tm *TrieMap) SumWithPrefix(prefix string) int {
	node := tm.findNode(prefix)
	if node == nil {
		return 0
	}
	return tm.sumValues(node)
}

func (tm *TrieMap) sumValues(node *TrieNode) int {
	total := 0
	if node.IsEnd {
		if v, ok := node.Value.(int); ok {
			total += v
		}
	}
	for _, child := range node.Children {
		total += tm.sumValues(child)
	}
	return total
}

func (tm *TrieMap) ItemsWithPrefix(prefix string) []struct {
	Key   string
	Value interface{}
} {
	node := tm.findNode(prefix)
	if node == nil {
		return nil
	}
	var items []struct {
		Key   string
		Value interface{}
	}
	tm.collectItems(node, prefix, &items)
	return items
}

func (tm *TrieMap) collectItems(node *TrieNode, prefix string, items *[]struct {
	Key   string
	Value interface{}
}) {
	if node.IsEnd {
		*items = append(*items, struct {
			Key   string
			Value interface{}
		}{prefix, node.Value})
	}
	for ch, child := range node.Children {
		tm.collectItems(child, prefix+string(ch), items)
	}
}

func (tm *TrieMap) Keys() []string {
	var keys []string
	tm.collectKeys(tm.root, "", &keys)
	return keys
}

func (tm *TrieMap) collectKeys(node *TrieNode, prefix string, keys *[]string) {
	if node.IsEnd {
		*keys = append(*keys, prefix)
	}
	for ch, child := range node.Children {
		tm.collectKeys(child, prefix+string(ch), keys)
	}
}

// CompressedTrie (Radix Tree) merges single-child chains for space efficiency.
type CompressedTrie struct {
	edges map[string]*compressedTrieNode
	count int
}

type compressedTrieNode struct {
	IsEnd    bool
	Children map[string]*compressedTrieNode
}

func NewCompressedTrie() *CompressedTrie {
	return &CompressedTrie{
		edges: make(map[string]*compressedTrieNode),
	}
}

func (ct *CompressedTrie) Size() int { return ct.count }

func commonPrefixLen(a, b string) int {
	n := len(a)
	if len(b) < n {
		n = len(b)
	}
	for i := 0; i < n; i++ {
		if a[i] != b[i] {
			return i
		}
	}
	return n
}

func (ct *CompressedTrie) Insert(word string) {
	if word == "" {
		return
	}
	ct.insertInto(ct.edges, word)
}

func (ct *CompressedTrie) insertInto(edges map[string]*compressedTrieNode, word string) {
	for label, node := range edges {
		cpl := commonPrefixLen(label, word)
		if cpl == 0 {
			continue
		}
		if cpl == len(label) && cpl == len(word) {
			if !node.IsEnd {
				node.IsEnd = true
				ct.count++
			}
			return
		}
		if cpl == len(label) {
			ct.insertInto(node.Children, word[cpl:])
			return
		}
		// Split needed
		delete(edges, label)
		splitNode := &compressedTrieNode{
			Children: make(map[string]*compressedTrieNode),
		}
		splitNode.Children[label[cpl:]] = node
		if cpl == len(word) {
			splitNode.IsEnd = true
		} else {
			splitNode.Children[word[cpl:]] = &compressedTrieNode{
				IsEnd:    true,
				Children: make(map[string]*compressedTrieNode),
			}
		}
		edges[word[:cpl]] = splitNode
		ct.count++
		return
	}
	edges[word] = &compressedTrieNode{
		IsEnd:    true,
		Children: make(map[string]*compressedTrieNode),
	}
	ct.count++
}

func (ct *CompressedTrie) Search(word string) bool {
	return ct.searchIn(ct.edges, word)
}

func (ct *CompressedTrie) searchIn(edges map[string]*compressedTrieNode, word string) bool {
	for label, node := range edges {
		if len(word) >= len(label) && word[:len(label)] == label {
			if len(word) == len(label) {
				return node.IsEnd
			}
			return ct.searchIn(node.Children, word[len(label):])
		}
	}
	return false
}

func (ct *CompressedTrie) StartsWith(prefix string) bool {
	return ct.startsWithIn(ct.edges, prefix)
}

func (ct *CompressedTrie) startsWithIn(edges map[string]*compressedTrieNode, prefix string) bool {
	for label, node := range edges {
		cpl := commonPrefixLen(label, prefix)
		if cpl == 0 {
			continue
		}
		if cpl >= len(prefix) {
			return true
		}
		if cpl == len(label) {
			return ct.startsWithIn(node.Children, prefix[cpl:])
		}
		return false
	}
	return false
}

// WordDictionary supports wildcard search with '.' matching any character.
type WordDictionary struct {
	Root *TrieNode
}

func NewWordDictionary() *WordDictionary {
	return &WordDictionary{Root: newTrieNode()}
}

func (wd *WordDictionary) AddWord(word string) {
	node := wd.Root
	for _, ch := range word {
		if _, ok := node.Children[ch]; !ok {
			node.Children[ch] = newTrieNode()
		}
		node = node.Children[ch]
	}
	node.IsEnd = true
}

func (wd *WordDictionary) SearchWord(word string) bool {
	return wd.searchHelper(wd.Root, []rune(word), 0)
}

func (wd *WordDictionary) searchHelper(node *TrieNode, word []rune, depth int) bool {
	if depth == len(word) {
		return node.IsEnd
	}

	ch := word[depth]
	if ch == '.' {
		for _, child := range node.Children {
			if wd.searchHelper(child, word, depth+1) {
				return true
			}
		}
		return false
	}

	child, ok := node.Children[ch]
	if !ok {
		return false
	}
	return wd.searchHelper(child, word, depth+1)
}
