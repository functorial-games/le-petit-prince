# Larry A. Taber — *Continuum Modeling in Mechanobiology*

**Reference:** Larry A. Taber, *Continuum Modeling in Mechanobiology*, Springer, 2020. DOI 10.1007/978-3-030-43209-6.

**Primary source used for this note:**
- https://link.springer.com/book/10.1007/978-3-030-43209-6

## Summary

Taber is a useful bridge between general nonlinear continuum mechanics and biological active tissues. The book develops vector/tensor analysis, continuum mechanics, nonlinear elasticity, soft-tissue biomechanics, contraction, growth, remodeling, and morphogenesis.

For this project, the chapter sequence through **soft tissue biomechanics → contraction** is particularly relevant. It helps separate the passive material response of tissue from internally generated active force.

## What it contributes to the pilot model

A face expression is not well described as an external force applied to a rubber head. Muscle activation creates internal stress in an anisotropic active material, which interacts with passive tissue, attachments, incompressibility, contact, and geometry.

A useful conceptual split is

[
sigma = sigma_{	ext{passive}}(F,	ext{tissue})
       + sigma_{	ext{active}}(F,	ext{fiber},a),
]

with activation (a) controlling active stress along muscle fibers.

That structure is much easier to reason about than a collection of special-case vertex displacement functions.

## Why Taber complements Holzapfel

Holzapfel gives a broad rigorous nonlinear-solid framework. Taber keeps the same continuum language but explicitly develops biological phenomena such as contraction. That makes it a good source for checking whether a proposed active law has a meaningful place in continuum mechanics.

## What it does not provide

It is not a facial anatomy reference and is not a recipe for the exact muscle activation law to use for every facial muscle. Experimental calibration still matters.

## Repository implication

Before implementing a reference solver, define tests on simple geometries where the answer is interpretable:

- passive block in tension/compression;
- incompressible block;
- transversely isotropic passive block;
- activated fiber block;
- activation under fixed length;
- activation with free contraction.

If the constitutive implementation fails those tests, running it on a face only makes the failure harder to see.
