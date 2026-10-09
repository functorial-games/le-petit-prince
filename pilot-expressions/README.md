# Photographed brow-knit expression: hypothesis for the pilot

Source: one user-supplied, backlit facial photograph discussed 2026-10-09.
The image is not uploaded to this public repository. The account link supplied
with the image is context, not evidence identifying the subject.

`intervals.idr` models **uncertain normalized muscle drive** for the pilot
character. It is an interval-valued estimate, not a measured muscle length,
measured EMG level, image-reconstructed attachment, or a calibrated statistical
confidence interval. Values q lie in [0,1] relative to the native Beauty rig's
arbitrary full activation. Subject-left/right is the model's side, not image
left/right. Differences between sides are tentative because of camera angle,
lighting, facial anatomy and the single observation.

| Anatomical actuator | Proposed normalized interval |
| --- | --- |
| `frontalis_medial_L` | [0.45, 0.67] |
| `frontalis_medial_R` | [0.38, 0.61] |
| `frontalis_lateral_L` | [0.05, 0.20] |
| `frontalis_lateral_R` | [0.04, 0.18] |
| `corrugator_supercilii_L` | [0.28, 0.50] |
| `corrugator_supercilii_R` | [0.32, 0.55] |
| `procerus` | [0.14, 0.34] |
| `levator_palpebrae_superioris_L` | [0.20, 0.41] |
| `levator_palpebrae_superioris_R` | [0.17, 0.38] |
| `depressor_anguli_oris_L` | [0.12, 0.30] |
| `depressor_anguli_oris_R` | [0.10, 0.27] |

The critical feature is coactivation: medial frontalis raises the brow while
corrugator draws the medial brow inward/downward. Lateral brow rise is weak,
the eyelid margin remains mostly open, and only a little downward oral drive
appears. No visible broad smile or open jaw is required.

29 of the pilot viewer's 102 named actuators receive explicit **uncertainty
ranges**. Unlisted actuator values are NOT inferred to be silent. A sparse
deterministic render uses zero extra model drive for these unspecified channels,
relative to the rig's built-in neutral face. It is a starting pose, not a
subject-specific facial inverse solution.

## Preview in the native pilot-face viewer

A generated C array (`android/native/face_photo_guess.h`) contains each
interval's midpoint. PRESET cycles to a new BROW KNIT state, which calls
`face_deform` with that 102-element vector directly. Manual edits and
other presets exit the guessed pose. Reset restores neutral.

Regenerate and audit against `actuator-audit/actuators.tsv`:

```sh
python3 pilot-expressions/check_intervals.py --write
python3 pilot-expressions/check_intervals.py
```

This checks index matching, uniqueness, interval ordering, coactivation
constraints and native-header freshness. It is **not** an Idris2 compilation.
With Idris2 installed, check the actual dependent-type construction:

```sh
idris2 --check pilot-expressions/intervals.idr
```

The current Android native viewer still uses Beauty's generic connected mesh,
not a finished pilot-likeness mesh. To fit the character's cheeks, eyelids,
mustache and brow shape, compare projected landmark changes with authored
pilot artwork without copying this photograph's facial proportions.
