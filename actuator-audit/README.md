# Audit of the 102 current low-level actuators

The native pilot-face runtime contains 102 independent actuator inputs derived from 52 independent rows of the canonical Beauty musculature ledger.

`actuators.tsv` is a **first-pass audit**, not a declaration that the mechanics are correct.

## Current finding

The useful distinction is:

- **anatomical identity/action:** often defensible at the named-structure level;
- **numeric anchors/radii:** explicitly approximate and unvalidated;
- **mechanical transmission to the skin:** presently heuristic.

The current system therefore remains valuable as an animation/control model and as a diagnostic surface, but it is not yet the reference biomechanics model.

## Audit columns

- `anatomical_structure`: the anatomical row represented by the actuator.
- `current_mechanism`: implementation mechanism in the current native model.
- `anatomy_status`: whether the named structure/action has an anatomical basis.
- `numeric_geometry_status`: status of origin/insertion/radius values.
- `mechanics_status`: status of the deformation law.
- `fat_dependency`: how important explicit fat-compartment modeling is likely to be for visible behavior.
- `retaining_structure_dependency`: how important ligaments/septa/fascial constraints are likely to be.
- `validation_target`: what should eventually be compared with anatomy/scan/solver evidence.

## Next audit pass

For every base structure:

1. cross-check origin, insertion, layer and action against the book/paper set in `../books/`;
2. replace point anchors with anatomical regions or volume geometry where appropriate;
3. add fiber architecture;
4. attach source citations to each assertion;
5. identify measured versus estimated material parameters;
6. write single-muscle validation expectations;
7. decide whether the actuator remains a biological degree of freedom or becomes only a high-level animation control.

The left/right rows remain separate because asymmetry is a required behavior, but evidence for the underlying anatomical structure is normally recorded once at the base-structure level.
