import { describe, test, expect } from "bun:test";
import {
  Trie,
  TrieMap,
  WordDictionary,
  TrieNode,
} from "../../src/data-structures/trie";

test("WordDictionary matches Unicode code points consistently", () => {
  const dictionary = new WordDictionary();
  dictionary.insert("😀");
  expect(dictionary.search("😀")).toBe(true);
  expect(dictionary.search(".")).toBe(true);
});

describe("TrieNode", () => {
  test("creates node with correct defaults", () => {
    const node = new TrieNode();
    expect(node.children.size).toBe(0);
    expect(node.isEnd).toBe(false);
    expect(node.value).toBeNull();
    expect(node.count).toBe(0);
  });
});

describe("Trie", () => {
  describe("empty trie", () => {
    test("should be empty initially", () => {
      const trie = new Trie();
      expect(trie.isEmpty()).toBe(true);
      expect(trie.size).toBe(0);
    });

    test("search returns false for empty trie", () => {
      const trie = new Trie();
      expect(trie.search("anything")).toBe(false);
    });

    test("startsWith returns false for empty trie", () => {
      const trie = new Trie();
      expect(trie.startsWith("any")).toBe(false);
    });

    test("delete returns false for empty trie", () => {
      const trie = new Trie();
      expect(trie.delete("word")).toBe(false);
    });

    test("getAllWords returns empty array", () => {
      const trie = new Trie();
      expect(trie.getAllWords()).toEqual([]);
    });

    test("autocomplete returns empty for empty trie", () => {
      const trie = new Trie();
      expect(trie.autocomplete("test")).toEqual([]);
    });
  });

  describe("insert", () => {
    test("insert single word", () => {
      const trie = new Trie();
      trie.insert("hello");
      expect(trie.size).toBe(1);
      expect(trie.search("hello")).toBe(true);
    });

    test("insert multiple words", () => {
      const trie = new Trie();
      trie.insert("hello");
      trie.insert("world");
      trie.insert("help");

      expect(trie.size).toBe(3);
      expect(trie.search("hello")).toBe(true);
      expect(trie.search("world")).toBe(true);
      expect(trie.search("help")).toBe(true);
    });

    test("insert duplicate word does not increase size", () => {
      const trie = new Trie();
      trie.insert("hello");
      trie.insert("hello");

      expect(trie.size).toBe(1);
      expect(trie.countWordsWithPrefix("hello")).toBe(1);
    });

    test("insert empty string", () => {
      const trie = new Trie();
      trie.insert("");
      expect(trie.size).toBe(1);
      expect(trie.search("")).toBe(true);
    });

    test("insert words with shared prefixes", () => {
      const trie = new Trie();
      trie.insert("cat");
      trie.insert("car");
      trie.insert("card");
      trie.insert("care");

      expect(trie.size).toBe(4);
      expect(trie.search("cat")).toBe(true);
      expect(trie.search("car")).toBe(true);
      expect(trie.search("card")).toBe(true);
      expect(trie.search("care")).toBe(true);
      expect(trie.search("ca")).toBe(false);
    });
  });

  describe("search", () => {
    test("search finds complete words only", () => {
      const trie = new Trie();
      trie.insert("hello");

      expect(trie.search("hello")).toBe(true);
      expect(trie.search("hell")).toBe(false);
      expect(trie.search("helloworld")).toBe(false);
      expect(trie.search("he")).toBe(false);
    });

    test("search is case sensitive", () => {
      const trie = new Trie();
      trie.insert("Hello");

      expect(trie.search("Hello")).toBe(true);
      expect(trie.search("hello")).toBe(false);
    });
  });

  describe("startsWith", () => {
    test("returns true for valid prefixes", () => {
      const trie = new Trie();
      trie.insert("hello");
      trie.insert("help");

      expect(trie.startsWith("hel")).toBe(true);
      expect(trie.startsWith("he")).toBe(true);
      expect(trie.startsWith("h")).toBe(true);
      expect(trie.startsWith("hello")).toBe(true);
    });

    test("returns false for invalid prefixes", () => {
      const trie = new Trie();
      trie.insert("hello");

      expect(trie.startsWith("hi")).toBe(false);
      expect(trie.startsWith("world")).toBe(false);
      expect(trie.startsWith("hellox")).toBe(false);
    });

    test("empty string prefix matches any trie with words", () => {
      const trie = new Trie();
      trie.insert("hello");
      expect(trie.startsWith("")).toBe(true);
    });
  });

  describe("delete", () => {
    test("delete existing word", () => {
      const trie = new Trie();
      trie.insert("hello");

      expect(trie.delete("hello")).toBe(true);
      expect(trie.search("hello")).toBe(false);
      expect(trie.size).toBe(0);
    });

    test("delete non-existing word", () => {
      const trie = new Trie();
      trie.insert("hello");
      expect(trie.delete("world")).toBe(false);
      expect(trie.size).toBe(1);
    });

    test("delete does not affect other words with shared prefix", () => {
      const trie = new Trie();
      trie.insert("hello");
      trie.insert("help");

      trie.delete("hello");
      expect(trie.search("hello")).toBe(false);
      expect(trie.search("help")).toBe(true);
      expect(trie.size).toBe(1);
    });

    test("delete prefix word does not affect longer words", () => {
      const trie = new Trie();
      trie.insert("car");
      trie.insert("card");

      trie.delete("car");
      expect(trie.search("car")).toBe(false);
      expect(trie.search("card")).toBe(true);
    });

    test("delete longer word does not affect prefix word", () => {
      const trie = new Trie();
      trie.insert("car");
      trie.insert("card");

      trie.delete("card");
      expect(trie.search("card")).toBe(false);
      expect(trie.search("car")).toBe(true);
    });

    test("delete cleans up unused nodes", () => {
      const trie = new Trie();
      trie.insert("hello");
      trie.insert("world");

      trie.delete("hello");
      // "hel" prefix should no longer exist
      expect(trie.startsWith("hel")).toBe(false);
      expect(trie.startsWith("wor")).toBe(true);
    });
  });

  describe("getAllWords", () => {
    test("returns all words", () => {
      const trie = new Trie();
      trie.insert("cat");
      trie.insert("car");
      trie.insert("dog");

      const words = trie.getAllWords();
      expect(words.sort()).toEqual(["car", "cat", "dog"]);
    });

    test("returns empty for empty trie", () => {
      const trie = new Trie();
      expect(trie.getAllWords()).toEqual([]);
    });
  });

  describe("autocomplete", () => {
    test("returns words matching prefix", () => {
      const trie = new Trie();
      trie.insert("cat");
      trie.insert("car");
      trie.insert("card");
      trie.insert("care");
      trie.insert("dog");

      const results = trie.autocomplete("car");
      expect(results.sort()).toEqual(["car", "card", "care"]);
    });

    test("respects limit", () => {
      const trie = new Trie();
      trie.insert("abc");
      trie.insert("abd");
      trie.insert("abe");
      trie.insert("abf");

      const results = trie.autocomplete("ab", 2);
      expect(results.length).toBe(2);
    });

    test("returns empty for non-matching prefix", () => {
      const trie = new Trie();
      trie.insert("hello");
      expect(trie.autocomplete("xyz")).toEqual([]);
    });

    test("returns exact match when it exists", () => {
      const trie = new Trie();
      trie.insert("hello");
      trie.insert("help");

      const results = trie.autocomplete("hello");
      expect(results).toContain("hello");
    });
  });

  describe("countWordsWithPrefix", () => {
    test("counts words with given prefix", () => {
      const trie = new Trie();
      trie.insert("cat");
      trie.insert("car");
      trie.insert("card");
      trie.insert("care");
      trie.insert("dog");

      expect(trie.countWordsWithPrefix("ca")).toBe(4);
      expect(trie.countWordsWithPrefix("car")).toBe(3);
      expect(trie.countWordsWithPrefix("cat")).toBe(1);
      expect(trie.countWordsWithPrefix("d")).toBe(1);
      expect(trie.countWordsWithPrefix("xyz")).toBe(0);
    });

    test("count updates after delete", () => {
      const trie = new Trie();
      trie.insert("hello");
      trie.insert("help");

      expect(trie.countWordsWithPrefix("hel")).toBe(2);
      trie.delete("hello");
      expect(trie.countWordsWithPrefix("hel")).toBe(1);
    });
  });

  describe("size and isEmpty", () => {
    test("size tracks correctly", () => {
      const trie = new Trie();
      expect(trie.size).toBe(0);

      trie.insert("a");
      expect(trie.size).toBe(1);

      trie.insert("b");
      expect(trie.size).toBe(2);

      trie.insert("a"); // Duplicate
      expect(trie.size).toBe(2);

      trie.delete("a");
      expect(trie.size).toBe(1);

      trie.delete("b");
      expect(trie.size).toBe(0);
      expect(trie.isEmpty()).toBe(true);
    });
  });

  describe("edge cases", () => {
    test("single character words", () => {
      const trie = new Trie();
      trie.insert("a");
      trie.insert("b");
      trie.insert("c");

      expect(trie.search("a")).toBe(true);
      expect(trie.search("d")).toBe(false);
      expect(trie.size).toBe(3);
    });

    test("long words", () => {
      const trie = new Trie();
      const longWord = "a".repeat(100);
      trie.insert(longWord);

      expect(trie.search(longWord)).toBe(true);
      expect(trie.search("a".repeat(99))).toBe(false);
    });

    test("special characters", () => {
      const trie = new Trie();
      trie.insert("hello-world");
      trie.insert("hello_world");
      trie.insert("hello.world");

      expect(trie.search("hello-world")).toBe(true);
      expect(trie.search("hello_world")).toBe(true);
      expect(trie.search("hello.world")).toBe(true);
    });
  });
});

