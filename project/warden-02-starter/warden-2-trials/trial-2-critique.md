# Trial II — *The Critique*

*Warden of the Middle Gates · Midterm 2 · 15 %*

This trial **requires** you to use an LLM. You will paste the prompt and the reply, then critique what you got. Reading and judging AI output is part of the job now; we are practicing it on purpose.

---

## The prompt

Open Claude (or your LLM of choice) and paste **this exact prompt**:

> *Write a C++ class template `Deque<T>` that wraps a doubly-linked list and supports `push_front`, `push_back`, `pop_front`, `pop_back`, `front`, `back`, `size`, and `empty`. Each method should be O(1). Briefly explain your design choices.*

Tell us which tool you used and the date:

- **Tool:** Copilot Chat, "Think Deeply" mode
- **Date of the conversation:** 2026-10-25

---

## The LLM's reply

Paste the **full reply**, code and prose. Use a fenced code block for the code. Do not edit the reply — we want to see what you actually got:

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
#include <cstddef>
#include <stdexcept>
#include <utility>

template<typename T>
class Deque {
private:
    struct Node {
        T value;
        Node* prev;
        Node* next;
        Node(const T& v) : value(v), prev(nullptr), next(nullptr) {}
        Node(T&& v) : value(std::move(v)), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    std::size_t sz;

    void clear_nodes() noexcept {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head = tail = nullptr;
        sz = 0;
    }

public:
    Deque() noexcept : head(nullptr), tail(nullptr), sz(0) {}
    ~Deque() noexcept { clear_nodes(); }

    Deque(const Deque&) = delete;
    Deque& operator=(const Deque&) = delete;

    Deque(Deque&& other) noexcept : head(other.head), tail(other.tail), sz(other.sz) {
        other.head = other.tail = nullptr;
        other.sz = 0;
    }

    Deque& operator=(Deque&& other) noexcept {
        if (this != &other) {
            clear_nodes();
            head = other.head;
            tail = other.tail;
            sz = other.sz;
            other.head = other.tail = nullptr;
            other.sz = 0;
        }
        return *this;
    }

    void push_front(const T& value) {
        Node* n = new Node(value);
        n->next = head;
        if (head) head->prev = n;
        head = n;
        if (!tail) tail = n;
        ++sz;
    }

    void push_front(T&& value) {
        Node* n = new Node(std::move(value));
        n->next = head;
        if (head) head->prev = n;
        head = n;
        if (!tail) tail = n;
        ++sz;
    }

    void push_back(const T& value) {
        Node* n = new Node(value);
        n->prev = tail;
        if (tail) tail->next = n;
        tail = n;
        if (!head) head = n;
        ++sz;
    }

    void push_back(T&& value) {
        Node* n = new Node(std::move(value));
        n->prev = tail;
        if (tail) tail->next = n;
        tail = n;
        if (!head) head = n;
        ++sz;
    }

    T pop_front() {
        if (empty()) throw std::out_of_range("pop_front from empty deque");
        Node* n = head;
        T val = std::move(n->value);
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete n;
        --sz;
        return val;
    }

    T pop_back() {
        if (empty()) throw std::out_of_range("pop_back from empty deque");
        Node* n = tail;
        T val = std::move(n->value);
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete n;
        --sz;
        return val;
    }

    T& front() {
        if (empty()) throw std::out_of_range("front on empty deque");
        return head->value;
    }

    const T& front() const {
        if (empty()) throw std::out_of_range("front on empty deque");
        return head->value;
    }

    T& back() {
        if (empty()) throw std::out_of_range("back on empty deque");
        return tail->value;
    }

    const T& back() const {
        if (empty()) throw std::out_of_range("back on empty deque");
        return tail->value;
    }

    std::size_t size() const noexcept { return sz; }
    bool empty() const noexcept { return sz == 0; }
};

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

Design choices
-Doubly-linked nodes: Each node stores prev and next so both ends can be updated in constant time.

-Head and tail pointers: Keeping both pointers lets push_front, push_back, pop_front, and pop_back run in O(1) by only adjusting a few pointers.

-Size counter: A sz member returns size() in O(1) without traversal.

-Move-friendly API: Overloads accept lvalues and rvalues so elements can be moved into the deque efficiently.

-Exception safety: front, back, and pop_* throw std::out_of_range on empty deque to avoid undefined behavior.

-Simple ownership: Destructor deletes nodes; copy operations are deleted to avoid accidental shallow copies. Move semantics are provided for efficient transfers.


---

## My critique  *( ≥ 150 words )*

Your critique must read like an engineer reviewing a colleague's pull request — specific, technical, and naming exactly what the LLM did. A vague critique ("looks fine to me, but it could be better") is *not* full credit even if the LLM's code was great.

Address each of these:

- **Compile.** Did you actually try to compile and run the code? On what types? What happened?
- **Is it really O(1)?** The prompt asks for all eight operations to be O(1). Audit the implementation. Did the LLM rely on `std::list` (whose `back()` and `pop_back()` are O(1) thanks to a tail pointer), or did it hand-roll storage that walks from `head` for `back()` (silently O(n))? The prose claim is not the same as the code.
- **Empty case.** What does `pop_front()` or `pop_back()` do on an empty deque? Is the LLM's choice reasonable? Remember the ADT contract makes *no promise* about popping when empty — that call is undefined behaviour at the contract level. Audit what your own Floor 7 code *actually* does (your `Chain<T>::pop_front` guards the empty case, so your `Queue<T>::dequeue` happens to no-op) — then say which of the two you should judge the LLM's deque against: the contract, or your implementation's accident.
- **Composition.** Did the LLM compose over your `Chain<T>` (the right move in your codebase), over `std::list` (also fine), or reimplement doubly-linked storage from scratch (duplication — mark down)?
- **Const-correctness.** Did the LLM give you both `T& front()` and `const T& front() const`? Both `T& back()` and `const T& back() const`? Or only the mutable overloads?
- **One concrete improvement.** Name one specific change you would make before merging this into a real project. Not "make it better" — a concrete code change.

The code compiles, and I was able to run some testbenches. The prompt really is O(1), by using a tail pointer.

The deque _does_ verify that there is something to pop. I am not sure how I feel about this, because there are cases where it is not necessary to check. Throwing an error is probably
the appropriate thing to do if the check _does_ find that there is nothing to pop, however.

I would _not_ mark down for the fact that the LLM created a new data structure from scratch; the prompt told tbe LLM to _create_ a structure, so it did. The prompt mentioned nothing of
using the Chain, so I would be somewhat concerned if it mysteriously knew how to code for it.

The LLM was const-correct. A way it could be materially improved could be to allow the error-checking, as I previously mentioned, to be determined through, e.g., separate pop_back and
pop_back_us functions, with the _unsafe functions _not_ checking to verify that the container is nonempty. That way, cases where the container is veritably nonempty can use the _us
functions to reduce overhead. Template arguments to change _what_ the safe functions do could also be appreciated (e.g., throw like the LLM did, return nullptr, no-op...) I do not bl-
ame the LLM for not implementing these, however, because the prompt did not specifically ask for such (peculiar) decisions to be made.

Something I _would_ really penalize the AI for, however, would be deleting the copy constructor instead of implementing one; not being able to copy may cause extra user friction.

8.1 points