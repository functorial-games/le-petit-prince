# Pilot wireframe / control-surface program

This folder defines the **pilot identity surface** before it becomes a production render mesh.

The immediate purpose is not to make a pretty head. It is to produce a geometrically explicit head whose curvature and topology can be inspected while muscles, fat compartments, retaining structures, and later a volume solver are developed.

## Non-negotiable separation

Do not collapse these into one mesh:

1. **identity/control surface** — pilot head shape and useful modeling loops;
2. **biomechanics volume** — tissue discretization chosen for numerical accuracy;
3. **render surface** — GPU/display tessellation.

The control surface may be mostly quads. The runtime renderer may triangulate it. Neither choice says that biological tissue is made of quads or triangles.

## Pilot identity target

The existing art packet fixes the broad character target:

- heavyset / fat rather than merely broad;
- broad convex cheek masses and substantial lower-face volume;
- sagging cheeks / jowl tendency;
- substantial brow and nose;
- heavy eyelids;
- thick mustache region;
- tired/exhausted neutral expression;
- enough orbital and jaw volume that astonishment can be layered over fatigue.

Do not infer unobserved depth from one shaded drawing. Use multiple drawings to constrain landmarks and record what remains underdetermined.

## First geometry checkpoint

Before sculpting a 3-D surface:

1. extract the landmarks in `landmarks.tsv` from the existing contact sheet / source drawings;
2. record which views genuinely constrain depth;
3. establish bilateral correspondences without forcing perfect symmetry;
4. construct a coarse skull/mandible envelope;
5. fit a low-resolution control surface to those constraints;
6. inspect front, 3/4, profile, silhouette, and curvature before adding density.

The result is allowed to remain underdetermined where the drawings are underdetermined. Those degrees of freedom should be explicit parameters rather than hidden guesses.

## Deformation-oriented topology

See `topology.md`. The topology is a numerical/design hypothesis to test, not anatomical truth.

## Control cage v0

The first geometric artifact is deliberately **not** a surface mesh.

`pilot-control-cage-v0.obj` is an OBJ line cage with 554 vertices and 49 named guide polylines. It records the head envelope and feature guides without prematurely deciding whether the later surface or mechanics discretization should use quads, triangles, tetrahedra, hexahedra, or another element family.

Files:

- `pilot-control-cage-v0.obj` — importable 3-D line cage;
- `pilot-control-cage-v0.svg` — deterministic front / three-quarter / profile preview;
- `pilot-control-cage-v0.points.tsv` — explicit generated vertex coordinates;
- `pilot-control-cage-v0.parameters.tsv` — which dimensions come from the drawings and which remain weak depth hypotheses;
- `reference-observations.md` — which prior drawings are valid identity references and which are only historical/anatomical references;
- `landmark-observations.tsv` — first approximate image-space landmark pass on a near-frontal heavy-pilot drawing;
- `generate_control_cage.py` — dependency-free deterministic generator.

Regenerate with:

```sh
python3 pilot-wireframe/generate_control_cage.py
python3 pilot-wireframe/generate_control_cage.py --check
```

### Important boundary

The cage is an **identity and curvature hypothesis**, not a biomechanics result.

Its frontal cheek/jowl proportions are constrained by the final heavy-pilot drawings. Its depth parameters remain low-confidence because the drawings do not supply calibrated cameras or orthographic profile views. Those values are deliberately exposed in the parameter ledger rather than hidden in a sculpt.

The next step is to fit an actual surface topology to this cage while preserving the option to use an independent volumetric discretization for biomechanics.

## Surface topology v1 baseline

`generate_surface_topology.py` now connects the transverse control-cage rings into a deliberately simple **envelope-only quad surface**. This is the first topology hypothesis, not a production face mesh.

Generated artifacts:

- `pilot-surface-envelope-v1-quads.obj` — 264 vertices / 240 quads;
- `pilot-surface-envelope-v1-tri-A.obj` and `...tri-B.obj` — the same vertices with opposite quad diagonals;
- `surface-topology-v1-metrics.tsv` — manifoldness, symmetry, diagonal sensitivity, triangle quality and refinement results;
- `surface-topology-v1-report.md` — interpretation and rejection/acceptance boundary.

The baseline passes its numerical tests, including alternate triangulation and mesh-density refinement. It is **not accepted facial topology** because it does not yet contain explicit eyelid/lip apertures or local facial patch structure. Its job is to give the next candidates something measurable to beat.
