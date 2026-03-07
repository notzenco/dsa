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
