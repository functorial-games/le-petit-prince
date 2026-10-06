# Model assumptions ledger

## Established enough to use as anatomical constraints

- facial soft tissue is layered rather than a homogeneous surface shell;
- superficial facial fat is compartmentalized;
- fibrous septa/retaining structures partition compartments and constrain relative motion;
- mimetic muscles have distinct anatomical paths and insertions, often into superficial soft tissues;
- deep masticatory structures and the mandible require different treatment from superficial mimetic muscles;
- lips and eyelids require contact-aware mechanics for high-fidelity closure;
- muscle force is directionally structured by fiber architecture.

## Supported direction, but not yet numerically identified for this pilot

- exact compartment volumes;
- exact retaining-structure geometry;
- ligament/septum stiffness and nonlinearity;
- fat constitutive parameters;
- dermis/SMAS constitutive parameters;
- friction/sliding parameters between layers/spaces;
- active muscle stress parameters;
- subject-specific skull and attachment coordinates.

These must remain parameters with provenance and sensitivity ranges until measured or fitted.

## Present native-model heuristics to avoid promoting into "science"

- ellipsoidal influence radii as if they were anatomical boundaries;
- direct vertex pull fields as if they were continuum stress;
- hand-written cheek-compression deltas;
- hand-written mentalis offsets;
- a single surface bulge rule for masseter/temporalis;
- jaw coefficients as if they were measured forces/torques;
- hyoid coefficients as if they were measured coupled mechanics;
- eyelid closure without explicit thickness/globe/contact;
- one surface mesh serving simultaneously as anatomy, mechanics and rendering.

These remain useful diagnostics and real-time approximations until replaced or validated.

## Planned hierarchy

The reference model should be capable of producing a deformed skin boundary. The Android renderer should consume that boundary or a reduced model fitted to it.

A later reduction step may deliberately recover something actuator-like for real-time use, but the approximation error should be measured against the reference solver.
