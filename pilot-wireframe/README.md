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