describe("TrieMap", () => {
  describe("basic operations", () => {
    test("insert and get", () => {
      const map = new TrieMap<number>();
      map.insert("hello", 1);
      map.insert("world", 2);

      expect(map.get("hello")).toBe(1);
      expect(map.get("world")).toBe(2);
    });

    test("get returns null for missing key", () => {
      const map = new TrieMap<number>();
      expect(map.get("missing")).toBeNull();
    });

    test("search checks existence", () => {
      const map = new TrieMap<number>();
      map.insert("key", 42);

      expect(map.search("key")).toBe(true);
      expect(map.search("missing")).toBe(false);
    });

    test("insert overwrites existing value", () => {
      const map = new TrieMap<number>();
      map.insert("key", 1);
      map.insert("key", 2);

      expect(map.get("key")).toBe(2);
      expect(map.size).toBe(1);
    });

    test("delete removes entry", () => {
      const map = new TrieMap<number>();
      map.insert("key", 42);

      expect(map.delete("key")).toBe(true);
      expect(map.get("key")).toBeNull();
      expect(map.search("key")).toBe(false);
      expect(map.size).toBe(0);
    });

    test("delete returns false for missing key", () => {
      const map = new TrieMap<number>();
      expect(map.delete("missing")).toBe(false);
    });

    test("delete cleans up unused nodes", () => {
      const map = new TrieMap<number>();
      map.insert("hello", 1);
      map.insert("world", 2);

      map.delete("hello");
      expect(map.search("hello")).toBe(false);
      expect(map.search("world")).toBe(true);
    });
  });

  describe("getAllEntries", () => {
    test("returns all entries", () => {
      const map = new TrieMap<number>();
      map.insert("cat", 1);
      map.insert("car", 2);
      map.insert("dog", 3);

      const entries = map.getAllEntries();
      expect(entries.sort((a, b) => a[0].localeCompare(b[0]))).toEqual([
        ["car", 2],
        ["cat", 1],
        ["dog", 3],
      ]);
    });

    test("returns empty for empty map", () => {
      const map = new TrieMap<number>();
      expect(map.getAllEntries()).toEqual([]);
    });
  });

  describe("size and isEmpty", () => {
    test("size tracks correctly", () => {
      const map = new TrieMap<string>();
      expect(map.size).toBe(0);
      expect(map.isEmpty()).toBe(true);

      map.insert("a", "value");
      expect(map.size).toBe(1);
      expect(map.isEmpty()).toBe(false);
    });
  });

  describe("various value types", () => {
    test("works with object values", () => {
      const map = new TrieMap<{ id: number; name: string }>();
      map.insert("user1", { id: 1, name: "Alice" });
      map.insert("user2", { id: 2, name: "Bob" });

      expect(map.get("user1")).toEqual({ id: 1, name: "Alice" });
      expect(map.get("user2")).toEqual({ id: 2, name: "Bob" });
    });

    test("works with array values", () => {
      const map = new TrieMap<number[]>();
      map.insert("key", [1, 2, 3]);
      expect(map.get("key")).toEqual([1, 2, 3]);
    });
  });

  describe("shared prefixes", () => {
    test("handles shared prefix keys correctly", () => {
      const map = new TrieMap<number>();
      map.insert("pre", 1);
      map.insert("prefix", 2);
      map.insert("prefixed", 3);

      expect(map.get("pre")).toBe(1);
      expect(map.get("prefix")).toBe(2);
      expect(map.get("prefixed")).toBe(3);

      // Delete middle one
      map.delete("prefix");
      expect(map.get("pre")).toBe(1);
      expect(map.get("prefix")).toBeNull();
      expect(map.get("prefixed")).toBe(3);
    });
  });
});

