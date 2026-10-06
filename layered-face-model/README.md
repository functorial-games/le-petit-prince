# Layered face model: fat, fascia, retaining structures, muscle, skin

The heavy pilot is exactly the kind of character for whom a muscle-to-skin shortcut is inadequate. Cheek and jowl shape depends strongly on soft-tissue volume and constraints between layers.

## Working anatomical stack

From deep to superficial, model conceptually:

1. skull / mandible and other rigid deep structures;
2. deep facial spaces and deep fat where relevant;
3. mimetic and masticatory muscle volumes with fiber directions;
4. SMAS / superficial fascial layer;
5. superficial fat compartments;
6. fibrous septa / retaining structures connecting fascia, periosteum and dermis;
7. dermis / visible skin boundary.

This list is a modeling decomposition, not a claim that every boundary is a clean separable membrane everywhere.

## Mechanics roles

- **muscle** generates active anisotropic stress;
- **fat** supplies deformable volume and changes local curvature/strain distribution;
- **fascia/SMAS** mechanically couples regions and carries embedded mimetic muscles;
- **retaining ligaments/septa** constrain shear and relative motion and partition some fat compartments;
- **bone** supplies rigid attachment/contact boundaries;
- **skin** is the visible boundary, not the whole physical model.

## Ledgers

- `fat-compartments.tsv` records the compartments worth representing or explicitly deciding to omit.
- `retaining-structures.tsv` records candidate retaining constraints.
- `model-assumptions.md` separates established anatomy from mechanics hypotheses.

No spring, penalty, or stiffness value should be inserted merely because it produces an attractive face. Parameters need provenance, measurement, identification, or an explicit sensitivity analysis.
