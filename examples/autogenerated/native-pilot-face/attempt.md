# Native pilot face

The requested runtime is explicitly portable C / Android NDK. `Activation` is
a finite double in [0,1], `Controls` has exactly 36 values in Beauty CSV order,
and `Muscles` has 102 independent values in Beauty compiled-ledger order.
`Point` is subject-left / superior / anterior millimetres. `FaceResult` holds
the jaw/hyoid pose, bounded tissue strain, junctions and connected surface.

Operations: Controls -> Muscles -> Skeleton -> PosedAttachments -> Skin.
Neutral is the identity; left/right inputs remain independent; malformed
values fail. Android lifecycle, rendering and camera never enter the core.

The user explicitly selects C for this port. No Idriç replacement or generated-C
backend is used. The host-only ECMAScript reference runner must import and
execute Beauty's actual ECMAScript module to serve as an independent oracle.
It generates rest data and fixtures, never application runtime code. No JS,
HTML, browser or dependency package is shipped in the APK.

The first language boundary for a later Idriç consumer remains the verified
native Android graphics/runtime path described in the application README.
Replacing this C slice would require the same full-vector comparison, native
ELF/package gates and separately recorded physical MIRO acceptance.
