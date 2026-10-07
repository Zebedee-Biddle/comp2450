// COMP 2450 — Floor 6 starter
// hero/Lint.cpp — body of isBalanced.  THE WORK IS HERE (Wednesday).

#include "Lint.h"

#include "Stack.h"

namespace dungeon {

// TODO Floor 6 (Wednesday).
//
// The classic stack-balancing algorithm. Walk `s` left-to-right:
//
//   * If c is '(', '[', or '{':   push it on the stack.
//   * If c is ')', ']', or '}':
//       - if the stack is empty, the string is unbalanced — return false
//         (we hit a closer with nothing waiting to be closed).
//       - otherwise peek at the top:
//           if top is the matching opener, pop and continue;
//           if it is the WRONG opener, return false
//             (a `)` cannot close a `{`).
//   * Any other character is ignored — text, spaces, digits, letters.
//
// At the end of the loop, the string is balanced iff the stack is empty.
// (If anything is left on the stack, those openers were never closed.)
//
// Reference body (do NOT paste — type it from memory in class):
//
//     Stack<char> s;
//     for (char c : input) {
//         switch (c) {
//             case '(': case '[': case '{':
//                 s.push(c); break;
//             case ')':
//                 if (s.empty() || s.top() != '(') return false;
//                 s.pop(); break;
//             case ']':
//                 if (s.empty() || s.top() != '[') return false;
//                 s.pop(); break;
//             case '}':
//                 if (s.empty() || s.top() != '{') return false;
//                 s.pop(); break;
//             default:
//                 break;
//         }
//     }
//     return s.empty();

bool isBalanced(const std::string& str) {
   Stack<char> stk;
   for (const auto& chr : str) if (chr == '{' || chr == '}' || chr == '(' || chr == ')' || chr == '[' || chr == ']') stk.push(chr);

   enum class chrt {BRACE, PAREN, BRACKET, START};
   Stack<chrt> prevChrT;
   prevChrT.push(chrt::START);

   for (bool first = true; !stk.empty(); first = false) {
	   char cmp = stk.top(); stk.pop();
	   switch (cmp) {
	   case '}': prevChrT.push(chrt::BRACE); break;
	   case ')': prevChrT.push(chrt::PAREN); break;
	   case ']': prevChrT.push(chrt::BRACKET); break;
	   case '{': if (prevChrT.top() != chrt::BRACE) return false; else prevChrT.pop(); if (prevChrT.empty()) return false; break;
	   case '(': if (prevChrT.top() != chrt::PAREN) return false; else prevChrT.pop(); if (prevChrT.empty()) return false; break;
	   default: if (prevChrT.top() != chrt::BRACKET) return false; else prevChrT.pop(); if (prevChrT.empty()) return false;
	   }
   }
   return prevChrT.top() == chrt::START;
}

}  // namespace dungeon
