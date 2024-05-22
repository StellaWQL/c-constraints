## Problem

Design APIs to travel all solutions, which are met the requirements of all
limitations.

- Support adding variables with name and varargs to collection.
- Support adding limitations for collection.
- Support registering travel function for all solutions.

You should implement follow functions like below:
```
- add_variable(name, val, ...)
- add_limit(func)
- travel_solution(func)
```

We implement a demo for this problem, but it just a demo. You can design your own APIs if you need.

### Limitation

**Limit 1**

We suppose that the type of variables stored in collection is Int(int32_t or int64_t).

**Limtt 2**

You can suppose all limits don't have bugs. Which means you only need to implement the most basic
protection mechanisms in your program.

## Requirement

- Use git to manage your codes.
- Obey linux-kernel coding style. (https://www.kernel.org/doc/html/v4.10/process/coding-style.html)
- You should provide a **test plan** for your program, and implement **some or all** of the test cases.
