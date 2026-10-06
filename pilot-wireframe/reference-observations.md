# Pilot reference observations used for control cage v0

The pilot drawings are exploratory and not mutually identical. The control cage therefore uses only the subset that is consistent with the current character brief.

## Primary identity references

Used directly for the broad facial proportions and massing:

- `Astonished Pilot Meets the Little Prince.png`
- `Weary Pilot Beneath the Twilight Dunes.png`
- `Pilot Meets the Little Prince in the Desert.png`

Shared features across those drawings:

- dark-skinned, heavyset pilot;
- very broad cheek mass compared with brow width;
- large rounded lower cheeks / early jowl shape;
- substantial nose projection;
- thick mustache region;
- heavy orbital/lower-lid tissue;
- chin narrower than the cheek/jowl envelope;
- strong asymmetry allowed by pose/expression.

## Secondary references

`Desert Pilot Anatomy Study.png` is useful for anatomical layering, feature placement vocabulary, and viewing-angle ideas, but its visible identity is much leaner than the current heavy pilot. It is **not** used as a metric identity target.

Earlier leaner pilot drawings are likewise historical design iterations rather than measurements to average into the final head.

## Measurement policy

The drawings are not calibrated cameras and do not provide camera intrinsics, exact pose, or orthographic views. Therefore:

- image-space coordinates are observations, not Euclidean coordinates;
- front-view width ratios are only moderate-confidence constraints;
- depth is weakly constrained and remains a free parameter;
- hidden/occluded landmarks are not invented;
- mustache edges are not treated as lip landmarks;
- expression-dependent aperture dimensions are not treated as neutral anatomy.

`landmark-observations.tsv` records one approximate image-space landmark pass on the strongest near-frontal heavy-pilot reference. It is a starting point for later pose/camera fitting, not photogrammetry.

## Why v0 is a line cage rather than a face mesh

The cage deliberately avoids choosing triangles, quads, tetrahedra, or hexahedra before the topology/mechanics question is settled.

It records:

- transverse head-envelope rings;
- longitudinal silhouette guides;
- orbital rims;
- brow guides;
- nose ridge and alar base;
- outer perioral guide;
- nasolabial guides;
- cheek-mass guides;
- jawline and labiomental guide.

A later surface can be fitted to this cage. A later biomechanics volume can be fitted to the same anatomical geometry independently.