describe("WordDictionary", () => {
  describe("basic operations", () => {
    test("insert and exact search", () => {
      const wd = new WordDictionary();
      wd.insert("hello");
      wd.insert("world");

      expect(wd.search("hello")).toBe(true);
      expect(wd.search("world")).toBe(true);
      expect(wd.search("missing")).toBe(false);
    });
  });

  describe("wildcard matching", () => {
    test("single dot matches any single character", () => {
      const wd = new WordDictionary();
      wd.insert("bad");
      wd.insert("dad");
      wd.insert("mad");

      expect(wd.search(".ad")).toBe(true);
      expect(wd.search("b.d")).toBe(true);
      expect(wd.search("ba.")).toBe(true);
      expect(wd.search("..d")).toBe(true);
      expect(wd.search("...")).toBe(true);
    });

    test("dot does not match empty character", () => {
      const wd = new WordDictionary();
      wd.insert("ab");

      expect(wd.search(".b")).toBe(true);
      expect(wd.search("a.")).toBe(true);
      expect(wd.search("..")).toBe(true);
      expect(wd.search("...")).toBe(false); // Too long
      expect(wd.search(".")).toBe(false); // Too short
    });

    test("multiple dots", () => {
      const wd = new WordDictionary();
      wd.insert("abc");
      wd.insert("abd");
      wd.insert("xyz");

      expect(wd.search("a..")).toBe(true);
      expect(wd.search("..c")).toBe(true);
      expect(wd.search("...")).toBe(true);
      expect(wd.search("a.c")).toBe(true);
      expect(wd.search("a.d")).toBe(true);
      expect(wd.search("x.z")).toBe(true);
    });

    test("wildcard with no match", () => {
      const wd = new WordDictionary();
      wd.insert("abc");

      expect(wd.search("b..")).toBe(false);
      expect(wd.search("....")).toBe(false);
    });

    test("all dots pattern", () => {
      const wd = new WordDictionary();
      wd.insert("cat");
      wd.insert("dog");

      expect(wd.search("...")).toBe(true);
      expect(wd.search("..")).toBe(false);
      expect(wd.search("....")).toBe(false);
    });
  });

  describe("exact vs wildcard search", () => {
    test("exact match", () => {
      const wd = new WordDictionary();
      wd.insert("pad");
      wd.insert("bad");
      wd.insert("dad");

      expect(wd.search("pad")).toBe(true);
      expect(wd.search("bad")).toBe(true);
      expect(wd.search("dad")).toBe(true);
      expect(wd.search("mad")).toBe(false);
    });

    test("partial word does not match", () => {
      const wd = new WordDictionary();
      wd.insert("hello");

      expect(wd.search("hell")).toBe(false);
      expect(wd.search("helloworld")).toBe(false);
    });
  });

  describe("edge cases", () => {
    test("empty word", () => {
      const wd = new WordDictionary();
      wd.insert("");

      expect(wd.search("")).toBe(true);
      expect(wd.search(".")).toBe(false);
    });

    test("single character words", () => {
      const wd = new WordDictionary();
      wd.insert("a");
      wd.insert("b");

      expect(wd.search("a")).toBe(true);
      expect(wd.search(".")).toBe(true);
      expect(wd.search("c")).toBe(false);
    });

    test("words of different lengths", () => {
      const wd = new WordDictionary();
      wd.insert("a");
      wd.insert("ab");
      wd.insert("abc");

      expect(wd.search("a")).toBe(true);
      expect(wd.search("ab")).toBe(true);
      expect(wd.search("abc")).toBe(true);
      expect(wd.search("abcd")).toBe(false);
      expect(wd.search(".")).toBe(true);
      expect(wd.search("..")).toBe(true);
      expect(wd.search("...")).toBe(true);
      expect(wd.search("....")).toBe(false);
    });

    test("complex wildcard patterns", () => {
      const wd = new WordDictionary();
      wd.insert("apple");
      wd.insert("apply");
      wd.insert("ample");

      expect(wd.search("a.pl.")).toBe(true); // matches "apple" or "apply"
      expect(wd.search("a..le")).toBe(true); // matches "apple" or "ample"
      expect(wd.search("ap.l.")).toBe(true); // matches "apple" or "apply"
      expect(wd.search("a.p.y")).toBe(true); // matches "apply"
      expect(wd.search(".....")).toBe(true);
      expect(wd.search("z....")).toBe(false);
    });
  });

  describe("LeetCode #211 examples", () => {
    test("example from problem description", () => {
      const wd = new WordDictionary();
      wd.insert("bad");
      wd.insert("dad");
      wd.insert("mad");

      expect(wd.search("pad")).toBe(false);
      expect(wd.search("bad")).toBe(true);
      expect(wd.search(".ad")).toBe(true);
      expect(wd.search("b..")).toBe(true);
    });
  });
});
