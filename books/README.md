# Face biomechanics reading notes

These notes collect every book and paper explicitly mentioned in the face-biomechanics discussion that led to the native pilot-face rainbow diagnostic.

They are **technical orientation summaries**, not substitutes for the full sources. Book notes are based on publisher/catalog descriptions, tables of contents, and established subject matter unless a full text was available. Paper notes are based on the paper/abstract or an accessible author manuscript where available. Claims that matter to the implementation should be checked against the original source before being promoted to a model invariant.

## Books

- [Larrabee, Makielski & Henderson — *Surgical Anatomy of the Face*](larrabee-surgical-anatomy-of-the-face.md)
- [Brennan, Standring & Wiseman — *Gray's Surgical Anatomy*](grays-surgical-anatomy.md)
- [Uldis Zarins — *Anatomy of Facial Expressions*](zarins-anatomy-of-facial-expressions.md)
- [Y. C. Fung — *Biomechanics: Mechanical Properties of Living Tissues*](fung-biomechanics-living-tissues.md)
- [Gerhard A. Holzapfel — *Nonlinear Solid Mechanics*](holzapfel-nonlinear-solid-mechanics.md)
- [Belytschko, Liu, Moran & Elkhodary — *Nonlinear Finite Elements for Continua and Structures*](belytschko-nonlinear-finite-elements.md)
- [Richard L. Lieber — *Skeletal Muscle Structure, Function, and Plasticity*](lieber-skeletal-muscle.md)
- [Larry A. Taber — *Continuum Modeling in Mechanobiology*](taber-continuum-modeling-mechanobiology.md)

## Papers

- [Sifakis, Neverov & Fedkiw (2005) — *Automatic Determination of Facial Muscle Activations from Sparse Motion Capture Marker Data*](sifakis-neverov-fedkiw-2005.md)
- [Wu, Hung, Hunter & Mithraratne (2013) — *Modelling facial expressions: a framework for simulating nonlinear soft tissue deformations using embedded 3-D muscles*](wu-hung-hunter-mithraratne-2013.md)
- [Wu, Hung & Mithraratne (2014) — *Generating Facial Expressions Using an Anatomically Accurate Biomechanical Model*](wu-hung-mithraratne-2014.md)

## Working conclusion for this repository

The current Android face proves the native rendering/control path and provides a useful actuator diagnostic. It does **not** establish that the present surface-mesh deformation law is a validated biomechanical model.

For a scientifically defensible reference model, keep three objects conceptually separate:

1. **Anatomical geometry:** skull, mandible, skin/subcutaneous tissue, SMAS, muscles, and relevant interfaces/attachments.
2. **Simulation discretization:** volume elements chosen for nonlinear, nearly incompressible, anisotropic soft-tissue mechanics and contact.
3. **Rendering surface:** a surface mesh chosen for display and artistic control, driven by the simulation rather than treated as the physical tissue itself.

The papers below demonstrate that this separation is practical. The textbooks provide the anatomy, continuum mechanics, muscle mechanics, and numerical-analysis background needed to judge the assumptions rather than merely copy a graphics technique.
