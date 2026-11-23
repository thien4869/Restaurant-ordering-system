# C++ Integration Status (23/11/2026)

## What has been verified
Both TV3 ZIP archives were extracted locally and compiled with GNU C++ 14.2 in C++17 mode. A CMake 3.31 trial produced four executables; CTest passed **2/2 executable-level checks**:
- `tv3_account_demo` / `tv3_account_tests`
- `tv3_orders_demo` / `tv3_orders_tests`

The repository now has `CMakeLists.txt` to build these prototypes **independently from the original unchanged ZIPs**. It extracts archives to `build/extracted/` during configuration; it does not rewrite or combine their source. This uses CMake >= 3.18.

## PowerShell from ProgAndTest_Group11

```powershell
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

Requires a C++17 compiler and CMake. For MinGW, use the correct generator when configuring if necessary. Do not use the existing PHP Docker Compose to run C++.

## Still NOT integrated
1. Two separate console programs, each with its own `main()`. No Group 11 unified application/GUI.
2. Account repository uses in-memory `std::vector<User>`, `std::hash` for passwords (not secure), and password reset without identity verification. No shared login/session/DB.
3. Checkout/Order repository uses in-memory storage and sample cart; `double` for VND should be replaced with integer money values.
4. This Orders submission overlaps TV2/TV4 responsibilities; assignment needs confirmation.
5. Existing test executables use `assert`, not Google Test. Compiling with `NDEBUG` may disable checks; use Debug until upgraded.
6. No common `src/main.cpp`, class contracts, or end-to-end tests. GitHub `tests/testcases/` has several 1-byte placeholder spreadsheets.

## Next integration milestones
Agree on one shared `User`, `Product`, `CartItem`, `Order` model, Repository and Service interfaces, persistence, and GUI. Then move reviewed implementation into `src/` with one application entry point; run full flow tests and update the README/Docker configuration. Preserve original submissions and Git history.
