# Larrabee, Makielski & Henderson — *Surgical Anatomy of the Face*

**Reference:** Wayne F. Larrabee Jr., Kathleen H. Makielski, Jenifer L. Henderson, *Surgical Anatomy of the Face*, 2nd ed., Lippincott Williams & Wilkins, 2004. ISBN 978-0-7817-4150-7.

**Primary source used for this note:** publisher/catalog descriptions and table of contents:
- https://www.wolterskluwer.com/en/solutions/ovid/surgical-anatomy-of-the-face-5342
- https://books.google.com/books?id=f2Ca-m0MjagC

## Summary

This is the most directly face-specific anatomical book in the reading set. Its purpose is surgical anatomy: not simply naming structures, but showing their spatial relationships as encountered when dissecting and operating on the face.

The chapters most relevant to a biomechanical face model are the hard-tissue foundation, skin and soft tissue, facial musculature, eyelids/anterior orbit, nose, and lips/chin. The second edition also emphasizes osteocutaneous and retaining ligaments and orbital supporting structures.

That emphasis matters computationally. A face should not be treated as a homogeneous rubber shell attached to a few muscle vectors. The visible surface is mechanically conditioned by the skull and mandible underneath it, by layers of soft tissue, by facial muscles that have unusual insertions into skin/SMAS, and by ligaments and septa that constrain relative motion.

## What it contributes to the pilot model

- **Geometric registration.** Muscle names alone are insufficient. Origins, insertions, depth, relation to bone, and relation to adjacent structures determine what a contraction can plausibly move.
- **Layering.** The skin surface is the boundary of a layered volume. Subcutaneous tissue and SMAS are mechanically relevant intermediates.
- **Constraint anatomy.** Retaining ligaments provide a reason for some regions to move less than neighboring tissue and for deformation to localize.
- **Regional special cases.** Eyelids, lips, nose, cheek, and chin should not all share one generic deformation rule.

For the heavy pilot, the cheek/jowl volume should be constructed on top of this anatomical scaffold rather than obtained by simply scaling a generic face outward.

## What it does not provide

It is an anatomy/surgery atlas, not a constitutive-mechanics text. It will not tell us a complete strain-energy function, element formulation, or calibrated material parameters for simulation. It establishes **where structures are and how they relate**, not a finished numerical law for deformation.

## Repository implication

Before changing the 102-actuator model into a reference biomechanics model, derive an anatomy ledger from sources like this one:

- muscle or structure;
- side and compartment;
- origin;
- insertion;
- depth/layer;
- approximate 3-D extent;
- fiber direction where known;
- neighboring fat/SMAS/ligament constraints;
- skeletal attachments;
- expected surface action.

Those facts should be separated from later numerical approximations.
