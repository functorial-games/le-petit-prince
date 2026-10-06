# Surface topology v1 numerical report

This is the **envelope-only baseline**, not an accepted facial production mesh.
It connects the transverse rings of the pilot control cage into a periodic quad
surface while leaving the top and bottom boundaries open. Eye and mouth
apertures are intentionally not cut yet.

## Measured results

- vertices: 264
- quads: 240
- nonmanifold edges: 0
- boundary edges: 48 (two expected 24-edge boundary loops)
- bilateral symmetry error: 8.618e-16
- total-area change from flipping every quad diagonal: 4.487e-16
- worst single-quad area change from diagonal flip: 0.001053
- minimum triangle angle, either diagonal convention: 7.734 degrees
- area refinement change 4x -> 8x: 0.0002055

## Interpretation

The coarse envelope passes the intended **baseline numerical tests**: it is
manifold apart from its two deliberate open boundaries, exactly symmetric to
floating-point precision, insensitive in total area to the arbitrary GPU
triangle diagonal, and its surface area is converging under refinement.

The minimum triangle angle is only about 7.7 degrees near the strongly tapering crown/chin bands. That is acceptable for this diagnostic baseline but is **not** good enough to promote this mesh into a contact or finite-element model.

Most importantly, these results do not validate facial topology. The baseline
still lacks explicit eyelid and lip apertures, modiolus topology, nasal folds,
and the local resolution needed for retaining structures and fat-compartment
boundaries. Those must be introduced as a second candidate and subjected to
the same tests rather than silently modifying this baseline.
