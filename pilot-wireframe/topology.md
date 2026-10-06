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
