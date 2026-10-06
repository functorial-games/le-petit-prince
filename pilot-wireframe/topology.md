# Pilot facial control-surface topology

## Why a control mesh

The current native face proves that a surface can be deformed and rendered, but it does not prove that its tessellation is suitable for high-fidelity facial mechanics.

A new pilot control surface should make important apertures, folds, attachment neighborhoods, and curvature changes easy to resolve and inspect.

## Proposed surface organization

Prefer mostly quad-like loops in the editable control surface around:

- eyelid apertures;
- orbicularis-oris / vermilion aperture;
- mouth-corner / modiolus neighborhoods;
- nasal ala and nasolabial region;
- brow/orbital rim;
- mandibular border.

Use poles and triangles where topology must terminate, but keep them away from expected high-strain/contact regions when possible.

This is a modeling convenience, **not a claim that muscle fibers follow quad edges**.

## What topology must support

### Eye
Enough resolution for:
- upper/lower lid curvature;
- globe-following motion;
- blink and squint;
- lid-to-lid contact;
- orbital fat / retaining-structure effects.

### Mouth
Enough resolution for:
- lip thickness and separate inner/outer surfaces;
- lip-to-lip contact;
- corner motion;
- circumferential contraction;
- mustache attachment without hiding mouth mechanics.

### Cheek
Enough resolution to distinguish:
- superficial cheek surface;
- nasolabial region;
- zygomatic/malar curvature;
- jowl/lower-cheek transition;
- fat-compartment and retaining-septum effects.

## Acceptance tests

A surface topology is not accepted because it looks clean.

It should survive:

- neutral curvature inspection;
- isolated actuator deformation;
- mesh-density refinement;
- alternate triangulation of the same quad control surface;
- left/right symmetry checks on a symmetric synthetic head;
- contact tests around eyelids and lips;
- comparison to the volume-solver boundary once that exists.

If the result changes materially when only a quad diagonal is flipped, the surface discretization is too influential.

## v1 envelope baseline result

The first candidate connects the cage's transverse rings into 240 quads with two deliberate open boundary loops.

Measured baseline:

- 264 vertices, 240 quads;
- 0 nonmanifold edges;
- bilateral symmetry error below (10^{-12});
- flipping every quad diagonal changes total area by about 4.49e-16;
- worst per-quad diagonal area sensitivity is about 0.105%;
- surface-area change from 4x to 8x refinement is about 0.021%;
- minimum triangle angle is only 7.73 degrees.

The final point is intentionally a warning: the crown/chin taper creates skinny triangles after GPU triangulation. This baseline is numerically useful but should **not** be reused as the eventual contact/FEM mesh.

The next topology candidate must introduce eye and mouth apertures plus local modiolus/nasolabial resolution, then rerun the same diagonal/refinement tests.
