"""
Data Structures implementations in Python.

This module contains implementations of fundamental and advanced data structures
with comprehensive documentation, complexity analysis, and LeetCode problem mappings.
"""

from .dynamic_array import DynamicArray
from .singly_linked_list import SinglyLinkedList, Node
from .doubly_linked_list import DoublyLinkedList, DNode
from .stack import Stack, MinStack, MaxStack
from .queue import Queue, CircularQueue, Deque, MonotonicQueue
from .binary_search_tree import BinarySearchTree, TreeNode
from .hash_table import HashTableChaining, HashTableOpenAddressing, HashMap, HashSet
from .heap import MinHeap, MaxHeap, PriorityQueue

__all__ = [
    # Arrays
    "DynamicArray",
    # Linked Lists
    "SinglyLinkedList",
    "DoublyLinkedList",
    "Node",
    "DNode",
    # Stacks
    "Stack",
    "MinStack",
    "MaxStack",
    # Queues
    "Queue",
    "CircularQueue",
    "Deque",
    "MonotonicQueue",
    # Trees
    "BinarySearchTree",
    "TreeNode",
    # Hash Tables
    "HashTableChaining",
    "HashTableOpenAddressing",
    "HashMap",
    "HashSet",
    # Heaps
    "MinHeap",
    "MaxHeap",
    "PriorityQueue",
]
