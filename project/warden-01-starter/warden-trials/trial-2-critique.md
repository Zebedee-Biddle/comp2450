# Trial II — *The Critique*

*Warden of the Foundations · Midterm 1 · 15 %*

This trial **requires** you to use an LLM. You will paste the prompt and the reply, then critique what you got. Reading and judging AI output is part of the job now; we are practicing it on purpose.

---

## The prompt

Open Claude (or your LLM of choice) and paste **this exact prompt**:

> *Write a C++ function template that returns the largest element of a `std::vector<T>`. Handle the case where the vector is empty. Briefly explain your design choices.*

Tell us which tool you used and the date:

- Copilot Chat in Think Deeper mode was the source of the response.
- 2026-09-20 (Sept. 20) was the date of the conversation.

---

## The LLM's reply

Paste the **full reply**, code and prose. Use a fenced code block for the code. Do not edit the reply — we want to see what you actually got.

```cpp
#include <vector>
#include <optional>
#include <algorithm>
#include <stdexcept>

template<typename T>
std::optional<T> largest_in_vector(const std::vector<T>& v) noexcept {
    if (v.empty()) return std::nullopt;
    auto it = std::max_element(v.begin(), v.end());
    return *it;
}
```

Design choices
-Return type std::optional<T>  
Signals the possibility of no result without throwing exceptions or using sentinel values; the caller must explicitly handle the empty case.

-Take const std::vector<T>&  
Avoids copying the container while allowing the function to work with any T that supports comparison.

-Use std::max_element  
Reuses a standard, well-tested algorithm that finds the maximum in one pass and works for any type with operator< defined.

-noexcept on the function  
The implementation itself does not throw (assuming T's copy/move and comparisons do not throw); marking noexcept documents intent and can help optimizations.

This implementation is simple, generic, and safe: it delegates comparison to the element type, avoids special sentinel values, and makes the empty-vector case explicit for callers.

---

## My critique  *( ≥ 150 words )*

Your critique must read like an engineer reviewing a colleague's pull request — specific, technical, and naming exactly what the LLM did. A vague critique ("looks fine to me, but it could be better") is *not* full credit even if the LLM's code was great.

Address each of these:

- **Compile.** Did you actually try to compile and run the code? On what types? What happened?
- **Empty case.** What does the empty-vector case actually do in this code? Is the LLM's choice reasonable? Is it different from what *you* would have chosen?
- **Requirements on `T`.** What does the code require of `T`? What happens if you instantiate it with a type that doesn't support `<` (e.g., a custom struct)?
- **Const-correctness.** Pass by value or by reference? Return by value or by reference? Would you change either?
- **`std::max_element`?** Did the LLM cheat by using the algorithm header, or did it write the loop? Which would *you* have shipped, and why?
- **One concrete improvement.** Name one specific change you would make before merging this into a real project. Not "make it better" — a concrete code change.

When I compiled and ran the code, it actually worked as expected on ints, doubles, and whatever primitive type there be. However:
When using a custom struct, the code refused to compile, giving the errors, "binary '<<': no operator found which takes a right-hand operand of type '_Ty' (or there is no acceptable conversion)" and "no operator "<<" matches these operands".
The LLM's choice is defensible from the perspective of ideological safety, but not when actually using logic to analyze what is actually good. Using optional<> technically invokes overhead, but the more important issue is the unnecessary syntax cluttering when using the function. This is because it is necessary to use value() whenever doing _anything_ with the return. Just returning the smallest possible value for the given T (-2'147'483'648, -NaN, "", nullptr, etc) would in practice not cause as much developer pain, since the error would likely be obvious when it happened to be important, and any cases where the highest of multiple of these returns are taken (likely common) would cause this problem to disappear.
The function correctly received the vector as a const reference. There is no distinction between value const and value non-const as far as the world outside of the function is concerned, so the return not being const was not a problem. Returning a reference from a free function is asking for trouble, so it is good that this was not picked.
The LLM 'cheated' by using std::max_element. This is what would likely be done in practice, and the only real objection I have is that it is not 'novel' for the purpose of example, which seems to be some of the purpose of AI chatbots. However, I am not concerned that the AI is incapable of doing this; but since most work ecosystems probably do not like paying their workers to reinvent the wheel, this code would probably be more favored in the real world, and so by me with concern to the LLM.
With the unlimited sanity of an AI to deal with metaprogramming woes, having some kind of template parameter to decide between these possibilities (whether returning a configurable number, or throwing an exception, or returning an optional<T> like Copilot did, as well as supporting not checking), and defaulting to returning the smallest possible value would likely be the perfectly ideal approach.