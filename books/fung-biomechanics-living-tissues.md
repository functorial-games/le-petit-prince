# Y. C. Fung — *Biomechanics: Mechanical Properties of Living Tissues*

**Reference:** Y. C. Fung, *Biomechanics: Mechanical Properties of Living Tissues*, 2nd ed., Springer, 1993. ISBN 978-0-387-97947-2.

**Primary sources used for this note:**
- Springer bibliographic references and academic syllabi
- https://cir.nii.ac.jp/crid/1971430859761518886

## Summary

Fung's central contribution is to treat living tissue as material whose mechanical behavior must be measured and modeled rather than assumed to resemble ordinary engineering solids.

The book develops biomechanics through constitutive behavior: stress, strain, elasticity, viscoelasticity, nonlinear response, and the distinctive properties of biological tissues. It is foundational for understanding why a hand-written vertex displacement field is not equivalent to a tissue model.

Soft tissues commonly undergo large deformation, show nonlinear stress-strain relationships, may be direction-dependent, and can exhibit time-dependent behavior. Biological function and structure are coupled to these properties.

## What it contributes to the pilot model

The main lesson is methodological:

[
	ext{geometry} + 	ext{constitutive law} + 	ext{loads/activation} + 	ext{constraints}
ightarrow 	ext{deformation}.
]

We should not start with a desired surface displacement and call it tissue physics.

For facial tissue this suggests testing at least:

- nonlinear rather than small-strain elasticity;
- near-incompressibility, because soft tissue generally does not freely lose volume during ordinary expression;
- heterogeneous properties where muscle, fat, dermal tissue, etc. differ;
- viscoelastic/time-dependent effects only where the behavior we want to explain requires them;
- parameter sensitivity, because uncertain material constants should not be hidden.

## Muscle relevance

Fung also provides biomechanical context for muscle and active biological tissue. For our purposes, passive tissue mechanics and active muscle mechanics must be separated. A muscle can create active stress along preferred directions while surrounding tissue resists and redistributes that stress.

That is already more physically meaningful than “move all vertices inside an ellipsoid toward an insertion point.”

## What it does not provide

Fung is not a face atlas and does not tell us where the zygomaticus or retaining ligaments belong in our pilot. It supplies constitutive thinking, not personalized facial geometry.

## Repository implication

Any future “reference solver” should state its material assumptions explicitly:

- strain-energy function;
- compressibility/incompressibility treatment;
- anisotropy;
- passive versus active stress;
- parameter values and provenance;
- whether time dependence is included;
- expected parameter range;
- sensitivity/convergence tests.

A model that cannot state those assumptions is an animation heuristic, not yet a biomechanical reference.
