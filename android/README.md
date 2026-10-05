# Native pilot-face test APK

This focused Android test slice exercises Beauty's anatomical face in portable C.
It is a diagnostic face viewer; pilot likeness is unfitted. The existing pilot
art packet and the tiny-planet experiment retain their own scope.

Runtime layers:

| File | Responsibility |
| --- | --- |
| `native/face_core.[ch]` | activation, jaw/hyoid pose, posed muscle attachments, skin, strain safety |
| `native/face_data.inc` | pinned Beauty rest mesh, anatomical parameters, control recruitment |
| `native/face_ui.[ch]` | 36 control values, selection, bilateral mode, presets, camera |
| `native/pilot_renderer.c` | GLES2 face and diagnostic control panel |
| `native/pilot_android.c` | Pauli-derived NativeActivity input, EGL, lifecycle, saved state |
| `AndroidManifest.xml` | application-owned package/library/launcher configuration |
| `Makefile` | native compilation, package, signer and artifact receipts |

The runtime has no DEX, Java, Kotlin, JNI, WebView, HTML or JavaScript. The APK
contains one `lib/armeabi-v7a/libpilot_face.so`. See [provenance](PROVENANCE.md).

## Build and tests

Use GNU Make's `-C` with the absolute application checkout and `-f android/Makefile`.
Targets: `host-test`, `render-test`, `apk`. Pass `BEAUTY` as the absolute clean
checkout of the pinned source if it is not `.beauty-reference`.
`render-test` needs host EGL/GLES2 development inputs and Mesa surfaceless support.
`apk` needs the pinned NDK, SDK34, build-tools36 and public test signer checked out
at the paths in the workflow. It fails for missing SDK/NDK/key or mismatched
certificate; it does not generate a replacement key. This build does not use Gradle.

From a fresh checkout the `Native pilot face MIRO APK` Actions workflow checks out
the exact PR head and pinned references, compares regenerated data, runs 213 native
comparison cases and a host GLES visual suite, cross-compiles ARM, checks ELF and
the packaged library, aligns/signs, checks NativeActivity/hasCode=false, verifies
the expected certificate, and uploads `le-petit-prince-pilot-face-miro-a1.apk`
plus digest and evidence files. The artifact is named
`le-petit-prince-pilot-face-miro-a1` and expires after 30 days.
The artifact's ZIP digest is distinct from `apk.sha256`, the actual installable APK.

## Interaction at MIRO A1 size

- Drag the face area to orbit.
- PREV/NEXT selects all 36 CSV controls; the current number/name/value is visible.
- Minus/plus changes activation by 0.1.
- PAIR ON links the counterpart for the 28 paired controls. PAIR OFF changes
  only the selected side. Singleton controls remain single.
- NEUTRAL restores all controls and camera to neutral.
- PRESET cycles neutral, smile, frown, unilateral smile, jaw open, jaw left, brows.
- BLINK toggles bilateral eye closure. JAW OPEN/LEFT/RIGHT and BROW give direct
  diagnostic access and select their corresponding control.

The host visual suite drives the same native UI and renderer, verifies changed
face pixels for materially different states, exact reset, drag and EGL recreation.
This proves hosted Mesa rendering; it does not prove PowerVR or MIRO behavior.

## Physical MIRO A1 checklist (pending)

Record the exact CI head, APK SHA-256 and certificate before installation.
Use the prebuilt APK, never build on the phone. Preserve an existing installation;
do not uninstall to bypass a signing conflict.

1. Install the APK on the actual MIRO A1.
2. Launch Pilot Face; the face appears without an immediate crash.
3. Drag the face; its view rotates.
4. Change a control; the face visibly deforms.
5. Try smile, frown, unilateral smile and brow/blink.
6. Try jaw opening and both lateral directions.
7. NEUTRAL restores the neutral face.
8. Repeat manipulation without an immediate crash; background/resume once.

Physical acceptance remains pending until this exact artifact is run on the device.
Record results with `physical-acceptance.txt`; do not promote CI or host images.
