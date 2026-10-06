# Facial feature topology v2 report

This candidate does **not** replace the v1 head envelope. It isolates the regions where topology has a direct functional reason to exist before any global stitching is attempted.

## Construction

- each eyelid aperture is surrounded by three concentric elliptical rings with 24 samples and quad bands;
- the lip aperture is surrounded by three concentric elliptical rings with 20 samples and quad bands;
- each nasolabial region is a three-rail quad strip from the nasal sidewall toward the mouth corner;
- each modiolus is a **filled hybrid patch**: 8 triangles meet at a central topological pole and an outer ring of 8 quads surrounds them.

The modiolus is deliberately not represented as a hole. This is the first place where the topology uses triangles for a specific reason rather than because the GPU happens to consume triangles.

## Numerical checks

Across all seven local patches:

- 322 vertices;
- 204 quads;
- 16 deliberate fixed triangles;
- no nonmanifold edges in any patch;
- global minimum triangulated angle: 13.482 degrees;
- worst local quad area sensitivity to the choice of diagonal: 0.181%;
- bilateral mirror error is zero to the precision of the generated coordinates.

Refining both angular and radial resolution gives:

- eye patch 2x -> 4x surface-area change: 0.215%;
- mouth patch 2x -> 4x surface-area change: 0.304%.

These local patches are numerically much better behaved than trying to force a single coarse latitude-longitude envelope to represent eyelids, lips and mouth-corner mechanics.

## Scientific boundary

The ellipses are topology/geometry hypotheses based on the existing control cage, **not measured anatomical aperture curves**. The nasolabial strip is likewise a topology guide, not a ligament or fold element.

The patches intentionally overlap conceptually and remain disconnected. Welding them now would hide an unresolved choice about how perioral, cheek, fat-compartment and retaining-structure regions should share degrees of freedom.

The next candidate should stitch these outer feature loops into a connected frontal sheet and locate any unavoidable poles/triangles in lower-strain transition regions. That connected sheet must rerun the v1 diagonal/refinement tests and preserve the v2 aperture/pole tests.
