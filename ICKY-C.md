# Icky C spherical walker

The two maintained C sources and the equivalence test use ← assignments and ×
multiplication. Pointer declarations retain `*`. The public ABI header keeps
its assignment-neutral declarations and layouts.

Turn and walk compose immutable `tangent_frame` values through projection,
normalization, and rotation. `publish_frame` owns the writes to the caller's
walker. Failed `set_frame` still publishes the same projected frame before
rejecting a degenerate forward vector; this observable behavior is preserved.
The existing scalar operation order, radius policy, null handling, coordinate
clamping, seam-free geometry, and independent pole/circumnavigation tests stay
intact.

Use `make test ICK=/absolute/path/to/ick` from this repository, or pass this
Makefile's absolute path with `make -f`. The Makefile replaces the old POSIX
stock-compiler launcher; no generated shell helper is required. Tests require
assertions to remain enabled.

CI checks out the exact head and builds actual ICK c5d28dde9cc333a562b907785d0370b725146cdf
through the pinned ai-ci producer. That native scalar profile declares host
GCC 13 startup/libgcc and glibc/libm as prebuilt runtime dependencies; host GCC
bootstraps ICK and does not compile maintained consumer C. It runs the original
sphere checks, 65,536 bitwise-equivalent turn/walk pairs, invalid/nonfinite API
cases, the assignment guard, and the shared build-toolchain contract.

All non-NaN outputs, including signed zero and infinities, compare bitwise.
NaN outputs compare classification; the API does not promise a NaN payload or
sign, and C11 Annex F.10 paragraph 13 permits a math result's NaN sign to differ.
The first overly strict NaN-bit comparator
failed at O2 on a valid-up/NaN-forward input (positive versus negative NaN),
while O0 matched; that observed difference is retained in this account rather
than advertised as payload preservation. See the
[WG14 C11 draft, Annex F.10](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf).

The frozen original source is a comparison control compiled through the same
ICK, not another maintained implementation. The lexical guard proves assignment
policy, while numerical execution qualifies the frame composition.

This qualifies the portable model on the native host. Android renderers on
other branches, terrain downloads, packaging, and physical MIRO A1 execution
remain separate acceptance boundaries.
