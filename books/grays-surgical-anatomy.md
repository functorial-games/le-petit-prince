# Brennan, Standring & Wiseman — *Gray's Surgical Anatomy*

**Reference:** Peter A. Brennan, Susan Standring, Sam M. Wiseman, eds., *Gray's Surgical Anatomy*, 2nd ed., Elsevier, 2026. ISBN 978-0-443-12568-3.

**Primary source used for this note:** Elsevier description and contents:
- https://shop.elsevier.com/books/grays-surgical-anatomy/brennan/978-0-443-12568-3

## Summary

*Gray's Surgical Anatomy* is a broad applied regional-anatomy reference rather than a face-only book. Its value here is conservatism and cross-checking. The head-and-neck section separates facial skeleton, face/scalp, orbit, eye, nose, temporomandibular joint, mouth, salivary structures, and neck.

That organization is useful because a facial mechanics model spans several anatomical systems that animation rigs often collapse together. Jaw motion belongs to the temporomandibular and masticatory system. Eyelid motion is constrained by orbital anatomy and the globe. Facial-expression muscles live in a different structural setting from the deep muscles of mastication. The mouth is a contact-rich opening rather than merely a painted line on a surface.

## What it contributes to the pilot model

Use Gray's as an anatomical **sanity check** against specialized or artist-oriented sources.

Particular checks should include:

- facial-bone landmarks used to anchor a personalized head;
- orbit and eye geometry before designing blink/squint mechanics;
- temporomandibular-joint geometry before accepting a simplified jaw transform;
- deep versus superficial muscle relationships;
- mouth and lip anatomy before assigning contact or closing constraints;
- facial-muscle dissections for relative depth and path.

## Why it matters for the wireframe question

Gray's does not prescribe triangles or quads. Anatomy determines the continuous structures; discretization comes later. The important issue is whether the computational mesh resolves the relevant anatomical boundaries and deformation gradients.

A surface edge loop that looks elegant but ignores an actual attachment or sliding boundary is not automatically better than a less regular mesh. Conversely, a simulation volume that cannot resolve eyelid thickness, lip contact, or a narrow muscle path is under-resolved even if its rendered surface looks smooth.

## What it does not provide

It is not a facial finite-element textbook and does not give validated constitutive laws for soft tissue. It should constrain geometry and relationships, while Fung/Holzapfel/Belytschko/Taber constrain the mechanics and numerics.

## Repository implication

When two anatomical sources disagree, do not resolve the conflict by tuning a renderer. Record the disagreement in the anatomy ledger and determine whether it reflects genuine anatomical variability, differing nomenclature, or a modeling error.
